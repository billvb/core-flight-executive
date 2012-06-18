###############################################################################
# File: Makefile
#
# Purpose:
#   Main Makefile for cFE core build and all apps
#
## $Id: Makefile 1.14 2009/07/31 16:28:32EDT rmcgraw Exp  $
#
# Purpose: Main Makefile for cFE core build and all apps
#
# $Log: Makefile  $
# Revision 1.14 2009/07/31 16:28:32EDT rmcgraw 
# DCR8291:8 Removed CF entries and added log
#
###############################################################################
##
## Top Level rules:
## If you want to build the testapps every time, just add the testapps targets
## to the primary targets below ( or uncomment the testapps versions ) 
## 
export CPUNAME:=cpu1

all:: cfecore apps

clean:: cleancore cleanapps cleandocs

install:: installcore installapps

depend:: dependapps dependcore

##############################################################################
## Build rules

#
# cfecore make rule will also install the core, because some apps depend on the
#     core to link properly
# 
cfecore::
	$(MAKE) -C cfe 
	$(MAKE) -C cfe install

apps::
	$(MAKE) -C sample_lib
	$(MAKE) -C sample_app

#-------------------------------------------------------------------------------
# Clean up all files prduced while compiling and linking.


cleancore ::
	$(MAKE) -C cfe clean

cleanapps ::
	$(MAKE) -C sample_lib clean
	$(MAKE) -C sample_app clean
	
cleandocs ::
	$(MAKE) -C cfe cleandocs

#-------------------------------------------------------------------------------
# Install rule for the core and apps


installcore :: 
	$(MAKE) -C cfe  install

installapps ::
	$(MAKE) -C sample_lib  install
	$(MAKE) -C sample_app  install
	
installdocs ::
	$(MAKE) -C cfe installdocs
	-cp ./docs/users_guide/html/*.*          $(CFS_MISSION)/docs/users_guide/html/$(CPUNAME)/
	-cp ./docs/users_guide/latex/*.*         $(CFS_MISSION)/docs/users_guide/latex/$(CPUNAME)/
	-cp ./docs/detailed_design/html/*.*      $(CFS_MISSION)/docs/detailed_design/html/$(CPUNAME)/
	-cp ./docs/detailed_design/latex/*.*     $(CFS_MISSION)/docs/detailed_design/latex/$(CPUNAME)/
	

#-------------------------------------------------------------------------------
# Doxygen rules for the core, apps and cpu


usersguide :: 
	$(MAKE) -C cfe usersguide
	doxygen user_doxy

detaileddocs :: 
	$(MAKE) -C cfe detaileddocs
	doxygen detail_doxy

#-------------------------------------------------------------------------------
# depend rules

dependcore::
	$(MAKE) -C cfe depend

dependapps::
	$(MAKE) -C sample_lib  depend
	$(MAKE) -C sample_app  depend
  

