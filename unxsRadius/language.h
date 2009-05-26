// $Id$
// English: USA
// Navigation bar buttons and results

#define LANG_NB_LIST "List"
#define LANG_NB_MODIFY "Modify"
#define LANG_NB_CONFIRMMOD "Confirm Modify"
#define LANG_NB_NEW "New"
#define LANG_NB_DELETE "Delete"
#define LANG_NB_CONFIRMNEW "Create New Record"
#define LANG_NBB_CONFIRMNEW "<input class=lrevButton title='Double check!' type=submit name=gcCommand value=\"Create New Record\">"
#define LANG_NB_CONFIRMDEL "Confirm Delete"
#define LANG_NBB_CONFIRMDEL "<input class=lwarnButton title='No undo available!' type=submit name=gcCommand value=\"Confirm Delete\">"
#define LANG_NBB_CONFIRMMOD "<input class=lalertButton title='Double check!' type=submit name=gcCommand value=\"Confirm Modify\">"


#define LANG_NBR_FOUND " Found"
#define LANG_NBR_MODIFIED " Modified"
#define LANG_NBR_NEW " New"
#define LANG_NBR_NORECS " No records found"
#define LANG_NBRF_SHOWING " (Showing %lu of %d)"

#define LANG_NBR_RECEXISTS "<blink>Record already exists"
#define LANG_NBR_NEWRECADDED "New record %u added"
#define LANG_NBR_RECDELETED "Record Deleted"
#define LANG_NBR_RECNOTDELETED "Record Not Deleted"
#define LANG_NBR_RECNOTEXIST "<blink>Record does not exist"
#define LANG_NBR_MULTRECS "<blink>Multiple rows!"
#define LANG_NBR_EXTMOD "<blink>This record was just modified by another user!"
#define LANG_NBRF_REC_MODIFIED "record %s modified"

#define LANG_NBB_SKIPFIRST "<input style='width:20px;' title=\"Skip to first record\" type=submit name=gcFind value=\"<<\" >"
#define LANG_NBB_SKIPBACK "<input style='width:20px;' title=\"Go back one record\" type=submit name=gcFind value=\"< \" >"
#define LANG_NBB_SKIPNEXT "<input style='width:20px;' title=\"Advance to next record\" type=submit name=gcFind value=\" >\" >"
#define LANG_NBB_SKIPLAST "<input style='width:20px;' title=\"Skip to last record\" type=submit name=gcFind value=\">>\" >\n"

//Modified for local .h use
#define LANG_NBB_NEWREV "<input class=revButton title=\"Create new record\" type=submit name=gcCommand value=New>"
#define LANG_NBB_NEW "<input  title=\"Create new record\" type=submit name=gcCommand value=New>"
#define LANG_NBB_DELETE "<input class=warnButton title=\"Delete current record\" type=submit name=gcCommand value=Delete>"
#define LANG_NBB_MODIFY "<input class=alertButton title=\"Modify current record\" type=submit name=gcCommand value=Modify>"

#define LANG_NBB_LIST "<input title=\"Paginated listing of all records\" type=submit name=gcFind value=List>"
#define LANG_NBB_SEARCH "<input class=revButton title=\"Find record with parameters set below\" type=submit name=gcFind value=Search>"

#define LANG_PAGEMACHINE_HINT "</center><table bgcolor=black><tr><td><font face=arial,helvetica><font color=white> No matching records found.<br><font size=1 color=red>-->Hint:</font><font size=1> Use %% wildcard. The %% is equal to one or more characters.</table>"
#define LANG_PAGEMACHINE_SHOWING "Page %d/%d (Records %d-%d of %d)\n"
#define LANG_PAGEMACHINE_SEARCH "<input type=submit name=gcFind value=Search >"
#define LANG_PAGEMACHINE_SEARCHBUTTON "Search"

//tUser
#define LANG_FL_tUser_uUser "uUser"
#define LANG_FT_tUser_uUser "Primary Key"
#define LANG_FL_tUser_cLogin "cLogin"
#define LANG_FT_tUser_cLogin "Radius user login"
#define LANG_FL_tUser_uOnHold "uOnHold"
#define LANG_FT_tUser_uOnHold "Users OnHold are not in raddb/users file"
#define LANG_FL_tUser_cEnterPasswd "cEnterPasswd"
#define LANG_FT_tUser_cEnterPasswd "Clear text passwd input field"
#define LANG_FL_tUser_uClearText "uClearText"
#define LANG_FT_tUser_uClearText "Clear text passwd for CHAP use"
#define LANG_FL_tUser_cPasswd "cPasswd"
#define LANG_FT_tUser_cPasswd "Encrypted passwd"
#define LANG_FL_tUser_uProfileName "uProfileName"
#define LANG_FT_tUser_uProfileName "Profile name"
#define LANG_FL_tUser_uSimulUse "uSimulUse"
#define LANG_FT_tUser_uSimulUse "Simultaneous use"
#define LANG_FL_tUser_cIP "cIP"
#define LANG_FT_tUser_cIP "Static IP if matches profile"
#define LANG_FL_tUser_uClient "uClient"
#define LANG_FL_tUser_uOwner "uOwner"
#define LANG_FT_tUser_uOwner "Record owner"
#define LANG_FL_tUser_uCreatedBy "uCreatedBy"
#define LANG_FT_tUser_uCreatedBy "uClient for last insert"
#define LANG_FL_tUser_uCreatedDate "uCreatedDate"
#define LANG_FT_tUser_uCreatedDate "Unix seconds date last insert"
#define LANG_FL_tUser_uModBy "uModBy"
#define LANG_FT_tUser_uModBy "uClient for last update"
#define LANG_FL_tUser_uModDate "uModDate"
#define LANG_FT_tUser_uModDate "Unix seconds date last update"
//tProfile
#define LANG_FL_tProfile_uProfile "uProfile"
#define LANG_FT_tProfile_uProfile "Primary Key"
#define LANG_FL_tProfile_uUser "uUser"
#define LANG_FT_tProfile_uUser "User specific profile if set"
#define LANG_FL_tProfile_uServer "uServer"
#define LANG_FT_tProfile_uServer "Server specific profile if set"
#define LANG_FL_tProfile_uProfileName "uProfileName"
#define LANG_FT_tProfile_uProfileName "Profile Name"
#define LANG_FL_tProfile_uClearText "uClearText"
#define LANG_FT_tProfile_uClearText "CHAP or PAP version of uProfileName"
#define LANG_FL_tProfile_uStaticIP "uStaticIP"
#define LANG_FT_tProfile_uStaticIP "Static IP version of uProfileName"
#define LANG_FL_tProfile_cComment "cComment"
#define LANG_FT_tProfile_cComment "Comments about profile"
#define LANG_FL_tProfile_cProfile "cProfile"
#define LANG_FT_tProfile_cProfile "Profile sprintf() template"
#define LANG_FL_tProfile_uHourStart "uHourStart"
#define LANG_FT_tProfile_uHourStart "Allow service starting at this daily hour"
#define LANG_FL_tProfile_uHourEnd "uHourEnd"
#define LANG_FT_tProfile_uHourEnd "Allow service ending at this daily hour"
#define LANG_FL_tProfile_uWeekDayStart "uWeekDayStart"
#define LANG_FT_tProfile_uWeekDayStart "Allow service starting on week day (sunday=0)"
#define LANG_FL_tProfile_uWeekDayEnd "uWeekDayEnd"
#define LANG_FT_tProfile_uWeekDayEnd "Allow service ending on this week day"
#define LANG_FL_tProfile_uActivationDate "uActivationDate"
#define LANG_FT_tProfile_uActivationDate "Activate service this date"
#define LANG_FL_tProfile_uDeactivationDate "uDeactivationDate"
#define LANG_FT_tProfile_uDeactivationDate "Deactivate service this date"
#define LANG_FL_tProfile_uOwner "uOwner"
#define LANG_FT_tProfile_uOwner "Record owner"
#define LANG_FL_tProfile_uCreatedBy "uCreatedBy"
#define LANG_FT_tProfile_uCreatedBy "uClient for last insert"
#define LANG_FL_tProfile_uCreatedDate "uCreatedDate"
#define LANG_FT_tProfile_uCreatedDate "Unix seconds date last insert"
#define LANG_FL_tProfile_uModBy "uModBy"
#define LANG_FT_tProfile_uModBy "uClient for last update"
#define LANG_FL_tProfile_uModDate "uModDate"
#define LANG_FT_tProfile_uModDate "Unix seconds date last update"
//tProfileName
#define LANG_FL_tProfileName_uProfileName "uProfileName"
#define LANG_FT_tProfileName_uProfileName "Primary Key"
#define LANG_FL_tProfileName_cLabel "cLabel"
#define LANG_FT_tProfileName_cLabel "Short label"
#define LANG_FL_tProfileName_uOwner "uOwner"
#define LANG_FT_tProfileName_uOwner "Record owner"
#define LANG_FL_tProfileName_uCreatedBy "uCreatedBy"
#define LANG_FT_tProfileName_uCreatedBy "uClient for last insert"
#define LANG_FL_tProfileName_uCreatedDate "uCreatedDate"
#define LANG_FT_tProfileName_uCreatedDate "Unix seconds date last insert"
#define LANG_FL_tProfileName_uModBy "uModBy"
#define LANG_FT_tProfileName_uModBy "uClient for last update"
#define LANG_FL_tProfileName_uModDate "uModDate"
#define LANG_FT_tProfileName_uModDate "Unix seconds date last update"
//tServer
#define LANG_FL_tServer_uServer "uServer"
#define LANG_FT_tServer_uServer "Primary Key"
#define LANG_FL_tServer_cLabel "cLabel"
#define LANG_FT_tServer_cLabel "Short label"
#define LANG_FL_tServer_uOwner "uOwner"
#define LANG_FT_tServer_uOwner "Record owner"
#define LANG_FL_tServer_uCreatedBy "uCreatedBy"
#define LANG_FT_tServer_uCreatedBy "uClient for last insert"
#define LANG_FL_tServer_uCreatedDate "uCreatedDate"
#define LANG_FT_tServer_uCreatedDate "Unix seconds date last insert"
#define LANG_FL_tServer_uModBy "uModBy"
#define LANG_FT_tServer_uModBy "uClient for last update"
#define LANG_FL_tServer_uModDate "uModDate"
#define LANG_FT_tServer_uModDate "Unix seconds date last update"
//tNAS
#define LANG_FL_tNAS_uNAS "uNAS"
#define LANG_FT_tNAS_uNAS "Primary Key"
#define LANG_FL_tNAS_cLabel "cLabel"
#define LANG_FT_tNAS_cLabel "Short label"
#define LANG_FL_tNAS_cKey "cKey"
#define LANG_FT_tNAS_cKey "Shared secret key"
#define LANG_FL_tNAS_cType "cType"
#define LANG_FT_tNAS_cType "NAS/RAS Type"
#define LANG_FL_tNAS_cIP "cIP"
#define LANG_FT_tNAS_cIP "IP Number for remote management of NAS"
#define LANG_FL_tNAS_cLogin "cLogin"
#define LANG_FT_tNAS_cLogin "Login for remote management of NAS"
#define LANG_FL_tNAS_cPasswd "cPasswd"
#define LANG_FT_tNAS_cPasswd "Passwd for remote management of NAS"
#define LANG_FL_tNAS_cInfo "cInfo"
#define LANG_FT_tNAS_cInfo "Location and hardware info of NAS"
#define LANG_FL_tNAS_uOwner "uOwner"
#define LANG_FT_tNAS_uOwner "Record owner"
#define LANG_FL_tNAS_uCreatedBy "uCreatedBy"
#define LANG_FT_tNAS_uCreatedBy "uClient for last insert"
#define LANG_FL_tNAS_uCreatedDate "uCreatedDate"
#define LANG_FT_tNAS_uCreatedDate "Unix seconds date last insert"
#define LANG_FL_tNAS_uModBy "uModBy"
#define LANG_FT_tNAS_uModBy "uClient for last update"
#define LANG_FL_tNAS_uModDate "uModDate"
#define LANG_FT_tNAS_uModDate "Unix seconds date last update"
//tJob
#define LANG_FL_tJob_uJob "uJob"
#define LANG_FL_tJob_cLabel "cLabel"
#define LANG_FT_tJob_cLabel "Label for user feedback"
#define LANG_FT_tJob_uJob "Primary Key"
#define LANG_FL_tJob_cServer "cServer"
#define LANG_FT_tJob_cServer "Server name ip or any server"
#define LANG_FL_tJob_cJobName "cJobName"
#define LANG_FT_tJob_cJobName "Subsystem.Function style job name"
#define LANG_FL_tJob_cJobData "cJobData"
#define LANG_FT_tJob_cJobData "Remote subsystem server function arguments"
#define LANG_FL_tJob_cRemoteMsg "cRemoteMsg"
#define LANG_FT_tJob_cRemoteMsg "Remote subsytem error message"
#define LANG_FL_tJob_uJobDate "uJobDate"
#define LANG_FL_tJob_uJobStatus "uJobStatus"
#define LANG_FT_tJob_uJobDate "Unix seconds for job to start to be considered"
#define LANG_FL_tJob_uOwner "uOwner"
#define LANG_FT_tJob_uOwner "Record owner"
#define LANG_FL_tJob_uCreatedBy "uCreatedBy"
#define LANG_FT_tJob_uCreatedBy "uClient for last insert"
#define LANG_FL_tJob_uCreatedDate "uCreatedDate"
#define LANG_FT_tJob_uCreatedDate "Unix seconds date last insert"
#define LANG_FL_tJob_uModBy "uModBy"
#define LANG_FT_tJob_uModBy "uClient for last update"
#define LANG_FL_tJob_uModDate "uModDate"
#define LANG_FT_tJob_uModDate "Unix seconds date last update"
//tConfiguration
#define LANG_FL_tConfiguration_uConfiguration "uConfiguration"
#define LANG_FT_tConfiguration_uConfiguration "Primary Key"
#define LANG_FL_tConfiguration_uServer "uServer"
#define LANG_FL_tConfiguration_cLabel "cLabel"
#define LANG_FT_tConfiguration_cLabel "Short label"
#define LANG_FL_tConfiguration_cValue "cValue"
#define LANG_FT_tConfiguration_cValue "Value of name(cLabel)/value pair"
#define LANG_FL_tConfiguration_cComment "cComment"
#define LANG_FT_tConfiguration_cComment "Comment about this configuration name/value pair"
#define LANG_FL_tConfiguration_uOwner "uOwner"
#define LANG_FT_tConfiguration_uOwner "Record owner"
#define LANG_FL_tConfiguration_uCreatedBy "uCreatedBy"
#define LANG_FT_tConfiguration_uCreatedBy "uClient for last insert"
#define LANG_FL_tConfiguration_uCreatedDate "uCreatedDate"
#define LANG_FT_tConfiguration_uCreatedDate "Unix seconds date last insert"
#define LANG_FL_tConfiguration_uModBy "uModBy"
#define LANG_FT_tConfiguration_uModBy "uClient for last update"
#define LANG_FL_tConfiguration_uModDate "uModDate"
#define LANG_FT_tConfiguration_uModDate "Unix seconds date last update"
//tServerGroup
#define LANG_FL_tServerGroup_uUser "uUser"
#define LANG_FT_tServerGroup_uUser "Servers per uUser glue table"
#define LANG_FL_tServerGroup_uServer "uServer"
#define LANG_FT_tServerGroup_uServer "Index into tServer"
//tNASGroup
#define LANG_FL_tNASGroup_uNAS "uNAS"
#define LANG_FT_tNASGroup_uNAS "Servers per uNAS glue table"
#define LANG_FL_tNASGroup_uServer "uServer"
#define LANG_FT_tNASGroup_uServer "Index into tServer"
//tClient
#define LANG_FL_tClient_uClient "uClient"
#define LANG_FT_tClient_uClient "Primary Key"
#define LANG_FL_tClient_cLabel "cLabel"
#define LANG_FT_tClient_cLabel "Short label"
#define LANG_FL_tClient_cInfo "cInfo"
#define LANG_FT_tClient_cInfo "Unformatted info/address etc."
#define LANG_FL_tClient_cEmail "cEmail"
#define LANG_FT_tClient_cEmail "Main Email"
#define LANG_FL_tClient_cCode "cCode"
#define LANG_FT_tClient_cCode "Enterprise wide customer/contact tracking"
#define LANG_FL_tClient_uOwner "uOwner"
#define LANG_FT_tClient_uOwner "Record owner"
#define LANG_FL_tClient_uCreatedBy "uCreatedBy"
#define LANG_FT_tClient_uCreatedBy "uClient for last insert"
#define LANG_FL_tClient_uCreatedDate "uCreatedDate"
#define LANG_FT_tClient_uCreatedDate "Unix seconds date last insert"
#define LANG_FL_tClient_uModBy "uModBy"
#define LANG_FT_tClient_uModBy "uClient for last update"
#define LANG_FL_tClient_uModDate "uModDate"
#define LANG_FT_tClient_uModDate "Unix seconds date last update"
//tAuthorize
#define LANG_FL_tAuthorize_uAuthorize "uAuthorize"
#define LANG_FT_tAuthorize_uAuthorize "Primary Key"
#define LANG_FL_tAuthorize_cLabel "cLabel"
#define LANG_FT_tAuthorize_cLabel "Short label"
#define LANG_FL_tAuthorize_cIpMask "cIpMask"
#define LANG_FT_tAuthorize_cIpMask "Allow user from this IP"
#define LANG_FL_tAuthorize_uPerm "uPerm"
#define LANG_FT_tAuthorize_uPerm "User permission level"
#define LANG_FL_tAuthorize_uCertClient "uCertClient"
#define LANG_FT_tAuthorize_uCertClient "User uClient"
#define LANG_FL_tAuthorize_cPasswd "cPasswd"
#define LANG_FT_tAuthorize_cPasswd "Unix crypt() password"
#define LANG_FL_tAuthorize_cClrPasswd "cClrPasswd"
#define LANG_FT_tAuthorize_cClrPasswd "Optionally used non encrypted login password"
#define LANG_FL_tAuthorize_uOwner "uOwner"
#define LANG_FT_tAuthorize_uOwner "Record owner"
#define LANG_FL_tAuthorize_uCreatedBy "uCreatedBy"
#define LANG_FT_tAuthorize_uCreatedBy "uClient for last insert"
#define LANG_FL_tAuthorize_uCreatedDate "uCreatedDate"
#define LANG_FT_tAuthorize_uCreatedDate "Unix seconds date last insert"
#define LANG_FL_tAuthorize_uModBy "uModBy"
#define LANG_FT_tAuthorize_uModBy "uClient for last update"
#define LANG_FL_tAuthorize_uModDate "uModDate"
#define LANG_FT_tAuthorize_uModDate "Unix seconds date last update"
//tLog
#define LANG_FL_tLog_uLog "uLog"
#define LANG_FT_tLog_uLog "Primary Key"
#define LANG_FL_tLog_cLabel "cLabel"
#define LANG_FT_tLog_cLabel "Short label"
#define LANG_FL_tLog_uLogType "uLogType"
#define LANG_FT_tLog_uLogType "Log Type"
#define LANG_FL_tLog_cHash "cHash"
#define LANG_FT_tLog_cHash "Security hash to complicate tampering"
#define LANG_FL_tLog_uPermLevel "uPermLevel"
#define LANG_FT_tLog_uPermLevel "User Perm Level"
#define LANG_FL_tLog_uLoginClient "uLoginClient"
#define LANG_FT_tLog_uLoginClient "Client Number"
#define LANG_FL_tLog_cLogin "cLogin"
#define LANG_FT_tLog_cLogin "Login name"
#define LANG_FL_tLog_cHost "cHost"
#define LANG_FT_tLog_cHost "Ip Address"
#define LANG_FL_tLog_uTablePK "uTablePK"
#define LANG_FT_tLog_uTablePK "Primar Key of the Table"
#define LANG_FL_tLog_cTableName "cTableName"
#define LANG_FT_tLog_cTableName "Name of the Table"
#define LANG_FL_tLog_cMessage "cMessage"
#define LANG_FT_tLog_cMessage "Message text"
#define LANG_FL_tLog_cServer "cServer"
#define LANG_FT_tLog_cServer "Server name"
#define LANG_FL_tLog_uOwner "uOwner"
#define LANG_FT_tLog_uOwner "Record owner"
#define LANG_FL_tLog_uCreatedBy "uCreatedBy"
#define LANG_FT_tLog_uCreatedBy "uClient for last insert"
#define LANG_FL_tLog_uCreatedDate "uCreatedDate"
#define LANG_FT_tLog_uCreatedDate "Unix seconds date last insert"
#define LANG_FL_tLog_uModBy "uModBy"
#define LANG_FT_tLog_uModBy "uClient for last update"
#define LANG_FL_tLog_uModDate "uModDate"
#define LANG_FT_tLog_uModDate "Unix seconds date last update"
//tLogType
#define LANG_FL_tLogType_uLogType "uLogType"
#define LANG_FT_tLogType_uLogType "Primary Key"
#define LANG_FL_tLogType_cLabel "cLabel"
#define LANG_FT_tLogType_cLabel "Short label"
#define LANG_FL_tLogType_uOwner "uOwner"
#define LANG_FT_tLogType_uOwner "Record owner"
#define LANG_FL_tLogType_uCreatedBy "uCreatedBy"
#define LANG_FT_tLogType_uCreatedBy "uClient for last insert"
#define LANG_FL_tLogType_uCreatedDate "uCreatedDate"
#define LANG_FT_tLogType_uCreatedDate "Unix seconds date last insert"
#define LANG_FL_tLogType_uModBy "uModBy"
#define LANG_FT_tLogType_uModBy "uClient for last update"
#define LANG_FL_tLogType_uModDate "uModDate"
#define LANG_FT_tLogType_uModDate "Unix seconds date last update"
//tLogMonth
#define LANG_FL_tLogMonth_uLog "uLog"
#define LANG_FT_tLogMonth_uLog "Primary Key"
#define LANG_FL_tLogMonth_cLabel "cLabel"
#define LANG_FT_tLogMonth_cLabel "Short label"
#define LANG_FL_tLogMonth_uLogType "uLogType"
#define LANG_FT_tLogMonth_uLogType "Log Type"
#define LANG_FL_tLogMonth_cHash "cHash"
#define LANG_FT_tLogMonth_cHash "Security hash to complicate tampering"
#define LANG_FL_tLogMonth_uPermLevel "uPermLevel"
#define LANG_FT_tLogMonth_uPermLevel "User Perm Level"
#define LANG_FL_tLogMonth_uLoginClient "uLoginClient"
#define LANG_FT_tLogMonth_uLoginClient "Client Number"
#define LANG_FL_tLogMonth_cLogin "cLogin"
#define LANG_FT_tLogMonth_cLogin "Login name"
#define LANG_FL_tLogMonth_cHost "cHost"
#define LANG_FT_tLogMonth_cHost "Ip Address"
#define LANG_FL_tLogMonth_uTablePK "uTablePK"
#define LANG_FT_tLogMonth_uTablePK "Primar Key of the Table"
#define LANG_FL_tLogMonth_cTableName "cTableName"
#define LANG_FT_tLogMonth_cTableName "Name of the Table"
#define LANG_FL_tLogMonth_uOwner "uOwner"
#define LANG_FT_tLogMonth_uOwner "Record owner"
#define LANG_FL_tLogMonth_uCreatedBy "uCreatedBy"
#define LANG_FT_tLogMonth_uCreatedBy "uClient for last insert"
#define LANG_FL_tLogMonth_uCreatedDate "uCreatedDate"
#define LANG_FT_tLogMonth_uCreatedDate "Unix seconds date last insert"
#define LANG_FL_tLogMonth_uModBy "uModBy"
#define LANG_FT_tLogMonth_uModBy "uClient for last update"
#define LANG_FL_tLogMonth_uModDate "uModDate"
#define LANG_FT_tLogMonth_uModDate "Unix seconds date last update"
//tMonth
#define LANG_FL_tMonth_uMonth "uMonth"
#define LANG_FT_tMonth_uMonth "Primary Key"
#define LANG_FL_tMonth_cLabel "cLabel"
#define LANG_FT_tMonth_cLabel "Name of Archive Table Ex. tNov2009"
#define LANG_FL_tMonth_uOwner "uOwner"
#define LANG_FT_tMonth_uOwner "Record owner"
#define LANG_FL_tMonth_uCreatedBy "uCreatedBy"
#define LANG_FT_tMonth_uCreatedBy "uClient for last insert"
#define LANG_FL_tMonth_uCreatedDate "uCreatedDate"
#define LANG_FT_tMonth_uCreatedDate "Unix seconds date last insert"
#define LANG_FL_tMonth_uModBy "uModBy"
#define LANG_FT_tMonth_uModBy "uClient for last update"
#define LANG_FL_tMonth_uModDate "uModDate"
#define LANG_FT_tMonth_uModDate "Unix seconds date last update"
//tGlossary
#define LANG_FL_tGlossary_uGlossary "uGlossary"
#define LANG_FT_tGlossary_uGlossary "Primary Key"
#define LANG_FL_tGlossary_cLabel "cLabel"
#define LANG_FT_tGlossary_cLabel "Short label"
#define LANG_FL_tGlossary_cText "cText"
#define LANG_FT_tGlossary_cText "Definition of the label referenced in cLabel"
#define LANG_FL_tGlossary_uOwner "uOwner"
#define LANG_FT_tGlossary_uOwner "Record owner"
#define LANG_FL_tGlossary_uCreatedBy "uCreatedBy"
#define LANG_FT_tGlossary_uCreatedBy "uClient for last insert"
#define LANG_FL_tGlossary_uCreatedDate "uCreatedDate"
#define LANG_FT_tGlossary_uCreatedDate "Unix seconds date last insert"
#define LANG_FL_tGlossary_uModBy "uModBy"
#define LANG_FT_tGlossary_uModBy "uClient for last update"
#define LANG_FL_tGlossary_uModDate "uModDate"
#define LANG_FT_tGlossary_uModDate "Unix seconds date last update"
//tJobStatus
#define LANG_FL_tJobStatus_uJobStatus "uJobStatus"
#define LANG_FT_tJobStatus_uJobStatus "Primary Key"
#define LANG_FL_tJobStatus_cLabel "cLabel"
#define LANG_FT_tJobStatus_cLabel "Short label"
#define LANG_FL_tJobStatus_uOwner "uOwner"
#define LANG_FT_tJobStatus_uOwner "Record owner"
#define LANG_FL_tJobStatus_uCreatedBy "uCreatedBy"
#define LANG_FT_tJobStatus_uCreatedBy "uClient for last insert"
#define LANG_FL_tJobStatus_uCreatedDate "uCreatedDate"
#define LANG_FT_tJobStatus_uCreatedDate "Unix seconds date last insert"
#define LANG_FL_tJobStatus_uModBy "uModBy"
#define LANG_FT_tJobStatus_uModBy "uClient for last update"
#define LANG_FL_tJobStatus_uModDate "uModDate"
#define LANG_FT_tJobStatus_uModDate "Unix seconds date last update"
//tTemplate
#define LANG_FL_tTemplate_uTemplate "uTemplate"
#define LANG_FT_tTemplate_uTemplate "Primary Key"
#define LANG_FL_tTemplate_cLabel "cLabel"
#define LANG_FT_tTemplate_cLabel "Short label"
#define LANG_FL_tTemplate_uTemplateSet "uTemplateSet"
#define LANG_FT_tTemplate_uTemplateSet "Short label"
#define LANG_FL_tTemplate_uTemplateType "uTemplateType"
#define LANG_FT_tTemplate_uTemplateType "Short label"
#define LANG_FL_tTemplate_cComment "cComment"
#define LANG_FT_tTemplate_cComment "About the template"
#define LANG_FL_tTemplate_cTemplate "cTemplate"
#define LANG_FT_tTemplate_cTemplate "Template itself"
#define LANG_FL_tTemplate_uOwner "uOwner"
#define LANG_FT_tTemplate_uOwner "Record owner"
#define LANG_FL_tTemplate_uCreatedBy "uCreatedBy"
#define LANG_FT_tTemplate_uCreatedBy "uClient for last insert"
#define LANG_FL_tTemplate_uCreatedDate "uCreatedDate"
#define LANG_FT_tTemplate_uCreatedDate "Unix seconds date last insert"
#define LANG_FL_tTemplate_uModBy "uModBy"
#define LANG_FT_tTemplate_uModBy "uClient for last update"
#define LANG_FL_tTemplate_uModDate "uModDate"
#define LANG_FT_tTemplate_uModDate "Unix seconds date last update"
//tTemplateSet
#define LANG_FL_tTemplateSet_uTemplateSet "uTemplateSet"
#define LANG_FT_tTemplateSet_uTemplateSet "Primary Key"
#define LANG_FL_tTemplateSet_cLabel "cLabel"
#define LANG_FT_tTemplateSet_cLabel "Short label"
#define LANG_FL_tTemplateSet_uOwner "uOwner"
#define LANG_FT_tTemplateSet_uOwner "Record owner"
#define LANG_FL_tTemplateSet_uCreatedBy "uCreatedBy"
#define LANG_FT_tTemplateSet_uCreatedBy "uClient for last insert"
#define LANG_FL_tTemplateSet_uCreatedDate "uCreatedDate"
#define LANG_FT_tTemplateSet_uCreatedDate "Unix seconds date last insert"
#define LANG_FL_tTemplateSet_uModBy "uModBy"
#define LANG_FT_tTemplateSet_uModBy "uClient for last update"
#define LANG_FL_tTemplateSet_uModDate "uModDate"
#define LANG_FT_tTemplateSet_uModDate "Unix seconds date last update"
//tTemplateType
#define LANG_FL_tTemplateType_uTemplateType "uTemplateType"
#define LANG_FT_tTemplateType_uTemplateType "Primary Key"
#define LANG_FL_tTemplateType_cLabel "cLabel"
#define LANG_FT_tTemplateType_cLabel "Short label"
#define LANG_FL_tTemplateType_uOwner "uOwner"
#define LANG_FT_tTemplateType_uOwner "Record owner"
#define LANG_FL_tTemplateType_uCreatedBy "uCreatedBy"
#define LANG_FT_tTemplateType_uCreatedBy "uClient for last insert"
#define LANG_FL_tTemplateType_uCreatedDate "uCreatedDate"
#define LANG_FT_tTemplateType_uCreatedDate "Unix seconds date last insert"
#define LANG_FL_tTemplateType_uModBy "uModBy"
#define LANG_FT_tTemplateType_uModBy "uClient for last update"
#define LANG_FL_tTemplateType_uModDate "uModDate"
#define LANG_FT_tTemplateType_uModDate "Unix seconds date last update"
