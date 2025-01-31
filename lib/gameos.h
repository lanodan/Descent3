/*
 * $Logfile: /DescentIII/Main/lib/gameos.h $
 * $Revision: 3 $
 * $Date: 7/28/99 3:24p $
 * $Author: Kevin $
 *
 * Operating system management library
 *
 * $Log: /DescentIII/Main/lib/gameos.h $
 * 
 * 3     7/28/99 3:24p Kevin
 * Mac
 * 
 * 2     5/10/99 10:53p Ardussi
 * changes to compile on Mac
 * 
 * 17    3/14/97 6:11 PM Jeremy
 * added get_user_name to osDatabase
 * 
 * 16    3/14/97 4:15p Matt
 * Changed title of OutrageMessageBox() dialogs
 * 
 * 15    3/13/97 11:04a Samir
 * Took out file functions. Kept console because the OS must initialize
 * the console before doing anything else to report messages.
 * 
 * 14    3/10/97 11:13a Samir
 * Changes made to reflect deletion of OSDebug class from system
 * 
 * 13    3/04/97 1:00p Samir
 * Fixed return type prototype errors.
 * 
 * 12    3/04/97 12:54p Samir
 * Added type constants for message boxes.
 * 
 * 11    3/03/97 3:12p Samir
 * Fixed GetWorkingDir prototype.
 * 
 * 10    3/03/97 1:16p Samir
 * Added some file operations
 * 
 * 9     2/28/97 11:04a Samir
 * Use a generic structure to pass in init info instead of params.
 * 
 * 8     2/27/97 5:26 PM Jeremy
 * removed mac\oemac_os.h and replaced it with #include "oemac_os.h"
 * because the mac compiler doesn't need the path and the "\" doesn't work
 * anyway.  mac path delimiter is a ":"
 * 
 * 7     2/26/97 7:36p Samir
 * include oexxx_os.h from mac or win directories.
 * 
 * 6     2/26/97 7:05p Samir
 * debug_break function in osObject.
 * 
 * 5     2/04/97 5:03p Samir
 * Adjusted create to take out code_write parm
 * 
 * 4     1/30/97 6:07p Samir
 * A more object oriented approach to our OS library
 *
 * $NoKeywords: $
 */
#ifndef GAMEOS_H
#define GAMEOS_H
#include "pstypes.h"
typedef struct gameos_packet {
	int code;
	unsigned time_stamp;
} gameos_packet;
const int	GAMEOS_QUIT				= 1,	//	This gameos object is shutting down
			GAMEOS_IDLE				= 2,	//	currently idle.
			GAMEOS_UNKNOWN			= 255;	//	Unknown message
/* class osObject
	contains code to initialize all the OS dependent info and structures for an application
	this class should be the parent of a class created by the programmer to match the
	platform he's developing on.  i.e. there should be a osWinObject for the Win32 version,
	and must have all the functionality of this base Class.
*/
class osObject 
{
protected:
	bool m_Init, m_Created;				// did we initialize this object or created and init.
	osObject *m_ParentOS;				// determines if this is a parent process
	virtual void os_init() = 0;			// os specific initialization
	
public:
	osObject() { m_Init = 0; m_Created = 0; };
	virtual ~osObject() {};
	
//	parent_os is the osObject that is the parent of this new object.
//	info is a device dependent structure passed in using device independent parms.
	virtual void init(osObject *parent_os, void *info) = 0;
	virtual bool create(osObject *parent_os, void *info) = 0;
//	returns a packet with a code telling us either to quit or that we are in some
//	sort of idle state, which means we perform tasks like a game.
	virtual gameos_packet *defer() = 0;		// defers game operation to OS.
	virtual void get_info(void *info, int size_str) = 0;
};
/* osDatabase
	to get info about the application from an OS managed database (or a custom info file)
	Again, this class should be the parent of a OS specific class like osWinDatabase, for instance.
*/
class osDatabase
{
public:
	osDatabase() {};
	virtual ~osDatabase() {};
	virtual bool init() = 0;
//	creates an empty classification or structure where you can store information
	virtual bool create_record(const char *pathname) = 0;
//	set current database focus to a particular record
	virtual bool lookup_record(const char *pathname) = 0;
//	read either an integer or string from the current record
	virtual bool read(const char *label, char *entry, int *entrylen) = 0;
	virtual bool read(const char *label, int *entry) = 0;
//	write either an integer or string to a record.
	virtual bool write(const char *label, char *entry, int entrylen) = 0;
	virtual bool write(const char *label, int *entry) = 0;
	
// get the current user's name from the os
	virtual void get_user_name(char* buffer, ulong* size) = 0;
};
//  Data structures
typedef struct os_date {
	ushort year;						// 1-65535 A.D. (or C.E.)
	ubyte month;
	ubyte day;
	ubyte hour;
	ubyte min;
	ubyte sec;
	ubyte pad;							// to keep dword aligned
} os_date;
//	---------------------------------------------------------------------------
//	Debug system functions
//	---------------------------------------------------------------------------
#define OSMBOX_OK		1
#define OSMBOX_YESNO	2
bool os_ErrorBox(const char *str);	// displays an error message on the screen
int os_MessageBox(int type, const char *title, const char *str);	
bool os_ConsoleInit();
void os_ConsoleOpen(int n, int row, int col, int width, int height, char * title );
void os_ConsoleClose(int n);
void os_ConsolePrintf( int n, char * format, ... );
void os_ConsolePrintf( int n, int row, int col, char * format, ... );
//	---------------------------------------------------------------------------
//	Operating system includes
//	---------------------------------------------------------------------------
//#if defined(WIN32)
//	#include "win\oewin_os.h"
//#elif defined(MACINTOSH)		// JCA: not needed
//	#include "oemac_os.h"
//#endif
#endif
