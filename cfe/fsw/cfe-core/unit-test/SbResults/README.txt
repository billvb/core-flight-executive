The Software Bus unit test for the post cFE 5.2.0 build had zero tests fail. 

================================================================================

NOTE: There is one endianness change that must be made manually to the 
ut_stubs.c file. In the function definition for UT_GetActualCmdCodeField, the 
pointer arithmetic is different depending on the endianess of the machine in
which you are running the unit test. In file ut_stubs.c, be sure one assignment 
(and only one) is uncommented:

/* Used when running on little endian machines */
/* Ptr8Bit += 7; */

/* Used when running on big endian machines */
/*  Ptr8Bit += 6; */

================================================================================

NOTE: If the *.gcov files (ex. cfe_sb_api.c.gcov) are double spaced, it's
because the line endings in the corresponding c files (ex. cfe_sb_api.c) are in
windows format (CRLF) instead of the unix format(LF). Similarly, if the
sbsim.out is double spaced, the sbsim.in has the windows line endings.

================================================================================

Here is a summary of the results from the intel mac run.

[mbp:sb]$ make gcov
Makefile:99: warning: overriding commands for target `sb_UT'
/Users/rmcgraw/Projects/cFE/fsw/cfe-core/src/make/make-rules.mak:36: warning: 
ignoring old commands for target `sb_UT'

Running unit test: gcov

./sb_UT

ut_cfe_sb PASSED 144 tests.
ut_cfe_sb FAILED 0 test(s).


gcov cfe_sb*.gcda ccsds.gcda | sed 'N;s/\n/ /' | \
			    sed -n '/File/p' | sed '/ads/d'  | \
		            sed 's/ Lines executed:/ /; s/File/gcov:/; s/of//'
gcov: '/Users/rmcgraw/Projects/cFE/fsw/cfe-core/src/sb/cfe_sb_api.c' 100.00%  443
gcov: '/Users/rmcgraw/Projects/cFE/fsw/cfe-core/src/sb/cfe_sb_buf.c' 100.00%  72
gcov: '/Users/rmcgraw/Projects/cFE/fsw/cfe-core/src/sb/cfe_sb_init.c' 100.00%  42
gcov: '/Users/rmcgraw/Projects/cFE/fsw/cfe-core/src/sb/cfe_sb_priv.c' 100.00%  141
gcov: '/Users/rmcgraw/Projects/cFE/fsw/cfe-core/src/sb/cfe_sb_task.c' 99.44%  356
gcov: '/Users/rmcgraw/Projects/cFE/fsw/cfe-core/src/sb/cfe_sb_util.c' 100.00%  88
gcov: '/Users/rmcgraw/Projects/cFE/fsw/cfe-core/src/sb/ccsds.c' 100.00%  22

[mbp:sb]$ 
[mbp:sb]$  




The Software Bus unit test results are expected to have the
following results for each of the SB source files:

==========================================================================

cfe_sb_task.c - 99.44% coverage (2 Lines of Code NOT executed)

The first line of code that is not executed in Unit Testing
is in the CFE_SB_TaskMain function.  The code snippet is
as follows:

        -:   91:    /* Main loop */
        3:   92:    while (Status == CFE_SUCCESS)
        -:   93:    {
        1:   94:        CFE_ES_PerfLogExit(CFE_SB_MAIN_PERF_ID);
        -:   95:
        -:   96:        /* Pend on receipt of packet */
        1:   97:        Status = CFE_SB_RcvMsg(&CFE_SB.CmdPipePktPtr,
        -:   98:                                CFE_SB.CmdPipe,
        -:   99:                                CFE_SB_PEND_FOREVER);
        -:  100:
        1:  101:        CFE_ES_PerfLogEntry(CFE_SB_MAIN_PERF_ID);
        -:  102:
        1:  103:        if(Status == CFE_SUCCESS)
        -:  104:        {
        -:  105:            /* Process cmd pipe msg */
    #####:  106:            CFE_SB_ProcessCmdPipePkt();
        -:  107:        }else{
        1:  108:            CFE_ES_WriteToSysLog("SB:Error reading cmd pipe,RC=0x%08X\n",Status);                  
        -:  109:        }/* end if */
        -:  110:
        -:  111:    }/* end while */

I have not found a way to execute this line while allowing the unit test 
terminate normally. 


The next line of code cannot be executed without altering EVS or SB cfg parameters.

 
        -:  235:    /* Be sure the number of events to register for filtering   
        -:  236:    ** does not exceed CFE_EVS_MAX_EVENT_FILTERS */
       12:  237:    if(CFE_EVS_MAX_EVENT_FILTERS < CfgFileEventsToFilter){
    #####:  238:      CfgFileEventsToFilter = CFE_EVS_MAX_EVENT_FILTERS;
        -:  239:    }/* end if */
        -:  240:
        -:  241:
        -:  242:    /* Register event filter table... */
       12:  243:    Status = CFE_EVS_Register(CFE_SB.EventFilters,
        -:  244:                              CfgFileEventsToFilter,
        -:  245:                              CFE_EVS_BINARY_FILTER);
       12:  246:    if(Status != CFE_SUCCESS){
        1:  247:      CFE_ES_WriteToSysLog("SB:Call to CFE_EVS_Register Failed:RC=0x%08X\n",Status);
        1:  248:      return Status;
        -:  249:    }/* end if */
        -:  250:    


==========================================================================

cfe_sb_api.c - 100% coverage

==========================================================================

cfe_sb_buf.c - 100% coverage

==========================================================================

cfe_sb_init.c - 100% coverage

==========================================================================

cfe_sb_priv.c - 100% coverage

==========================================================================

cfe_sb_util.c - 100% coverage

==========================================================================

