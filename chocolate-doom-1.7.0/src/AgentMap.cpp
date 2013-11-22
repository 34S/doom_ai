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

void AgentMap::_buildMap(node_t* nodes)
{
	// Dig through and find all portal type walls that we can pass through
	std::vector<BSPLeaf> leaves;
	findLeaves(nodes, numnodes - 1, leaves);
	_bspLeaves = leaves;

	// Match portals to create structure to travel through
//	for (std::vector<BSPLeaf>::iterator it; it!=_bspLeaves.end(); ++it) {
//		std::vector<seg_t> portal_single = (*it).portals();
		
		// First iterate through all leaves
//		for (std::vector<BSPLeaf>::iterator innerIt; innerIt!=_bspLeaves.end(); ++innerIt) {
			
			// For each leaf, go through and try to match the portals
//			std::vector<seg_t> portal_match = (*innerIt).portals();
//			for (std::vector<seg_t>::iterator innerIt; innerIt!=_bspLeaves.end(); ++innerIt
//				<#statements#>
//			}
//				continue;
//			if ((*it)->li
//		}
//	}
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
		
		/****************** only for debugging ********************************/
		// drop in an object to the scene so that we can do some visuals of where we
		// think the portals are located
		mapthing_t thing;
		thing.angle = 90;
		thing.type = MT_UNDEAD;
		thing.options = 7;
		for (unsigned int i=0; i<leaf.portals().size(); ++i) {
			thing.x = leaf.portals()[i].v1->x >> FRACBITS;
			thing.y = leaf.portals()[i].v1->y >> FRACBITS;
			P_SpawnMapThing(&thing);
		}
		
		return;
	}
		
	bsp = &nodes[bspNum];
	
	// Recursively search children
	findLeaves(nodes, bsp->children[0], leaves);
	findLeaves(nodes, bsp->children[1], leaves);
}