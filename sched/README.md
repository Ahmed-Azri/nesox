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

