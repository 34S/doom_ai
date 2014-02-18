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

extern "C" void P_SpawnMapThing (mapthing_t* mthing);

#ifdef __APPLE__
#pragma mark -
#pragma mark Initialization
#endif

extern int numnodes;

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
		
		// Populate our sector map as we go through
		Sector sec;
		if (_sectors.find(front_sec_t) == _sectors.end()) {
			sec.setDoomSector(front_sec_t);
			sec.addLine(&lines[i]);
			_sectors[front_sec_t] = sec;
		}
		else
			_sectors[front_sec_t].addLine(&lines[i]);

		if (_sectors.find(back_sec_t) == _sectors.end()) {
			if (back_sec_t) {
				sec.setDoomSector(back_sec_t);
				sec.addLine(&lines[i]);
				_sectors[back_sec_t] = sec;
			}
		}
		else
			_sectors[back_sec_t].addLine(&lines[i]);
		
		// Look for endgame
		if (lines[i].special == 11)
			_endLevelSector = lines[i].frontsector;
		
		// If it's two sided, there might be a portal here
		if (back_sec_t) {
			mobj_t thing; // this can be invalid and is handled properly
			thing.radius = 528576;
			thing.height = 528576;
			thing.flags = thing.flags & 0x0111;
			float pos_x = ((lines[i].v1->x) + (lines[i].v2->x)) / 2.0;
			float pos_y = ((lines[i].v1->y) + (lines[i].v2->y)) / 2.0;

			if (P_CheckPosition(&thing, pos_x, pos_y))
			{
				mapthing_t m_thing;
				m_thing.angle = 90;
				m_thing.type = MT_UNDEAD;
				m_thing.options = 7;
				m_thing.x = (int) pos_x >> FRACBITS;
				m_thing.y = (int) pos_y >> FRACBITS;
//				P_SpawnMapThing(&m_thing);
				
				_sectors[front_sec_t].addPortal(&_sectors[back_sec_t], &lines[i]);
				_sectors[back_sec_t].addPortal(&_sectors[front_sec_t], &lines[i]);
			}
		}
	}
	
//	for (std::map<sector_t*, Sector>::iterator it = _sectors.begin(); it!=_sectors.end(); ++it) {
//		std::vector<line_t*> lines = (*it).second.lines();
//		
//		for (unsigned int i=0; i<lines.size(); ++i) {
//			float pos_x2 = ((lines[i]->v1->x) + (lines[i]->v2->x)) / 2.0;
//			float pos_y2 = ((lines[i]->v1->y) + (lines[i]->v2->y)) / 2.0;
//			mobj_t thing2; // this can be invalid and is handled properly
//			thing2.radius = 528576;
//			thing2.height = 528576;
//			thing2.flags = thing2.flags & 0x0111;
//			mapthing_t m_thing2;
//			m_thing2.angle = 90;
//			m_thing2.type = MT_UNDEAD;
//			m_thing2.options = 7;
//			m_thing2.x = (int) pos_x2 >> FRACBITS;
//			m_thing2.y = (int) pos_y2 >> FRACBITS;
//			if (!P_CheckPosition(&thing2, pos_x2, pos_y2))
//			{
//				P_SpawnMapThing(&m_thing2);
//			}
//		}
//	}

}

void AgentMap::clearSearch()
{
	for (std::map<sector_t*, Sector>::iterator it = _sectors.begin();
		 it!=_sectors.end(); ++it) {
		it->second.setSearched(false, NULL);
	}
}