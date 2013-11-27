/**
 *  AgentMap.cpp
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

// Local headers
#include "AgentMap.h"
#include "Sector.h"


#ifdef __APPLE__
#pragma mark -
#pragma mark Initialization
#endif

extern int numnodes;
extern "C" void P_SpawnMapThing (mapthing_t* mthing);

AgentMap::AgentMap()
{
	_buildMap();
}

AgentMap::~AgentMap()
{
    ;
}

void AgentMap::_buildMap()
{
	for (unsigned int i=0; i<numlines; ++i) {
		sector_t* front_sec_t = lines[i].frontsector;
		sector_t* back_sec_t = lines[i].backsector;
		
		// Look for endgame
		if (lines[i].special == 11)
			_endLevelSector = lines[i].frontsector;
		
		// If it's two sided, there might be a portal here
		if (back_sec_t) {
			if (_sectors.find(front_sec_t) == _sectors.end()) {
				Sector sec;
				_sectors[front_sec_t] = sec;
			}
			if (_sectors.find(back_sec_t) == _sectors.end()) {
				Sector sec;
				_sectors[back_sec_t] = sec;
			}
			
			mobj_t thing; // this can be invalid and is handled properly
			thing.radius = 528576;
			thing.height = 528576;
			thing.flags = thing.flags & 0x0111;
			float pos_x = ((lines[i].v1->x) + (lines[i].v2->x)) / 2.0;
			float pos_y = ((lines[i].v1->y) + (lines[i].v2->y)) / 2.0;

			if (P_CheckPosition(&thing, pos_x, pos_y))
			{
				_sectors[front_sec_t].addPortal(&_sectors[back_sec_t], &lines[i]);
				_sectors[back_sec_t].addPortal(&_sectors[front_sec_t], &lines[i]);
			}
		}
	}
}

void AgentMap::clearSearch()
{
	for (std::map<sector_t*, Sector>::iterator it = _sectors.begin();
		 it!=_sectors.end(); ++it) {
		it->second.setSearched(false, NULL);
	}
}