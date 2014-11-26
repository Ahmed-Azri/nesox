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
        self.point = 0
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
        self.table_learning = 100

        self.flows = []
        self.transfermap = {}
        self.transfers = listdir(transdir)
        self.OFflows = []
        self.meters = {}
        self.matches = {}

        self.throughput = 1000000
        self.adapter = 1
        self.datapath = None
        self.packetin_counter = 0
        self.monitor_on = False
        self.monitor_on = True
        self.monitor_priority = 0
        self.monitor_frequency = 1
        self.pipeline_mode = True
        self.packet_count = 0
        self.byte_count = 0
        self.packet_size = 0
        self.addressportmap = {}
        self.addresslearninglayer = 3


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

    def attach_meter(self, datapath, tid, match, pri, mid):
        protocol = datapath.ofproto
        parser = datapath.ofproto_parser
        instructions = [parser.OFPInstructionMeter(meter_id=mid)]
        modification = parser.OFPFlowMod(datapath=datapath, table_id=tid, command=protocol.OFPFC_MODIFY, match=match, instructions=instructions)
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

    def insert_bytemeter(self, datapath, mid, rate):
        protocol = datapath.ofproto
        parser = datapath.ofproto_parser
        bands = [parser.OFPMeterBandDrop(rate=rate)]
        modification = parser.OFPMeterMod(datapath=datapath, flags=protocol.OFPMF_KBPS, meter_id=mid, bands=bands)
        datapath.send_msg(modification)

    def insert_packetmeter(self, datapath, mid, rate):
        protocol = datapath.ofproto
        parser = datapath.ofproto_parser
        bands = [parser.OFPMeterBandDrop(rate=rate)]
        modification = parser.OFPMeterMod(datapath=datapath, flags=protocol.OFPMF_PKTPS, meter_id=mid, bands=bands)
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
        self.flows = []
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
        self.insert_controller(datapath, self.table_learning, m, p)


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
        meter id: 1 - 5
        """
        rates = [4, 3, 2, 1, 0]
        for mid in range(0, len(rates)):
            self.insert_packetmeter(datapath, mid+1, rates[mid])


        """
        monitor flows
        """
        m = parser.OFPMatch()
        if self.monitor_on: self.request_flowstats(datapath, self.table_learning, m)


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
        IP (layer3) addresses (source, destination)
        """
        if inlayer3 is not None:
            sip = inlayer3.src
            dip = inlayer3.dst
            if self.debug: self.logger.info("packetin:(%s) >> (%s)", sip, dip)

        """
        initialize `addressportmap` using `datapathid` as primary key
        """
        datapathid = datapath.id
        self.addressportmap.setdefault((datapathid, 2), {})
        self.addressportmap.setdefault((datapathid, 3), {})

        """
        learn the `packet in` mapping of address and port
        """
        self.addressportmap[(datapathid, 2)][sethernet] = inport
        if inlayer3 is not None: self.addressportmap[(datapathid, 3)][sip] = inport

        """
        look up the learned dictionary for destination port (otherwise, flood the packet)
        """
        if (inlayer3 is not None) and (dip in self.addressportmap[(datapathid, 3)]):
            outport = self.addressportmap[(datapathid, 3)][dip]
            t = self.table_learning
            # m = parser.OFPMatch(eth_type = 0x0800, ipv4_dst = dip)
            m = parser.OFPMatch(eth_type = 0x0800, ipv4_src = sip, ipv4_dst = dip)
            p = 3
            mid = 1
            actions = [parser.OFPActionOutput(outport)]
            instructions = [parser.OFPInstructionActions(protocol.OFPIT_APPLY_ACTIONS, actions),parser.OFPInstructionMeter(meter_id=mid)]
            modification = parser.OFPFlowMod(datapath=datapath, table_id=t, match=m, priority=p, instructions=instructions)
            datapath.send_msg(modification)
            f = (t,m,p)
            self.OFflows.append(f)
            self.meters[f] = mid

        if dethernet in self.addressportmap[(datapathid, 2)]:
            outport = self.addressportmap[(datapathid, 2)][dethernet]
            t = self.table_learning
            m = parser.OFPMatch(eth_type = 0x0800, eth_dst = dethernet)
            p = 2
            mid = 1
            self.insert_output(datapath, t, m, p, outport)
            self.attach_meter(datapath, t, m, p, mid)
            f = (t,m,p)
            self.OFflows.append(f)
            self.meters[f] = mid
        else: outport = protocol.OFPP_FLOOD

        """
        send the packet out from controller to its destination port
        """
        if message.buffer_id == protocol.OFP_NO_BUFFER: data = message.data
        actions = [parser.OFPActionOutput(outport)]
        packetout = parser.OFPPacketOut(datapath=datapath, buffer_id=message.buffer_id, in_port=inport, actions=actions, data=data)
        datapath.send_msg(packetout)


        if self.trace: self.logger.info("SCHEDULE [Handler = Packet In]: leave!")


    @set_ev_cls(ofp_event.EventOFPFlowStatsReply, MAIN_DISPATCHER)
    def handler_flowstats(self, event):
        if self.trace: self.logger.info("SCHEDULE [Handler = Flow Stats]: enter!")
        datapath = event.msg.datapath
        flowstat = event.msg.body
        parser = datapath.ofproto_parser

        """
        book monitored counters into `counters`
        """
        counters = []
        for stat in flowstat:
            if stat.priority > self.monitor_priority:
                counters.append((stat.table_id, stat.match, stat.instructions, stat.priority, stat.packet_count, stat.byte_count))
        if self.debug: self.logger.info("counters: %s", counters)
        if self.point: self.logger.info("counters: %s", counters)


        """
        caculate packet size
        """
        if (counters) and (counters[-1][4] != self.packet_count):
            self.packet_size = (counters[-1][5] - self.byte_count) / (counters[-1][4] - self.packet_count)
        if self.debug: self.logger.info("packet size: %s", self.packet_size)
        if counters:
            self.byte_count = counters[-1][5]
            self.packet_count = counters[-1][4]

        """
        caculate remaining bytes to transfer
        """
        remaining = 0

        for counter in counters:
            match = counter[1]
            packetcount = counter[4]
            bytecount = counter[5]
            if (match.__contains__('eth_type')) and (match.get(key='eth_type') == 0x0800):
                if (match.__contains__('ipv4_src')): sip = match.get(key='ipv4_src')
                if (match.__contains__('ipv4_dst')): dip = match.get(key='ipv4_dst')
                if self.debug: self.logger.info("(%s)>>(%s)", sip, dip)
                s = int(sip[-1])
                d = int(dip[-1])
                self.matches[(s,d)] = match
                f = self.transfermap[(s,d)]
                if f.remaining > 0: f.remaining -= bytecount * self.adapter
                if f.remaining < 0: f.remaining = 0
        for f in self.flows: remaining += f.remaining
        if self.debug: self.logger.info("remaining: %s", remaining)

        """
        caculate `rates` to assign to `flows`
        attatch `meters` (rates) on flows
        """
        if remaining != 0:
            for f in self.flows:
                f.ratio = f.remaining / float(remaining)
                rate = f.ratio * self.throughput
                if self.debug: self.logger.info("!!!(%s,%s) - rate: %s!!!", f.source, f.destination, rate)
                if (f.source, f.destination) in self.matches:
                    match = self.matches[(f.source, f.destination)]
                    if (self.table_learning, match, 3) in self.meters:
                        mid = self.meters[(self.table_learning, match, 3)]
                        if self.debug: self.logger.info("[%s]: %s [%s]", f, rate, mid)
                        self.change_meter(datapath, mid, rate)

        """
        request `reading counter`
        """
        tid = self.table_learning
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

