$Log: readme.txt  $
Revision 1.1 2008/04/17 08:08:52EDT ruperera 
Initial revision
Member added to project c:/MKSDATA/MKS-REPOSITORY/MKS-CFE-PROJECT/tools/tlmutils/project.pj
Revision 1.3 2006/02/07 16:37:51EST rjmcgraw 
Added close program instructions

The python script (packet-recv.py) in this directory allows the user to view
telemetry packets sent by the Telemetry Output (TO) application.

To run the script, python must be installed on the machine that will be used
for viewing the packets.

To run the script type the following command at the command prompt:
.../tlmutils]$python packet-recv.py

To close the program, Ctl-C must be entered twice.

Be sure the TO application has received a command that has the IP address of
the machine running this script. Simply running the cFE will produce
housekeeping telemetry packets.
