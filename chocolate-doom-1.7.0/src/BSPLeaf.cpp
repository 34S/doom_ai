/**
 *  BSPLeaf.cpp
 *  doom-ai
 *
 *  Created by Trevor Richardson on 11/18/13.
 *  Copyright 2013 Trevor Richardson. All rights reserved.
 */

// System headers
#include <iostream>

// DOOM headers
extern "C" {
	#include "p_local.h" 
}
//#include "r_defs.h"

// Local headers
#include "BSPLeaf.h"


#ifdef __APPLE__
#pragma mark -
#pragma mark Initialization
#endif

extern subsector_t*	subsectors;
extern seg_t* segs;

BSPLeaf::BSPLeaf()
{
    ;
}

BSPLeaf::BSPLeaf(int bspNum)
{
	// Find the borders of this leaf poly that contain linedefs we can travel through
	subsector_t* ss = &subsectors[bspNum & ~NF_SUBSECTOR];
	short num_segments = ss->numlines;
	short first_seg = ss->firstline;
	for (unsigned int i=0; i<num_segments; ++i)
	{
		seg_t* line_seg = &segs[first_seg + i];
		mobj_t* thing; // this can be invalid and is handled properly
		if (P_CheckPosition(thing, line_seg->v1->x, line_seg->v1->y))
			_portals.push_back(*line_seg);
	}
}

BSPLeaf::~BSPLeaf()
{
    ;
}