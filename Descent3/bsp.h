/*
 * $Logfile: /DescentIII/Main/bsp.h $
 * $Revision: 13 $
 * $Date: 4/19/00 5:30p $
 * $Author: Matt $
 *
 * Header for bsp.cpp
 *
 * $Log: /DescentIII/Main/bsp.h $
 * 
 * 13    4/19/00 5:30p Matt
 * From Duane for 1.4
 * Added extern
 * 
 * 12    4/14/99 3:56a Jeff
 * fixed case mismatch in #includes
 * 
 * 11    9/22/98 12:01p Matt
 * Added SourceSafe headers
 * 
 */


#ifndef BSP_H
#define BSP_H

#include "list.h"
#include "vecmat.h"
#include "CFILE.H"

#define BSP_IN_FRONT		1
#define BSP_BEHIND		2
#define BSP_ON_PLANE		3
#define BSP_SPANNING		4
#define BSP_COINCIDENT	5

#define BSP_EPSILON		.00005f

#define BSP_NODE	0
#define BSP_EMPTY_LEAF	1
#define BSP_SOLID_LEAF	2

typedef struct
{
	float a,b,c,d;
	ubyte used;
} bspplane;

typedef struct
{
	vector *verts;
	int nv;
	bspplane plane;

	short roomnum;
	short facenum;
	sbyte subnum;

	int color;

}	bsppolygon;

typedef struct bspnode 
{
	ubyte type;
	bspplane plane;
	ushort node_facenum;
	ushort node_roomnum;
	sbyte node_subnum;
	
	bspnode  *front;
	bspnode  *back;

	list *polylist;
	int num_polys;
} bspnode;

typedef struct bsptree 
{
	list           *vertlist;
	list				*polylist;
	bspnode        *root;
} bsptree;

// Builds a bsp tree for the indoor rooms
void BuildBSPTree ();

// Runs a ray through the bsp tree
// Returns true if a ray is occludes
int BSPRayOccluded(vector *start, vector *end, bspnode *node);
int BSPReportStatus(vector *start, bspnode *node);

// Walks the BSP tree and frees up any nodes/polygons that we might be using
void DestroyBSPTree (bsptree *tree);

// Saves and BSP node to an open file and recurses with the nodes children
void SaveBSPNode (CFILE *outfile,bspnode *node);

// Loads a bsp node from an open file and recurses with its children
void LoadBSPNode (CFILE *infile,bspnode **node);

// Initializes some variables for the indoor bsp tree
void InitDefaultBSP ();

// Destroy indoor bsp tree
void DestroyDefaultBSPTree ();

// Builds a bsp tree for a single room
void BuildSingleBSPTree (int roomnum);

// Reports the current mine's checksum
int BSPGetMineChecksum();

extern bsptree MineBSP;
extern int BSPChecksum;
extern ubyte BSP_initted;
extern ubyte UseBSP;

#endif


