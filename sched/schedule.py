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
        if (self.debug): self.logger.info("tramsfermap: %s", self.transfermap)

        """
        priority = 1
        initialize pipeline: (ANY) packet pass through the pipeline to controller
        """
        tid = self.table_start
        m = parser.OFPMatch()
        p = 1
        for gototid in self.soft_tables:
            self.insert_goto(datapath, tid, m, p, gototid)
            tid = gototid
        self.insert_controller(datapath, tid, m, p)

        self.request_flowstats(datapath, tid, m)

        self.logger.info("SCHEDULE [Handler = Switch Features]: leave!")

    @set_ev_cls(ofp_event.EventOFPPacketIn, MAIN_DISPATCHER)
    def handler_packetin(self, event):
        self.logger.info("SCHEDULE [Handler = Packet In]: enter! [%s]", self.packetin_counter)
        self.packetin_counter += 1

        self.logger.info("SCHEDULE [Handler = Packet In]: leave!")

    @set_ev_cls(ofp_event.EventOFPFlowStatsReply, MAIN_DISPATCHER)
    def handler_flowstats(self, event):
        self.logger.info("SCHEDULE [Handler = Flow Stats]: enter!")
        """
        initialize datapath
        """
        datapath = event.msg.datapath
        flowstat = event.msg.body
        parser = datapath.ofproto_parser

        counters = []
        for stat in flowstat:
            counters.append((stat.table_id, stat.priority, stat.match, stat.byte_count))
        if (self.debug): self.logger.info("counters: %s", counters)

        tip = 203
        m = parser.OFPMatch()
        self.request_flowstats(datapath, tid, m)
        sleep(1)

        self.logger.info("SCHEDULE [Handler = Flow Stats]: leave!")

