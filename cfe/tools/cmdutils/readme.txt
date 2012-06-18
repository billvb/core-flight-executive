$Log: readme.txt  $
Revision 1.1 2008/04/17 08:08:44EDT ruperera 
Initial revision
Member added to project c:/MKSDATA/MKS-REPOSITORY/MKS-CFE-PROJECT/tools/cmdutils/project.pj
Revision 1.3 2006/11/07 14:33:16EST apcudmore 
Updated command utils with a byte swap switch.
Revision 1.2 2006/02/07 16:14:43EST rjmcgraw 
Changed some wording and added the log.

This directory contains files needed to build and run a program that allows
the user to send commands to the cFE.

The program must be built for the platform it is to be run on.  For example,
if the commands are to be sent from a mac, all the files in this directory
must reside in a single directory on the mac. Then the user must navigate to
that directory, and type 'make' at the command prompt. This will produce an
executable that will be used by the to-enable-tlm.sh file (or a custom *.sh
file).

The to-enable-tlm.sh is a script that sends a single command to the Telemetry
Output (TO) application.

The user can modify a copy of the to-enable-tlm.sh script to include the
desired commands to be sent.

To send the commands, just execute the script at the command prompt like this:
.../tools/cmdutils]$./to-enable-tlm.sh

The first parameter to sendUdpCmd is either BE or LE. This parameter is
designed to swap the bytes of the command when it is being sent from a little
endian host ( a PC ) to a big endian target ( a PPC or coldfire board ).
If you are sending a command from a PC to a PC, use BE ( dont swap )
If you are sending a command from a PPC Mac to a PPC Mac, PPC board, or
Coldfire board, use BE ( dont swap ).
If you are sending a command from an x86 Mac or PC to a PPC or Coldfire, use
LE ( swap bytes ).

------------

The "pytos.py" script is an example of a Command GUI that can be built to call
the sendUdpCmd program. pytos uses the PyGTK packages to create a simple
tabbed GUI to send commands.

To run use the ./commands.sh script or execute: python ./pytos.py
This script is known to work on recent Linux distributions.


