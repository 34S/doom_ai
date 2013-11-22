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

Agent::Agent(player_t* inGameAgent, node_t* bspNodes)
{
	_inGameAgent = inGameAgent;
	setupAgent(bspNodes);
}

Agent::~Agent()
{
    ;
}

void Agent::setupAgent(node_t *nodes)
{
	_agentMap = std::shared_ptr<AgentMap>(new AgentMap(nodes));
}

void Agent::update()
{
	std::cout << "Updating the path for the agent" << std::endl;
	
	_agentMap->chooseMove(_inGameAgent);
	
}