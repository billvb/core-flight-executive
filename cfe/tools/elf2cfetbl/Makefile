##
## Setup the include path for this tool
##
## NOTES:
##    1) 'pc-linux' PSP is assumed since the elf2cfetbl tool 
##       is typically run on the host build machine
##    2) the '../cpu1/inc' directory may need to be changed for
##       mission directory structures which name their processors
##       or platforms something more significant.  This particular
##       include path must include the directory containing the relevant
##       cfe_platform_cfg.h file for the target in question.
##
INCLUDE_PATH = \
-I$(OSAL_SRC)/inc \
-I$(CFE_CORE_SRC)/tbl \
-I$(CFE_CORE_SRC)/inc \
-I$(CFE_PSP_SRC)/pc-linux/inc \
-I$(CFE_PSP_SRC)/inc \
-I$(CFS_MISSION_INC) \
-I$(CFS_MISSION_INC)/../cpu1/inc
##
## Target Defines for the OS, Hardware Arch, etc..
## These settings are assuming that the tool is being run on an x86 processor
## with a linux (or linux-like) operating system
TARGET_DEFS=-D__ix86__ -D_ix86_ -D_LINUX_OS_ -DHWARCH=x86 -Dx86 -Dlinux -DX86PC -DBUILD=$(BUILD) -D_REENTRANT -D _EMBED_  

## 
## Endian Defines
##
ENDIAN_DEFS=-D_EL -DENDIAN=_EL -DSOFTWARE_LITTLE_BIT_ORDER 



tool:
	gcc $(TARGET_DEFS) $(ENDIAN_DEFS) $(INCLUDE_PATH) -m32 -g -o elf2cfetbl elf2cfetbl.c

sample:
	gcc $(TARGET_DEFS) $(ENDIAN_DEFS) $(INCLUDE_PATH) -m32 -c -o SampleTblImg SampleTblImg.c

clean:
	rm elf2cfetbl
	rm SampleTblImg
		