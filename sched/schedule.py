from ryu import utils
from ryu.base import app_manager
from ryu.ofproto import ofproto_v1_3
from ryu.controller import ofp_event
from ryu.controller.handler import set_ev_cls
from ryu.controller.handler import HANDSHAKE_DISPATCHER, CONFIG_DISPATCHER, MAIN_DISPATCHER
from ryu.lib.packet import packet, ethernet, ipv4

from os import listdir
from time import sleep

import nesox
from nesox import flow
from nesox import transdir

class SCHEDULE(app_manager.RyuApp):
    OFP_VERSIONS = [ofproto_v1_3.OFP_VERSION]

    def __init__(self, *args, **kwargs):
        super(SCHEDULE, self).__init__(*args, **kwargs)
        self.debug = 1
        self.trace = 0

        self.table_start = 100
        self.table_terminate = 203
        self.hard_table_id = 100
        self.hard_table_no = 1
        self.hard_tables = [100]
        self.soft_table_id = 200
        self.soft_table_no = 4
        self.soft_tables = [200, 201, 202, 203]
        self.table_learning = 203

        self.flows = []
        self.transfermap = {}
        self.transfers = listdir(transdir)
        self.datapath = None
        self.packetin_counter = 0
        self.monitor_on = True
        self.monitor_frequency = 1
        self.pipeline_mode = True
        self.packet_count = 0
        self.byte_count = 0
        self.packet_size = 0
        self.addressportmap = {}


    def insert_actions(self, datapath, tid, match, pri, actions):
        protocol = datapath.ofproto
        parser = datapath.ofproto_parser
        instructions = [parser.OFPInstructionActions(protocol.OFPIT_APPLY_ACTIONS, actions)]
        modification = parser.OFPFlowMod(datapath=datapath, table_id=tid, match=match, priority=pri, instructions=instructions)
        datapath.send_msg(modification)

    def insert_output(self, datapath, tid, match, pri, port):
        protocol = datapath.ofproto
        parser = datapath.ofproto_parser
        actions = [parser.OFPActionOutput(port)]
        instructions = [parser.OFPInstructionActions(protocol.OFPIT_APPLY_ACTIONS, actions)]
        modification = parser.OFPFlowMod(datapath=datapath, table_id=tid, match=match, priority=pri, instructions=instructions)
        datapath.send_msg(modification)

    def insert_controller(self, datapath, tid, match, pri):
        protocol = datapath.ofproto
        parser = datapath.ofproto_parser
        actions = [parser.OFPActionOutput(protocol.OFPP_CONTROLLER, protocol.OFPCML_NO_BUFFER)]
        instructions = [parser.OFPInstructionActions(protocol.OFPIT_APPLY_ACTIONS, actions)]
        modification = parser.OFPFlowMod(datapath=datapath, table_id=tid, match=match, priority=pri, instructions=instructions)
        datapath.send_msg(modification)

    def insert_goto(self, datapath, tid, match, pri, gototid):
        protocol = datapath.ofproto
        parser = datapath.ofproto_parser
        instructions = [parser.OFPInstructionGotoTable(gototid)]
        modification = parser.OFPFlowMod(datapath=datapath, table_id=tid, match=match, priority=pri, instructions=instructions)
        datapath.send_msg(modification)

    def request_flowstats(self, datapath, tid, match):
        protocol = datapath.ofproto
        parser = datapath.ofproto_parser
        cookie = cookie_mask = 0
        request = parser.OFPFlowStatsRequest(datapath, 0, tid, protocol.OFPP_ANY, protocol.OFPG_ANY, 0, 0, match)
        datapath.send_msg(request)

    def request_flowstatsall(self, datapath, match):
        protocol = datapath.ofproto
        parser = datapath.ofproto_parser
        cookie = cookie_mask = 0
        request = parser.OFPFlowStatsRequest(datapath, 0, protocol.OFPTT_ALL, protocol.OFPP_ANY, protocol.OFPG_ANY, 0, 0, match)
        datapath.send_msg(request)

    def request_meterfeature(self, datapath):
        parser = datapath.ofproto_parser
        request = parser.OFPMeterFeaturesStatsRequest(datapath, 0)
        datapath.send_msg(request)

    def request_meterconfig(self, datapath):
        protocol = datapath.ofproto
        parser = datapath.ofproto_parser
        request = parser.OFPMeterConfigStatsRequest(datapath, 0, protocol.OFPM_ALL)
        datapath.send_msg(request)

    def request_meterstats(self, datapath):
        protocol = datapath.ofproto
        parser = datapath.ofproto_parser
        request = parser.OFPMeterStatsRequest(datapath, 0, protocol.OFPM_ALL)
        datapath.send_msg(request)

    def insert_meter(self, datapath, mid, rate):
        protocol = datapath.ofproto
        parser = datapath.ofproto_parser
        bands = [parser.OFPMeterBandDrop(rate=rate)]
        modification = parser.OFPMeterMod(datapath=datapath, flags=protocol.OFPMF_KBPS, meter_id=mid, bands=bands)
        datapath.send_msg(modification)

    def change_meter(self, datapath, mid, rate):
        protocol = datapath.ofproto
        parser = datapath.ofproto_parser
        bands = [parser.OFPMeterBandDrop(rate=rate)]
        modification = parser.OFPMeterMod(datapath=datapath, command=protocol.OFPMC_MODIFY, meter_id=mid, bands=bands)
        datapath.send_msg(modification)

    def delete_meter(self, datapath, mid):
        protocol = datapath.ofproto
        parser = datapath.ofproto_parser
        modification = parser.OFPMeterMod(datapath=datapath, command=protocol.OFPMC_DELETE, meter_id=mid)
        datapath.send_msg(modification)


    @set_ev_cls(ofp_event.EventOFPSwitchFeatures, CONFIG_DISPATCHER)
    def handler_switch_features(self, event):
        if self.trace: self.logger.info("SCHEDULE [Handler = Switch Features]: enter!")

        """
        initialize packetin counter
        """
        self.packetin_counter = 0

        """
        initialize datapath
        """
        datapath = event.msg.datapath
        parser = datapath.ofproto_parser

        self.datapath = datapath

        """
        read `transfer` requirement into a `list` and a `dictionary`
        """
        for tran in self.transfers: transfile = open(transdir + tran)
        for line in transfile: self.flows.append(nesox.flow(int(line[0]), int(line[2]), int(line[6:].rstrip()), int(line[4])))
        for flow in self.flows: self.transfermap[(flow.source, flow.destination)] = flow
        if self.debug: self.logger.info("tramsfermap: %s", self.transfermap)

        """
        !!! Note: flow is defined by a match and a priority !!!
        """

        """
        create flows (default)
        priority = 1
        initialize pipeline: (ANY) packet pass through the pipeline to controller
        """
        p = 1
        m = parser.OFPMatch()
        t = self.table_start
        for gototid in self.soft_tables:
            self.insert_goto(datapath, t, m, p, gototid)
            t = gototid
        self.insert_controller(datapath, t, m, p)


        """
        create flows (service dispatching)
        priority = 2
        for data transfer service port family 8***
        (still does NOT work!)
        """
        P = 2
        m = parser.OFPMatch(eth_type=0x0800, tcp_dst=8478)
        t = self.soft_table_id
        gototid = self.table_terminate
        self.insert_goto(datapath, t, m, p, gototid)


        """
        The Ethernet MTU is 1500 bytes! (from google)
        It means the largest IP packet (or some other payload) an Ethernet frame can contain is 1500 bytes.
        """

        """
        create meters (static)
        """
        rates = [20*1000, 40*1000, 80*1000, 100*1000, 0]
        for mid in range(0, len(rates)):
            self.insert_meter(datapath, mid+1, rates[mid])


        """
        monitor flows
        """
        m = parser.OFPMatch()
        if self.monitor_on: self.request_flowstats(datapath, self.table_terminate, m)


        """
        monitor meters
        """
        if self.monitor_on: self.request_meterfeature(datapath)
        if self.monitor_on: self.request_meterconfig(datapath)
        if self.monitor_on: self.request_meterstats(datapath)



        if self.trace: self.logger.info("SCHEDULE [Handler = Switch Features]: leave!")


    @set_ev_cls(ofp_event.EventOFPPacketIn, MAIN_DISPATCHER)
    def handler_packetin(self, event):
        if self.trace: self.logger.info("SCHEDULE [Handler = Packet In]: enter! [%s]", self.packetin_counter)
        """
        increment `packet in` counter
        """
        self.packetin_counter += 1

        """
        initialize ryu event driven class hierarchy
        """
        message = event.msg
        datapath = message.datapath
        protocol = datapath.ofproto
        parser = datapath.ofproto_parser
        data = message.data
        inport = message.match['in_port']

        """
        parse `packet in` data using Ethernet/IP stack
        """
        inpacket = packet.Packet(data)
        inlayer2 = inpacket.get_protocols(ethernet.ethernet)[0]
        inlayer3 = inpacket.get_protocol(ipv4.ipv4)

        """
        Ethernet (layer2) addresses (source, destination)
        """
        sethernet = inlayer2.src
        dethernet = inlayer2.dst

        """
        initialize `addressportmap` using `datapathid` as primary key
        """
        datapathid = datapath.id
        self.addressportmap.setdefault(datapathid, {})

        """
        learn the `packet in` mapping of address and port
        """
        self.addressportmap[datapathid][sethernet] = inport

        """
        look up the learned dictionary for destination port (otherwise, flood the packet)
        """
        if dethernet in self.addressportmap[datapathid]:
            outport = self.addressportmap[datapathid][dethernet]
            m = parser.OFPMatch(in_port = inport, eth_dst = dethernet)
            p = 2
            t = self.table_learning
            self.insert_output(datapath, t, m, p, outport)
        else: outport = protocol.OFPP_FLOOD

        """
        send the packet out from controller to its destination port
        """
        if message.buffer_id == protocol.OFP_NO_BUFFER: data = message.data
        actions = [parser.OFPActionOutput(outport)]
        packetout = parser.OFPPacketOut(datapath=datapath, buffer_id=message.buffer_id, in_port=inport, actions=actions, data=data)
        datapath.send_msg(packetout)


        """
        IP (layer3) addresses (source, destination)
        """
        if inlayer3 is not None:
            sip = inlayer3.src
            dip = inlayer3.dst
            if self.debug: self.logger.info("packetin:(%s) >> (%s)", sip, dip)

        if self.trace: self.logger.info("SCHEDULE [Handler = Packet In]: leave!")


    @set_ev_cls(ofp_event.EventOFPFlowStatsReply, MAIN_DISPATCHER)
    def handler_flowstats(self, event):
        if self.trace: self.logger.info("SCHEDULE [Handler = Flow Stats]: enter!")
        datapath = event.msg.datapath
        flowstat = event.msg.body
        parser = datapath.ofproto_parser

        counters = []
        for stat in flowstat:
            if stat.priority == 1:
                counters.append((stat.table_id, stat.match, stat.priority, stat.packet_count, stat.byte_count))
        if self.debug: self.logger.info("counters: %s", counters)
        if counters[0][3] != self.packet_count:
            self.packet_size = (counters[0][4] - self.byte_count) / (counters[0][3] - self.packet_count)
        if self.debug: self.logger.info("packet size: %s", self.packet_size)
        self.byte_count = counters[0][4]
        self.packet_count = counters[0][3]

        tid = self.table_terminate
        m = parser.OFPMatch()
        sleep(self.monitor_frequency)
        if self.monitor_on: self.request_flowstats(datapath, tid, m)

        if self.trace: self.logger.info("SCHEDULE [Handler = Flow Stats]: leave!")


    @set_ev_cls(ofp_event.EventOFPMeterFeaturesStatsReply, MAIN_DISPATCHER)
    def handler_meterfeature(self, event):
        if self.trace: self.logger.info("SCHEDULE [Handler = Meter Features]: enter!")
        datapath = event.msg.datapath
        features = event.msg.body

        meterfeatures = []
        for feature in features:
            meterfeatures.append((feature.max_meter, feature.band_types, feature.max_band, feature.capabilities))
        if self.debug: self.logger.info("meter features: %s", meterfeatures)

        if self.trace: self.logger.info("SCHEDULE [Handler = Meter Features]: leave!")


    @set_ev_cls(ofp_event.EventOFPMeterConfigStatsReply, MAIN_DISPATCHER)
    def handler_meterconfig(self, event):
        if self.trace: self.logger.info("SCHEDULE [Handler = Meter Configurations]: enter!")
        datapath = event.msg.datapath
        configs = event.msg.body

        meterconfigs = []
        for config in configs:
            meterconfigs.append((config.meter_id, config.bands, config.flags, config.length))
        if self.debug: self.logger.info("meter configs: %s", meterconfigs)

        if self.trace: self.logger.info("SCHEDULE [Handler = Meter Configurations]: leave!")


    @set_ev_cls(ofp_event.EventOFPMeterStatsReply, MAIN_DISPATCHER)
    def handler_meterstats(self, event):
        if self.trace: self.logger.info("SCHEDULE [Handler = Meter Stats]: enter!")
        datapath = event.msg.datapath
        stats = event.msg.body

        meterstats = []
        for stat in stats:
            meterstats.append((stat.meter_id, stat.flow_count, stat.packet_in_count, stat.byte_in_count, stat.band_stats))

        if self.debug: self.logger.info("meters: %s", meterstats)

        sleep(self.monitor_frequency)
        if self.monitor_on: self.request_meterstats(datapath)
        if self.trace: self.logger.info("SCHEDULE [Handler = Meter Stats]: leave!")

