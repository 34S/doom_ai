/**
 *  AgentMap.cpp
 *  doom-ai
 *
 *  Created by Trevor Richardson on 11/18/13.
 *  Copyright 2013 Trevor Richardson. All rights reserved.
 */

// System headers
#include <iostream>

// Local headers
#include "AgentMap.h"
#include "BSPLeaf.h"


#ifdef __APPLE__
#pragma mark -
#pragma mark Initialization
#endif

extern int numnodes;
extern "C" void P_SpawnMapThing (mapthing_t* mthing);

AgentMap::AgentMap()
{
    ;
}

AgentMap::AgentMap(node_t* nodes)
{
	_buildMap(nodes);
}

AgentMap::~AgentMap()
{
    ;
}

void AgentMap::chooseMove(player_t* inGamePlayer)
{
	for (std::vector<BSPLeaf>::iterator it = _bspLeaves.begin(); it!=_bspLeaves.end(); ++it) {
		if (inGamePlayer->mo->subsector == (*it).subsector())
		{
			std::cout << "start here" << std::endl;
//			startSearch((*it));
			if ((*it).portals().size()) {
				Portal p = (*it).portals()[0];
				int move_x = p.seg->v1->x - inGamePlayer->mo->x;
				int move_y = p.seg->v1->y - inGamePlayer->mo->y;
				inGamePlayer->mo->momx = move_x / 400;
				inGamePlayer->mo->momy = move_y / 400;
			}
		}
	}
}

void AgentMap::_buildMap(node_t* nodes)
{
	// Dig through and find all portal type walls that we can pass through
	std::vector<BSPLeaf> leaves;
	findLeaves(nodes, numnodes - 1, leaves);
	_bspLeaves = leaves;

	// Match portals to create structure to travel through
	for (std::vector<BSPLeaf>::iterator outer_it = _bspLeaves.begin(); outer_it!=_bspLeaves.end(); ++outer_it) {
		std::vector<seg_t> portal_outer = (*outer_it).lineSegments();
		
		// First iterate through all leaves
		for (std::vector<BSPLeaf>::iterator inner_it = _bspLeaves.begin(); inner_it!=_bspLeaves.end(); ++inner_it) {
			if (outer_it == inner_it)
				continue;
			if ((*inner_it).pathsBuilt())
				continue;
			
			// For each leaf, go through and try to match the portals
			std::vector<seg_t> portal_inner = (*inner_it).lineSegments();
			for (std::vector<seg_t>::iterator outer_line_it=portal_outer.begin();
											outer_line_it!=portal_outer.end(); ++outer_line_it) {
				for (std::vector<seg_t>::iterator inner_line_it=portal_inner.begin();
											inner_line_it!=portal_inner.end(); ++inner_line_it) {
					int outer_x1 = (*outer_line_it).v1->x; int outer_x2 = (*outer_line_it).v2->x;
					int inner_x1 = (*inner_line_it).v1->x; int inner_x2 = (*inner_line_it).v2->x;
					int outer_y1 = (*outer_line_it).v1->y; int outer_y2 = (*outer_line_it).v2->y;
					int inner_y1 = (*inner_line_it).v1->y; int inner_y2 = (*inner_line_it).v2->y;
					if ((outer_x1 == inner_x1 || outer_x1 == inner_x2) &&
						(outer_x2 == inner_x1 || outer_x2 == inner_x2) &&
						(outer_y1 == inner_y1 || outer_y1 == inner_y2) &&
						(outer_y2 == inner_y1 || outer_y2 == inner_y2))
					{
						(*outer_it).addPortal(&(*inner_it), &(*outer_line_it));
						
						/****************** only for debugging ********************************/
						// drop in an object to the scene so that we can do some visuals of where we
						// think the portals are located
						mapthing_t thing;
						thing.angle = 90;
						thing.type = MT_UNDEAD;
						thing.options = 7;
						
						float pos_x = (((*inner_line_it).v1->x >> FRACBITS) + ((*inner_line_it).v2->x >> FRACBITS)) / 2.0;
						float pos_y = (((*inner_line_it).v1->y >> FRACBITS) + ((*inner_line_it).v2->y >> FRACBITS)) / 2.0;
						thing.x = pos_x;
						thing.y = pos_y;
						P_SpawnMapThing(&thing);
					}
				}
			}
		}
		// Set so that we don't search it again
		(*outer_it).setPathsBuilt(true);
	}
}

void AgentMap::findLeaves(node_t* nodes, const int& bspNum, std::vector<BSPLeaf>& leaves) const
{
	node_t*	bsp;
	
	static unsigned int leaf_count = 0;
	
	// Found a subsector?
	if (bspNum & NF_SUBSECTOR)
	{
		std::cout << "Found leaf: " << leaf_count << std::endl;
		++leaf_count;
		
		BSPLeaf leaf(bspNum);
		leaves.push_back(leaf);
		
		return;
	}
		
	bsp = &nodes[bspNum];
	
	// Recursively search children
	findLeaves(nodes, bsp->children[0], leaves);
	findLeaves(nodes, bsp->children[1], leaves);
}