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
        self.table_start = 100
        self.table_terminate = 203
        self.hard_table_id = 100
        self.hard_table_no = 1
        self.hard_tables = [100]
        self.soft_table_id = 200
        self.soft_table_no = 4
        self.soft_tables = [200, 201, 202, 203]
        self.flows = []
        self.transfermap = {}
        self.transfers = listdir(transdir)
        self.datapath = None
        self.packetin_counter = 0
        self.monitor_on = True
        self.monitor_frequency = 1
        self.pipeline_mode = True


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
        self.logger.info("SCHEDULE [Handler = Switch Features]: enter!")

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
        """
        P = 2
        m = parser.OFPMatch(eth_type = 0x800, tcp_src=8478)
        t = self.table_start + 1
        gototid = self.table_terminate
        self.insert_goto(datapath, t, m, p, gototid)


        """
        create meters
        """


        """
        monitor flows
        """
        if self.monitor_on: self.request_flowstats(datapath, tid, m)


        """
        monitor meters
        """
        if self.monitor_on: self.request_meterfeature(datapath)
        if self.monitor_on: self.request_meterconfig(datapath)
        if self.monitor_on: self.request_meterstats(datapath)


        self.logger.info("SCHEDULE [Handler = Switch Features]: leave!")


    @set_ev_cls(ofp_event.EventOFPPacketIn, MAIN_DISPATCHER)
    def handler_packetin(self, event):
        self.logger.info("SCHEDULE [Handler = Packet In]: enter! [%s]", self.packetin_counter)
        self.packetin_counter += 1

        self.logger.info("SCHEDULE [Handler = Packet In]: leave!")


    @set_ev_cls(ofp_event.EventOFPFlowStatsReply, MAIN_DISPATCHER)
    def handler_flowstats(self, event):
        self.logger.info("SCHEDULE [Handler = Flow Stats]: enter!")
        datapath = event.msg.datapath
        flowstat = event.msg.body
        parser = datapath.ofproto_parser

        counters = []
        for stat in flowstat:
            if stat.priority == 1:
                counters.append((stat.table_id, stat.match, stat.priority, stat.packet_count, stat.byte_count))
        if self.debug: self.logger.info("counters: %s", counters)

        if not self.monitor_on: return
        tid = self.table_terminate
        m = parser.OFPMatch()
        sleep(self.monitor_frequency)
        self.request_flowstats(datapath, tid, m)

        self.logger.info("SCHEDULE [Handler = Flow Stats]: leave!")


    @set_ev_cls(ofp_event.EventOFPMeterFeaturesStatsReply, MAIN_DISPATCHER)
    def handler_meterfeature(self, event):
        self.logger.info("SCHEDULE [Handler = Meter Features]: enter!")
        datapath = event.msg.datapath
        features = event.msg.body

        meterfeatures = []
        for feature in features:
            meterfeatures.append((feature.max_meter, feature.band_types, feature.max_band, feature.capabilities))
        if self.debug: self.logger.info("meter features: %s", meterfeatures)

        self.logger.info("SCHEDULE [Handler = Meter Features]: leave!")


    @set_ev_cls(ofp_event.EventOFPMeterConfigStatsReply, MAIN_DISPATCHER)
    def handler_meterconfig(self, event):
        self.logger.info("SCHEDULE [Handler = Meter Configurations]: enter!")
        datapath = event.msg.datapath
        configs = event.msg.body

        meterconfigs = []
        for config in configs:
            meterconfigs.append((config.meter_id, config.bands, config.flags, config.length))
        if self.debug: self.logger.info("meter configs: %s", meterconfigs)

        self.logger.info("SCHEDULE [Handler = Meter Configurations]: leave!")


    @set_ev_cls(ofp_event.EventOFPMeterStatsReply, MAIN_DISPATCHER)
    def handler_meterstats(self, event):
        self.logger.info("SCHEDULE [Handler = Meter Stats]: enter!")
        datapath = event.msg.datapath
        stats = event.msg.body

        meterstats = []
        for stat in stats:
            meterstats.append((stat.meter_id, stat.flow_count, stat.packet_in_count, stat.byte_in_count, stat.band_stats))

        if self.debug: self.logger.info("meter configs: %s", meterstats)

        if self.monitor_on: self.request_meterstats(datapath)
        self.logger.info("SCHEDULE [Handler = Meter Stats]: leave!")

