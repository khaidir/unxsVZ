/*
FILE
        $Id: local.h.default 1913 2012-04-13 23:58:37Z Dylan $
PURPOSE
        Localization constants, configuration settings and release info.
LEGAL
        Copyright 2012-2013 Gary Wallis
*/

#define LOCALCOPYRIGHT "&copy; 2000-2012 Unixservice, LLC. All Rights Reserved."

//Change for your setup DO NOT USE THESE DEFAULTS: DANGER!
#define DBIP0 NULL
#define DBIP1 NULL
#define DBNAME "unxssps"
#define DBLOGIN "unxssps"
#define DBPASSWD "wsxedc"
#define DBPORT 0
#define DBSOCKET NULL
//Example for host running two mysqld
//See makefile and mysqlrad.h CustomExample sections
//#define DBPORT 4306
//#define DBSOCKET "/var/lib/mysql/mysql4.sock"

//Master tClient and related tAuthorize can be centralized
// to be used by all mysqlISP2 related subsystems.
#define TCLIENT "tClient"
//#define TCLIENT "mysqlISP2.tClient"
#define TAUTHORIZE "tAuthorize"
//#define TAUTHORIZE "mysqlISP2.tAuthorize"

//unxssps can report to us once a week some useful for
//debugging and development statistics.
//This is very helpful to us and in the end we hope for yourself.
//For improving and developing this software
//#define DEBUG_REPORT_STATS_OFF

//Setup your own CA and sign a cert and start up your Apache/mod_ssl server
//for security.
//Make sure mySQL ports are firewalled correctly if used on other servers.
//VPN them?
#define SSLONLY

//You should customize these for your own ISP
#define EXTISPNAME "unixservice"
#define EXTISPURL "unixservice.com"
#define HEADER_TITLE "unxsVZ"

//End-users: Do not change these!
//Also edit main.c header RAD status line if you change the following
#define RELEASE "0.01/rpm-0.1"
#define REV "0.01/rpm-0.1"

//For interfaces
#define cMAILTO "noc@someisp.net"
#define cBCC ""
#define cFROM "noc@someisp.net"
#define cSUBJECT "OneLogin system message"
#define cUNXSVZORGLOGFILE "/var/log/unxsspsOrg.log"
