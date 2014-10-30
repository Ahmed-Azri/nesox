Nesox Scheduling Module
======================

**Nesox scheduling module is actually a set of Ryu application implemented scheduling algorithms.**

For each functions, develop an app, then in the paper, for each app, there can be a paragraph.
But the precondition is that multiple apps can work cooperately in ryu-manager app-list.

It is OK to run 2 applications at the same time!!!



event -> message -> datapath -> protocol ->
							 -> parser
				 -> data => packet -> protocols -> ethernet
				 								-> ipv4


MeterFeaturesStats: ['max_meter=126 band_types=0x00000003 capabilities=0x0000000b max_band=2 max_color=8']
MeterStats: ['meter_id=0x00000005 len=56 flow_count=0 packet_in_count=0 byte_in_count=0 duration_sec=3661188478 duration_ns
ec=3379967296 band_stats=[OFPMeterBandStats(byte_band_count=0,packet_band_count=0)]', 'meter_id=0x00000001 len=56 flow_coun
t=0 packet_in_count=0 byte_in_count=0 duration_sec=3661188478 duration_nsec=3383967296 band_stats=[OFPMeterBandStats(byte_b
and_count=0,packet_band_count=0)]', 'meter_id=0x00000002 len=56 flow_count=0 packet_in_count=0 byte_in_count=0 duration_sec
=3661188478 duration_nsec=3379967296 band_stats=[OFPMeterBandStats(byte_band_count=0,packet_band_count=0)]']

MeterConfigStats: ['length=24 flags=0x0001 meter_id=0x00000005 bands=[OFPMeterBandDscpRemark(burst_size=0,len=16,prec_level
=0,rate=55,type=2)]', 'length=24 flags=0x0001 meter_id=0x00000001 bands=[OFPMeterBandDrop(burst_size=0,len=16,rate=0,type=1
)]', 'length=24 flags=0x0001 meter_id=0x00000002 bands=[OFPMeterBandDrop(burst_size=0,len=16,rate=22,type=1)]']

