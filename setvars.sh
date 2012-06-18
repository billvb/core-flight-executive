#
# $Id: setvars.sh 1.22 2011/12/05 10:07:52EST lwalling Exp  $
#
# This is a shell script that sets the environment variables for 
# a CFS mission build. The makefiles depend on the variables being
# set correctly.
# 
# This script requires the "bash" shell to run. It should work on Linux, 
# Mac OS X, and Cygwin.
#
# $Log: setvars.sh  $
# Revision 1.22 2011/12/05 10:07:52EST lwalling 
# Restore improperly changed file
# Revision 1.21 2011/11/25 16:02:23EST lwalling 
# Move test zip file from mission build folder to cfe unit test folder
# Revision 1.20 2009/10/13 14:53:25EDT wmoleski 
# Adding the changes to the setvars.sh file in the CFS Mission Tree to contain a definition for the 
# location of the UTF source.
# Revision 1.19 2009/09/28 10:12:43EDT rmcgraw 
# Error in previous release, setting version back to 0.0.0
# Revision 1.18 2009/09/28 10:06:27EDT rmcgraw 
# Set version number to 2.0.0 for release
# Revision 1.17 2009/09/28 09:59:54EDT rmcgraw 
# Reset version number back to post release value of 0.0.0
# Revision 1.16 2009/07/08 14:02:07EDT rmcgraw 
# DCR8291:1 Changed variables to the correct defaults
# Revision 1.15 2009/07/08 13:49:21EDT rmcgraw 
# DCR8291:1 Changed line endings from windows to unix
# Revision 1.14 2009/07/08 10:53:30EDT rmcgraw 
# DCR8291:1 Renamed CFE_MISSION_INC to CFS_MISSION_INC
# Revision 1.13 2009/06/16 13:49:49EDT rmcgraw 
# DCR8291:1 Added MISSION_TEMPLATE_VERSION and change history log
#
# 
# Variable: CFS_MISSION
#
# CFS_MISSION is the top level directory of the Mission project tree.
# It is set here using the 'pwd' command so the entire directory can be moved to 
# a different place without editing this script.
# It can also be set to an absolute path like the following examples:
#   export CFS_MISSION=/opt/missions/mission-xyz
#   export CFS_MISSION=C:/Projects/mission-xyz
# Having an absolute path would be necessary for including this file in a 
# shared login script.
#
export CFS_MISSION=`pwd`
# 
# Variable: CFS_MISSION_INC
#
# CFS_MISSION_INC is the location of the CFS mission include directory. This 
# variable is separate because the entire "build" subdirectory is portable
# with the exception of this CFS_MISSION_INC directory.
# This variable is typically set to $CFS_MISSION/build/mission_inc 
# 
export CFS_MISSION_INC=$CFS_MISSION/build/mission_inc
#
# Variable: CFS_APP_SRC
#
# CFS_APPS is the location of the CFS Application source directory
# Normally, the this is located in the "apps" subdirectory of the mission
# directory, but it can be located anywhere like the following examples:
#   export CFS_APP_SRC=/opt/cfs-app-repository/
#   export CFS_APP_SRC=E:/projects/cfs/apps
#
export CFS_APP_SRC=$CFS_MISSION/apps
#
# Variable: CFE_CORE_SRC
#
# CFE_CORE_SRC is the location of the cFE core source code.
# The variable must point to the location of the "fsw/cfe-core/src" subdirectory
# of the cFE distribution.
# within the cFE distribution. For example: cfe-5.1/fsw/cfe-core/src
# Normally, the cFE source is located in the top level of the mission build 
# directory. If the cFE source is stored somewhere else in the file system, it 
# can be set to an absolute path like the following examples:
#   export CFE_CORE_SRC=/opt/cfs/cfe-5.1/fsw/cfe-core/src
#   export CFE_CORE_SRC=F:/cfe-archive/cfe-5.1/fsw/cfe-core/src   
#
export CFE_CORE_SRC=$CFS_MISSION/cfe/fsw/cfe-core/src
# 
# Variable: CFE_PSP_SRC
#
# CFE_PSP_SRC is the location of the cFE Platform Support Package (PSP) source 
# code. 
# For example:
#    export CFE_PSP_SRC=$CFS_MISSION/custom-psp
# 
export CFE_PSP_SRC=$CFS_MISSION/psp/fsw
# 
# Variable: OSAL_SRC
#
# OSAL_SRC is the location of the OS Abstraction layer source code.
# This variable must point to the location of the "src/os" subdirectory of
# the OS Abstraction Layer distribution. 
# Normally, the OSAL source is located in the top level of the mission build
# directory. If the OSAL source is stored somewhere else in the file system, 
# it can be set to an absolute path like the following examples:
#    export OSAL_SRC=/opt/cfs/osal-2.11/src/os
#    export OSAL_SRC=F:/osal-archive/osal-2.11/src/os
#
export OSAL_SRC=$CFS_MISSION/osal/src/os
# 
# Variable: UTF_SRC
#
# UTF_SRC is the location of the cFE Unit Test Framework source code.
# This variable must point to the location of the "cfe/tools/utf/" subdirectory.
#
export UTF_SRC=$CFS_MISSION/cfe/tools/utf
###############################################################################
#             Please do not change anything below this line 
###############################################################################
#
# Variable: MISSION_TEMPLATE_VERSION
#
# MISSION_TEMPLATE_VERSION shows the version number of the mission template
# directory tree. 
#
export MISSION_TEMPLATE_VERSION=0.0.0
