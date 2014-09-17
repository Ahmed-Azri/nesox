Nesox: network resource scheduler leveraging SDN technique
=====


About
-----
This multi-purpose "README" file tends to providing a manual-like explaination on Nesox project:
(1) providing overview and background;
(2) illustrating operations in detail;


Introduction
------------

Nesox is first developed on OpenFlow 1.3 instance of [HP2920 switch][3] and [Ryu controller framework][2].


Choose a Controller Framework
-----------------------------
Nox controller is currently not supporting OF-v1.3.x, which is actually quite a huge amount of work.
It is a good choice based on which to implement Nesox if one is written in C/C++ whose talent is system (level) programming.
However, nowadays, high-level languages are also used in prototype system software (e.g., distributed systems),
such as Spark in Scala, Jane Street using OCaml, Goldman Sach using Erlang, etc.

Ryu controller framework is relatively a slow runtime, however, its support of growing OpenFlow protocols is nearly the best in open source community.
OpenFlow v1.3.1 is supported by Ryu in the following file "/ryu/ryu/ofproto/ofproto_v1_3_parser.py", 5908 lines of code reflecting its amount of work,
(actually a translation from ONF .c file to .py file fitted into its own framework).

> Currently, Nesox only runs on Ryu.


HP2920 Switch
-------------

### First Things First


**How to login the switch**

[root@debian0][140917111817][/root]ssh manager@switch

We'd like to keep you up to date about:
  * Software feature updates
  * New product announcements
  * Special events

Please register your products now at:  www.hp.com/networking/register


manager@switch's password:

> HP J9726A 2920-24G Switch
> Software revision **WB.15.14.0007**

Copyright (C) 1991-2014 Hewlett-Packard Development Company, L.P.


                   RESTRICTED RIGHTS LEGEND

	Confidential computer software.  Valid license from HP required for possession,
	use or copying. Consistent with FAR 12.211 and 12.212, Commercial Computer
	Software, Computer Software Documentation, and Technical Data for Commercial
	Items are licensed to the U.S. Government under vendor's standard commercial
	license.
                   HEWLETT-PACKARD DEVELOPMENT COMPANY, L.P.
                   20555 State Highway 249, Houston, TX 77070

Press any key to continue


**OpenFlow v1.3 Support**

Since **K/KA/WB 15.14**, HP Added OpenFlow v1.3 support.
The Switch Software Version is now updated to **WB.15.14.0007** from factory setup.
This update step is ignored here, interested readers could retrieve help from HP website.



**Important OpenFlow features supported by HP2920**
* Multiple Flow tables: **Pipeline processing**
* Auxiliary connections
* Multiple controllers
* Rate-limiting the amount of OpenFlow traffic sent to the controller

**Unsupported OpenFlow features**
* Set-Queue action


### OpenFlow Configuration

A number of configurations of the switch affect the OpenFlow behaviour.
What listed bellow are some important ones, others which are not directly related to the **OpenFlow** are not included.
Please refer to the [HP Switch Website][3] for the readers' own interests.

+ 1. Enable OpenFlow
```Bash
openflow
openflow instance instance-name
openflow instance instance-name enable
```

+ 2. Configure OpenFlow instances
```Bash
openflow instance { instance-name | aggregate } [ enable | disable ]
```

+ 3. Configure OpenFlow instance members
```Bash
openflow instance instance-name member vlan vlan-id
```

+ 4. Set OpenFlow instance mode
```Bash
openflow instance { instance-name | aggregate } mode { active | passive }
```

+ 5. Set Flow location
```Bash
openflow instance { instance-name | aggregate } flow-location hardware-only
```
> An error is returned to the controller if the flow cannot be added in hardware and the flow-location is set as hardware-only.


+ 6. Configure software and hardware rate limiting
```Bash
openflow instance { instance-name | aggregate } limit { hardware-rate kbps | software-rate pps }
```
> kbps: Default: 0; Range: 0 — 10,000,000;
> pps: Limits the number of packets per second per module that this instance can send to the software path. Default: 100 ppsRange: 1 — 10,000 pps;
> Note: Increasing the software rate limit increases CPU consumption and may impact system performance.


+ 7. Configure listener ports: listen for incoming connections from an OpenFlow controller
```Bash
openflow instance { instance-name | aggregate } listen-port [tcp-port] [oobm]
```

+ 8. Configure controller IP and port
```Bash
openflow controller-id <id> [ip <ip-address>] [port <tcp-port>]
controller-interface { vlan <vlan-id> | oobm }
openflow instance { instance-name | aggregate } controller-id controller-ID
```
> A VLAN that is a member of an OpenFlow instance cannot be added as an OpenFlow controller interface.
> When an OpenFlow controller is associated with an OpenFlow instance it cannot be deleted.

+ 9. OpenFlow Monitoring
```Bash
show openflow resources
show openflow controllers
show openflow flow-table
show openflow instance instance-name
show openflow instance instance-name flows
show openflow instance instance-name meters
show openflow instance instance-name limiters
show openflow instance instance-name port-statistics
show openflow instance instance-name message-statistics
show openflow instance instance-name capacities
```

+ 10. **Configuring number of software flow tables per instance**
```Bash
openflow-instance-name# software-flow-table value
```
> Default: 1, Range: 1–4
> Only applicable for an OpenFlow version 1.3 instance.

+ 11. Setting maximum backoff interval for an instance
```Bash
openflow instance { instance-name | aggregate } max-backoff-interval secs
```
> Default: 60 secondsRange: 1 — 3600 seconds

+ 12. Configuring IP Control Table Mode
```Bash
openflow# ip-control-table-mode
```
> Include IP control table in the OpenFlow packet processing pipeline. Default disabled.

+ 13. Hardware statistics refresh rate
```Bash
openflow-instance-name# hardware statistics | refresh rate policy-engine-table <value>
```
> Refresh rate for policy engine table statistics. Default: 0–3600.

+ 14. Troubleshooting OpenFlow
```Bash
debug openflow <errors|events|instance|packets>
```


Testbed Configuration
---------------------
### The HP2920 Switch

	HP-2920-24G(config)# show system

 	Status and Counters - General System Information

	  System Name        : HP-2920-24G
	  System Contact     :
	  System Location    :

	  MAC Age Time (sec) : 300

	  Time Zone          : 8
	  Daylight Time Rule : None

	  Software revision  : WB.15.14.0007        Base MAC Addr      : c4346b-90c4c0
	  ROM Version        : WB.15.05             Serial Number      : SG45FLWXVL

	  Up Time            : 47 days              Memory   - Total   : 170,250,752
	  CPU Util (%)       : 2                               Free    : 85,696,996

	  IP Mgmt  - Pkts Rx : 317,248              Packet   - Total   : 6750
	             Pkts Tx : 193,942              Buffers    Free    : 5009
	                                                       Lowest  : 4951
	                                                       Missed  : 0






Run Nesox Scheduler
-------------------
To run the (ryu-based, maybe nox-based in near future and/or others) nesox scheduler::

	% ryu-manager ryu-nesox.py

This command line will run up the nesox scheduler and print output on screen.




Reference
---------
[Markdown Syntax][1]
[Ryu SDN Framework][2]
[HP2920 Switch Series][3]

[1]: http://daringfireball.net/projects/markdown/syntax "Markdown"
[2]: http://osrg.github.io/ryu/ "Ryu SDN Framework"
[3]: http://h17007.www1.hp.com/us/en/networking/products/switches/HP_2920_Switch_Series/index.aspx "HP2920 Switch Series"

