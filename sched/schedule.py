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
        self.soft_table_id = 200
        self.soft_table_no = 4
        self.flows = []
        self.transfermap = {}
        self.transfers = listdir(transdir)



    @set_ev_cls(ofp_event.EventOFPSwitchFeatures, CONFIG_DISPATCHER)
    def switch_features_handler(self, event):
        self.logger.info("SCHEDULE: Handler = Switch Features: enter!")

        """
        read the transfer requirement into a `list` and a `dictionary`
        """
        for tran in self.transfers: transfile = open(transdir + tran)
        for line in transfile: self.flows.append(nesox.flow(int(line[0]), int(line[2]), int(line[6:].rstrip()), int(line[4])))
        for flow in self.flows: self.transfermap[(flow.source, flow.destination)] = flow
        if (self.debug): self.logger.info("tramsfermap: %s", self.transfermap)

        """
        initialize the pipeline
        """
        datapath = event.msg.datapath
        protocol = datapath.ofproto
        prparser = datapath.ofproto_parser


        self.logger.info("SCHEDULE: Handler = Switch Features: leave!")

