/**
 *  Agent.cpp
 *  doom-ai
 *
 *  Created by Trevor Richardson on 11/18/13.
 *  Copyright 2013 Trevor Richardson. All rights reserved.
 */

// System headers
#include <iostream>

// Local headers
#include "Agent.h"


#ifdef __APPLE__
#pragma mark -
#pragma mark Initialization
#endif

Agent::Agent()
{
//    std::cout << "***************** ---------------something funny" << std::endl;	
}

Agent::Agent(node_t* nodes)
{
	setupAgent(nodes);
}

Agent::~Agent()
{
    ;
}

void Agent::setupAgent(node_t *nodes)
{
	_agentMap = std::shared_ptr<AgentMap>(new AgentMap(nodes));
}