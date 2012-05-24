/*
FILE
	$Id$
	(Built initially by unixservice.com mysqlRAD2)
PURPOSE
	Non schema-dependent table and application table related functions.
AUTHOR
	(C) 2001-2009 Gary Wallis for Unixservice.
 
*/

//ModuleFunctionProtos()
void AddDefaultFields(void);

void tTableNavList(void);
void tTableFieldNavList(void);

void ExtProcesstTableVars(pentry entries[], int x)
{
	/*
	register int i;
	for(i=0;i<x;i++)
	{
	}
	*/
}//void ExtProcesstTableVars(pentry entries[], int x)


void ExttTableCommands(pentry entries[], int x)
{

	if(!strcmp(gcFunction,"tTableTools"))
	{
		//ModuleFunctionProcess()

		if(!strcmp(gcCommand,LANG_NB_NEW))
                {
			if(guPermLevel>=9)
			{
	                        ProcesstTableVars(entries,x);
                        	guMode=2000;
	                        tTable(LANG_NB_CONFIRMNEW);
			}
			else
				tTable("<blink>Error</blink>: Denied by permissions settings");
                }
		else if(!strcmp(gcCommand,LANG_NB_CONFIRMNEW))
                {
			if(guPermLevel>=9)
			{
				unsigned uContactParentCompany=0;
                        	ProcesstTableVars(entries,x);
				GetClientOwner(guLoginClient,&uContactParentCompany);
				
                        	guMode=2000;
				//Check entries here
				if(cLabel[0]!='t')
					tTable("<blink>Error</blink>: Table must start with lower case t");
                        	guMode=0;

				uTable=0;
				uCreatedBy=guLoginClient;
				uOwner=uContactParentCompany;
				uModBy=0;//Never modified
				uModDate=0;//Never modified
				NewtTable(0);
			}
			else
				tTable("<blink>Error</blink>: Denied by permissions settings");
		}
		else if(!strcmp(gcCommand,LANG_NB_DELETE))
                {
                        ProcesstTableVars(entries,x);
			if(uAllowDel(uOwner,uCreatedBy))
			{
	                        guMode=2001;
				tTable(LANG_NB_CONFIRMDEL);
			}
			else
				tTable("<blink>Error</blink>: Denied by permissions settings");
                }
                else if(!strcmp(gcCommand,LANG_NB_CONFIRMDEL))
                {
                        ProcesstTableVars(entries,x);
			if(uAllowDel(uOwner,uCreatedBy))
			{
				guMode=5;
				DeletetTable();
			}
			else
				tTable("<blink>Error</blink>: Denied by permissions settings");
                }
		else if(!strcmp(gcCommand,LANG_NB_MODIFY))
                {
                        ProcesstTableVars(entries,x);
			if(uAllowMod(uOwner,uCreatedBy))
			{
				guMode=2002;
				tTable(LANG_NB_CONFIRMMOD);
			}
			else
				tTable("<blink>Error</blink>: Denied by permissions settings");
                }
                else if(!strcmp(gcCommand,LANG_NB_CONFIRMMOD))
                {
                        ProcesstTableVars(entries,x);
			if(uAllowMod(uOwner,uCreatedBy))
			{
                        	guMode=2002;
				//Check entries here
				if(cLabel[0]!='t')
					tTable("<blink>Error</blink>: Table must start with lower case t");
                        	guMode=0;

				uModBy=guLoginClient;
				ModtTable();
			}
			else
				tTable("<blink>Error</blink>: Denied by permissions settings");
                }
		else if(!strcmp(gcCommand,"Add standard fields"))
                {
                        ProcesstTableVars(entries,x);
			if(uAllowMod(uOwner,uCreatedBy))
			{
                        	guMode=0;
				if(!uTable)
					tTable("<blink>Error</blink>: No uTable");
				if(!uProject)
					tTable("<blink>Error</blink>: No uProject");
				AddDefaultFields();
				tTable("Standard fields added");
			}
			else
				tTable("<blink>Error</blink>: Denied by permissions settings");
                }
	}
}//void ExttTableCommands(pentry entries[], int x)


void ExttTableButtons(void)
{
	OpenFieldSet("tTable Aux Panel",100);
	switch(guMode)
        {
                case 2000:
			printf("<p><u>Enter/mod data</u><br>");
                        printf(LANG_NBB_CONFIRMNEW);
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
			printf("<p><u>Record Context Info</u><br>");
			printf("<p><u>Operations</u><br>");
			printf("<br><input type=submit class=largeButton title='Add standard primary key, cLabel and audit fields'"
				" name=gcCommand value='Add standard fields'>");
			tTableNavList();
			tTableFieldNavList();
	}
	CloseFieldSet();

}//void ExttTableButtons(void)


void ExttTableAuxTable(void)
{

}//void ExttTableAuxTable(void)


void ExttTableGetHook(entry gentries[], int x)
{
	register int i;

	for(i=0;i<x;i++)
	{
		if(!strcmp(gentries[i].name,"uTable"))
		{
			sscanf(gentries[i].val,"%u",&uTable);
			guMode=6;
		}
	}
	tTable("");

}//void ExttTableGetHook(entry gentries[], int x)


void ExttTableSelect(void)
{

	unsigned uContactParentCompany=0;

	GetClientOwner(guLoginClient,&uContactParentCompany);

	if(guLoginClient==1 && guPermLevel>11)//Root can read access all
		sprintf(gcQuery,"SELECT %s FROM tTable ORDER BY"
				" uTable",
				VAR_LIST_tTable);
	else //If you own it, the company you work for owns the company that owns it,
		//you created it, or your company owns it you can at least read access it
		//select tTemplateSet.cLabel from tTemplateSet,tClient where tTemplateSet.uOwner=tClient.uClient and tClient.uOwner in (select uClient from tClient where uOwner=81 or uClient=51);
	sprintf(gcQuery,"SELECT %s FROM tTable,tClient WHERE tTable.uOwner=tClient.uClient"
				" AND tClient.uOwner IN (SELECT uClient FROM tClient WHERE uOwner=%u OR uClient=%u)"
				" ORDER BY uTable",
					VAR_LIST_tTable,uContactParentCompany,uContactParentCompany);
					

}//void ExttTableSelect(void)


void ExttTableSelectRow(void)
{
	unsigned uContactParentCompany=0;

	GetClientOwner(guLoginClient,&uContactParentCompany);

	if(guLoginClient==1 && guPermLevel>11)//Root can read access all
                sprintf(gcQuery,"SELECT %s FROM tTable WHERE uTable=%u",
			VAR_LIST_tTable,uTable);
	else
                sprintf(gcQuery,"SELECT %s FROM tTable,tClient"
                                " WHERE tTable.uOwner=tClient.uClient"
				" AND tClient.uOwner IN (SELECT uClient FROM tClient WHERE uOwner=%u OR uClient=%u)"
				" AND tTable.uTable=%u",
                        		VAR_LIST_tTable
					,uContactParentCompany,uContactParentCompany
					,uTable);

}//void ExttTableSelectRow(void)


void ExttTableListSelect(void)
{
	char cCat[512];
	unsigned uContactParentCompany=0;
	
	GetClientOwner(guLoginClient,&uContactParentCompany);

	if(guLoginClient==1 && guPermLevel>11)//Root can read access all
		sprintf(gcQuery,"SELECT %s FROM tTable",
				VAR_LIST_tTable);
	else
		sprintf(gcQuery,"SELECT %s FROM tTable,tClient"
				" WHERE tTable.uOwner=tClient.uClient"
				" AND tClient.uOwner IN (SELECT uClient FROM tClient WHERE uOwner=%u OR uClient=%u)",
				VAR_LIST_tTable
				,uContactParentCompany
				,uContactParentCompany);

	//Changes here must be reflected below in ExttTableListFilter()
        if(!strcmp(gcFilter,"uTable"))
        {
                sscanf(gcCommand,"%u",&uTable);
		if(guPermLevel<10)
			strcat(gcQuery," AND ");
		else
			strcat(gcQuery," WHERE ");
		sprintf(cCat,"tTable.uTable=%u"
						" ORDER BY uTable",
						uTable);
		strcat(gcQuery,cCat);
        }
        else if(1)
        {
                //None NO FILTER
                strcpy(gcFilter,"None");
		strcat(gcQuery," ORDER BY uTable");
        }

}//void ExttTableListSelect(void)


void ExttTableListFilter(void)
{
        //Filter
        printf("&nbsp;&nbsp;&nbsp;Filter on ");
        printf("<select name=gcFilter>");
        if(strcmp(gcFilter,"uTable"))
                printf("<option>uTable</option>");
        else
                printf("<option selected>uTable</option>");
        if(strcmp(gcFilter,"None"))
                printf("<option>None</option>");
        else
                printf("<option selected>None</option>");
        printf("</select>");

}//void ExttTableListFilter(void)


void ExttTableNavBar(void)
{
	printf(LANG_NBB_SKIPFIRST);
	printf(LANG_NBB_SKIPBACK);
	printf(LANG_NBB_SEARCH);

	if(guPermLevel>=7 && !guListMode)
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

}//void ExttTableNavBar(void)


void tTableNavList(void)
{
        MYSQL_RES *res;
        MYSQL_ROW field;

	if(guLoginClient==1 && guPermLevel>11)//Root can read access all
		sprintf(gcQuery,"SELECT uTable,cLabel FROM tTable ORDER BY cLabel");
	else
		sprintf(gcQuery,"SELECT tTable.uTable,"
				" tTable.cLabel"
				" FROM tTable,tClient"
				" WHERE tTable.uOwner=tClient.uClient"
				" AND tClient.uOwner IN (SELECT uClient FROM tClient WHERE uOwner=%u OR uClient=%u)",
					guCompany
					,guLoginClient);
        mysql_query(&gMysql,gcQuery);
        if(mysql_errno(&gMysql))
        {
        	printf("<p><u>tTableNavList</u><br>\n");
                printf("%s",mysql_error(&gMysql));
                return;
        }

        res=mysql_store_result(&gMysql);
	if(mysql_num_rows(res))
	{	
        	printf("<p><u>tTableNavList</u><br>\n");

	        while((field=mysql_fetch_row(res)))
			printf("<a class=darkLink href=unxsRAD.cgi?gcFunction=tTable"
				"&uTable=%s>%s</a><br>\n",
				field[0],field[1]);
	}
        mysql_free_result(res);

}//void tTableNavList(void)


void tTableFieldNavList(void)
{
	if(!uTable) return;

        MYSQL_RES *res;
        MYSQL_ROW field;
	
	if(guLoginClient==1 && guPermLevel>11)//Root can read access all
		sprintf(gcQuery,"SELECT uField,cLabel FROM tField WHERE uTable=%u ORDER BY uOrder,cLabel",uTable);
	else
		sprintf(gcQuery,"SELECT tField.uField,"
				" tField.cLabel"
				" FROM tField,tClient"
				" WHERE tField.uOwner=tClient.uClient"
				" AND tField.uTable=%u"
				" AND tClient.uOwner IN (SELECT uClient FROM tClient WHERE uOwner=%u OR uClient=%u)"
				" ORDER BY tField.uOrder,tField.cLabel",
					uTable,guCompany,guLoginClient);
        mysql_query(&gMysql,gcQuery);
        if(mysql_errno(&gMysql))
        {
        	printf("<p><u>tFieldNavList</u><br>\n");
                printf("%s",mysql_error(&gMysql));
                return;
        }

        res=mysql_store_result(&gMysql);
	if(mysql_num_rows(res))
	{	
        	printf("<p><u>tFieldNavList</u><br>\n");

	        while((field=mysql_fetch_row(res)))
			printf("<a class=darkLink href=unxsRAD.cgi?gcFunction=tField"
				"&uField=%s>%s</a><br>\n",
				field[0],field[1]);
	}
        mysql_free_result(res);

}//void tTableFieldNavList(void)




 //Native MySQL fields
#define COLTYPE_DECIMAL 0
#define COLTYPE_MONEY 1102 //For BCD in future. Now allows $250,000.00 style input and output.
#define COLTYPE_INTUNSIGNED 3
#define COLTYPE_TIMESTAMP 7
#define COLTYPE_DATETIME 12
#define COLTYPE_CHAR 254
#define COLTYPE_VARCHAR 253
#define COLTYPE_TEXT 252
#define COLTYPE_IMAGE 1101
#define COLTYPE_FKIMAGE 1200
#define COLTYPE_BIGINT 8

 //Our special gui fields
#define COLTYPE_SELECTTABLE 1000
#define COLTYPE_SELECTTABLE_OWNER 1100
#define COLTYPE_RADPRI 1001
#define COLTYPE_PRIKEY 1002
#define COLTYPE_YESNO 1003
#define COLTYPE_UNIXTIMECREATE 1004
#define COLTYPE_UNIXTIMEUPDATE 1005
#define COLTYPE_EXTFUNC 1006
#define COLTYPE_FOREIGNKEY 1007
#define COLTYPE_UINTUKEY 1008
#define COLTYPE_VARCHARUKEY 1009
#define COLTYPE_UNIXTIME 2000
static unsigned uRADPRI;
static unsigned uVARCHAR;
static unsigned uFOREIGNKEY;
static unsigned uUNIXTIMECREATE;
static unsigned uUNIXTIMEUPDATE;
static unsigned uUNSIGNED;
void GetuFieldTypes(void)
{	
	MYSQL_RES *res;
	MYSQL_ROW field;

	sprintf(gcQuery,"SELECT uFieldType FROM tFieldType WHERE uRADType=%u LIMIT 1",COLTYPE_RADPRI);
	mysql_query(&gMysql,gcQuery);
	if(mysql_errno(&gMysql)) tTable(mysql_error(&gMysql));
	res=mysql_store_result(&gMysql);
	field=mysql_fetch_row(res);
	sscanf(field[0],"%u",&uRADPRI);
	mysql_free_result(res);

	sprintf(gcQuery,"SELECT uFieldType FROM tFieldType WHERE uRADType=%u LIMIT 1",COLTYPE_VARCHAR);
	mysql_query(&gMysql,gcQuery);
	if(mysql_errno(&gMysql)) tTable(mysql_error(&gMysql));
	res=mysql_store_result(&gMysql);
	field=mysql_fetch_row(res);
	sscanf(field[0],"%u",&uVARCHAR);
	mysql_free_result(res);

	sprintf(gcQuery,"SELECT uFieldType FROM tFieldType WHERE uRADType=%u LIMIT 1",COLTYPE_FOREIGNKEY);
	mysql_query(&gMysql,gcQuery);
	if(mysql_errno(&gMysql)) tTable(mysql_error(&gMysql));
	res=mysql_store_result(&gMysql);
	field=mysql_fetch_row(res);
	sscanf(field[0],"%u",&uFOREIGNKEY);
	mysql_free_result(res);

	sprintf(gcQuery,"SELECT uFieldType FROM tFieldType WHERE uRADType=%u LIMIT 1",COLTYPE_UNIXTIMEUPDATE);
	mysql_query(&gMysql,gcQuery);
	if(mysql_errno(&gMysql)) tTable(mysql_error(&gMysql));
	res=mysql_store_result(&gMysql);
	field=mysql_fetch_row(res);
	sscanf(field[0],"%u",&uUNIXTIMEUPDATE);
	mysql_free_result(res);

	sprintf(gcQuery,"SELECT uFieldType FROM tFieldType WHERE uRADType=%u LIMIT 1",COLTYPE_UNIXTIMECREATE);
	mysql_query(&gMysql,gcQuery);
	if(mysql_errno(&gMysql)) tTable(mysql_error(&gMysql));
	res=mysql_store_result(&gMysql);
	field=mysql_fetch_row(res);
	sscanf(field[0],"%u",&uUNIXTIMECREATE);
	mysql_free_result(res);

	sprintf(gcQuery,"SELECT uFieldType FROM tFieldType WHERE uRADType=%u LIMIT 1",COLTYPE_INTUNSIGNED);
	mysql_query(&gMysql,gcQuery);
	if(mysql_errno(&gMysql)) tTable(mysql_error(&gMysql));
	res=mysql_store_result(&gMysql);
	field=mysql_fetch_row(res);
	sscanf(field[0],"%u",&uUNSIGNED);
	mysql_free_result(res);

}//void GetuFieldTypes(void)


//Uses file scoped vars from form
void AddDefaultFields(void)
{
	GetuFieldTypes();

	//Primary key: Given tSomething then it will be uSomething	
	char cFieldName[32]={""};
	sprintf(cFieldName,"u%.30s",cLabel+1);
	sprintf(gcQuery,"INSERT INTO tField SET uTable=%u,uProject=%u,cLabel='%s',uOrder=1,"
			"cTitle='Primary key',uFieldType=%u,uSQLSize=10,uReadLevel=20,uOwner=%u,uCreatedBy=%u",
			uTable,
			uProject,
			cFieldName,
			uRADPRI,guCompany,guLoginClient);
	mysql_query(&gMysql,gcQuery);
	if(mysql_errno(&gMysql))
		tTable(mysql_error(&gMysql));

	//cLabel
	sprintf(gcQuery,"INSERT INTO tField SET uTable=%u,uProject=%u,cLabel='cLabel',uOrder=2,"
			"cTitle='Short label',uFieldType=%u,uSQLSize=32",
			uTable,
			uProject,
			uVARCHAR,guCompany,guLoginClient);
	mysql_query(&gMysql,gcQuery);
	if(mysql_errno(&gMysql))
		tTable(mysql_error(&gMysql));

	//uOwner
	sprintf(gcQuery,"INSERT INTO tField SET uReadLevel=20,uTable=%u,uProject=%u,cLabel='uOwner',uOrder=1000,"
			"cTitle='Record owner',uFieldType=%u,uSQLSize=10,cFKSpec='\"tClient\",\"cLabel\",uOwner'",
			uTable,
			uProject,
			uFOREIGNKEY,guCompany,guLoginClient);
	mysql_query(&gMysql,gcQuery);
	if(mysql_errno(&gMysql))
		tTable(mysql_error(&gMysql));

	//uCreatedBy
	sprintf(gcQuery,"INSERT INTO tField SET uReadLevel=20,uTable=%u,uProject=%u,cLabel='uCreatedBy',uOrder=1001,"
			"cTitle='Record created by',uFieldType=%u,uSQLSize=10,cFKSpec='\"tClient\",\"cLabel\",uCreatedBy'",
			uTable,
			uProject,
			uFOREIGNKEY,guCompany,guLoginClient);
	mysql_query(&gMysql,gcQuery);
	if(mysql_errno(&gMysql))
		tTable(mysql_error(&gMysql));

	//uCreatedDate
	sprintf(gcQuery,"INSERT INTO tField SET uReadLevel=20,uTable=%u,uProject=%u,cLabel='uCreatedDate',uOrder=1002,"
			"cTitle='Unix timestamp for creation date',uFieldType=%u,uSQLSize=10",
			uTable,
			uProject,
			uUNIXTIMECREATE,guCompany,guLoginClient);
	mysql_query(&gMysql,gcQuery);
	if(mysql_errno(&gMysql))
		tTable(mysql_error(&gMysql));

	//uModBy
	sprintf(gcQuery,"INSERT INTO tField SET uReadLevel=20,uTable=%u,uProject=%u,cLabel='uModBy',uOrder=1003,"
			"cTitle='Record modifed by',uFieldType=%u,uSQLSize=10,cFKSpec='\"tClient\",\"cLabel\",uModBy'",
			uTable,
			uProject,
			uFOREIGNKEY);
	mysql_query(&gMysql,gcQuery,guCompany,guLoginClient);
	if(mysql_errno(&gMysql))
		tTable(mysql_error(&gMysql));

	//uModDate
	sprintf(gcQuery,"INSERT INTO tField SET uReadLevel=20,uTable=%u,uProject=%u,cLabel='uModDate',uOrder=1004,"
			"cTitle='Unix timestamp for last update',uFieldType=%u,uSQLSize=10",
			uTable,
			uProject,
			uUNIXTIMEUPDATE,guCompany,guLoginClient);
	mysql_query(&gMysql,gcQuery);
	if(mysql_errno(&gMysql))
		tTable(mysql_error(&gMysql));

}//void AddDefaultFields(void)

