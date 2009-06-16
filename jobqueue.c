/*
FILE
	jobqueue.c
	$Id$
PURPOSE
	Command line processing of jobs in the tJob queue.
AUTHOR
	Gary Wallis for Unxiservice (C) 2008-2009. GPL2 License applies.
NOTES
	1-. Most if not all void functions should report then exit on failure.
	2-. Skip all jobs that have errors, we try to process as many as possible.
TODO
	1-. Set a running job status.
*/

#include "mysqlrad.h"
#include <openisp/template.h>

#define mysqlrad_Query_TextErr_Exit	mysql_query(&gMysql,gcQuery);\
					if(mysql_errno(&gMysql))\
					{\
						printf("%s\n",mysql_error(&gMysql));\
						exit(2);\
					}

//external protos
void TextConnectDb(void); //main.c

//local protos
void ProcessJobQueue(void);
void ProcessJob(unsigned uJob,unsigned uDatacenter,unsigned uNode,
		unsigned uContainer,char *cJobName,char *cJobData);
void tJobErrorUpdate(unsigned uJob, char *cErrorMsg);
void tJobDoneUpdate(unsigned uJob);
void InstallConfigFile(unsigned uJob,unsigned uContainer,char *cJobData);
void ChangeIPContainer(unsigned uJob,unsigned uContainer,char *cJobData);
void ChangeHostnameContainer(unsigned uJob,unsigned uContainer);
void MountFilesContainer(unsigned uJob,unsigned uContainer);
void NewContainer(unsigned uJob,unsigned uContainer);
void DestroyContainer(unsigned uJob,unsigned uContainer);
void StopContainer(unsigned uJob,unsigned uContainer);
void StartContainer(unsigned uJob,unsigned uContainer);
void MigrateContainer(unsigned uJob,unsigned uContainer,char *cJobData);
void CloneContainer(unsigned uJob,unsigned uContainer,char *cJobData);
void GetNodeProp(const unsigned uNode,const char *cName,char *cValue);
void GetGroupProp(const unsigned uGroup,const char *cName,char *cValue);
void GetContainerProp(const unsigned uContainer,const char *cName,char *cValue);
void UpdateContainerUBC(unsigned uJob,unsigned uContainer,const char *cJobData);
void SetContainerUBC(unsigned uJob,unsigned uContainer,const char *cJobData);
void TemplateContainer(unsigned uJob,unsigned uContainer,const char *cJobData);

//extern protos
void SetContainerStatus(unsigned uContainer,unsigned uStatus);
void SetContainerNode(unsigned uContainer,unsigned uNode);


//Using the local server hostname get max 100 jobs for this node from the tJob queue.
//Then dispatch jobs via ProcessJob() this function in turn calls specific functions for
//each known cJobName.
static char cHostname[100]={""};//file scope
void ProcessJobQueue(void)
{
        MYSQL_RES *res;
        MYSQL_ROW field;
	unsigned uDatacenter=0;
	unsigned uNode=0;
	unsigned uContainer=0;
	unsigned uJob=0;
	time_t luClock;

	if(gethostname(cHostname,99)!=0)
	{
		printf("gethostname() failed: aborted\n");
		exit(1);
	}

	TextConnectDb();//Uses login data from local.h
	guLoginClient=1;//Root user

	sprintf(gcQuery,"SELECT uNode,uDatacenter FROM tNode WHERE cLabel='%.99s'",cHostname);
	mysql_query(&gMysql,gcQuery);
	if(mysql_errno(&gMysql))
	{
		printf("%s\n",mysql_error(&gMysql));
		exit(2);
	}
        res=mysql_store_result(&gMysql);
	if((field=mysql_fetch_row(res)))
	{
		sscanf(field[0],"%u",&uNode);
		sscanf(field[1],"%u",&uDatacenter);
	}
	mysql_free_result(res);


	if(!uNode)
	{
		printf("could not determine uNode: aborted\n");
		exit(1);
	}

	//debug only
	//printf("ProcessJobQueue() for %s (uNode=%u,uDatacenter=%u)\n",
	//		cHostname,uNode,uDatacenter);

	//Main loop
	time(&luClock);
	sprintf(gcQuery,"SELECT uJob,uContainer,cJobName,cJobData FROM tJob WHERE uJobStatus=1"
				" AND uDatacenter=%u AND uNode=%u"
				" AND uJobDate<=%lu LIMIT 100",
						uDatacenter,uNode,luClock);
	mysql_query(&gMysql,gcQuery);
	if(mysql_errno(&gMysql))
	{
		printf("%s\n",mysql_error(&gMysql));
		exit(2);
	}
        res=mysql_store_result(&gMysql);
	while((field=mysql_fetch_row(res)))
	{
		sscanf(field[0],"%u",&uJob);
		sscanf(field[1],"%u",&uContainer);
		//Job dispatcher based on cJobName
		ProcessJob(uJob,uDatacenter,uNode,uContainer,field[2],field[3]);
	}
	mysql_free_result(res);


	//debug only
	//printf("ProcessJobQueue() End\n");
	exit(0);

}//void ProcessJobQueue(void)



void ProcessJob(unsigned uJob,unsigned uDatacenter,unsigned uNode,
		unsigned uContainer,char *cJobName,char *cJobData)
{
	static unsigned uCount=0;

	//Some jobs may take quite some time, we need to make sure we don't run again!
	sprintf(gcQuery,"UPDATE tJob SET uJobStatus=2,cRemoteMsg='Running',uModBy=1,"
				"uModDate=UNIX_TIMESTAMP(NOW()) WHERE uJob=%u",uJob);
	mysql_query(&gMysql,gcQuery);
	if(mysql_errno(&gMysql))
		printf("(%s) but we continue...\n",mysql_error(&gMysql));


	//if debug
	printf("%3.3u uJob=%u uContainer=%u cJobName=%s; cJobData=%s;\n",
			uCount++,uJob,uContainer,cJobName,cJobData);

	if(!strcmp(cJobName,"MigrateContainer"))
	{
		MigrateContainer(uJob,uContainer,cJobData);
	}
	else if(!strcmp(cJobName,"CloneContainer"))
	{
		CloneContainer(uJob,uContainer,cJobData);
	}
	else if(!strcmp(cJobName,"NewContainer"))
	{
		NewContainer(uJob,uContainer);
	}
	else if(!strcmp(cJobName,"MountFilesContainer"))
	{
		MountFilesContainer(uJob,uContainer);
	}
	else if(!strcmp(cJobName,"StartContainer"))
	{
		StartContainer(uJob,uContainer);
	}
	else if(!strcmp(cJobName,"ChangeHostnameContainer"))
	{
		ChangeHostnameContainer(uJob,uContainer);
	}
	else if(!strcmp(cJobName,"ChangeIPContainer"))
	{
		ChangeIPContainer(uJob,uContainer,cJobData);
	}
	else if(!strcmp(cJobName,"StopContainer"))
	{
		StopContainer(uJob,uContainer);
	}
	else if(!strcmp(cJobName,"DestroyContainer"))
	{
		DestroyContainer(uJob,uContainer);
	}
	else if(!strcmp(cJobName,"UpdateContainerUBCJob"))
	{
		UpdateContainerUBC(uJob,uContainer,cJobData);
	}
	else if(!strcmp(cJobName,"SetUBCJob"))
	{
		SetContainerUBC(uJob,uContainer,cJobData);
	}
	else if(!strcmp(cJobName,"TemplateContainer"))
	{
		TemplateContainer(uJob,uContainer,cJobData);
	}
	else if(!strcmp(cJobName,"InstallConfigFile"))
	{
		InstallConfigFile(uJob,uContainer,cJobData);
	}
	else if(1)
	{
		printf("Not-implemented cJobName=%s skipping...\n",cJobName);
		tJobErrorUpdate(uJob,cJobName);
	}

}//ProcessJob(...)


//Shared functions


void tJobErrorUpdate(unsigned uJob, char *cErrorMsg)
{
	sprintf(gcQuery,"UPDATE tJob SET uJobStatus=14,cRemoteMsg='%.31s',uModBy=1,"
				"uModDate=UNIX_TIMESTAMP(NOW()) WHERE uJob=%u",
									cErrorMsg,uJob);
	mysql_query(&gMysql,gcQuery);
	if(mysql_errno(&gMysql))
	{
		printf("%s\n",mysql_error(&gMysql));
		exit(2);
	}

	sprintf(gcQuery,"INSERT INTO tLog SET"
		" cLabel='unxsVZ.cgi ProcessJobQueue Error',"
		"uLogType=4,uLoginClient=1,"
		"cLogin='unxsVZ.cgi',cMessage=\"%s uJob=%u\","
		"cServer='%s',uOwner=1,uCreatedBy=1,"
		"uCreatedDate=UNIX_TIMESTAMP(NOW())",
					cErrorMsg,uJob,cHostname);
	mysql_query(&gMysql,gcQuery);
	if(mysql_errno(&gMysql))
	{
		printf("%s\n",mysql_error(&gMysql));
		exit(2);
	}

}//void tJobErrorUpdate(unsigned uJob, char *cErrorMsg)


void tJobDoneUpdate(unsigned uJob)
{
	sprintf(gcQuery,"UPDATE tJob SET uJobStatus=3,uModBy=1,cRemoteMsg='tJobDoneUpdate() ok',"
				"uModDate=UNIX_TIMESTAMP(NOW()) WHERE uJob=%u",uJob);
	mysql_query(&gMysql,gcQuery);
	if(mysql_errno(&gMysql))
	{
		printf("%s\n",mysql_error(&gMysql));
		exit(2);
	}

}//void tJobDoneUpdate(unsigned uJob, char *cErrorMsg)


//Specific job handlers


void InstallConfigFile(unsigned uJob,unsigned uContainer,char *cJobData)
{
        MYSQL_RES *res;
        MYSQL_ROW field;
	unsigned uConfiguration=0;
	char cDir[32]={""};
	char *cp;

	if((cp=strstr(cJobData,"uConfiguration=")))
		sscanf(cp+15,"%u",&uConfiguration);

	if(!uConfiguration)
	{
		printf("InstallConfigFile() error: No uConfiguration. Skipping...\n");
		tJobErrorUpdate(uJob,"uConfiguration=0");
	}

	//debug only
	printf("InstallConfigFile() uConfiguration=%u\n",uConfiguration);

	if(uContainer)
	{
		struct stat statInfo;

		if(uContainer>100 && uContainer<99999)
		{
			sprintf(cDir,"/vz/private/%u",uContainer);
		}
		else
		{
			printf("InstallConfigFile() error: Out of range uContainer=%u.\n",uContainer);
			tJobErrorUpdate(uJob,"Outofrange uContainer");
			return;
		}

		if(stat(cDir,&statInfo)!=0)
		{
			printf("InstallConfigFile() error: No %s.\n",cDir);
			tJobErrorUpdate(uJob,cDir);
			return;
		}
	}


	sprintf(gcQuery,"SELECT cLabel,cValue,cComment FROM tConfiguration WHERE uConfiguration=%u",
				uConfiguration);
	mysql_query(&gMysql,gcQuery);
	if(mysql_errno(&gMysql))
	{
		printf("%s\n",mysql_error(&gMysql));
		exit(2);
	}
        res=mysql_store_result(&gMysql);
	if((field=mysql_fetch_row(res)))
	{
		char *cp2;
		char cSystem[256]={""};
		char cOwnerGroup[100]={""};
		char cPostOp[100]={""};
		unsigned uFilePerms=0;
		unsigned uError=0;
		char cFilename[256]={""};
		FILE *fp;

		//Parse cLabel file;root:root;400;none;
		if((cp=strstr(field[0],"file;")))
		{
			//find end of cOwnerGroup
			if((cp2=strchr(cp+6,';')))
			{
				*cp2=0;
				sprintf(cOwnerGroup,"%.99s",cp+5);
				//find end of uFilePerms
				if((cp=strchr(cp2+1,';')))
					sscanf(cp2+1,"%u",&uFilePerms);
				//find end of cPostOp
				if((cp2=strchr(cp+1,';')))
				{
					*cp2=0;
					sprintf(cPostOp,"%.99s",cp+1);
				}
			}
		}
		//debug only
		printf("InstallConfigFile() cOwnerGroup=%s; uFilePerms=%u cPostOp=%s; cFilename=%s;\n",
			cOwnerGroup,uFilePerms,cPostOp,cFilename);

		//Install file
		if((fp=fopen(cFilename,"w"))==NULL)
		{
			printf("InstallConfigFile() error: Can't install %s.\n",cFilename);
			tJobErrorUpdate(uJob,cFilename);
			return;//Do not continue if we can't even open file
		}
		//we may need to filter out PC cr-lf's
		fprintf(fp,"%s\n",field[2]);
		if(fclose(fp))
		{
			printf("InstallConfigFile() error: fclose() %s.\n",cFilename);
			tJobErrorUpdate(uJob,cFilename);
			return;//Do not continue if we can't close the file.
		}

		sprintf(cSystem,"/bin/chown %s %s",cOwnerGroup,cFilename);
		if(system(cSystem))
		{
			printf("InstallConfigFile() error: %s.\n",cSystem);
			tJobErrorUpdate(uJob,cSystem);
			uError++;
		}

		sprintf(cSystem,"/bin/chmod %u %s",uFilePerms,cFilename);
		if(system(cSystem))
		{
			printf("InstallConfigFile() error: %s.\n",cSystem);
			tJobErrorUpdate(uJob,cSystem);
			uError++;
		}

		//Do not run post file install command if none or if chown and/or chmod failed
		if(strcmp(cPostOp,"none") && !uError)
		{
			//Limits on cPostOP:
			//The post install command must return 0 if everything went ok;
			if(system(cPostOp))
			{
				printf("InstallConfigFile() error: %s.\n",cPostOp);
				tJobErrorUpdate(uJob,cPostOp);
				return;
			}
		}

		if(!uError)
			tJobDoneUpdate(uJob);
	}
	mysql_free_result(res);


}//void InstallConfigFile(...)


void NewContainer(unsigned uJob,unsigned uContainer)
{
        MYSQL_RES *res;
        MYSQL_ROW field;

	sprintf(gcQuery,"SELECT tContainer.cLabel,tContainer.cHostname,tIP.cLabel"
			",tOSTemplate.cLabel,tNameserver.cLabel,tSearchdomain.cLabel,tConfig.cLabel"
			" FROM tContainer,tOSTemplate,tNameserver,tSearchdomain,tConfig,tIP WHERE uContainer=%u"
			" AND tContainer.uOSTemplate=tOSTemplate.uOSTemplate"
			" AND tContainer.uNameserver=tNameserver.uNameserver"
			" AND tContainer.uConfig=tConfig.uConfig"
			" AND tContainer.uIPv4=tIP.uIP"
			" AND tContainer.uSearchdomain=tSearchdomain.uSearchdomain",uContainer);
	mysql_query(&gMysql,gcQuery);
	if(mysql_errno(&gMysql))
	{
		printf("%s\n",mysql_error(&gMysql));
		exit(2);
	}
        res=mysql_store_result(&gMysql);
	if((field=mysql_fetch_row(res)))
	{


		//0-. Create VEID.mount and VEID.umount files if container so specifies via tProperty
		//	and everything needed is available: template and template vars from tProperty.
        	MYSQL_RES *res2;
	        MYSQL_ROW field2;
		FILE *fp;
		char cTemplateName[256]={""};//required
		char cFile[100]={""};//required
		char cVeID[32]={""};//required
		char cService1[256]={"80"};//default
		char cService2[256]={"443"};//default
		char cNodeIP[256]={""};//required
		char cNetmask[256]={"255.255.255.0"};//default
		char cPrivateIPs[256]={"10.0.0.0/24"};//default

		sprintf(cVeID,"%u",uContainer);	

		GetContainerProp(uContainer,"cVEID.mount",cTemplateName);
		if(cTemplateName[0])
		{
			GetContainerProp(uContainer,"cNodeIP",cNodeIP);
			GetContainerProp(uContainer,"cNetmask",cNetmask);
			GetContainerProp(uContainer,"cPrivateIPs",cPrivateIPs);
			GetContainerProp(uContainer,"cService1",cService1);
			GetContainerProp(uContainer,"cService2",cService2);

			sprintf(cFile,"/etc/vz/conf/%u.mount",uContainer);
			if((fp=fopen(cFile,"w"))==NULL)
			{
				printf("NewContainer() error: %s\n",cFile);
				tJobErrorUpdate(uJob,"VEID.mount create failed");
				goto CommonExit;
			}

			TemplateSelect(cTemplateName);
			res2=mysql_store_result(&gMysql);
			if((field2=mysql_fetch_row(res2)))
			{
				struct t_template template;

				template.cpName[0]="cNodeIP";
				template.cpValue[0]=cNodeIP;
						
				template.cpName[1]="cNetmask";
				template.cpValue[1]=cNetmask;
						
				template.cpName[2]="cPrivateIPs";
				template.cpValue[2]=cPrivateIPs;
						
				template.cpName[3]="cVeID";
				template.cpValue[3]=cVeID;

				template.cpName[4]="cService1";
				template.cpValue[4]=cService1;
						
				template.cpName[5]="cService2";
				template.cpValue[5]=cService2;
						
				template.cpName[6]="";
				Template(field2[0],&template,fp);
			}
			mysql_free_result(res2);
			fclose(fp);
			chmod(cFile,S_IRUSR|S_IWUSR|S_IXUSR);

			cTemplateName[0]=0;
			GetContainerProp(uContainer,"cVEID.umount",cTemplateName);
			if(cTemplateName[0])
			{
				sprintf(cFile,"/etc/vz/conf/%u.umount",uContainer);
				if((fp=fopen(cFile,"w"))==NULL)
				{
					printf("NewContainer() error: %s\n",cFile);
					tJobErrorUpdate(uJob,"VEID.umount create failed");
					goto CommonExit;
				}
	
				TemplateSelect(cTemplateName);
				res2=mysql_store_result(&gMysql);
				if((field2=mysql_fetch_row(res2)))
				{
					struct t_template template;
	
					template.cpName[0]="cNodeIP";
					template.cpValue[0]=cNodeIP;
							
					template.cpName[1]="cNetmask";
					template.cpValue[1]=cNetmask;
							
					template.cpName[2]="cPrivateIPs";
					template.cpValue[2]=cPrivateIPs;
							
					template.cpName[3]="cVeID";
					template.cpValue[3]=cVeID;
	
					template.cpName[4]="cService1";
					template.cpValue[4]=cService1;
							
					template.cpName[5]="cService2";
					template.cpValue[5]=cService2;
							
					template.cpName[6]="";
					Template(field2[0],&template,fp);
				}
				mysql_free_result(res2);
				fclose(fp);
				chmod(cFile,S_IRUSR|S_IWUSR|S_IXUSR);
			}
			else
			{
				printf("NewContainer() error: %s missing\n",cTemplateName);
				tJobErrorUpdate(uJob,"Missing VEID.umount!");
				goto CommonExit;
			}
		}


		//vzctl [flags] create veid --ostemplate name] [--config name] [--private path] 
		//[--root path] [--ipadd addr] [--hostname name]
		//1-.
		sprintf(gcQuery,"/usr/sbin/vzctl --verbose create %u --ostemplate %.32s --hostname %.32s --ipadd %.15s --name %.32s --config %.32s",
				uContainer,field[3],field[1],field[2],field[0],field[6]);
		if(system(gcQuery))
		{
			printf("NewContainer() error: %s\n",gcQuery);
			tJobErrorUpdate(uJob,"vzctl create failed");
			goto CommonExit;
		}

		//2-.
		sprintf(gcQuery,"/usr/sbin/vzctl --verbose set %u --nameserver \"%.99s\" --searchdomain %.32s --save",
				uContainer,field[4],field[5]);
		if(system(gcQuery))
		{
			printf("NewContainer() error: %s\n",gcQuery);
			tJobErrorUpdate(uJob,"vzctl set failed");
			//Roll back step 1-.
			sprintf(gcQuery,"/usr/sbin/vzctl destroy %u",uContainer);
			if(system(gcQuery))
			{
				printf("NewContainer() error: %s\n",gcQuery);
				tJobErrorUpdate(uJob,"rb: vzctl destroy failed");
			}
			goto CommonExit;
		}

		//3-.
		sprintf(gcQuery,"/usr/sbin/vzctl --verbose start %u",uContainer);
		if(system(gcQuery))
		{
			printf("NewContainer() error: %s\n",gcQuery);
			tJobErrorUpdate(uJob,"vzctl start failed");
			//Roll back step 1-.
			sprintf(gcQuery,"/usr/sbin/vzctl destroy %u",uContainer);
			if(system(gcQuery))
			{
				printf("NewContainer() error: %s\n",gcQuery);
				tJobErrorUpdate(uJob,"rb: vzctl destroy failed");
			}
			goto CommonExit;
		}
	}
	else
	{
		printf("NewContainer() error: Select for %u failed.\n",uContainer);
		tJobErrorUpdate(uJob,"Select failed");
		goto CommonExit;
	}

	//Everything went ok;
	SetContainerStatus(uContainer,1);//Active
	tJobDoneUpdate(uJob);

CommonExit:
	mysql_free_result(res);


}//void NewContainer(...)


void DestroyContainer(unsigned uJob,unsigned uContainer)
{

	//1-.
	sprintf(gcQuery,"/usr/sbin/vzctl --verbose stop %u",uContainer);
	if(system(gcQuery))
	{
		printf("DestroyContainer() error: %s\n",gcQuery);
		tJobErrorUpdate(uJob,"vzctl stop failed");
		goto CommonExit;
	}

	//2-.
	sprintf(gcQuery,"/usr/sbin/vzctl --verbose destroy %u",uContainer);
	if(system(gcQuery))
	{
		printf("DestroyContainer() error: %s\n",gcQuery);
		tJobErrorUpdate(uJob,"vzctl destroy failed");
		goto CommonExit;
	}

	//Everything ok
	SetContainerStatus(uContainer,11);//Initial
	tJobDoneUpdate(uJob);

CommonExit:
	return;

}//void DestroyContainer(...)


void ChangeIPContainer(unsigned uJob,unsigned uContainer,char *cJobData)
{

	char cIPOld[256]={""};
	char cIPNew[31]={""};
	char *cp;
        MYSQL_RES *res;
        MYSQL_ROW field;

	//0-. Get required data
	sprintf(gcQuery,"SELECT tIP.cLabel"
			" FROM tContainer,tIP WHERE uContainer=%u"
			" AND tContainer.uIPv4=tIP.uIP",uContainer);
	mysql_query(&gMysql,gcQuery);
	if(mysql_errno(&gMysql))
	{
		printf("%s\n",mysql_error(&gMysql));
		exit(2);
	}
	res=mysql_store_result(&gMysql);
	if((field=mysql_fetch_row(res)))
		sprintf(cIPNew,"%.31s",field[0]);
	if(!cIPNew[0])	
	{
		printf("ChangeIPContainer() error 1\n");
		tJobErrorUpdate(uJob,"Get cIPNew failed");
		goto CommonExit;
	}
	sscanf(cJobData,"cIPOld=%31s;",cIPOld);
	if((cp=strchr(cIPOld,';')))
		*cp=0;
	if(!cIPOld[0])	
	{
		printf("ChangeIPContainer() error 2\n");
		tJobErrorUpdate(uJob,"Get cIPOld failed");
		goto CommonExit;
	}

	//0-.
	sprintf(gcQuery,"/usr/sbin/vzctl --verbose stop %u",uContainer);
	if(system(gcQuery))
	{
		printf("ChangeIPContainer() error: %s\n",gcQuery);
		tJobErrorUpdate(uJob,"vzctl stop failed");
		goto CommonExit;
	}

	//1-.
	sprintf(gcQuery,"/usr/sbin/vzctl --verbose set %u --ipadd %s --save",uContainer,cIPNew);
	if(system(gcQuery))
	{
		printf("ChangeIPContainer() error: %s\n",gcQuery);
		tJobErrorUpdate(uJob,"vzctl set ipadd failed");
		goto CommonExit;
	}


	//2-.
	sprintf(gcQuery,"/usr/sbin/vzctl --verbose set %u --ipdel %s --save",uContainer,cIPOld);
	if(system(gcQuery))
	{
		printf("ChangeIPContainer() error: %s\n",gcQuery);
		tJobErrorUpdate(uJob,"vzctl set ipdel failed");
		goto CommonExit;
	}

	//3-.
	sprintf(gcQuery,"UPDATE tIP SET uAvailable=1 WHERE cLabel='%.31s'",cIPOld);
	mysql_query(&gMysql,gcQuery);
	if(mysql_errno(&gMysql))
	{
		printf("%s\n",mysql_error(&gMysql));
		tJobErrorUpdate(uJob,"Release old IP from tIP failed");
		goto CommonExit;
	}

	//4-.
	sprintf(gcQuery,"/usr/sbin/vzctl --verbose start %u",uContainer);
	if(system(gcQuery))
	{
		printf("ChangeIPContainer() error: %s\n",gcQuery);
		tJobErrorUpdate(uJob,"vzctl start failed");
		goto CommonExit;
	}


	//Everything ok
	SetContainerStatus(uContainer,1);//Active
	tJobDoneUpdate(uJob);

CommonExit:
	mysql_free_result(res);
	return;

}//void ChangeIPContainer(...)


void ChangeHostnameContainer(unsigned uJob,unsigned uContainer)
{
	char cHostname[100]={""};
	char cName[100]={""};
        MYSQL_RES *res;
        MYSQL_ROW field;

	sprintf(gcQuery,"SELECT tContainer.cLabel,tContainer.cHostname"
			" FROM tContainer WHERE uContainer=%u",uContainer);
	mysql_query(&gMysql,gcQuery);
	if(mysql_errno(&gMysql))
	{
		printf("%s\n",mysql_error(&gMysql));
		exit(2);
	}
        res=mysql_store_result(&gMysql);
	if((field=mysql_fetch_row(res)))
	{
		sprintf(cName,"%.99s",field[0]);
		sprintf(cHostname,"%.99s",field[1]);
	}

	if(!cHostname[0] || !cName[0])	
	{
		printf("ChangeHostnameContainer() error: %s\n",gcQuery);
		tJobErrorUpdate(uJob,"Get cHostname failed");
		goto CommonExit;
	}

	//1-.
	sprintf(gcQuery,"/usr/sbin/vzctl --verbose set %u --hostname %s --name %s --save",
				uContainer,cHostname,cName);
	if(system(gcQuery))
	{
		printf("ChangeHostnameContainer() error: %s\n",gcQuery);
		tJobErrorUpdate(uJob,"vzctl set hostname failed");
		goto CommonExit;
	}


	//Everything ok
	SetContainerStatus(uContainer,1);//Active
	tJobDoneUpdate(uJob);

CommonExit:
	mysql_free_result(res);
	return;

}//void ChangeHostnameContainer(...)


void StopContainer(unsigned uJob,unsigned uContainer)
{

	//1-.
	sprintf(gcQuery,"/usr/sbin/vzctl --verbose stop %u",uContainer);
	if(system(gcQuery))
	{
		printf("StopContainer() error: %s\n",gcQuery);
		tJobErrorUpdate(uJob,"vzctl stop failed");
		goto CommonExit;
	}

	//Everything ok
	SetContainerStatus(uContainer,31);//Stopped
	tJobDoneUpdate(uJob);

CommonExit:
	return;

}//void StopContainer(...)


void StartContainer(unsigned uJob,unsigned uContainer)
{

	//1-.
	sprintf(gcQuery,"/usr/sbin/vzctl --verbose start %u",uContainer);
	if(system(gcQuery))
	{
		sprintf(gcQuery,"/usr/sbin/vzctl --verbose restart %u",uContainer);
		if(system(gcQuery))
		{
			printf("StartContainer() error: %s\n",gcQuery);
			tJobErrorUpdate(uJob,"vzctl start/restart failed");
			goto CommonExit;
		}
	}

	//Everything ok
	SetContainerStatus(uContainer,1);//Active
	tJobDoneUpdate(uJob);

CommonExit:
	return;

}//void StartContainer(...)


void MigrateContainer(unsigned uJob,unsigned uContainer,char *cJobData)
{
	char cTargetNodeIPv4[256]={""};
	unsigned uTargetNode=0;

	sscanf(cJobData,"uTargetNode=%u;",&uTargetNode);
	if(!uTargetNode)
	{
		printf("MigrateContainer() error: Could not determine uTargetNode\n");
		tJobErrorUpdate(uJob,"uTargetNode==0");
		goto CommonExit;
	}

	GetNodeProp(uTargetNode,"IPv4",cTargetNodeIPv4);
	if(!cTargetNodeIPv4[0])
	{
		printf("MigrateContainer() error: Could not determine cTargetNodeIPv4. uTargetNode=%u;\n",uTargetNode);
		tJobErrorUpdate(uJob,"cTargetNodeIPv4");
		goto CommonExit;
	}

	//vzmigrate --online -v <destination_address> <veid>
	sprintf(gcQuery,"export PATH=/usr/sbin:/usr/bin:/bin;"
				"/usr/sbin/vzmigrate --keep-dst --online -v %s %u",cTargetNodeIPv4,uContainer);
	if(system(gcQuery))
	{
		printf("MigrateContainer() error: %s.\nTrying offline migration (check kernel compat)...\n",gcQuery);
		tJobErrorUpdate(uJob,"Live failed trying offline");

		sprintf(gcQuery,"export PATH=/usr/sbin:/usr/bin:/bin:/usr/local/bin:/usr/local/sbin;"
				"/usr/sbin/vzmigrate -v %s %u",cTargetNodeIPv4,uContainer);
		if(system(gcQuery))
		{
			printf("MigrateContainer() error: %s.\nGiving up!\n",gcQuery);
			tJobErrorUpdate(uJob,"vzmigrate on/off-line failed");
			goto CommonExit;
		}
	}

	//Everything ok
	SetContainerStatus(uContainer,1);//Active
	SetContainerNode(uContainer,uTargetNode);//Migrated!
	tJobDoneUpdate(uJob);

CommonExit:
	return;

}//void MigrateContainer(...)


void GetNodeProp(const unsigned uNode,const char *cName,char *cValue)
{
        MYSQL_RES *res;
        MYSQL_ROW field;

	if(uNode==0) return;

	sprintf(gcQuery,"SELECT cValue FROM tProperty WHERE uKey=%u AND uType=2 AND cName='%s'",
				uNode,cName);
	mysql_query(&gMysql,gcQuery);
	if(mysql_errno(&gMysql))
	{
		printf("%s\n",mysql_error(&gMysql));
		exit(2);
	}
        res=mysql_store_result(&gMysql);
	if((field=mysql_fetch_row(res)))
	{
		char *cp;
		if((cp=strchr(field[0],'\n')))
			*cp=0;
		sprintf(cValue,"%.255s",field[0]);
	}
	mysql_free_result(res);

}//void GetNodeProp(...)


void GetGroupProp(const unsigned uGroup,const char *cName,char *cValue)
{
        MYSQL_RES *res;
        MYSQL_ROW field;

	if(uGroup==0) return;

	sprintf(gcQuery,"SELECT cValue FROM tProperty WHERE uKey=%u AND uType=11 AND cName='%s'",
				uGroup,cName);
	mysql_query(&gMysql,gcQuery);
	if(mysql_errno(&gMysql))
	{
		printf("%s\n",mysql_error(&gMysql));
		exit(2);
	}
        res=mysql_store_result(&gMysql);
	if((field=mysql_fetch_row(res)))
	{
		char *cp;
		if((cp=strchr(field[0],'\n')))
			*cp=0;
		sprintf(cValue,"%.255s",field[0]);
	}
	mysql_free_result(res);

}//void GetGroupProp(...)


void GetContainerProp(const unsigned uContainer,const char *cName,char *cValue)
{
        MYSQL_RES *res;
        MYSQL_ROW field;

	if(uContainer==0) return;

	sprintf(gcQuery,"SELECT cValue FROM tProperty WHERE uKey=%u AND uType=3 AND cName='%s'",
				uContainer,cName);
	mysql_query(&gMysql,gcQuery);
	if(mysql_errno(&gMysql))
	{
		printf("%s\n",mysql_error(&gMysql));
		exit(2);
	}
        res=mysql_store_result(&gMysql);
	if((field=mysql_fetch_row(res)))
	{
		char *cp;
		if((cp=strchr(field[0],'\n')))
			*cp=0;
		sprintf(cValue,"%.255s",field[0]);
	}
	mysql_free_result(res);

}//void GetContainerProp(...)


void UpdateContainerUBC(unsigned uJob,unsigned uContainer,const char *cJobData)
{

	float luBar=0,luLimit=0;
	char cBuf[256]={""};
	char cResource[256]={""};
	char cApplyConfigPath[100]={""};
	char cContainerPath[100]={""};
	char cUBC[64]={""};
	char *cp;

	sscanf(cJobData,"cResource=%32s;",cResource);
	if((cp=strchr(cResource,';')))
		*cp=0;
	if(!cResource[0])
	{
		printf("UpdateContainerUBC() error: Could not determine cResource\n");
		tJobErrorUpdate(uJob,"cResource[0]==0");
		goto CommonExit;
	}

	sprintf(cUBC,"%.32s.luBarrier",cResource);
	GetContainerProp(uContainer,cUBC,cBuf);
	sscanf(cBuf,"%f",&luBar);

	sprintf(cUBC,"%.32s.luLimit",cResource);
	GetContainerProp(uContainer,cUBC,cBuf);
	sscanf(cBuf,"%f",&luLimit);

	//No PID control yet. 10% increase
	//No resource based rules
	//No node resources based adjustments
	//No expert per resource rules applied
	luBar = luBar + luBar * 0.1;
	luLimit = luLimit + luLimit * 0.1;

	//1-.
	sprintf(gcQuery,"/usr/sbin/vzctl set %u --%s %.0f:%.0f --save",
		uContainer,cResource,luBar,luLimit);
	if(system(gcQuery))
	{
		printf("UpdateContainerUBC() error: %s\n",gcQuery);
		tJobErrorUpdate(uJob,"vzctl set failed");
		goto CommonExit;
	}

	//2-. validate and/or repair conf file
	sprintf(gcQuery,"/usr/sbin/vzcfgvalidate -r /etc/vz/conf/%u.conf > "
			" /tmp/UpdateContainerUBC.vzcfgcheck.output 2>&1",uContainer);
	if(system(gcQuery))
	{
		printf("UpdateContainerUBC() error: %s\n",gcQuery);
		tJobErrorUpdate(uJob,"vzcfgvalidate failed");
		goto CommonExit;
	}

	//2a-. See if vzcfgvalidate -r changed anything if so report in log and then use
	// changed conf file for updates
	FILE *fp;
	unsigned uChange=0;
	if((fp=fopen("/tmp/UpdateContainerUBC.vzcfgcheck.output","r")))
	{
		while(fgets(cBuf,255,fp)!=NULL)
		{
			if(strncmp(cBuf,"set to ",7))
				continue;
			//May have multiple set to lines, the last one is the one we want
			sscanf(cBuf,"set to %f:%f",&luBar,&luLimit);
			uChange++;
		}
		fclose(fp);
	}

	if(uChange)
	{
		//3-. See 4-.
		sprintf(cContainerPath,"/etc/vz/conf/%u.conf",uContainer);
		sprintf(cApplyConfigPath,"/etc/vz/conf/ve-%u.conf-sample",uContainer);
		if(symlink(cContainerPath,cApplyConfigPath))
		{
			printf("UpdateContainerUBC() error: 3-. symlink failed\n");
			tJobErrorUpdate(uJob,"symlink failed");
			goto CommonExit;
		}

		//4-. Apply any changes produced by vzcfgvalidate -r
		sprintf(gcQuery,"/usr/sbin/vzctl set %u --applyconfig %u --save",
			uContainer,uContainer);
		if(system(gcQuery))
		{
			printf("UpdateContainerUBC() error: %s\n",gcQuery);
			tJobErrorUpdate(uJob,"vzctl set 4 failed");
			goto CommonExit;
		}
		unlink(cApplyConfigPath);
	}

	//5-.
	sprintf(gcQuery,"INSERT INTO tLog SET"
		" cLabel='UpdateContainerUBC()',"
		"uLogType=4,uLoginClient=1,"
		"cLogin='unxsVZ.cgi',cMessage='set %u --%s %.0f:%.0f (c=%u)',"
		"cServer='%s',uOwner=1,uCreatedBy=1,"
		"uCreatedDate=UNIX_TIMESTAMP(NOW())",
				uContainer,cResource,luBar,luLimit,uChange,cHostname);
	mysql_query(&gMysql,gcQuery);
	if(mysql_errno(&gMysql))
	{
		printf("UpdateContainerUBC() error: %s\n",mysql_error(&gMysql));
		tJobErrorUpdate(uJob,"INSERT INTO tLog failed");
		goto CommonExit;
	}

	//Everything ok
	tJobDoneUpdate(uJob);

CommonExit:
	return;

}//void UpdateContainerUBC(...)


void SetContainerUBC(unsigned uJob,unsigned uContainer,const char *cJobData)
{

	long unsigned luBar=0,luLimit=0;
	char cBuf[256];
	char cResource[64]={""};
	char cApplyConfigPath[100]={""};
	char cContainerPath[100]={""};
	char *cp;

	sscanf(cJobData,"--%63s %lu:%lu",cResource,&luBar,&luLimit);
	if((cp=strchr(cResource,' ')))
		*cp=0;
	if(!cResource[0])
	{
		printf("SetContainerUBC() error: Could not determine cResource\n");
		tJobErrorUpdate(uJob,"cResource[0]==0");
		goto CommonExit;
	}
	if(!luBar || !luLimit)
	{
		printf("SetContainerUBC() error: Could not determine luBar||luLimit\n");
		tJobErrorUpdate(uJob,"!luBar||!luLimit");
		goto CommonExit;
	}

	//1-.
	sprintf(gcQuery,"/usr/sbin/vzctl set %u --%s %lu:%lu --save",
		uContainer,cResource,luBar,luLimit);
	if(system(gcQuery))
	{
		printf("SetContainerUBC() error: %s\n",gcQuery);
		tJobErrorUpdate(uJob,"vzctl set failed");
		goto CommonExit;
	}

	//2-. validate and/or repair conf file
	sprintf(gcQuery,"/usr/sbin/vzcfgvalidate -r /etc/vz/conf/%u.conf > "
			" /tmp/SetContainerUBC.vzcfgcheck.output 2>&1",uContainer);
	if(system(gcQuery))
	{
		printf("SetContainerUBC() error: %s\n",gcQuery);
		tJobErrorUpdate(uJob,"vzcfgvalidate failed");
		goto CommonExit;
	}

	//2a-. See if vzcfgvalidate -r changed anything if so report in log and then use
	// changed conf file for updates
	FILE *fp;
	unsigned uChange=0;
	if((fp=fopen("/tmp/SetContainerUBC.vzcfgcheck.output","r")))
	{
		while(fgets(cBuf,255,fp)!=NULL)
		{
			if(strncmp(cBuf,"set to ",7))
				continue;
			//May have multiple set to lines, the last one is the one we want
			sscanf(cBuf,"set to %lu:%lu",&luBar,&luLimit);
			uChange++;
		}
		fclose(fp);
	}

	if(uChange)
	{
		//3-. See 4-.
		sprintf(cContainerPath,"/etc/vz/conf/%u.conf",uContainer);
		sprintf(cApplyConfigPath,"/etc/vz/conf/ve-%u.conf-sample",uContainer);
		if(symlink(cContainerPath,cApplyConfigPath))
		{
			printf("SetContainerUBC() error: 3-. symlink failed\n");
			tJobErrorUpdate(uJob,"symlink failed");
			goto CommonExit;
		}

		//4-. Apply any changes produced by vzcfgvalidate -r
		sprintf(gcQuery,"/usr/sbin/vzctl set %u --applyconfig %u --save",
			uContainer,uContainer);
		if(system(gcQuery))
		{
			printf("SetContainerUBC() error: %s\n",gcQuery);
			tJobErrorUpdate(uJob,"vzctl set 4 failed");
			goto CommonExit;
		}
		unlink(cApplyConfigPath);
	}

	//5-.
	sprintf(gcQuery,"INSERT INTO tLog SET"
		" cLabel='SetContainerUBC()',"
		"uLogType=4,uLoginClient=1,"
		"cLogin='unxsVZ.cgi',cMessage='set %u --%s %lu:%lu (c=%u)',"
		"cServer='%s',uOwner=1,uCreatedBy=1,"
		"uCreatedDate=UNIX_TIMESTAMP(NOW())",
				uContainer,cResource,luBar,luLimit,uChange,cHostname);
	mysql_query(&gMysql,gcQuery);
	if(mysql_errno(&gMysql))
	{
		printf("SetContainerUBC() error: %s\n",mysql_error(&gMysql));
		tJobErrorUpdate(uJob,"INSERT INTO tLog failed");
		goto CommonExit;
	}

	//Everything ok
	tJobDoneUpdate(uJob);

CommonExit:
	return;

}//void SetContainerUBC(...)


void TemplateContainer(unsigned uJob,unsigned uContainer,const char *cJobData)
{
        MYSQL_RES *res;
        MYSQL_ROW field;

	char cConfigLabel[32]={""};
	char cOSTemplateBase[100]={"centos-5-x86_64"};
	char *cp;
	register int i;
	struct stat statInfo;

	//Parse data and basic sanity checks
	sscanf(cJobData,"tConfig.Label=%31s;",cConfigLabel);
	if((cp=strchr(cConfigLabel,';')))
		*cp=0;
	if(!cConfigLabel[0])
	{
		printf("TemplateContainer() error: Could not determine cConfigLabel\n");
		tJobErrorUpdate(uJob,"cConfigLabel[0]==0");
		goto CommonExit;
	}

	sprintf(gcQuery,"SELECT tOSTemplate.cLabel"
			" FROM tContainer,tOSTemplate WHERE tContainer.uContainer=%u",uContainer);
	mysql_query(&gMysql,gcQuery);
	if(mysql_errno(&gMysql))
	{
		printf("%s\n",mysql_error(&gMysql));
		tJobErrorUpdate(uJob,"SELECT tOSTemplate.cLabel");
		goto CommonExit;
	}
        res=mysql_store_result(&gMysql);
	if((field=mysql_fetch_row(res)))
		sprintf(cOSTemplateBase,"%.99s",field[0]);
	mysql_free_result(res);
	if(!cOSTemplateBase[0])
	{
		printf("TemplateContainer() error: Could not determine cOSTemplateBase\n");
		tJobErrorUpdate(uJob,"cOSTemplateBase[0]==0");
		goto CommonExit;
	}
	for(i=strlen(cOSTemplateBase)-1;i>0;i--)
	{
		if(cOSTemplateBase[i]=='-')
		{
			cOSTemplateBase[i]=0;	
			break;
		}
	}
	//debug only
	//printf("cConfigLabel=%s; cOSTemplateBase=%s;\n",cConfigLabel,cOSTemplateBase);
	//exit(0);

	//1-. Stop container
	sprintf(gcQuery,"/usr/sbin/vzctl --verbose stop %u",uContainer);
	if(system(gcQuery))
	{
		printf("TemplateContainer() error: %s\n",gcQuery);
		tJobErrorUpdate(uJob,"vzctl stop failed");
		goto CommonExit;
	}

	//2-. Make template cache tar. No gzip for less down time
	sprintf(gcQuery,"cd /vz/private/%u; /bin/tar cf /vz/template/cache/%s-%s.tar --numeric-owner ./",
		uContainer,cOSTemplateBase,cConfigLabel);
	if(system(gcQuery))
	{
		printf("TemplateContainer() error: %s\n",gcQuery);
		tJobErrorUpdate(uJob,"tar container failed");
		goto CommonExit;
	}

	//3-. Start container
	sprintf(gcQuery,"/usr/sbin/vzctl --verbose start %u",uContainer);
	if(system(gcQuery))
	{
		printf("TemplateContainer() error: %s\n",gcQuery);
		tJobErrorUpdate(uJob,"vzctl start failed");
		goto CommonExit;
	}

	//4-. gzip then scp template to all nodes
	//Note dependency on custom per datacenter script!
	sprintf(gcQuery,"/bin/gzip -f /vz/template/cache/%s-%s.tar",cOSTemplateBase,cConfigLabel);
	if(system(gcQuery))
	{
		printf("TemplateContainer() error: %s\n",gcQuery);
		tJobErrorUpdate(uJob,"gzip container failed");
		goto CommonExit;
	}
	if(!stat("/usr/sbin/allnodescp.sh",&statInfo))
	{
		sprintf(gcQuery,"/usr/sbin/allnodescp.sh /vz/template/cache/%s-%s.tar.gz",cOSTemplateBase,cConfigLabel);
		if(system(gcQuery))
		{
			printf("TemplateContainer() error: %s\n",gcQuery);
			tJobErrorUpdate(uJob,"allnodescp.sh .tar.gz failed");
			goto CommonExit;
		}
	}

	//4b-. Make /etc/vz/conf tConfig file and scp to all nodes. ve-proxpop.conf-sample
	sprintf(gcQuery,"/bin/cp /etc/vz/conf/%u.conf /etc/vz/conf/ve-%s.conf-sample",
		uContainer,cConfigLabel);
	if(system(gcQuery))
	{
		printf("TemplateContainer() error: %s\n",gcQuery);
		tJobErrorUpdate(uJob,"cp conf container failed");
		goto CommonExit;
	}
	if(!stat("/usr/sbin/allnodescp.sh",&statInfo))
	{
		sprintf(gcQuery,"/usr/sbin/allnodescp.sh /etc/vz/conf/ve-%s.conf-sample",cConfigLabel);
		if(system(gcQuery))
		{
			printf("TemplateContainer() error: %s\n",gcQuery);
			tJobErrorUpdate(uJob,"allnodescp.sh .conf failed");
			goto CommonExit;
		}
	}

	//5-. Add tOSTemplate and tConfig entries
	sprintf(gcQuery,"INSERT tOSTemplate SET cLabel='%s-%s',uOwner=1,uCreatedBy=1,"
				"uCreatedDate=UNIX_TIMESTAMP(NOW())",cOSTemplateBase,cConfigLabel);
	mysql_query(&gMysql,gcQuery);
	if(mysql_errno(&gMysql))
	{
		printf("%s\n",mysql_error(&gMysql));
		tJobErrorUpdate(uJob,"INSERT tOSTemplate");
		goto CommonExit;
	}
	sprintf(gcQuery,"INSERT tConfig SET cLabel='%s',uOwner=1,uCreatedBy=1,"
				"uCreatedDate=UNIX_TIMESTAMP(NOW())",cConfigLabel);
	mysql_query(&gMysql,gcQuery);
	if(mysql_errno(&gMysql))
	{
		printf("%s\n",mysql_error(&gMysql));
		tJobErrorUpdate(uJob,"INSERT tConfig");
		goto CommonExit;
	}

	//Everything ok
	SetContainerStatus(uContainer,1);//Active
	tJobDoneUpdate(uJob);

CommonExit:
	return;

}//void TemplateContainer(...)


void MountFilesContainer(unsigned uJob,unsigned uContainer)
{
	//0-. Create VEID.mount and VEID.umount files if container so specifies via tProperty
	//	and everything needed is available: template and template vars from tProperty.
        MYSQL_RES *res2;
        MYSQL_ROW field2;
	FILE *fp;
	char cTemplateName[256]={""};//required
	char cFile[100]={""};//required
	char cVeID[32]={""};//required
	char cService1[256]={"80"};//default
	char cService2[256]={"443"};//default
	char cNodeIP[256]={""};//required
	char cNetmask[256]={"255.255.255.0"};//default
	char cPrivateIPs[256]={"10.0.0.0/24"};//default
	struct stat statInfo;

	sprintf(cVeID,"%u",uContainer);	

	GetContainerProp(uContainer,"cVEID.mount",cTemplateName);
	sprintf(cFile,"/etc/vz/conf/%u.mount",uContainer);
	if(cTemplateName[0] && stat(cFile,&statInfo))
	{
		GetContainerProp(uContainer,"cNodeIP",cNodeIP);
		GetContainerProp(uContainer,"cNetmask",cNetmask);
		GetContainerProp(uContainer,"cPrivateIPs",cPrivateIPs);
		GetContainerProp(uContainer,"cService1",cService1);
		GetContainerProp(uContainer,"cService2",cService2);

		if((fp=fopen(cFile,"w"))==NULL)
		{
			printf("MountFilesContainer() error: %s\n",cFile);
			tJobErrorUpdate(uJob,"VEID.mount create failed");
			goto CommonExit;
		}

		TemplateSelect(cTemplateName);
		res2=mysql_store_result(&gMysql);
		if((field2=mysql_fetch_row(res2)))
		{
			struct t_template template;

			template.cpName[0]="cNodeIP";
			template.cpValue[0]=cNodeIP;
					
			template.cpName[1]="cNetmask";
			template.cpValue[1]=cNetmask;
					
			template.cpName[2]="cPrivateIPs";
			template.cpValue[2]=cPrivateIPs;
					
			template.cpName[3]="cVeID";
			template.cpValue[3]=cVeID;

			template.cpName[4]="cService1";
			template.cpValue[4]=cService1;
					
			template.cpName[5]="cService2";
			template.cpValue[5]=cService2;
					
			template.cpName[6]="";
			Template(field2[0],&template,fp);
		}
		mysql_free_result(res2);
		fclose(fp);
		chmod(cFile,S_IRUSR|S_IWUSR|S_IXUSR);

		cTemplateName[0]=0;
		GetContainerProp(uContainer,"cVEID.umount",cTemplateName);
		sprintf(cFile,"/etc/vz/conf/%u.umount",uContainer);
		if(cTemplateName[0] && stat(cFile,&statInfo))
		{
			if((fp=fopen(cFile,"w"))==NULL)
			{
				printf("MountFilesContainer() error: %s\n",cFile);
				tJobErrorUpdate(uJob,"VEID.umount create failed");
				goto CommonExit;
			}

			TemplateSelect(cTemplateName);
			res2=mysql_store_result(&gMysql);
			if((field2=mysql_fetch_row(res2)))
			{
				struct t_template template;

				template.cpName[0]="cNodeIP";
				template.cpValue[0]=cNodeIP;
						
				template.cpName[1]="cNetmask";
				template.cpValue[1]=cNetmask;
						
				template.cpName[2]="cPrivateIPs";
				template.cpValue[2]=cPrivateIPs;
						
				template.cpName[3]="cVeID";
				template.cpValue[3]=cVeID;

				template.cpName[4]="cService1";
				template.cpValue[4]=cService1;
						
				template.cpName[5]="cService2";
				template.cpValue[5]=cService2;
						
				template.cpName[6]="";
				Template(field2[0],&template,fp);
			}
			mysql_free_result(res2);
			fclose(fp);
			chmod(cFile,S_IRUSR|S_IWUSR|S_IXUSR);
		}
		else
		{
			printf("MountFilesContainer() error: %s missing\n",cTemplateName);
			tJobErrorUpdate(uJob,"Missing VEID.umount!");
			goto CommonExit;
		}
	}

	tJobDoneUpdate(uJob);

CommonExit:
	return;

}//void MountFilesContainer(unsigned uJob,unsigned uContainer);


void CloneContainer(unsigned uJob,unsigned uContainer,char *cJobData)
{
	char cTargetNodeIPv4[256]={""};
	unsigned uTargetNode=0;
	char cSourceContainerIP[32]={""};
	char cNewIP[32]={""};
	char cHostname[100]={""};
	unsigned uNewVeid=0;

	//CloneWizard created a new tContainer with "Awaiting Clone" status.
	//CloneWizard created a job (this job) that runs on the source container node.
	//The created job has job data to complete the operations on source and
	//target nodes.
	//vzdump script must be installed on all datacenter nodes.

	sscanf(cJobData,"uTargetNode=%u;",&uTargetNode);
	if(!uTargetNode)
	{
		printf("CloneContainer() error: Could not determine uTargetNode\n");
		tJobErrorUpdate(uJob,"uTargetNode==0");
		goto CommonExit;
	}

	sscanf(cJobData,"cSourceContainerIP=%s;",cSourceContainerIP);
	if(!cNewIP[0])
	{
		printf("CloneContainer() error: Could not determine cSourceContainerIP\n");
		tJobErrorUpdate(uJob,"No cSourceContainerIP");
		goto CommonExit;
	}

	sscanf(cJobData,"cNewIP=%s;",cNewIP);
	if(!cNewIP[0])
	{
		printf("CloneContainer() error: Could not determine cNewIP\n");
		tJobErrorUpdate(uJob,"No cNewIP");
		goto CommonExit;
	}

	sscanf(cJobData,"cHostname=%s;",cHostname);
	if(!cNewIP[0])
	{
		printf("CloneContainer() error: Could not determine cHostname\n");
		tJobErrorUpdate(uJob,"No cHostname");
		goto CommonExit;
	}

	sscanf(cJobData,"uNewVeid=%u;",&uNewVeid);
	if(!uNewVeid)
	{
		printf("CloneContainer() error: Could not determine uNewVeid\n");
		tJobErrorUpdate(uJob,"uNewVeid==0");
		goto CommonExit;
	}

	GetNodeProp(uTargetNode,"IPv4",cTargetNodeIPv4);
	if(!cTargetNodeIPv4[0])
	{
		printf("CloneContainer() error: Could not determine cTargetNodeIPv4. uTargetNode=%u;\n",uTargetNode);
		tJobErrorUpdate(uJob,"cTargetNodeIPv4");
		goto CommonExit;
	}

	//1-. vzdump w/suspend on source node
	//2-. scp dump to target node
	//3-. restore on target node to new veid
	//4-. change ip and hostname
	//5-. change any other /etc/vz/conf/veid.x files for new IP
	//6-. start new veid
	//7-. update source container status
	//8-. update target container status


	//1-.
	sprintf(gcQuery,"vzdump --suspend %u",uContainer);
	if(system(gcQuery))
	{
		printf("CloneContainer() error: %s.\n",gcQuery);
		tJobErrorUpdate(uJob,"error 1");
		goto CommonExit;
	}

	//2-.
	sprintf(gcQuery,"nice scp /vz/dump/vzdump-%u.tar %s:/vz/dump/vzdump-%u.tar",
				uContainer,cTargetNodeIPv4,uContainer);
	if(system(gcQuery))
	{
		printf("CloneContainer() error: %s.\n",gcQuery);
		tJobErrorUpdate(uJob,"error 2");
		goto CommonExit;
	}

	//3-.
	sprintf(gcQuery,"ssh %s 'vzdump --restore /vz/dump/vzdump-%u.tar %u'",
				cTargetNodeIPv4,uContainer,uNewVeid);
	if(system(gcQuery))
	{
		printf("CloneContainer() error: %s.\n",gcQuery);
		tJobErrorUpdate(uJob,"error 3");
		goto CommonExit;
	}

	//4a-.
	sprintf(gcQuery,"ssh %s 'vzctl set %u --hostname %s --save'",
				cTargetNodeIPv4,uNewVeid,cHostname);
	if(system(gcQuery))
	{
		printf("CloneContainer() error: %s.\n",gcQuery);
		tJobErrorUpdate(uJob,"error 4a");
		goto CommonExit;
	}

	//4b-.
	sprintf(gcQuery,"ssh %s 'vzctl set %u --ipdel %s --save'",
				cTargetNodeIPv4,uNewVeid,cSourceContainerIP);
	if(system(gcQuery))
	{
		printf("CloneContainer() error: %s.\n",gcQuery);
		tJobErrorUpdate(uJob,"error 4b");
		goto CommonExit;
	}

	//4c-.
	sprintf(gcQuery,"ssh %s 'vzctl set %u --ipadd %s --save'",
				cTargetNodeIPv4,uNewVeid,cNewIP);
	if(system(gcQuery))
	{
		printf("CloneContainer() error: %s.\n",gcQuery);
		tJobErrorUpdate(uJob,"error 4c");
		goto CommonExit;
	}

	//5a-.
	sprintf(gcQuery,"ssh %s 'rm -f /etc/vz/conf/%u.umount /etc/vz/conf/%u.mount'",
				cTargetNodeIPv4,uNewVeid,uNewVeid);
	if(system(gcQuery))
	{
		printf("CloneContainer() error: %s.\n",gcQuery);
		tJobErrorUpdate(uJob,"error 5a");
		goto CommonExit;
	}

	//5b-. Create new umount and mount files if applicable

	//6-.
	sprintf(gcQuery,"ssh %s 'vzctl start %u'",cTargetNodeIPv4,uNewVeid);
	if(system(gcQuery))
	{
		printf("CloneContainer() error: %s.\n",gcQuery);
		tJobErrorUpdate(uJob,"error 6");
		goto CommonExit;
	}

	//Everything ok
	SetContainerStatus(uContainer,1);//Active
	SetContainerStatus(uNewVeid,1);//Active
	tJobDoneUpdate(uJob);

CommonExit:
	return;

}//void CloneContainer(...)



//Required by libtemplate
void AppFunctions(FILE *fp,char *cFunction)
{
}//void AppFunctions(FILE *fp,char *cFunction)
