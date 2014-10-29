from ryu import utils
from ryu.base import app_manager
from ryu.ofproto import ofproto_v1_3
from ryu.controller import ofp_event
from ryu.controller.handler import set_ev_cls
from ryu.controller.handler import HANDSHAKE_DISPATCHER, CONFIG_DISPATCHER, MAIN_DISPATCHER
from ryu.lib.packet import packet, ethernet, ipv4


class DISPATCHER(app_manager.RyuApp):
    OFP_VERSIONS = [ofproto_v1_3.OFP_VERSION]

    def __init__(self, *args, **kwargs):
        super(DISPATCHER, self).__init__(*args, **kwargs)

    """
    this insertflow `apply` the actions but OFP also support `write` actions:
        OFPIT_WRITE_ACTIONS
    """

    def insertflow(self, datapath, table_id, priority, match, actions):
        protocol = datapath.ofproto
        parser = datapath.ofproto_parser
        instruction = [parser.OFPInstructionActions(protocol.OFPIT_APPLY_ACTIONS, actions)]
        modification = parser.OFPFlowMod(datapath=datapath,
            table_id=table_id, priority=priority, match=match, instructions=instruction)
        datapath.send_msg(modification)

    def insertgoto(self, datapath, table_id, priority, match, goto_tid):
        protocol = datapath.ofproto
        parser = datapath.ofproto_parser
        instruction = [parser.OFPInstructionGotoTable(goto_tid)]
        modification = parser.OFPFlowMod(datapath=datapath,
            table_id=table_id, priority=priority, match=match, instructions=instruction)
        datapath.send_msg(modification)

    @set_ev_cls(ofp_event.EventOFPSwitchFeatures, CONFIG_DISPATCHER)
    def switch_features_handler(self, event):
        self.logger.info("DISPATCHER: Handler = Switch Features: enter!")
        message = event.msg
        datapath = message.datapath
        protocol = datapath.ofproto
        parser = datapath.ofproto_parser

        # match = parser.OFPMatch()
        # self.insertgoto(datapath, 200, 0, match, 203)

        match = parser.OFPMatch(tcp_src=8478)
        if 'tcp_src' in match:
            print match['tcp_src']
        self.insertgoto(datapath, 200, 1, match, 201)

        # nodes = [8171, 8172, 8173, 8174]
        # for node in nodes:
        #     match = parser.OFPMatch(tcp_src=node)
        #     self.insertgoto(datapath, 200, 1, match, 201)
        #     print match

        self.logger.info("DISPATCHER: Handler = Switch Features: leave!")
