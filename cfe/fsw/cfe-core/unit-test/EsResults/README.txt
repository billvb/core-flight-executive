The ES Services unit test results are expected to have the
following results for each of the ES services source files:

==========================================================================

cfe_es_api.c - 98.80% coverage (5 Lines of Code NOT executed)

The five lines of code that is not executed in Unit Testing
is in the CFE_ES_ExitApp API function.  The code snippet is
as follows:

        -:  479:       else /* It is an external App */
        -:  480:       {
        -:  481:          
    #####:  482:          CFE_ES_WriteToSysLog ("Application %s called CFE_ES_ExitApp\n",
        -:  483:                                 CFE_ES_Global.AppTable[AppID].StartParams.Name);
    #####:  484:          CFE_ES_Global.AppTable[AppID].StateRecord.AppState = CFE_ES_APP_STATE_STOPPED;
        -:  485:
        -:  486:
        -:  487:          /*
        -:  488:          ** Unlock the ES Shared data before suspending the app 
        -:  489:          */
    #####:  490:          CFE_ES_UnlockSharedData(__func__,__LINE__);
        -:  491:
        -:  492:          /*
        -:  493:          ** Suspend the Application until ES kills it.
        -:  494:          ** It might be better to have a way of suspending the app in the OS
        -:  495:          */
        -:  496:          while(1)
        -:  497:          {
    #####:  498:             OS_TaskDelay(500);
    #####:  499:          }
        -:  500:
        -:  501:       } /* end if */


REASON: Unit testing this branch is not possible because of the need for 
        the infinite loop.

==========================================================================
cfe_es_apps.c - 100.00% coverage

==========================================================================

cfe_es_cds.c - 98.45% coverage (3 Lines of Code NOT executed)

The LOC not executed are found in CFE_ES_CDS_EarlyInit.  A code snippet
is as follows:

        -:  219:            else /* Unrecoverable error while reading the CDS */
        -:  220:            {
    #####:  221:                CFE_ES_WriteToSysLog("CFE_CDS:EarlyInit-Read error validating CDS (Err=0x%08X)\n", Status);
    #####:  222:                CFE_ES_Global.CDSVars.MemPoolSize = 0;
    #####:  223:                return Status;
        -:  224:            }


REASON:  All possible error conditions are checked in previous if/else. This
         is a catch-all in case OS_BSPGetCDSSize returns something unexpected.

==========================================================================

cfe_es_cds_mempool.c - 98.59% coverage (3 lines of code NOT executed)

The LOC not executed are found in CFE_ES_RebuildCDSPool:

        -:  286:                    else
        -:  287:                    {
    #####:  288:                        CFE_ES_CDSMemPool.CheckErrCntr++;
    #####:  289:                        CFE_ES_WriteToSysLog("CFE_ES:RebuildCDS-Invalid Block Descriptor \n");
    #####:  290:                        Status = CFE_ES_CDS_ACCESS_ERROR;
        -:  291:                    }

REASON: This block of code executes in response to an error from a call to
        an ES function (CFE_ES_CDSGetBinIndex). Since this is an ES unit
        test we cannot stub out this function.

==========================================================================

cfe_es_erlog.c - 100.00% coverage

==========================================================================

cfe_esmempool.c - 100.00% coverage

==========================================================================

cfe_es_perf.c - 100 % coverage 
==========================================================================

cfe_es_shell.c - 100.00% coverage
==========================================================================

cfe_es_start.c - 100.00% coverage

==========================================================================

cfe_es_task.c - 99.02% coverage (6 Lines of Code NOT executed)

Four LOC not executed are found in CFE_ES_StartAppCmd.  A
code snippet is as follows:

        9:  825:       else if (cmd->AppEntryPoint == NULL)
        -:  826:       {
    #####:  827:          CFE_ES_TaskData.ErrCounter++;
    #####:  828:          CFE_EVS_SendEvent(CFE_ES_START_INVALID_ENTRY_POINT_ERR_EID, CFE_EVS_ERROR,
        -:  829:                           "CFE_ES_StartAppCmd: App Entry Point is NULL.");
        -:  830:       }
        9:  831:       else if (cmd->Application == NULL)
        -:  832:       {
    #####:  833:          CFE_ES_TaskData.ErrCounter++;
    #####:  834:          CFE_EVS_SendEvent(CFE_ES_START_NULL_APP_NAME_ERR_EID, CFE_EVS_ERROR,
        -:  835:                           "CFE_ES_StartAppCmd: App Name is NULL.");
        -:  836:       }


REASON: The structure used in the unit tests containing the command cannot
        have a null value, so these two checks against null cannot be satisfied.

One LOC not executed is found in CFE_ES_QueryAllTasksCmd:

        4: 1296:       if (QueryAllFilename[0] == '\0')
        -: 1297:       {
    #####: 1298:           strncpy(QueryAllFilename, CFE_ES_DEFAULT_TASK_LOG_FILE, OS_MAX_PATH_LEN);
        -: 1299:       }

REASON: The structure used in the unit tests containing the command cannot
        have a null value, so apparently this checks cannot be satisfied.
        
One LOC not executed is found in CFE_ES_DumpCDSRegCmd:

        4: 2026:    if (DumpFilename[0] == '\0')
        -: 2027:    {
    #####: 2028:        strncpy(DumpFilename, CFE_ES_DEFAULT_CDS_REG_DUMP_FILE, OS_MAX_PATH_LEN);
        -: 2029:    }

REASON: The structure used in the unit tests containing the command cannot
        have a null value, so apparently this check cannot be satisfied.

==========================================================================
