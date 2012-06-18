The TBL Services unit test results are expected to have the
following results for each of the TBL services source files:

==========================================================================

cfe_tbl_api.c - 99.58% coverage (2 Lines of Code NOT executed)

The first line of code that is not executed in Unit Testing
is in the CFE_TBL_Update API function.  The code snippet is
as follows:

        -:  974:    /* On Error conditions, notify ground of screw up */
        4:  975:    if (Status < 0)
        -:  976:    {
        1:  977:        if (RegRecPtr != NULL)
        -:  978:        {
    #####:  979:            CFE_EVS_SendEventWithAppID(CFE_TBL_UPDATE_ERR_EID,
        -:  980:                                       CFE_EVS_ERROR,
        -:  981:                                       CFE_TBL_TaskData.TableTaskAppId,
        -:  982:                                       "%s Failed to Update '%s', Status=0x%08X",
        -:  983:                                       AppName, RegRecPtr->Name, Status);
        -:  984:        }

This event message is never issued in unit testing because in order
for it to occur, the function CFE_TBL_UpdateInternal would be required
to return an error code.  At the current time, the CFE_TBL_UpdateInternal
function only returns either CFE_SUCCESS or informational status.  The
event message is being maintained in the event the CFE_TBL_UpdateInternal
function is modified to return an error.

The second line of code that is not executed in Unit Testing
is in the CFE_TBL_Manage API function.  The code snippet is
as follows:

        -: 1326:        /* Determine if the table has a validation or update that needs to be performed */
       20: 1327:        Status = CFE_TBL_GetStatus(TblHandle);
        -: 1328:
       20: 1329:        if (Status == CFE_TBL_INFO_VALIDATION_PENDING)
        -: 1330:        {
        -: 1331:            /* Validate the specified Table */
        8: 1332:            Status = CFE_TBL_Validate(TblHandle);
        -: 1333:
        8: 1334:            if (Status != CFE_SUCCESS)
        -: 1335:            {
        -: 1336:                /* If an error occurred during Validate, then do not perform any more managing */
    #####: 1337:                FinishedManaging = TRUE;
        -: 1338:            }
        -: 1339:        }


The FinishedManaging variable can currently never be set because the
only error conditions that CFE_TBL_Validate can return are already
captured by the validation performed in the CFE_TBL_GetStatus function.

==========================================================================

cfe_tbl_task.c - 100% coverage

==========================================================================

cfe_tbl_task_cmds.c - 99.77% coverage (1 Line of Code NOT executed)

The line of code that is not executed in Unit Testing is in the
CFE_TBL_LoadCmd function.  The code snippet is as follows:

        1:  617:                        else if (Status == CFE_TBL_ERR_NO_BUFFER_AVAIL)
        -:  618:                        {
        1:  619:                            CFE_EVS_SendEvent(CFE_TBL_NO_WORK_BUFFERS_ERR_EID,
        -:  620:                                              CFE_EVS_ERROR,
        -:  621:                                              "No working buffers available for table '%s'",
        -:  622:                                              TblFileHeader.TableName);
        -:  623:                        }
        -:  624:                        else
        -:  625:                        {
    #####:  626:                            CFE_EVS_SendEvent(CFE_TBL_INTERNAL_ERROR_ERR_EID,
        -:  627:                                              CFE_EVS_ERROR,
        -:  628:                                              "Internal Error (Status=0x%08X)",
        -:  629:                                              Status);
        -:  630:                        }

This event message is only issued when an unexpected error is detected
in underlying function calls.  In the current implementation, there isn't
any possibility of an unexpected error to occur.

==========================================================================

cfe_tbl_internal.c - 100% coverage

==========================================================================

