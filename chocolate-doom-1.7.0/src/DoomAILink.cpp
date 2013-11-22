/**
 *  DoomAILink.cpp
 *  doom-ai
 *
 *  Created by Trevor Richardson on 11/18/13.
 *  Copyright 2013 Trevor Richardson. All rights reserved.
 */

// Local headers
#include "Agent.h"
#include "DoomAILink.h"

#include <iostream>

#ifdef __APPLE__
#pragma mark -
#pragma mark Initialization
#endif

static Agent* intelligent_agent;

bool createAgent(player_t* agent, node_t* nodes)
{
	intelligent_agent = new Agent(agent, nodes);
	return true;
}

void updateAgent()
{
	intelligent_agent->update();
}
