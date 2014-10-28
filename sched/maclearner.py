from ryu import utils
from ryu.base import app_manager
from ryu.ofproto import ofproto_v1_3
from ryu.controller import ofp_event
from ryu.controller.handler import set_ev_cls
from ryu.controller.handler import HANDSHAKE_DISPATCHER, CONFIG_DISPATCHER, MAIN_DISPATCHER
from ryu.lib.packet import packet, ethernet, ipv4


class MACLEARNER(app_manager.RyuApp):
    OFP_VERSIONS = [ofproto_v1_3.OFP_VERSION]

    def __init__(self, *args, **kwargs):
        super(MACLEARNER, self).__init__(*args, **kwargs)
        self.mac_to_port = {}

    """
    todo: these basic flow operation could be moved to a `base` .py file
    """
    def insertflow(self, datapath, table_id, priority, match, actions):
        protocol = datapath.ofproto
        parser = datapath.ofproto_parser
        instruction = [parser.OFPInstructionActions(protocol.OFPIT_APPLY_ACTIONS, actions)]
        modification = parser.OFPFlowMod(datapath=datapath,
            table_id=table_id, priority=priority, match=match, instructions=instruction)
        datapath.send_msg(modification)

    @set_ev_cls(ofp_event.EventOFPSwitchFeatures, CONFIG_DISPATCHER)
    def switch_features_handler(self, ev):
        self.logger.info("Handler = Switch Basic Features: enter!")
        datapath = ev.msg.datapath
        ofproto = datapath.ofproto
        parser = datapath.ofproto_parser

        message = ev.msg
        version = message.version
        msgtype = message.msg_type
        msgsize = message.msg_len
        mbuffer = message.buf
        tranxid = message.xid

        self.logger.info("Negotiated OpenFlow Version: %s", version)
        self.logger.info("Transaction ID: %s", tranxid)
        self.logger.info("Message Type: [%s]", msgtype)
        self.logger.info("Message Size: [%s]", msgsize)


        datapath = message.datapath
        dpid = datapath.id
        address = datapath.address
        ports = datapath.ports
        dpxid = datapath.xid

        self.logger.info("Datapath ID: %s", dpid)
        self.logger.info("Transaction ID: %s", dpxid)
        self.logger.info("Address: %s", address)
        self.logger.info("Ports: %s", ports)

        protocol = datapath.ofproto
        prparser = datapath.ofproto_parser

        match = prparser.OFPMatch()

        match = parser.OFPMatch()
        actions = [parser.OFPActionOutput(ofproto.OFPP_CONTROLLER, ofproto.OFPCML_NO_BUFFER)]

        self.insertflow(datapath, 100, 0, match, actions)
        self.insertflow(datapath, 200, 0, match, actions)
        self.insertflow(datapath, 201, 0, match, actions)
        self.insertflow(datapath, 202, 0, match, actions)
        self.insertflow(datapath, 203, 0, match, actions)

        self.logger.info("Handler = Switch Basic Features: enter!")

    @set_ev_cls(ofp_event.EventOFPPacketIn, MAIN_DISPATCHER)
    def packet_in_handler(self, ev):
        self.logger.info("Handler = Packet In: enter!")
        msg = ev.msg
        datapath = msg.datapath
        ofproto = datapath.ofproto
        parser = datapath.ofproto_parser
        in_port = msg.match['in_port']

        pkt = packet.Packet(msg.data)
        eth = pkt.get_protocols(ethernet.ethernet)[0]
        ip4 = pkt.get_protocol(ipv4.ipv4)

        dst = eth.dst
        src = eth.src

        dpid = datapath.id
        self.mac_to_port.setdefault(dpid, {})

        # self.logger.info("packet in %s %s %s %s", dpid, src, dst, in_port)
        if ip4 is not None:
            self.logger.info("PacketIN(%s):%s->%s:%s)", dpid, ip4.src, ip4.dst, in_port)

        """
        learn a mac address to avoid FLOOD next time.
        """
        self.mac_to_port[dpid][src] = in_port

        if dst in self.mac_to_port[dpid]:
            out_port = self.mac_to_port[dpid][dst]
        else:
            out_port = ofproto.OFPP_FLOOD

        self.logger.info("mac to port mapping: ", self.mac_to_port)

        actions = [parser.OFPActionOutput(out_port)]

        """
        install a flow to avoid packet_in next time
        """

        if out_port != ofproto.OFPP_FLOOD:
            match = parser.OFPMatch(in_port=in_port, eth_dst=dst)
            self.insertflow(datapath, 200, 1, match, actions)

        data = None
        if msg.buffer_id == ofproto.OFP_NO_BUFFER:
            data = msg.data

        out = parser.OFPPacketOut(datapath=datapath, buffer_id=msg.buffer_id, in_port=in_port, actions=actions, data=data)
        datapath.send_msg(out)

        self.logger.info("Handler = Packet In: enter!")

