#ifndef TEXPAGE_H
#define TEXPAGE_H


#include "manage.h"
#include "CFILE.H"
#include "pstypes.h"
#include "procedurals.h"

typedef struct 
{
	char bitmap_name[PAGENAME_LEN];			// filename for the bitmap associated with this texture
	char destroy_name[PAGENAME_LEN];		// filename for the destroyed bitmap
	char sound_name[PAGENAME_LEN];		// filename for the destroyed bitmap
	texture tex_struct;

	int num_proc_elements;

	ubyte proc_heat,proc_light,proc_thickness,osc_value;

	float proc_evaluation_time,osc_time;

	ubyte proc_type[MAX_PROC_ELEMENTS];
	
	ubyte proc_frequency[MAX_PROC_ELEMENTS];
	ubyte proc_speed[MAX_PROC_ELEMENTS];
	
	ubyte proc_size[MAX_PROC_ELEMENTS];

	ubyte proc_x1[MAX_PROC_ELEMENTS];
	ubyte proc_y1[MAX_PROC_ELEMENTS];
	ubyte proc_x2[MAX_PROC_ELEMENTS];
	ubyte proc_y2[MAX_PROC_ELEMENTS];

	ushort proc_palette[256];
} mngs_texture_page;


// Texture page functions
//---------------------------------------------------------------

// Reads a texture page from an open file.  Returns 0 on error.  

int mng_ReadTexturePage (CFILE *infile,mngs_texture_page *texpage);


// Given an open file pointer and a texture handle, writes that texture page out
void mng_WriteTexturePage (CFILE *outfile,mngs_texture_page *texpage);

// Reads a texture page from an open file.  Returns 0 on error.  

int mng_ReadNewTexturePage (CFILE *infile,mngs_texture_page *texpage);


// Given an open file pointer and a texture handle, writes that texture page out
void mng_WriteNewTexturePage (CFILE *outfile,mngs_texture_page *texpage);

// Given a texture page, allocs a texture and loads that textures associated bitmap
// returns texture handle on success, -1 if fail
int mng_SetAndLoadTexture (mngs_texture_page *texpage);

// Reads in a texture page from the local table file, superseding any texture 
// already in RAM with that same name
void mng_LoadLocalTexturePage(CFILE *);

// Reads in a page off the net
void mng_LoadNetTexturePage (CFILE *,bool overlay=false);

// Reads in the texpage named "name" into texpage struct
// Returns 0 on error, else 1 if all is good
int mng_FindSpecificTexPage (char *name,mngs_texture_page *texpage,int offset=0);

// Given a texpage and a texture handle, attempts to make texture n correspond to 
// to the texpage. 
int mng_AssignTexPageToTexture(mngs_texture_page *texpage,int n,CFILE *infile=NULL);

// Copies values from a texture_page into a texture
void mng_AssignTextureToTexPage(int n,mngs_texture_page *texpage);

// First searches through the texture index to see if the texture is already
// loaded.  If not, searches in the table file and loads it.  
// Returns index of texture found, -1 if not
int mng_GetGuaranteedTexturePage (char *name,CFILE *infile=NULL);

// Given some texture names, finds them in the table file and deletes them 
// If local is 1, deletes from the local table file
int mng_DeleteTexPageSeries (char *names[],int num_textures,int local);

#endif
