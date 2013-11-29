/**
 *  Agent.cpp
 *  doom-ai
 *
 *  Created by Trevor Richardson on 11/18/13.
 *  Copyright 2013 Trevor Richardson. All rights reserved.
 */

// System headers
#include <iostream>
#include <math.h>

// DOOM headers
extern "C" {
#include "p_local.h"
}


// Local headers
#include "Agent.h"
#include "Sector.h"

extern "C" void P_SpawnMapThing (mapthing_t* mthing);

#ifdef __APPLE__
#pragma mark -
#pragma mark Initialization
#endif

Agent::Agent()
{
	;
}

Agent::Agent(player_t* inGameAgent, node_t* bspNodes)
{
	_inGameAgent = inGameAgent;
	_stuck = false;
	_findNewRoute = true;
	setupAgent();
}

Agent::~Agent()
{
    ;
}

void Agent::setupAgent()
{
	_agentMap = std::shared_ptr<AgentMap>(new AgentMap());
}

void Agent::update()
{
	//	std::cout << "Updating the path for the agent" << std::endl;
	
//	static int count = 0;
//	if (count > 1){
//		return;
//	}
//	++count;
//	count = 0;
	
	// Figure out what sector the agent is located in
	Sector agent_sector;
	for (std::map<sector_t*, Sector>::iterator it = _agentMap->sectors().begin();
		 it!=_agentMap->sectors().end(); ++it) {
		if (_inGameAgent->mo->subsector->sector == it->first) {
			_currentSector = &it->second;
			break;
		}
	}
	
	// If we need to re-route, perform a new search
	if (_findNewRoute) {
		
		// First find the end game sector from the current sector
		Sector* endSector = NULL;
		_nodeCount = 0;
		endSector = depthFirstSearch(_currentSector, NULL);
		std::cout << "Nodes searched: " << _nodeCount << std::endl;
		if (endSector == NULL) {
			return;
		}
		
		// Create the search tree
		std::shared_ptr<Portal> parent = endSector->searchParent();
		std::shared_ptr<Portal> prior = endSector->searchParent();
		while (parent != NULL) {
			prior = parent;
			parent = parent->sector->searchParent();
			if (parent)
				parent->sector->setSearchChild(prior);
		}
		_nextMove = prior;
		
		_findNewRoute = false;
	}
	
	// If we're close to the target, aim for the next target
	int target_distance = (abs(_nextMove->line->v1->x - _inGameAgent->mo->x) +
									abs(_nextMove->line->v1->y - _inGameAgent->mo->y)) >> FRACBITS;
	if (target_distance < 200) {
		_nextMove = _currentSector->searchChild();
	}
	
	chooseMove();
}

#ifdef __APPLE__
#pragma mark -
#pragma mark Intelligence methods
#endif

void Agent::chooseMove()
{	
	// Move the agent
	float attract_x=0; float attract_y=0;
	sumAttractive(attract_x, attract_y);
	float repulse_x=0; float repulse_y=0;
	sumRepulsive(repulse_x, repulse_y);
	
	// Implement low-pass filter
	float constant = 0.2;
	float mom_x = ((attract_x + repulse_x) * pow(2, FRACBITS)) * constant + _priorX * (1-constant);
	float mom_y = ((attract_y + repulse_y) * pow(2, FRACBITS)) * constant + _priorY * (1-constant);
	
	// Update the agents momentum
	_inGameAgent->mo->momx = mom_x;
	_inGameAgent->mo->momy = mom_y;
	
	// Get setup for the next tic
	_agentMap->clearSearch();
	_priorX = _inGameAgent->mo->momx;
	_priorY = _inGameAgent->mo->momy;
	_stuck = false;
}

void Agent::sumAttractive(float& attractX, float& attractY)
{
	// Attraction to all target portals to the end of the game
	float target_weight = 500;
	bool found_target = addTargetForces(attractX, attractY);
	if (!found_target) {
		attractX = 0;
		attractY = 0;
	}
	attractX *= target_weight;
	attractY *= target_weight;
}

bool Agent::addTargetForces(float& attractX, float& attractY)
{
	float agent_x = (_inGameAgent->mo->x) >> FRACBITS;
	float agent_y = (_inGameAgent->mo->y) >> FRACBITS;
	
	// Attract the agent to the
	float temp_x = (_nextMove->line->v1->x + _nextMove->line->v2->x) / 2.0;
	int target_x = (int) temp_x >> FRACBITS;
	float temp_y = (_nextMove->line->v1->y + _nextMove->line->v2->y) / 2.0;
	int target_y = (int) temp_y >> FRACBITS;
	float mag = distanceMagnitude(agent_x, agent_y, target_x, target_y);
	attractX += (target_x - agent_x) / mag;
	attractY += (target_y - agent_y) / mag;
	
	return true;
}

void Agent::sumRepulsive(float& repulseX, float& repulseY)
{
	float agent_x = (_inGameAgent->mo->x) >> FRACBITS;
	float agent_y = (_inGameAgent->mo->y) >> FRACBITS;
	
	// Apply a force for each wall
	float line_force = 1;
	std::vector<line_t*> lines = _currentSector->lines();
	float line_repulse_x, line_repulse_y;
	
	// Use a thing to check if the wall is walkable
	mobj_t thing; // this can be invalid and is handled properly
	thing.radius = 1000576;
	thing.height = 528576;
	thing.flags = thing.flags & 0x0111;
	for (unsigned int i=0; i<lines.size(); ++i) {
		float temp_x = (lines[i]->v1->x + lines[i]->v2->x) / 2.0;
		float temp_y = (lines[i]->v1->y + lines[i]->v2->y) / 2.0;
		
		// If we can walk through this line, don't add repulsion forces
		if (P_CheckPosition(&thing, temp_x, temp_y))
			continue;

		int x = (int)temp_x >> FRACBITS;
		int y = (int)temp_y >> FRACBITS;
		
		float mag = distanceMagnitude(agent_x, agent_y, x, y);
		line_repulse_x = (x - agent_x) / mag;
		line_repulse_y = (y - agent_y) / mag;
		
		repulseX -= line_force * line_repulse_x;
		repulseY -= line_force * line_repulse_x;
	}
	
	// Apply a force for each "thing"
	float thing_force = 1;
	mobj_t* sec_thing = _currentSector->doomSector()->thinglist;
	float thing_repulse_x, thing_repulse_y;
	while (sec_thing) {
		mobjtype_t type = sec_thing->type;
		if (type & MF_SOLID) {
			int x = sec_thing->x >> FRACBITS;
			int y = sec_thing->y >> FRACBITS;
			
			float mag = distanceMagnitude(agent_x, agent_y, x, y);
			thing_repulse_x = (x - agent_x) / mag;
			thing_repulse_y = (y - agent_y) / mag;
			
			repulseX -= thing_force * thing_repulse_x;
			repulseY -= thing_force * thing_repulse_y;
		}
		sec_thing = sec_thing->snext;
	}
}

#ifdef __APPLE__
#pragma mark -
#pragma mark Search Methods
#endif

Sector* Agent::depthFirstSearch(Sector* sec, std::shared_ptr<Portal> parent)
{
	// Early out if we can, otherwise set the node to searched
	if (sec->searched())
		return NULL;
	else
		sec->setSearched(true, parent);
	++_nodeCount;
	
	if (!sec->portals().size())
		return NULL;
	
	// Recursively launch off DFS if we find more children until will find the end game
	for (std::vector<Portal>::const_iterator it=sec->portals().begin();
		 it!=sec->portals().end(); ++it)
	{
		std::shared_ptr<Portal> portal = std::shared_ptr<Portal>(new Portal);
		portal->sector = sec;
		portal->line = (*it).line;
		if ((*it).line->frontsector == _agentMap->endLevelSector())
		{
			(*it).sector->setSearched(true, portal);
			return (*it).sector;
		}
		Sector* endSector = depthFirstSearch((*it).sector, portal);
		if (endSector != NULL) {
			return endSector;
		}
	}
	
	return NULL;
}

#ifdef __APPLE__
#pragma mark -
#pragma mark Utility Methods
#endif

float Agent::distanceMagnitude(float agentX, float agentY, float itemX, float itemY)
{
	return pow(abs(itemX - agentX), 2) + pow(abs(itemY - agentY), 2);
}
