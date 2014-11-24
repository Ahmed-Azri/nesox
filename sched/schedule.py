from ryu import utils
from ryu.base import app_manager
from ryu.ofproto import ofproto_v1_3
from ryu.controller import ofp_event
from ryu.controller.handler import set_ev_cls
from ryu.controller.handler import HANDSHAKE_DISPATCHER, CONFIG_DISPATCHER, MAIN_DISPATCHER
from ryu.lib.packet import packet, ethernet, ipv4

from os import listdir

import nesox
from nesox import flow
from nesox import transdir

class SCHEDULE(app_manager.RyuApp):
    OFP_VERSIONS = [ofproto_v1_3.OFP_VERSION]

    def __init__(self, *args, **kwargs):
        super(SCHEDULE, self).__init__(*args, **kwargs)
        self.debug = 1
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

    def insert_output(self, datapath, tid, match, pri, port):
        protocol = datapath.ofproto
        parser = datapath.ofproto_parser
        actions = [parser.OFPActionOutput(port)]
        instructions = [parser.OFPInstructionActions(protocol.OFPIT_APPLY_ACTIONS, action)]
        modification = parser.OFPFlowMod(datapath=datapath, table_id=tid, match=match, priority=pri, instructions=instructions)
        datapath.send_msg(modification)

    def insert_goto(self, datapath, tid, match, pri, gototid):
        protocol = datapath.ofproto
        parser = datapath.ofproto_parser
        instructions = [parser.OFPInstructionGotoTable(gotoid)]
        modification = parser.OFPFlowMod(datapath=datapath, table_id=tid, match=match, priority=pri, instructions=instructions)
        datapath.send_msg(modification)


    @set_ev_cls(ofp_event.EventOFPSwitchFeatures, CONFIG_DISPATCHER)
    def switch_features_handler(self, event):
        self.logger.info("SCHEDULE: Handler = Switch Features: enter!")

        """
        initialize datapath
        """
        datapath = event.msg.datapath
        self.datapath = datapath

        """
        read `transfer` requirement into a `list` and a `dictionary`
        """
        for tran in self.transfers: transfile = open(transdir + tran)
        for line in transfile: self.flows.append(nesox.flow(int(line[0]), int(line[2]), int(line[6:].rstrip()), int(line[4])))
        for flow in self.flows: self.transfermap[(flow.source, flow.destination)] = flow
        if (self.debug): self.logger.info("tramsfermap: %s", self.transfermap)

        """
        initialize pipeline
        """
        tid = 100
        for gototid in self.soft_tables:
            m = parser.OFPMatch()
            p = 4
            self.insert_goto(datapath, tid, m, p, gototid)
            tid = gototid

        self.logger.info("SCHEDULE: Handler = Switch Features: leave!")



