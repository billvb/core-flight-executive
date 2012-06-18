###############################################################################
#
# File: CFE Core Subsystem Unit Test Makefile 
#
# $Id: Makefile 1.6 2011/11/30 14:20:28EST jmdagost Exp  $
#
# $Log: Makefile  $
# Revision 1.6 2011/11/30 14:20:28EST jmdagost 
# Removed references to unused and empty ut_estask_stubs file.
# Revision 1.5 2009/07/09 12:04:31EDT rmcgraw 
# DCR8291:1 Changed CFE_MISSION_INC to CFS_MISSION_INC and added log if needed
#
###############################################################################  

##
## Identify the cFE Core Subsystem here. 
##
SUBSYS = time

##
## The Unit Test target to build
##
TARGET = time_UT

##
## Specify extra C Flags needed to build this subsystem
##
LOCAL_COPTS = -pg -fprofile-arcs

##
## The Unit Test Objects needed
##
UT_OBJS = ut_stubs.o \
ut_bsp_stubs.o \
ut_osapi_stubs.o \
ut_evs_stubs.o \
ut_es_stubs.o \
ut_sb_stubs.o \
ut_fs_stubs.o \
ut_tbl_stubs.o \
time_UT.o

##
## The cFE Subsystem Objects needed
##
SUBSYS_OBJS = cfe_time_task.o cfe_time_api.o cfe_time_utils.o cfe_time_tone.o 
 
## 
## Include all necessary cFE make rules
## Any of these can be copied to a local file and 
## changed if needed.
##
##
##       cfe-config.mak contians arch, PSP, and OS selection
##
include ../../cfe-config.mak
##
##       debug-opts.mak contains debug switches
##
include ../../debug-opts.mak
##
##       compiler-opts.mak contains compiler definitions and switches/defines
##
include $(CFE_PSP_SRC)/$(PSP)/make/compiler-opts.mak

##
## Define the OBJS macro for the compile and make clean rules
## 
OBJS = $(SUBSYS_OBJS) $(UT_OBJS)
 
##
## Setup the include path for this subsystem
## The OS specific includes are in the compiler-opts.mak file
##
INCLUDE_PATH = \
-I$(OSAL_SRC)/inc \
-I$(CFE_CORE_SRC)/$(SUBSYS) \
-I$(CFE_CORE_SRC)/inc \
-I$(CFE_PSP_SRC)/inc \
-I$(CFE_PSP_SRC)/$(PSP)/inc \
-I$(CFS_MISSION_INC) \
-I$(CFE_CORE_SRC)/../unit-test \
-I../../../inc

##
## Define the VPATH make variable. 
## This can be modified to include source from another directory.
##
VPATH = $(CFE_CORE_SRC)/$(SUBSYS):$(CFE_CORE_SRC)/../unit-test

##
## Include the common make rules for the cFE Core subsystems
##
include $(CFE_CORE_SRC)/make/make-rules.mak

##
## Subsystem Link rule -- Overrides the default rule
## 
$(TARGET): $(OBJS)
	$(COMPILER) -o $(TARGET) $(OBJS) -m32 -pg -fprofile-arcs -ftest-coverage

##
## gcov rule
## 
gcov:
	@echo
	@echo "Running unit test: $@"
	@echo
	./$(TARGET)
	@rm -f gmon.out
	@echo
	gcov cfe_$(SUBSYS)*.gcda | sed 'N;s/\n/ /' | \
			    sed -n '/File/p' | sed '/ads/d'  | \
		            sed 's/ Lines executed:/ /; s/File/gcov:/; s/of//'
	@echo
   
