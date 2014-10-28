from ryu import utils
from ryu.base import app_manager
from ryu.ofproto import ofproto_v1_3
from ryu.controller.handler import set_ev_cls
from ryu.controller.handler import HANDSHAKE_DISPATCHER, CONFIG_DISPATCHER, MAIN_DISPATCHER
from ryu.controller import ofp_event


class MACLEARNER(app_manager.RyuApp):
    OFP_VERSIONS = [ofproto_v1_3.OFP_VERSION]

    def __init__(self, *args, **kwargs):
        super(MACLEARNER, self).__init__(*args, **kwargs)

    def send_echo_request(self, datapath, data):
        ofp = datapath.ofproto
        ofp_parser = datapath.ofproto_parser
        req = ofp_parser.OFPEchoRequest(datapath, data)
        datapath.send_msg(req)

    @set_ev_cls(ofp_event.EventOFPEchoRequest, [HANDSHAKE_DISPATCHER, CONFIG_DISPATCHER, MAIN_DISPATCHER])
    def echo_request_handler(self, ev):
        self.logger.debug('OFPEchoRequest received: data=%s', utils.hex_array(ev.msg.data))

