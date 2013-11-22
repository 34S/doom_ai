/**
 *  AgentMap.h
 *  doom-ai
 *
 *  Created by Trevor Richardson on 11/18/13.
 *  Copyright 2013 Trevor Richardson. All rights reserved.
 */

#ifndef __AGENT_MAP_H__
#define __AGENT_MAP_H__

// System headers
#include <vector>

// DOOM headers
#include "d_player.h"
#include "r_defs.h"

// Forward declarations
class BSPLeaf;


/** TODO: class description. */
class AgentMap
{
public:
    
	/** Default constructor. */
	AgentMap();
	
    /** Constructor. */
    AgentMap(node_t* nodes);
    
    /** Destructor. */
    ~AgentMap();
	
	/** Selects the next move for the agent. */
	void chooseMove(player_t* inGamePlayer);
	
	/** Parses the entire node tree looking for leaves. */
	void findLeaves(node_t* nodes, const int& bspNum, std::vector<BSPLeaf>& leaves) const;
	
protected:
	
	/** Handles high-level map building procedure. */
	void _buildMap(node_t* nodes);
	
	/** iVars. */
	std::vector<BSPLeaf>	_bspLeaves;
};

#endif  // End of __AGENT_MAP_H__