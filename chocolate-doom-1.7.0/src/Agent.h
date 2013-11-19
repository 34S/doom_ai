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

// Local headers
#include "AgentMap.h"


/** TODO: class description. */
class Agent
{
public:
    
    /** Default constructor. */
	Agent();
	
	/** Constructor. */
    Agent(node_t* nodes);
    
    /** Destructor. */
    ~Agent();
	
	/** Finds the leaf nodes given the root and builds . */
	
	/** Sets up the agent given the current scene. */
	void setupAgent(node_t* nodes);


protected:
	
	std::shared_ptr<AgentMap>		_agentMap;
};

#endif  // End of __Agent__
