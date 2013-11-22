/**
 *  Agent.h
 *  doom-ai
 *
 *  Created by Trevor Richardson on 11/18/13.
 *  Copyright 2013 Trevor Richardson. All rights reserved.
 */

#ifndef __Agent__
#define __Agent__

// System headers
#include <memory>

// DOOM headers
#include "r_defs.h"
#include "d_player.h"

// Local headers
#include "AgentMap.h"


/** TODO: class description. */
class Agent
{
public:
    
    /** Default constructor. */
	Agent();
	
	/** Constructor. */
    Agent(player_t* inGameAgent, node_t* bspNodes);
    
    /** Destructor. */
    ~Agent();
	
	/** Updates knowledge with new scene information. */
	void update();
	
	/** Sets up the agent given the current scene. */
	void setupAgent(node_t* nodes);


protected:
	
	player_t*						_inGameAgent;
	std::shared_ptr<AgentMap>		_agentMap;
};

#endif  // End of __Agent__
