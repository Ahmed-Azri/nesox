Nesox: network resource scheduler leveraging SDN technique
=====


About
-----
This multi-purpose "README" file tends to providing a manual-like explaination on Nesox project:
(1) providing overview and background;
(2) illustrating operations in detail;


Introduction
------------

Nesox is first developed on OpenFlow 1.3 instance of HP2920 switch and Ryu controller framework.


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


**OpenFlow Configuration**
+ 1. Enable OpenFlow

	openflow
	openflow instance <instance-name>
	openflow instance <instance-name> enable

+ 2. Configure OpenFlow instances
+ 3. Configure OpenFlow instance members
+ 4. Set OpenFlow instance mode
+ 5. Set Flow location
+ 6. Configure software and hardware rate limiting
+ 7. Configure listener ports
+ 8. Configure controller IP and port
+ 9. Configure policy engine resources


Testbed Configuration
---------------------



Run Nesox Scheduler
-------------------
To run the (ryu-based, maybe nox-based in near future and/or others) nesox scheduler::

	% ryu-manager ryu-nesox.py

This command line will run up the nesox scheduler and print output on screen.





Reference
---------
[Ryu SDN Framework](http://osrg.github.io/ryu/)
[HP2920 Switch Series](http://h17007.www1.hp.com/us/en/networking/products/switches/HP_2920_Switch_Series/index.aspx)

