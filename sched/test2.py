from ryu import utils
from ryu.base import app_manager
from ryu.ofproto import ofproto_v1_3
from ryu.controller import ofp_event
from ryu.controller.handler import set_ev_cls
from ryu.controller.handler import HANDSHAKE_DISPATCHER, CONFIG_DISPATCHER, MAIN_DISPATCHER
from ryu.lib.packet import packet, ethernet, ipv4

from os import listdir
from nesox import flow
from nesox import transdir

class TEST200(app_manager.RyuApp):
    OFP_VERSIONS = [ofproto_v1_3.OFP_VERSION]

    def __init__(self, *args, **kwargs):
        super(TEST200, self).__init__(*args, **kwargs)

    @set_ev_cls(ofp_event.EventOFPSwitchFeatures, CONFIG_DISPATCHER)
    def switch_features_handler(self, event):
        self.logger.info("TEST200: Handler = Switch Features: enter!")

        flows = []
        transfermap = {}
        transfers = listdir(transdir)

        for tran in transfers: transfile = open(transdir + tran)
        for line in transfile: flows.append(flow(int(line[0]), int(line[2]), int(line[6:].rstrip()), int(line[4])))
        for flow in flows: transfermap[(flow.source, flow.destination)] = flow

        transfermap[(1,2)].show(1)

        self.logger.info("TEST200: Handler = Switch Features: leave!")

