###############################################################################
# File: makefile
#
# Purpose:
#   Main makefile for Mission build and all apps
#
# History:
#
###############################################################################
##
## Top Level rules:
## If you want to build the testapps every time, just add the testapps targets
## to the primary targets below ( or uncomment the testapps versions ) 
## 
export MISSIONNAME:=Mission

##############################################################################
## Build rules

all ::
	$(MAKE) -C pc-linux all
#	$(MAKE) -C mcp750-vxworks6 all
	
clean ::
	$(MAKE) -C pc-linux clean
#	$(MAKE) -C mcp750-vxworks6 clean

install ::
	$(MAKE) -C pc-linux install
#	$(MAKE) -C mcp750-vxworks6 install
	
depend ::
	$(MAKE) -C pc-linux depend
#	$(MAKE) -C mcp750-vxworks6 depend
		
#-------------------------------------------------------------------------------
# Doxygen rules 


usersguide :: 
	$(MAKE) -C pc-linux  usersguide
#	$(MAKE) -C mcp750-vxworks6  usersguide

detaileddocs :: 
	$(MAKE) -C pc-linux  detaileddocs
#	$(MAKE) -C mcp750-vxworks6  detaileddocs

installdocs ::
	$(MAKE) -C pc-linux  installdocs
#	$(MAKE) -C mcp750-vxworks6  installdocs

cleandocs ::
	$(MAKE) -C pc-linux cleandocs
#	$(MAKE) -C mcp750-vxworks6  cleandocs
	  

