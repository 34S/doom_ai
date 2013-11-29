/**
 *  AgentMap.h
 *  doom-ai
 *
 *  Created by Trevor Richardson on 11/18/13.
 *  Copyright 2013 Trevor Richardson. All rights reserved.
 */

#ifndef __AGENT_MAP_H__
#define __AGENT_MAP_H__

// System headers
#include <map>
#include <vector>

// DOOM headers
#include "d_player.h"
#include "r_defs.h"

// Forward declarations
class Sector;


/** TODO: class description. */
class AgentMap
{
public:
    
	/** Default constructor. */
	AgentMap();

    /** Destructor. */
    ~AgentMap();
	
	/** Clears all search markers. */
	void clearSearch();
	
	/** Accessor for the sectors. */
	std::map<sector_t*, Sector>& sectors() { return _sectors; }
	
	/** Accessor for the endgame sector. */
	sector_t* endLevelSector() { return _endLevelSector; }
	
	
protected:
	
	/** Handles high-level map building procedure. */
	void _buildMap();
	
	/** iVars. */
	std::map<sector_t*, Sector>		_sectors;
	sector_t*						_endLevelSector;
	Sector*							_nextSector;
};

#endif  // End of __AGENT_MAP_H__