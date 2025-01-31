
#define OMFCMD_NUM			22

#define OMFCMD_STREAM		0							// associates a symbol with a stream filename
#define OMFCMD_SECTION		1							// begins a theme
#define OMFCMD_PLAY			2							// plays a sample specified through llpt and lplc
#define OMFCMD_ENDSECTION	3							// ends a theme
#define OMFCMD_LABEL			4							// defines a label for branching
#define OMFCMD_COMPARE		5							// compares a register rX to what's loaded via lcmp
#define OMFCMD_LCMP			6							// loads NUMBER into lcmp
#define OMFCMD_LLPT			7							// specifies next play will load the sample specified
#define OMFCMD_LPLC			8							// specifies next play will loop sample n times
#define OMFCMD_INCI			9							// increments 'I' counter
#define OMFCMD_SETI			10							// sets 'I' counter
#define OMFCMD_IFI			11							// if 'I' counter == n then inst1;inst2;...;inst n
#define OMFCMD_REGION		12							// specifies a start of a named region
#define OMFCMD_ENDREGION	13							// specifies the end of a named region

#define OMFCMD_BRANCHING	15
#define OMFCMD_BLT			15							// branch if rX < lcmp
#define OMFCMD_BGT			16							// branch if rX > lcmp
#define OMFCMD_BEQ			17							// branch if rX == lcmp
#define OMFCMD_BNIF			18							// branch if stream playing is NOT finished.
#define OMFCMD_GOTO			19							// unconditional goto a label
#define OMFCMD_WAIT			20							// tells the system to wait


// undocumented codes.
#define OMFCMD_MPLAY			200						// internal code.
#define OMFCMD_ENDIFI		201						// end of ifi conditional.