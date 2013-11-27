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

#ifdef __APPLE__
#pragma mark -
#pragma mark Utility
#endif

void Agent::chooseMove()
{
//	static bool if_done_once = false;
//	if (if_done_once)
//		return;
//	if_done_once = true;
//	
	for (std::map<sector_t*, Sector>::iterator it = _agentMap->sectors().begin();
															it!=_agentMap->sectors().end(); ++it) {
		if (_inGameAgent->mo->subsector->sector == it->first)
		{
			Sector* endSector = NULL;
			endSector = depthFirstSearch(&it->second, NULL);
			std::cout << "Nodes searched: " << _nodeCount << std::endl;
			
			if (endSector == NULL) {
				std::cout << "Could not find the end sector.. Returning" << std::endl;
				return;
			}
			
			const Portal* parent = endSector->searchParent();
			const Portal* prior = endSector->searchParent();
			const Portal* next_move = endSector->searchParent();
			while (parent != NULL) {
				
				float pos_x = ((parent->line->v1->x) + (parent->line->v2->x)) / 2.0;
				float pos_y = ((parent->line->v1->y) + (parent->line->v2->y)) / 2.0;
				
				mapthing_t thing3;
				thing3.angle = 90;
				thing3.type = MT_UNDEAD;
				thing3.options = 7;
				thing3.x = (int) pos_x >> FRACBITS;
				thing3.y = (int) pos_y >> FRACBITS;
				P_SpawnMapThing(&thing3);
				
				next_move = prior;
				prior = parent;
				parent = parent->sector->searchParent();
			}
			
			int move_x = next_move->line->v1->x - _inGameAgent->mo->x;
			int move_y = next_move->line->v1->y - _inGameAgent->mo->y;
			_inGameAgent->mo->momx = move_x / 400;
			_inGameAgent->mo->momy = move_y / 400;
			
			_agentMap->clearSearch();
			_nodeCount = 0;
			
			return;
		}
	}
}

void Agent::update()
{
//	std::cout << "Updating the path for the agent" << std::endl;
	
	chooseMove();
}

#ifdef __APPLE__
#pragma mark -
#pragma mark Search Methods
#endif

Sector* Agent::depthFirstSearch(Sector* sec, Portal* parent)
{
	if (sec->searched())
		return NULL;
	else
		sec->setSearched(true, parent);
	
	if (!sec->portals().size())
		return NULL;
	
	++_nodeCount;
	
	for (std::vector<Portal>::const_iterator it=sec->portals().begin();
		 it!=sec->portals().end(); ++it)
	{
		// TODO: cleanup memory
		Portal* portal = new Portal;
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
	// TODO: clear search
}
