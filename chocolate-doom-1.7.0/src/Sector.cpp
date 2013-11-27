/**
 *  Sector.cpp
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
#include "Sector.h"


#ifdef __APPLE__
#pragma mark -
#pragma mark Initialization
#endif

//extern subsector_t*	subsectors;
//extern seg_t* segs;
//
//extern "C" void P_SpawnMapThing (mapthing_t* mthing);

Sector::Sector()
{
	_pathsBuilt = false;
	_searched = false;

	// Find the borders of this leaf poly that contain linedefs we can travel through
//	_subsector = &subsectors[bspNum & ~NF_SUBSECTOR];
//	short num_segments = _doomSector->numlines;
//	short first_seg = _subsector->firstline;
//	for (unsigned int i=0; i<num_segments; ++i)
//	{
//		seg_t* line_seg = &segs[first_seg + i];
//		
//		mobj_t thing; // this can be invalid and is handled properly
//		thing.radius = 1048576;
//		thing.height = 1048576;
//		thing.flags = thing.flags & 0x0111;
//		float pos_x = ((line_seg->v1->x) + (line_seg->v2->x)) / 2.0;
//		float pos_y = ((line_seg->v1->y) + (line_seg->v2->y)) / 2.0;
//		
//		mapthing_t thing3;
//		thing3.angle = 90;
//		thing3.type = MT_UNDEAD;
//		thing3.options = 7;
//		thing3.x = (int) pos_x >> FRACBITS;
//		thing3.y = (int) pos_y >> FRACBITS;
//		P_SpawnMapThing(&thing3);
//		
//		
//		
//		if (P_CheckPosition(&thing, pos_x, pos_y))
//			_lineSegs.push_back(*line_seg);
//		else
//			std::cout << "couldn't do it" << std::endl;
//	}
}

Sector::~Sector()
{
    ;
}