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
	#include "d_event.h"
	#include "p_local.h"
}

// Local headers
#include "Agent.h"
#include "Sector.h"


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
	_agentProximity = 300;
	_inGameAgent = inGameAgent;
	_findNewRoute = true;
	_levelComplete = false;
	_priorX = 0; _priorY = 0;
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
	// We're at the end of the level, activate the end-game
	if (_levelComplete) {
		useSpecials();
		return;
	}
	
	// Figure out what sector the agent is located in
	Sector agent_sector;
	for (std::map<sector_t*, Sector>::iterator it = _agentMap->sectors().begin();
		 it!=_agentMap->sectors().end(); ++it) {
		if (_inGameAgent->mo->subsector->sector == it->first) {
			_currentSector = &it->second;
			break;
		}
	}
	
	// If we need to re-route, perform a new search. This is only done once at the very
	// beginning currently, but can be used later if we encounter something we need to avoid.
	if (_findNewRoute) {
		
		// First find the end game sector from the current sector
		Sector* endSector = NULL;
		_nodeCount = 0;
		endSector = depthFirstSearch(_currentSector, NULL);
		std::cout << "Nodes searched: " << _nodeCount << std::endl;
		if (endSector == NULL) {
			std::cout << "No end sector found.. Returning" << std::endl;
			return;
		}
		
		// Create the search tree so that we can search parent->child or child->parent
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
	
	// If we've reached the end sector of the level, return and do nothing
	if (_currentSector->doomSector() == _agentMap->endLevelSector()) {
		std::cout << "We're done with the level.. Completing" << std::endl;
		_levelComplete = true;
		
		return;
	}
	
	// If we're close to the current target location, change target to next location
	int target_distance = (abs(_nextMove->line->v1->x - _inGameAgent->mo->x) +
									abs(_nextMove->line->v1->y - _inGameAgent->mo->y)) >> FRACBITS;
	if (target_distance < _agentProximity) {
		if (_currentSector->searchChild()) {
			_nextMove = _currentSector->searchChild();
		}
	}
	
	// Calculates the necessary movement given all force fields surrounding our agent
	chooseMove();
	
	// If there are any special lines in front of us, use them
	useSpecials();
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
	float constant = 0.1;
	float mom_x = ((attract_x + repulse_x) * pow(2, FRACBITS)) * constant + _priorX * (1-constant);
	float mom_y = ((attract_y + repulse_y) * pow(2, FRACBITS)) * constant + _priorY * (1-constant);
	
	// Update the agents momentum
	int speed = 2 << FRACBITS;
	float mag = sqrtf(mom_x*mom_x + mom_y*mom_y);
	_inGameAgent->mo->momx = (mom_x / mag) * speed;
	_inGameAgent->mo->momy = (mom_y / mag) * speed;
	
	// Get setup for the next tic
	_agentMap->clearSearch();
	_priorX = _inGameAgent->mo->momx;
	_priorY = _inGameAgent->mo->momy;
}

void Agent::sumAttractive(float& attractX, float& attractY)
{
	// Attraction to all target portals to the end of the game
	float target_force = 600;

	// Get the current agent position
	float agent_x = (_inGameAgent->mo->x) >> FRACBITS;
	float agent_y = (_inGameAgent->mo->y) >> FRACBITS;
	
	// Attract the agent to the next target
	float temp_x = (_nextMove->line->v1->x + _nextMove->line->v2->x) / 2.0;
	int target_x = (int) temp_x >> FRACBITS;
	float temp_y = (_nextMove->line->v1->y + _nextMove->line->v2->y) / 2.0;
	int target_y = (int) temp_y >> FRACBITS;
	float mag = distanceMagnitude(agent_x, agent_y, target_x, target_y);
	attractX += ((target_x - agent_x) / mag) * target_force;
	attractY += ((target_y - agent_y) / mag) * target_force;
}

void Agent::sumRepulsive(float& repulseX, float& repulseY)
{
	float agent_x = (_inGameAgent->mo->x) >> FRACBITS;
	float agent_y = (_inGameAgent->mo->y) >> FRACBITS;
	
	// Apply a force for each wall
	float line_force = 20;
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
	float thing_force = 15;
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

void Agent::useSpecials()
{
	std::vector<line_t*> lines = _currentSector->lines();
	if (_currentSector->searchChild()) {
		lines.insert(lines.end(), _currentSector->searchChild()->sector->lines().begin(),
					 _currentSector->searchChild()->sector->lines().end());
	}
	for (unsigned int i=0; i<lines.size(); ++i) {
		int line_distance = (abs(lines[i]->v1->x - _inGameAgent->mo->x) +
							 abs(lines[i]->v1->y - _inGameAgent->mo->y)) >> FRACBITS;
		switch (lines[i]->special)
		{
			case 11:
			{
				P_UseSpecialLine(_inGameAgent->mo, lines[i],0);
				return;
			}
				
			case 1:		// Vertical Door
			case 26:		// Blue Door/Locked
			case 27:		// Yellow Door /Locked
			case 28:		// Red Door /Locked
				
			case 31:		// Manual door open
			case 32:		// Blue locked door open
			case 33:		// Red locked door open
			case 34:		// Yellow locked door open
				
			case 117:		// Blazing door raise
			case 118:		// Blazing door open
			{
				if (line_distance > _agentProximity) {
					continue;
				}
				sector_t* sec = sides[lines[i]->sidenum[0^1]].sector;
				vldoor_t* door = (vldoor_t*)sec->specialdata;
				if (sec->specialdata) {
					if (door->direction == 1) {
						return;
					}
				}
				EV_VerticalDoor (lines[i], _inGameAgent->mo);
				break;
			}
			default:
				break;
		}
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
