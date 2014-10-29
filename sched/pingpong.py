from ryu import utils
from ryu.base import app_manager
from ryu.ofproto import ofproto_v1_3
from ryu.controller import ofp_event
from ryu.controller.handler import set_ev_cls
from ryu.controller.handler import HANDSHAKE_DISPATCHER, CONFIG_DISPATCHER, MAIN_DISPATCHER
from ryu.lib.packet import packet, ethernet, ipv4


class PINGPONG(app_manager.RyuApp):
    OFP_VERSIONS = [ofproto_v1_3.OFP_VERSION]

    def __init__(self, *args, **kwargs):
        super(PINGPONG, self).__init__(*args, **kwargs)

    def send_echo_request(self, datapath, data):
        protocol = datapath.ofproto
        parser = datapath.ofproto_parser
        request = parser.OFPEchoRequest(datapath, data)
        datapath.send_msg(request)


    @set_ev_cls(ofp_event.EventOFPSwitchFeatures, CONFIG_DISPATCHER)
    def switch_features_handler(self, event):
        self.logger.info("PINGPONG: Handler = Switch Features: enter!")
        datapath = event.msg.datapath
        self.send_echo_request(datapath, "Hello, this is Nesox pingpong app!")
        self.logger.info("PINGPONG: Handler = Switch Features: leave!")

    @set_ev_cls(ofp_event.EventOFPEchoRequest, [HANDSHAKE_DISPATCHER, CONFIG_DISPATCHER, MAIN_DISPATCHER])
    def echo_request_handler(self, event):
        self.logger.info('OFPEchoRequest received: data=%s', utils.hex_array(event.msg.data))
        datapath = event.msg.datapath
        data = event.msg.data
        self.send_echo_request(datapath, data)

    @set_ev_cls(ofp_event.EventOFPEchoReply, [HANDSHAKE_DISPATCHER, CONFIG_DISPATCHER, MAIN_DISPATCHER])
    def echo_reply_handler(self, event):
        self.logger.info('OFPEchoReply received: data=%s', event.msg.data)
        datapath = event.msg.datapath
        data = event.msg.data
        self.send_echo_request(datapath, data)
