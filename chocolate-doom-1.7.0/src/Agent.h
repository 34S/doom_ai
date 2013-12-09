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

// Forward declaration
class BSPLeaf;
struct Portal;

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
	
	/** Selects the next move for the agent. */
	void chooseMove();
    
    /** Selects where the agent should aim. */
    void chooseAim();
	
	/** Search methods returning next position to move toward. */
	Sector* depthFirstSearch(Sector* leaf, std::shared_ptr<Portal> parent);
	
	/** Utility for calculating the square of the distance. */
	float distanceMagnitude(float agentX, float agentY, float targetX, float targetY);
	
	/** Sets up the agent given the current scene. */
	void setupAgent();
	
	/** Sums all attractive forces for the agent. */
	void sumAttractive(float& attractX, float& attractY);
	
	/** Sums all repulsive forces for the agent. */
	void sumRepulsive(float& repulseX, float& repulseY);
	
	/** Uses any specials contains in the current sector. */
	void useSpecials();

	/** Updates knowledge with new scene information. */
	void update();


protected:
	
	std::shared_ptr<AgentMap>		_agentMap;
	unsigned int					_agentProximity;
	Sector*							_currentSector;
	bool							_levelComplete;
	bool							_findNewRoute;
	player_t*						_inGameAgent;
	std::shared_ptr<Portal>			_nextMove;
	int								_nodeCount;
	int								_priorX, _priorY;
};

#endif  // End of __Agent__
