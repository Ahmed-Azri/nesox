from ryu import utils
from ryu.base import app_manager
from ryu.ofproto import ofproto_v1_3
from ryu.controller import ofp_event
from ryu.controller.handler import set_ev_cls
from ryu.controller.handler import HANDSHAKE_DISPATCHER, CONFIG_DISPATCHER, MAIN_DISPATCHER
from ryu.lib.packet import packet, ethernet, ipv4

import time

class RATEALLOCATOR(app_manager.RyuApp):
    OFP_VERSIONS = [ofproto_v1_3.OFP_VERSION]

    def __init__(self, *args, **kwargs):
        super(RATEALLOCATOR, self).__init__(*args, **kwargs)
        self.estimateuperbound = 102400
        self.frequency = 1

    def gettransferset(self):
        self.logger.info("get transfer set!")

    def gettransfer(self):
        self.logger.info("get transfer!")
        """
        todo: get the transfer info
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

    def insertdropmeterbps(self, datapath, meter_id, rate):
        protocol = datapath.ofproto
        parser = datapath.ofproto_parser
        bands = [parser.OFPMeterBandDrop(rate=rate)]
        modification = parser.OFPMeterMod(datapath=datapath, flags=protocol.OFPMF_KBPS, meter_id=meter_id, bands=bands)
        datapath.send_msg(modification)

    def changedropmeter(self, datapath, meter_id, rate):
        protocol = datapath.ofproto
        parser = datapath.ofproto_parser
        bands = [parser.OFPMeterBandDrop(rate=rate)]
        modification = parser.OFPMeterMod(datapath=datapath, command=protocol.OFPMC_MODIFY, meter_id=meter_id, bands=bands)
        datapath.send_msg(modification)

    def attachmeter(self, datapath, table_id, match, meter_id):
        protocol = datapath.ofproto
        parser = datapath.ofproto_parser
        instruction = [parser.OFPInstructionMeter(meter_id=meter_id)]
        modification = parser.OFPFlowMod(datapath=datapath,
            table_id=table_id, command=protocol.OFPFC_ADD, match=match, instructions=instruction)
        datapath.send_msg(modification)

    def send_flow_stats_request(self, datapath, match, table_id):
        ofp = datapath.ofproto
        ofp_parser = datapath.ofproto_parser
        cookie = cookie_mask = 0
        req = ofp_parser.OFPFlowStatsRequest(datapath, 0, ofp.table_id, ofp.OFPP_ANY, ofp.OFPG_ANY, cookie, cookie_mask, match)
        datapath.send_msg(req)


    @set_ev_cls(ofp_event.EventOFPSwitchFeatures, CONFIG_DISPATCHER)
    def switch_features_handler(self, event):
        self.logger.info("RATEALLOCATOR: Handler = Switch Features: enter!")
        message = event.msg
        datapath = message.datapath
        protocol = datapath.ofproto
        parser = datapath.ofproto_parser


        match = parser.OFPMatch()
        self.insertgoto(datapath, 202, 1, match, 203)
        self.send_flow_stats_request(datapath, match, 203)

        self.logger.info("RATEALLOCATOR: Handler = Switch Features: leave!")


    @set_ev_cls(ofp_event.EventOFPFlowStatsReply, MAIN_DISPATCHER)
    def flow_stats_reply_handler(self, ev):
        datapath = ev.msg.datapath
        flows = []
        totalbytes = 0
        totalpackets = 0
        meterid = 1
        for stat in ev.msg.body:
            packetcounter = stat.packet_count
            bytecounter = stat.byte_count
            totalpackets = totalpackets + packetcounter
            totalbytes = totalbytes + packetcounter

        for stat in ev.msg.body:
            packetcounter = stat.packet_count
            bytecounter = stat.byte_count
            match = stat.match
            tableid = stat.table_id

            ratio_bytes = float(bytecounter) / float(totalbytes)
            ratio_pckts = float(packetcounter) / float(totalpackets)

            rate = ratio_bytes * float(estimateuperbound)

            insertdropmeterbps(datapath, datapath, meterid, rate)
            attachmeter(datapath, tableid, match, meterid)

        time.sleep(self.frequency)

        if not flows:
            self.logger.info('flows is empty!')
