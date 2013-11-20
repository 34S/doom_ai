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
	for (std::vector<BSPLeaf>::iterator it; it!=_bspLeaves.end(); ++it) {
		std::vector<seg_t> portal_single = (*it).portals();
		for (std::vector<BSPLeaf>::iterator innerIt; innerIt!=_bspLeaves.end(); ++innerIt) {
			std::vector<seg_t> portal_match = (*innerIt).portals();
//			for (std::vector<seg>::iterator innerIt; innerIt!=_bspLeaves.end(); ++innerIt
//				<#statements#>
//			}
//				continue;
//			if ((*it)->li
		}
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
		
		leaves.push_back(BSPLeaf(bspNum));
		
		return;
	}
		
	bsp = &nodes[bspNum];
	
	// Recursively search children
	findLeaves(nodes, bsp->children[0], leaves);
	findLeaves(nodes, bsp->children[1], leaves);
}