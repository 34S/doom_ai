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
	std::vector<BSPLeaf> leaves;
	findLeaves(nodes, numnodes - 1, leaves);
	_bspLeaves = leaves;
}

void AgentMap::findLeaves(node_t* nodes, const int& bspNum, std::vector<BSPLeaf>& leaves) const
{
	node_t*	bsp;
	
	static unsigned int leaf_count = 0;
	
	// Found a subsector?
	if (bspNum & NF_SUBSECTOR)
	{
		std::cout << "Need to build leaves..." << leaf_count << std::endl;
		++leaf_count;
		return;
	}
		
	bsp = &nodes[bspNum];
	
	// Recursively search children
	findLeaves(nodes, bsp->children[0], leaves);
	findLeaves(nodes, bsp->children[1], leaves);
}