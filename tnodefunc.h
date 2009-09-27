/*
FILE
	$Id$
PURPOSE
	Non schema-dependent table and application table related functions.
AUTHOR/LEGAL
	(C) 2001-2009 Unixservice, LLC. GPLv2 license applies.
 
*/

static unsigned uClone=0;
static unsigned uTargetNode=0;
static char cuTargetNodePullDown[256]={""};
static unsigned uWizIPv4=0;
static char cuWizIPv4PullDown[32]={""};


//ModuleFunctionProtos()
void CopyProperties(unsigned uOldNode,unsigned uNewNode,unsigned uType);
void DelProperties(unsigned uNode,unsigned uType);
void tNodeNavList(unsigned uDataCenter);
void tContainerNavList(unsigned uNode);//tcontainerfunc.h
void htmlGroups(unsigned uNode, unsigned uContainer);

//external
//tcontainerfunc.h
void htmlHealth(unsigned uContainer,unsigned uType);
void htmlNodeHealth(unsigned uNode);
unsigned CloneContainerJob(unsigned uDatacenter, unsigned uNode, unsigned uContainer,
				unsigned uTargetNode, unsigned uNewVeid);
unsigned CloneNode(unsigned uSourceNode, unsigned uTargetNode, unsigned uWizIPv4);
char *cRatioColor(float *fRatio);
//tcontainer.c
void tTablePullDownAvail(const char *cTableName, const char *cFieldName,
                        const char *cOrderby, unsigned uSelector, unsigned uMode);

void ExtProcesstNodeVars(pentry entries[], int x)
{
	register int i;
	for(i=0;i<x;i++)
	{
		if(!strcmp(entries[i].name,"uClone")) 
			uClone=1;
		else if(!strcmp(entries[i].name,"cuTargetNodePullDown"))
		{
			sprintf(cuTargetNodePullDown,"%.255s",entries[i].val);
			uTargetNode=ReadPullDown("tNode","cLabel",cuTargetNodePullDown);
		}
		else if(!strcmp(entries[i].name,"cuWizIPv4PullDown"))
		{
			sprintf(cuWizIPv4PullDown,"%.31s",entries[i].val);
			uWizIPv4=ReadPullDown("tIP","cLabel",cuWizIPv4PullDown);
		}
	}

}//void ExtProcesstNodeVars(pentry entries[], int x)


void ExttNodeCommands(pentry entries[], int x)
{

	if(!strcmp(gcFunction,"tNodeTools"))
	{
        	MYSQL_RES *res;
		time_t uActualModDate= -1;

		if(!strcmp(gcCommand,LANG_NB_NEW))
                {
			if(guPermLevel>=9)
			{
	                        ProcesstNodeVars(entries,x);
                        	guMode=2000;
	                        tNode(LANG_NB_CONFIRMNEW);
			}
                }
		else if(!strcmp(gcCommand,LANG_NB_CONFIRMNEW))
                {
			if(guPermLevel>=9)
			{
                        	ProcesstNodeVars(entries,x);
				unsigned uOldNode=uNode;

                        	guMode=2000;
				//Check entries here
				if(strlen(cLabel)<3)
					tNode("<blink>Error</blink>: Must supply valid cLabel. Min 3 chars!");
				if(!uDatacenter)
					tNode("<blink>Error</blink>: Must supply valid uDatacenter!");
				sprintf(gcQuery,"SELECT uNode FROM tNode WHERE cLabel='%s'",
						cLabel);
        			mysql_query(&gMysql,gcQuery);
				if(mysql_errno(&gMysql))
						htmlPlainTextError(mysql_error(&gMysql));
        			res=mysql_store_result(&gMysql);
				if(mysql_num_rows(res))
				{
					mysql_free_result(res);
					tNode("<blink>Error</blink>: Node cLabel is used!");
				}
                        	guMode=0;

				uNode=0;
				uCreatedBy=guLoginClient;
				uOwner=guCompany;
				uStatus=1;//Initially active
				uModBy=0;//Never modified
				uModDate=0;//Never modified
				NewtNode(1);//Come back here uNode should be set
				if(!uNode)
					tNode("<blink>Error</blink>: New node was not created!");

				sprintf(gcQuery,"INSERT INTO tProperty SET uKey=%u,uType=2"
						",cName='Name',cValue='%s',uOwner=%u,uCreatedBy=%u"
						",uCreatedDate=UNIX_TIMESTAMP(NOW())"
							,uNode,cLabel,guCompany,guLoginClient);
				mysql_query(&gMysql,gcQuery);
				if(mysql_errno(&gMysql))
						htmlPlainTextError(mysql_error(&gMysql));

				if(uNode && uClone && uOldNode)
				{
					CopyProperties(uOldNode,uNode,2);
					tNode("New node created and properties copied");
				}
				tNode("New node created");
			}
		}
		else if(!strcmp(gcCommand,LANG_NB_DELETE))
                {
                        ProcesstNodeVars(entries,x);
			if(uAllowDel(uOwner,uCreatedBy))
			{
	                        guMode=0;
				sprintf(gcQuery,"SELECT uNode FROM tContainer WHERE uNode=%u",
									uNode);
        			mysql_query(&gMysql,gcQuery);
				if(mysql_errno(&gMysql))
						htmlPlainTextError(mysql_error(&gMysql));
        			res=mysql_store_result(&gMysql);
				if(mysql_num_rows(res))
				{
					mysql_free_result(res);
					tNode("<blink>Error</blink>: Can't delete a node"
							" used by a container!");
				}
	                        guMode=2001;
				tNode(LANG_NB_CONFIRMDEL);
			}
			else
				tNode("<blink>Error</blink>: Denied by permissions settings");
                }
                else if(!strcmp(gcCommand,LANG_NB_CONFIRMDEL))
                {
                        ProcesstNodeVars(entries,x);
			if(uAllowDel(uOwner,uCreatedBy))
			{
				guMode=5;
				sscanf(ForeignKey("tNode","uModDate",uNode),"%lu",&uActualModDate);
				if(uModDate!=uActualModDate)
					tNode("<blink>Error</blink>: This record was modified. Reload it.");

				sprintf(gcQuery,"SELECT uNode FROM tContainer WHERE uNode=%u",
									uNode);
        			mysql_query(&gMysql,gcQuery);
				if(mysql_errno(&gMysql))
						htmlPlainTextError(mysql_error(&gMysql));
        			res=mysql_store_result(&gMysql);
				if(mysql_num_rows(res))
				{
					mysql_free_result(res);
					tNode("<blink>Error</blink>: Can't delete a node"
							" used by a container!");
				}
	                        guMode=0;
				DelProperties(uNode,2);
				DeletetNode();
			}
			else
				tNode("<blink>Error</blink>: Denied by permissions settings");
                }
		else if(!strcmp(gcCommand,LANG_NB_MODIFY))
                {
                        ProcesstNodeVars(entries,x);
			if(uAllowMod(uOwner,uCreatedBy))
			{
				guMode=2002;
				tNode(LANG_NB_CONFIRMMOD);
			}
			else
				tNode("<blink>Error</blink>: Denied by permissions settings");
                }
                else if(!strcmp(gcCommand,LANG_NB_CONFIRMMOD))
                {
                        ProcesstNodeVars(entries,x);
			if(uAllowMod(uOwner,uCreatedBy))
			{
                        	guMode=2002;
				sscanf(ForeignKey("tNode","uModDate",uNode),"%lu",&uActualModDate);
				if(uModDate!=uActualModDate)
					tNode("<blink>Error</blink>: This record was modified. Reload it.");
				if(strlen(cLabel)<3)
					tNode("<blink>Error</blink>: Must supply valid cLabel. Min 3 chars!");
				if(!uDatacenter)
					tNode("<blink>Error</blink>: Must supply valid uDatacenter!");
                        	guMode=0;

				uModBy=guLoginClient;
				ModtNode();
			}
			else
				tNode("<blink>Error</blink>: Denied by permissions settings");
                }
                else if(!strcmp(gcCommand,"Clone Node Wizard"))
                {
                        ProcesstNodeVars(entries,x);
			if(uStatus==1 && uAllowMod(uOwner,uCreatedBy))
			{
                        	guMode=0;

				sscanf(ForeignKey("tNode","uModDate",uNode),"%lu",&uActualModDate);
				if(uModDate!=uActualModDate)
					tNode("<blink>Error</blink>: This record was modified. Reload it.");
				
				guMode=7001;
				tNode("Select Target Node and IPv4 range");
			}
			else
			{
				tNode("<blink>Error</blink>: Denied by permissions settings");
			}
		}
                else if(!strcmp(gcCommand,"Confirm Clone Node"))
                {
                        ProcesstNodeVars(entries,x);
			if(uStatus==1 && uAllowMod(uOwner,uCreatedBy))
			{
				char cTargetNodeIPv4[32]={""};
				unsigned uRetVal=0;

                        	guMode=0;

				sscanf(ForeignKey("tNode","uModDate",uNode),"%lu",&uActualModDate);
				if(uModDate!=uActualModDate)
					tNode("<blink>Error</blink>: This record was modified. Reload it.");

                        	guMode=7001;
				if(!uWizIPv4)
					tNode("<blink>Error</blink>: You must select a start IP!");
				if(uTargetNode==0)
					tNode("<blink>Error</blink>: Please select a valid target node!");
				if(uTargetNode==uNode)
					tNode("<blink>Error</blink>: Can't clone to same node!");
				GetNodeProp(uTargetNode,"cIPv4",cTargetNodeIPv4);
				if(!cTargetNodeIPv4[0])
					tNode("<blink>Error</blink>: Your target node is"
							" missing it's cIPv4 property!");
                        	guMode=0;

				
				uRetVal=CloneNode(uNode,uTargetNode,uWizIPv4);
				if(uRetVal==1)
					tNode("<blink>Error</blink>: You did not select a valid start IP"
						" no containers for cloning created!");
				else if(uRetVal==2)
					tNode("<blink>Error</blink>: No clone jobs created. Already cloned!");
				else if(uRetVal==3)
					tNode("<blink>Error</blink>: You did not select a valid start IP"
						" only some containers for cloning created!");
				else if(uRetVal)
					tNode("<blink>Error</blink>: Unexpected CloneNode() error!");
				else if(!uRetVal)
					tNode("All clone node jobs created ok");
					

			}
		}
	}

}//void ExttNodeCommands(pentry entries[], int x)


void ExttNodeButtons(void)
{
	OpenFieldSet("tNode Aux Panel",100);
	switch(guMode)
        {
                case 7001:
                        printf("<p><u>Clone Wizard</u><br>");
			printf("Here you will select the hardware node target. If the selected node is"
				" oversubscribed, not available, or scheduled for maintenance. You will"
				" be informed at the next step.\n<p>\n"
				"You also must carefully select a new IP range that usually will be of"
				" the same type as the source container. This is done by selecting a"
				" starting IPv4 from the select below. Please note that enough available"
				" IPs starting at the one given must be available for the node clone operation"
				" to be accomplished. The IP range selected must be contiguous via tIP.uIP.<p>"
				"Any mount/umount files of the source container will not be used"
				" by the new cloned container. This issue will be left for manual"
				" or automated failover to the cloned container. If you wish to"
				" keep the source and clone containers sync'ed you must specify that"
				" in each clone container via a 'cSyncSchedule' entry in it's properties table.<p>");
			printf("Select target node ");
			tTablePullDown("tNode;cuTargetNodePullDown","cLabel","cLabel",uTargetNode,1);
			printf("<br>Select start IPv4 ");
			tTablePullDownAvail("tIP;cuWizIPv4PullDown","cLabel","cLabel",uWizIPv4,1);
			printf("<p><input title='Create clone jobs for all the current node`s containers"
					" that don`t already have clones'"
					" type=submit class=largeButton"
					" name=gcCommand value='Confirm Clone Node'>\n");
                break;

                case 2000:
			printf("<p><u>Enter/mod data</u><br>");
                        printf(LANG_NBB_CONFIRMNEW);
			if(uNode)
				printf("<p>Copy properties <input title='Copies all properties'"
					" type=checkbox name=uClone checked>\n");
                break;

                case 2001:
                        printf("<p><u>Think twice</u><br>");
                        printf(LANG_NBB_CONFIRMDEL);
                break;

                case 2002:
			printf("<p><u>Review changes</u><br>");
                        printf(LANG_NBB_CONFIRMMOD);
                break;

		default:
			printf("<u>Table Tips</u><br>");
			printf("Hardware nodes are defined here. Hardware nodes host containers, and allow"
				" for the autonomic migration to other nodes that may be better suited"
				" at specific points in time to accomplish QoS or other system admin"
				" created policies herein.");
			printf("<p><u>Record Context Info</u><br>");
			if(uDatacenter && uNode)
			{
				printf("Node belongs to ");
				printf("<a class=darkLink href=unxsVZ.cgi?gcFunction=tDatacenter&uDatacenter=%u>%s<a>",
						uDatacenter,ForeignKey("tDatacenter","cLabel",uDatacenter));
				htmlGroups(uNode,0);
			}
			tContainerNavList(uNode);
			tNodeNavList(0);
			if(uNode)
			{
				htmlHealth(uNode,2);
				htmlNodeHealth(uNode);
				printf("<p><input type=submit class=largeButton title='Clone all containers"
					" on this node to another node'"
					" name=gcCommand value='Clone Node Wizard'><br>");
			}
	}
	CloseFieldSet();

}//void ExttNodeButtons(void)


void ExttNodeAuxTable(void)
{
	if(!uNode) return;

        MYSQL_RES *res;
        MYSQL_ROW field;

	sprintf(gcQuery,"%s Property Panel",cLabel);
	OpenFieldSet(gcQuery,100);
	sprintf(gcQuery,"SELECT uProperty,cName,cValue FROM tProperty WHERE uKey=%u AND uType=2 ORDER BY cName",uNode);

        mysql_query(&gMysql,gcQuery);
        if(mysql_errno(&gMysql))
		htmlPlainTextError(mysql_error(&gMysql));

        res=mysql_store_result(&gMysql);
	if(mysql_num_rows(res))
	{
		printf("<table>");
		while((field=mysql_fetch_row(res)))
		{
			printf("<tr>");
			printf("<td width=200 valign=top><a class=darkLink href=unxsVZ.cgi?"
					"gcFunction=tProperty&uProperty=%s&cReturn=tNode_%u>"
					"%s</a></td><td>%s</td>\n",
						field[0],uNode,field[1],field[2]);
			printf("</tr>");
		}
		printf("</table>");
	}

	CloseFieldSet();

}//void ExttNodeAuxTable(void)


void ExttNodeGetHook(entry gentries[], int x)
{
	register int i;

	for(i=0;i<x;i++)
	{
		if(!strcmp(gentries[i].name,"uNode"))
		{
			sscanf(gentries[i].val,"%u",&uNode);
			guMode=6;
		}
	}
	tNode("");

}//void ExttNodeGetHook(entry gentries[], int x)


void ExttNodeSelect(void)
{
	ExtSelect("tNode",VAR_LIST_tNode);

}//void ExttNodeSelect(void)


void ExttNodeSelectRow(void)
{
	ExtSelectRow("tNode",VAR_LIST_tNode,uNode);

}//void ExttNodeSelectRow(void)


void ExttNodeListSelect(void)
{
	char cCat[512];

	ExtListSelect("tNode",VAR_LIST_tNode);
	
	//Changes here must be reflected below in ExttNodeListFilter()
        if(!strcmp(gcFilter,"uNode"))
        {
                sscanf(gcCommand,"%u",&uNode);
		if(guPermLevel<10)
			strcat(gcQuery," AND ");
		else
			strcat(gcQuery," WHERE ");
		sprintf(cCat,"tNode.uNode=%u \
						ORDER BY uNode",
						uNode);
		strcat(gcQuery,cCat);
        }
        else if(1)
        {
                //None NO FILTER
                strcpy(gcFilter,"None");
		strcat(gcQuery," ORDER BY uNode");
        }

}//void ExttNodeListSelect(void)


void ExttNodeListFilter(void)
{
        //Filter
        printf("&nbsp;&nbsp;&nbsp;Filter on ");
        printf("<select name=gcFilter>");
        if(strcmp(gcFilter,"uNode"))
                printf("<option>uNode</option>");
        else
                printf("<option selected>uNode</option>");
        if(strcmp(gcFilter,"None"))
                printf("<option>None</option>");
        else
                printf("<option selected>None</option>");
        printf("</select>");

}//void ExttNodeListFilter(void)


void ExttNodeNavBar(void)
{
	printf(LANG_NBB_SKIPFIRST);
	printf(LANG_NBB_SKIPBACK);
	printf(LANG_NBB_SEARCH);

	if(guPermLevel>=9 && !guListMode)
		printf(LANG_NBB_NEW);

	if(uAllowMod(uOwner,uCreatedBy))
		printf(LANG_NBB_MODIFY);

	if(uAllowDel(uOwner,uCreatedBy))
		printf(LANG_NBB_DELETE);

	if(uOwner)
		printf(LANG_NBB_LIST);

	printf(LANG_NBB_SKIPNEXT);
	printf(LANG_NBB_SKIPLAST);
	printf("&nbsp;&nbsp;&nbsp;\n");

}//void ExttNodeNavBar(void)


void tNodeNavList(unsigned uDatacenter)
{
        MYSQL_RES *res;
        MYSQL_ROW field;

	if(uDatacenter)
		sprintf(gcQuery,"SELECT uNode,cLabel FROM tNode WHERE uDatacenter=%u AND (uOwner=%u OR"
				" uOwner IN (SELECT uClient FROM " TCLIENT
				" WHERE uOwner=%u)) ORDER BY cLabel",
					uDatacenter,guCompany,guCompany);
	else
		sprintf(gcQuery,"SELECT uNode,cLabel FROM tNode WHERE (uOwner=%u OR"
				" uOwner IN (SELECT uClient FROM " TCLIENT
				" WHERE uOwner=%u)) ORDER BY cLabel",
					guCompany,guCompany);
        mysql_query(&gMysql,gcQuery);
        if(mysql_errno(&gMysql))
        {
        	printf("<p><u>tNodeNavList</u><br>\n");
                printf("%s",mysql_error(&gMysql));
                return;
        }

        res=mysql_store_result(&gMysql);
	if(mysql_num_rows(res))
	{	
        	printf("<p><u>tNodeNavList</u><br>\n");

	        while((field=mysql_fetch_row(res)))
			printf("<a class=darkLink href=unxsVZ.cgi?gcFunction=tNode"
				"&uNode=%s>%s</a><br>\n",field[0],field[1]);
	}
        mysql_free_result(res);

}//void tNodeNavList()



void CopyProperties(unsigned uOldNode,unsigned uNewNode,unsigned uType)
{
        MYSQL_RES *res;
        MYSQL_ROW field;

	if(uOldNode==0 || uNewNode==0 || uType==0) return;

	sprintf(gcQuery,"SELECT cName,cValue FROM tProperty WHERE uKey=%u AND uType=%u",
					uOldNode,uType);
        mysql_query(&gMysql,gcQuery);
        if(mysql_errno(&gMysql))
		htmlPlainTextError(mysql_error(&gMysql));
        res=mysql_store_result(&gMysql);
	while((field=mysql_fetch_row(res)))
	{
		sprintf(gcQuery,"INSERT INTO tProperty SET uKey=%u,cName='%s',cValue='%s',uType=%u,uOwner=%u,"
				"uCreatedBy=%u,uCreatedDate=UNIX_TIMESTAMP(NOW())",
					uNewNode,
					field[0],TextAreaSave(field[1]),
					uType,
					guLoginClient,guLoginClient);
        	mysql_query(&gMysql,gcQuery);
        	if(mysql_errno(&gMysql))
			htmlPlainTextError(mysql_error(&gMysql));
	}
	mysql_free_result(res);

}//void CopyProperties(uNode,uNewNode)


void DelProperties(unsigned uNode,unsigned uType)
{
	if(uNode==0 || uType==0) return;
	sprintf(gcQuery,"DELETE FROM tProperty WHERE uType=%u AND uKey=%u",
				uType,uNode);
        mysql_query(&gMysql,gcQuery);
        if(mysql_errno(&gMysql))
		htmlPlainTextError(mysql_error(&gMysql));

}//void DelProperties()


void htmlNodeHealth(unsigned uNode)
{
        MYSQL_RES *res;
        MYSQL_ROW field;
	char *cColor;
	float fRatio;

	//printf("<u>Critical Node Usage Data</u><br>\n");
	//Check fundamental memory constraints of containers per node:
	//The memory gap between privvmpages and the two resource guarantees (vmguarpages and
	// oomguarpages) is not safe to use in an ongoing basis if the sum of all container
	// privvmpages exceeds RAM + swap of the hardware node.
	sprintf(gcQuery,"SELECT SUM(CONVERT(tProperty.cValue,UNSIGNED)) FROM tProperty,tContainer WHERE"
				" tProperty.cName='privvmpages.luMaxheld'"
				" AND tProperty.uType=3"
				" AND tProperty.uKey=tContainer.uContainer"
				" AND tContainer.uStatus!=11"//Probably active not initial setup
				" AND tContainer.uStatus!=31"// and not stopped
				" AND tContainer.uNode=%u",uNode);
        mysql_query(&gMysql,gcQuery);
        if(mysql_errno(&gMysql))
	{
		printf("%s",mysql_error(&gMysql));
		mysql_free_result(res);
		return;
	}
        res=mysql_store_result(&gMysql);
	if((field=mysql_fetch_row(res)))
	{
		char cluPrivvmpagesHeld[256];
		long unsigned luContainerPrivvmpagesMaxHeld=0;
		long unsigned luInstalledRam=0;

		if(field[0]==NULL)
		{
			printf("No data available, no containers?<br>\n");
			goto NextSection;
		}

		GetNodeProp(uNode,"luInstalledRam",cluPrivvmpagesHeld);
		sscanf(field[0],"%lu",&luContainerPrivvmpagesMaxHeld);
		sscanf(cluPrivvmpagesHeld,"%lu",&luInstalledRam);
		if(luInstalledRam==0)
			luInstalledRam=1;

		fRatio= ((float) luContainerPrivvmpagesMaxHeld/ (float) luInstalledRam) * 100.00 ;
		cColor=cRatioColor(&fRatio);
		printf("Max held privvmpages ratio %2.2f%%:"
			" %lu/%lu <font color=%s>[#######]</font><br>\n",
				fRatio,luContainerPrivvmpagesMaxHeld,luInstalledRam,cColor);
	}
NextSection:
	mysql_free_result(res);


	//Inform if hardware node is overcommitted: 1-. cpu power
	sprintf(gcQuery,"SELECT SUM(CONVERT(tProperty.cValue,UNSIGNED)) FROM tProperty,tContainer WHERE"
				" tProperty.cName='vzcpucheck.fCPUUnits'"
				" AND tProperty.uType=3"
				" AND tProperty.uKey=tContainer.uContainer"
				" AND tContainer.uStatus!=11"//Probably active not initial setup
				" AND tContainer.uStatus!=31"// and not stopped
				" AND tContainer.uNode=%u",uNode);
        mysql_query(&gMysql,gcQuery);
        if(mysql_errno(&gMysql))
	{
		printf("%s",mysql_error(&gMysql));
		mysql_free_result(res);
		return;
	}
        res=mysql_store_result(&gMysql);
	if((field=mysql_fetch_row(res)))
	{
		char cfNodeCPUUnits[256];
		float fAllContainerCPUUnits=0.0;
		float fNodeCPUUnits=1.0;

		if(field[0]==NULL)
		{
			printf("No power data available<br>\n");
			goto NextSection2;
		}

		GetNodeProp(uNode,"vzcpucheck-nodepwr.fCPUUnits",cfNodeCPUUnits);
		sscanf(cfNodeCPUUnits,"%f",&fNodeCPUUnits);
		sscanf(field[0],"%f",&fAllContainerCPUUnits);

		fRatio= (fAllContainerCPUUnits/fNodeCPUUnits) * 100.00 ;
		cColor=cRatioColor(&fRatio);
		printf("Container/Node power %2.2f%%:"
			" %2.2f/%2.2f <font color=%s>[#######]</font><br>\n",
				fRatio,fAllContainerCPUUnits,fNodeCPUUnits,cColor);
	}
NextSection2:
	mysql_free_result(res);

	//Check all node activity via tProperty
	sprintf(gcQuery,"SELECT tNode.cLabel,FROM_UNIXTIME(MAX(tProperty.uModDate)),"
			"(UNIX_TIMESTAMP(NOW()) - MAX(tProperty.uModDate) > 300 ) FROM"
			" tProperty,tNode WHERE tProperty.uKey=tNode.uNode AND"
			" tProperty.uType=2 GROUP BY tProperty.uKey");
        mysql_query(&gMysql,gcQuery);
        if(mysql_errno(&gMysql))
	{
		printf("%s",mysql_error(&gMysql));
		mysql_free_result(res);
		return;
	}
        res=mysql_store_result(&gMysql);
	if(mysql_num_rows(res)>0)
		printf("<u>Node Roll Call</u><br>\n");
	while((field=mysql_fetch_row(res)))
	{
		printf("<font color=");
		if(field[2][0]=='1') 
			printf("red>");
		else
			printf("black>");
		printf("%s last contact: %s</font><br>\n",field[0],field[1]);
	}
	mysql_free_result(res);

}//void htmlNodeHealth(unsigned uNode)
