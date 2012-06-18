## 
## cfe-config.mak
##
## This file specifies the OS, and PSP to build for the cFE.
##
## NOTE: Make sure each selection does not have any spaces
##       after it!
##
##--------------------------------------------------------- 
## Supported Settings 
## 
##  OS          PSP 
## 
##  posix       mac-osx
##  posix       pc-linux
##  posix       pc-cygwin
##  rtems       mcf5235-rtems
##  vxworks6    rad750-vxworks6.4
##  vxworks6    mcp750-vxworks6.4
## 
##---------------------------------------------------------

##---------------------------------------------------------
## Operating System
## OS = The operating system selected for the Abstraction implementation
##---------------------------------------------------------
OS = posix

##---------------------------------------------------------
## PSP -- CFE Platform Support Package
##---------------------------------------------------------
PSP = pc-linux

