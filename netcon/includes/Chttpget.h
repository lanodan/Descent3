/*
* $Logfile: /DescentIII/Main/lib/Chttpget.h $
* $Revision: 1.2 $
* $Date: 2004/02/09 04:14:51 $
* $Author: kevinb $
*
* HTTP Client class (get only)
*
* $Log: Chttpget.h,v $
* Revision 1.2  2004/02/09 04:14:51  kevinb
* Added newlines to all headers to reduce number of warnings printed
*
* Made some small changes to get everything compiling.
*
* All Ready to merge the 1.5 tree.
*
* Revision 1.1.1.1  2000/04/18 00:00:38  icculus
* initial checkin
*
 * 
 * 7     8/21/99 9:14p Kevin
 * Added support for redirection
 * 
 * 6     8/21/99 6:48a Jeff
 * Linux port
 * 
 * 5     8/20/99 3:01p Kevin
 * Added support for Proxies (I hope!)
 * 
 * 4     7/31/98 12:19p Nate
 * Fixed http abort problem.
 * 
 * 3     7/31/98 11:57a Kevin
 * Added new functions for getting state
 * 
 * 2     6/01/98 10:10a Kevin
 * Added DLL connection interface and auto update DLL
 * 
 * 1     5/27/98 9:54a Kevin
 * 
 * 1     5/25/98 5:31p Kevin
 * Initial version
*
* $NoKeywords: $
*/
#ifndef _CHTTPGET_HEADER_
#define _CHTTPGET_HEADER_

#define HTTP_STATE_INTERNAL_ERROR		0
#define HTTP_STATE_SOCKET_ERROR			1
#define HTTP_STATE_URL_PARSING_ERROR	2
#define HTTP_STATE_CONNECTING				3
#define HTTP_STATE_HOST_NOT_FOUND		4
#define HTTP_STATE_CANT_CONNECT			5
#define HTTP_STATE_CONNECTED				6
#define HTTP_STATE_FILE_NOT_FOUND		10
#define HTTP_STATE_RECEIVING				11
#define HTTP_STATE_FILE_RECEIVED			12
#define HTTP_STATE_UNKNOWN_ERROR			13
#define HTTP_STATE_RECV_FAILED			14
#define HTTP_STATE_CANT_WRITE_FILE		15
#define HTTP_STATE_STARTUP					16

#define MAX_URL_LEN	300

class ChttpGet
{
public:
	ChttpGet(char *URL,char *localfile);
	ChttpGet(char *URL,char *localfile,char *proxyip,unsigned short proxyport);
	void PrepSocket(char *URL);
	~ChttpGet();
	void GetFile(char *URL,char *localfile);
	int GetStatus();
	unsigned int GetBytesIn();
	unsigned int GetTotalBytes();
	void AbortGet();
	void WorkerThread();
	bool m_Aborted;

protected:
	int ConnectSocket();
	char *GetHTTPLine();
	unsigned int ReadDataChannel();
	unsigned int m_iBytesIn;
	unsigned int m_iBytesTotal;
	unsigned int m_State;
	bool m_ProxyEnabled;
	char *m_ProxyIP;
	char m_URL[MAX_URL_LEN];
	unsigned short m_ProxyPort;

	char m_szUserName[100];
	char m_szPassword[100];
	char m_szHost[200];
	char m_szDir[200];
	char m_szFilename[100];
	
	bool m_Aborting;


	SOCKET m_DataSock;
	
	FILE *LOCALFILE;
	char recv_buffer[1000];

};










#endif

