from ryu import utils
from ryu.base import app_manager
from ryu.ofproto import ofproto_v1_3
from ryu.controller import ofp_event
from ryu.controller.handler import set_ev_cls
from ryu.controller.handler import HANDSHAKE_DISPATCHER, CONFIG_DISPATCHER, MAIN_DISPATCHER
from ryu.lib.packet import packet, ethernet, ipv4


class METER(app_manager.RyuApp):
    OFP_VERSIONS = [ofproto_v1_3.OFP_VERSION]

    def __init__(self, *args, **kwargs):
        super(METER, self).__init__(*args, **kwargs)

    def send_meter_features_stats_request(self, datapath):
        parser = datapath.ofproto_parser
        request = parser.OFPMeterFeaturesStatsRequest(datapath, 0)
        datapath.send_msg(request)

    def send_meter_config_stats_request(self, datapath):
        ofp = datapath.ofproto
        ofp_parser = datapath.ofproto_parser
        req = ofp_parser.OFPMeterConfigStatsRequest(datapath, 0, ofp.OFPM_ALL)
        datapath.send_msg(req)

    def send_meter_stats_request(self, datapath):
        protocol = datapath.ofproto
        parser = datapath.ofproto_parser
        request = parser.OFPMeterStatsRequest(datapath, 0, protocol.OFPM_ALL)
        datapath.send_msg(request)

    """
    meteradd is used for testing purpose
    """
    def meteradd(self, datapath, meter_id):
        protocol = datapath.ofproto
        parser = datapath.ofproto_parser
        bands = [parser.OFPMeterBandDrop()]
        # bands = [parser.OFPMeterBandDscpRemark()]
        """
        The following line is not OK!
        """
        # bands = [parser.OFPMeterBandExperimenter()]
        modification = parser.OFPMeterMod(datapath=datapath, meter_id=meter_id, bands=bands)
        datapath.send_msg(modification)

    def insertdropmeterbps(self, datapath, meter_id, rate):
        protocol = datapath.ofproto
        parser = datapath.ofproto_parser
        bands = [parser.OFPMeterBandDrop(rate=rate)]
        modification = parser.OFPMeterMod(datapath=datapath, flags=protocol.OFPMF_KBPS, meter_id=meter_id, bands=bands)
        datapath.send_msg(modification)

    def insertdropmeterpps(self, datapath, meter_id, rate):
        protocol = datapath.ofproto
        parser = datapath.ofproto_parser
        bands = [parser.OFPMeterBandDrop(rate=rate)]
        modification = parser.OFPMeterMod(datapath=datapath, flags=protocol.OFPMF_PKTPS, meter_id=meter_id, bands=bands)
        datapath.send_msg(modification)

    def changedropmeter(self, datapath, meter_id, rate):
        protocol = datapath.ofproto
        parser = datapath.ofproto_parser
        bands = [parser.OFPMeterBandDrop(rate=rate)]
        modification = parser.OFPMeterMod(datapath=datapath, command=protocol.OFPMC_MODIFY, meter_id=meter_id, bands=bands)
        datapath.send_msg(modification)

    def deletedropmeter(self, datapath, meter_id):
        protocol = datapath.ofproto
        parser = datapath.ofproto_parser
        modification = parser.OFPMeterMod(datapath=datapath, command=protocol.OFPMC_DELETE, meter_id=meter_id)
        datapath.send_msg(modification)


    def insertmarkmeterbps(self, datapath, meter_id, rate):
        protocol = datapath.ofproto
        parser = datapath.ofproto_parser
        bands = [parser.OFPMeterBandDscpRemark(rate=rate)]
        modification = parser.OFPMeterMod(datapath=datapath, flags=protocol.OFPMF_KBPS, meter_id=meter_id, bands=bands)
        datapath.send_msg(modification)

    def insertmarkmeterpps(self, datapath, meter_id, rate):
        protocol = datapath.ofproto
        parser = datapath.ofproto_parser
        bands = [parser.OFPMeterBandDscpRemark(rate=rate)]
        modification = parser.OFPMeterMod(datapath=datapath, flags=protocol.OFPMF_PKTPS, meter_id=meter_id, bands=bands)
        datapath.send_msg(modification)

    def changemarkmeter(self, datapath, meter_id, rate):
        protocol = datapath.ofproto
        parser = datapath.ofproto_parser
        bands = [parser.OFPMeterBandDscpRemark(rate=rate)]
        modification = parser.OFPMeterMod(datapath=datapath, command=protocol.OFPMC_MODIFY, meter_id=meter_id, bands=bands)
        datapath.send_msg(modification)

    def deletemarkmeter(self, datapath, meter_id):
        protocol = datapath.ofproto
        parser = datapath.ofproto_parser
        modification = parser.OFPMeterMod(datapath=datapath, command=protocol.OFPMC_DELETE, meter_id=meter_id)
        datapath.send_msg(modification)


    @set_ev_cls(ofp_event.EventOFPSwitchFeatures, CONFIG_DISPATCHER)
    def switch_features_handler(self, event):
        self.logger.info("METER: Handler = Switch Features: enter!")
        datapath = event.msg.datapath
        self.send_meter_stats_request(datapath)
        self.send_meter_config_stats_request(datapath)
        self.send_meter_features_stats_request(datapath)

        self.meteradd(datapath, 1)
        self.insertdropmeterbps(datapath, 2, 2)
        self.insertdropmeterpps(datapath, 3, 3)
        self.insertmarkmeterbps(datapath, 4, 4)
        self.insertmarkmeterpps(datapath, 5, 5)

        self.changedropmeter(datapath, 2, 22)
        self.changemarkmeter(datapath, 5, 55)

        self.deletedropmeter(datapath, 3)
        self.deletemarkmeter(datapath, 4)

        self.logger.info("METER: Handler = Switch Features: leave!")


    @set_ev_cls(ofp_event.EventOFPMeterStatsReply, MAIN_DISPATCHER)
    def meter_stats_reply_handler(self, ev):
        datapath = ev.msg.datapath
        meters = []
        for stat in ev.msg.body:
            meters.append('meter_id=0x%08x len=%d flow_count=%d '
                          'packet_in_count=%d byte_in_count=%d '
                          'duration_sec=%d duration_nsec=%d '
                          'band_stats=%s' %
                          (stat.meter_id, stat.len, stat.flow_count,
                           stat.packet_in_count, stat.byte_in_count,
                           stat.duration_sec, stat.duration_nsec,
                           stat.band_stats))
        self.logger.info('MeterStats: %s', meters)
        self.send_meter_stats_request(datapath)

    @set_ev_cls(ofp_event.EventOFPMeterConfigStatsReply, MAIN_DISPATCHER)
    def meter_config_stats_reply_handler(self, ev):
        datapath = ev.msg.datapath
        configs = []
        for stat in ev.msg.body:
            configs.append('length=%d flags=0x%04x meter_id=0x%08x '
                           'bands=%s' %
                           (stat.length, stat.flags, stat.meter_id,
                            stat.bands))
        self.logger.info('MeterConfigStats: %s', configs)
        self.send_meter_config_stats_request(datapath)

    @set_ev_cls(ofp_event.EventOFPMeterFeaturesStatsReply, MAIN_DISPATCHER)
    def meter_features_stats_reply_handler(self, ev):
        datapath = ev.msg.datapath
        features = []
        for stat in ev.msg.body:
            features.append('max_meter=%d band_types=0x%08x '
                            'capabilities=0x%08x max_band=%d '
                            'max_color=%d' %
                            (stat.max_meter, stat.band_types,
                             stat.capabilities, stat.max_band,
                             stat.max_color))
        self.logger.info('MeterFeaturesStats: %s', features)
        self.send_meter_features_stats_request(datapath)


