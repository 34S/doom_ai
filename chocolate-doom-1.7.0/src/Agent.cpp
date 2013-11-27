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
	
	chooseMove();
}

#ifdef __APPLE__
#pragma mark -
#pragma mark Intelligence methods
#endif

void Agent::chooseMove()
{
	// If we haven't encountered anything, stay on the same course
	
	
//	static int run_counter = 0;
//	++run_counter;
//	if (run_counter <= 5)
//		return;
//	run_counter = 0;
	
//	for (std::map<sector_t*, Sector>::iterator it = _agentMap->sectors().begin();
//															it!=_agentMap->sectors().end(); ++it) {
//		int current_x = _inGameAgent->mo->x;
//		int current_y = _inGameAgent->mo->y;
//		
////		int travel_distance = abs((current_x-_priorX) + (current_y-_priorY));
////		if (travel_distance < (1 << FRACBITS)) {
////			_inGameAgent->mo->momx = _inGameAgent->mo->momx * 20;
////			_inGameAgent->mo->momy = _inGameAgent->mo->momy * 20;
////		}
//		
//		if (_inGameAgent->mo->subsector->sector == it->first)
//		{
//			Sector* endSector = NULL;
//			endSector = depthFirstSearch(&it->second, NULL);
//			std::cout << "Nodes searched: " << _nodeCount << std::endl;
//			
//			if (endSector == NULL) {
//				std::cout << "Could not find the end sector.. Returning" << std::endl;
//				return;
//			}
//			
//			std::shared_ptr<Portal> parent = endSector->searchParent();
//			std::shared_ptr<Portal> prior = endSector->searchParent();
//			std::shared_ptr<Portal> next_move = endSector->searchParent();
//			while (parent != NULL) {
//				
////				float pos_x = ((parent->line->v1->x) + (parent->line->v2->x)) / 2.0;
////				float pos_y = ((parent->line->v1->y) + (parent->line->v2->y)) / 2.0;
//				
////				mapthing_t thing3;
////				thing3.angle = 90;
////				thing3.type = MT_UNDEAD;
////				thing3.options = 7;
////				thing3.x = (int) pos_x >> FRACBITS;
////				thing3.y = (int) pos_y >> FRACBITS;
////				P_SpawnMapThing(&thing3);
//				
////				int travel_distance = abs((current_x-_priorX) + (current_y-_priorY));
////				if (travel_distance < 20 && _nextMove) {
////					++_moveAttempts;
////				}
////				else {
////					_nextMove = parent;
////				}
//				
//				prior = parent;
//				parent = parent->sector->searchParent();
//				if (parent)
//					parent->sector->setSearchChild(prior);
////				++current_depth;
//			}
////			int travel_distance = abs((current_x-_priorX) + (current_y-_priorY));
////			if (travel_distance < (1 << FRACBITS)) {
////				++_moveAttempts;
////			}
////			else {
////				_moveAttempts = 0;
////			}
//			
////			updateAgentMomentum();
//			
//			// Make sure we're not walking toward a target that is right beside us
//			int target_distance = abs(prior->line->v1->x - _inGameAgent->mo->x) +
//												abs(prior->line->v1->y - _inGameAgent->mo->y);
//			next_move = prior->sector->searchChild();
//			while (target_distance < (1 << FRACBITS)) {
//				target_distance = abs(next_move->line->v1->x - _inGameAgent->mo->x) +
//												abs(next_move->line->v1->y - _inGameAgent->mo->y);
//				next_move = next_move->sector->searchChild();
//			}
//				
//			int move_speed = 7 << FRACBITS;
//			float mag = pow(next_move->line->v1->x - _inGameAgent->mo->x, 2) +
//												pow(next_move->line->v1->y - _inGameAgent->mo->y,2);
//			float move_x = move_speed*(pow(next_move->line->v1->x - _inGameAgent->mo->x,2) / mag);
//			float move_y = move_speed*(pow(next_move->line->v1->y - _inGameAgent->mo->y, 2) / mag);

	Sector agent_sector;
	for (std::map<sector_t*, Sector>::iterator it = _agentMap->sectors().begin();
															it!=_agentMap->sectors().end(); ++it) {
		if (_inGameAgent->mo->subsector->sector == it->first) {
			agent_sector = it->second;
			break;
		}
	}
	
	float attract_x=0; float attract_y=0;
//	sumAttractive(agent_sector, attract_x, attract_y);
	float repulse_x=0; float repulse_y=0;
	sumRepulsive(agent_sector, repulse_x, repulse_y);
			
	_inGameAgent->mo->momx = (attract_x + repulse_x) * pow(2, FRACBITS);
	_inGameAgent->mo->momy = (attract_y + repulse_y) * pow(2, FRACBITS);
	
	_agentMap->clearSearch();
}

void Agent::sumAttractive(Sector& agentSector, float& attractX, float& attractY)
{
//	float agent_x = (_inGameAgent->mo->x) >> FRACBITS;
//	float agent_y = (_inGameAgent->mo->y) >> FRACBITS;
	
	// Attraction to all target portals to the end of the game
	float target_weight = 400;
	bool found_target = addTargetForces(agentSector, attractX, attractY);
	if (!found_target) {
		attractX = 0;
		attractY = 0;
	}
	attractX += target_weight * attractX;
	attractY += target_weight * attractY;
}

void Agent::sumRepulsive(Sector& agentSector, float& repulseX, float& repulseY)
{
	float agent_x = (_inGameAgent->mo->x) >> FRACBITS;
	float agent_y = (_inGameAgent->mo->y) >> FRACBITS;
	
	// Apply a force for each wall
	float line_force = 400;
	std::vector<line_t*> lines = agentSector.lines();
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
		
		repulseX += line_force * line_repulse_x;
		repulseY += line_force * line_repulse_x;
	}
	
	// Apply a force for each thing
	float thing_force = 50;
	mobj_t* sec_thing = agentSector.doomSector()->thinglist;
	float thing_repulse_x, thing_repulse_y;
	while (sec_thing) {
		mobjtype_t type = sec_thing->type;
		if (type & MF_SOLID) {
			int x = sec_thing->x >> FRACBITS;
			int y = sec_thing->y >> FRACBITS;
			
			float mag = distanceMagnitude(agent_x, agent_y, x, y);
			thing_repulse_x = (x - agent_x) / mag;
			thing_repulse_y = (y - agent_y) / mag;
			
//			repulseX -= thing_force * thing_repulse_x;
//			repulseY -= thing_force * thing_repulse_y;
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

bool Agent::addTargetForces(Sector& startSector, float& attractX, float& attractY)
{
	float agent_x = (_inGameAgent->mo->x) >> FRACBITS;
	float agent_y = (_inGameAgent->mo->y) >> FRACBITS;
	
	// First find the end game sector
	Sector* endSector = NULL;
	_nodeCount = 0;
	endSector = depthFirstSearch(&startSector, NULL);
	std::cout << "Nodes searched: " << _nodeCount << std::endl;
	if (endSector == NULL) {
		return false;
	}
	
	// Climb back up and build the path to our target
	std::shared_ptr<Portal> parent = endSector->searchParent();
	std::shared_ptr<Portal> prior = endSector->searchParent();
	while (parent != NULL) {
		
		// Find force from the portal that we transfered through starting at the end game
		float temp_x = (parent->line->v1->x + parent->line->v2->x) / 2.0;
		int target_x = (int) temp_x >> FRACBITS;
		float temp_y = (parent->line->v1->y + parent->line->v2->y) / 2.0;
		int target_y = (int) temp_y >> FRACBITS;
		float mag = distanceMagnitude(agent_x, agent_y, target_x, target_y);
		
//		if (mag > 15000) {
			attractX -= (target_x - agent_x) / mag;
			attractY -= (target_y - agent_y) / mag;
//		}
		
		// Climb to the next parent until we reach our current position
		prior = parent;
		parent = parent->sector->searchParent();
		if (parent)
			parent->sector->setSearchChild(prior);
	}
	
	// Make sure we're not walking toward a target that is right beside us
//	std::shared_ptr<Portal> next_move = prior->sector->searchChild();
//	int target_distance = abs(next_move->line->v1->x - _inGameAgent->mo->x) +
//												abs(next_move->line->v1->y - _inGameAgent->mo->y);
//	while (target_distance < (1 << FRACBITS)) {
//		next_move = next_move->sector->searchChild();
//		target_distance = abs(next_move->line->v1->x - _inGameAgent->mo->x) +
//												abs(next_move->line->v1->y - _inGameAgent->mo->y);
//	}
//
//	float x_temp_calc = (next_move->line->v1->x + next_move->line->v2->x) / 2.0;
//	float y_temp_calc = (next_move->line->v1->y + next_move->line->v2->y) / 2.0;
//	xPos = ((int)x_temp_calc) >> FRACBITS;
//	yPos = ((int)y_temp_calc) >> FRACBITS;
	
	return true;
}
