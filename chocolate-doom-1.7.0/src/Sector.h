/**
 *  Sector.h
 *  doom-ai
 *
 *  Created by Trevor Richardson on 11/18/13.
 *  Copyright 2013 Trevor Richardson. All rights reserved.
 */

#ifndef __SECTOR_H__
#define __SECTOR_H__

// System headers
#include <vector>

// DOOM headers
#include "p_local.h"
#include "r_defs.h"


class Sector;

struct Portal {
	Sector* sector;
	line_t* line;
};

/** TODO: class description. */
class Sector
{
public:
    
    /** Default constructor. */
    Sector();
	
    /** Destructor. */
    ~Sector();
	
	/** Accessor for the possible paths from this sector. */
	inline void addPortal(Sector* sector, line_t* line) { _portals.push_back(Portal{sector, line}); }
	inline const std::vector<Portal>& portals() { return _portals; }
		
	/** Accessor for whether this leaf has been searched. */
	inline const bool& searched() { return _searched; }
	inline const Portal* searchParent() { return _searchParent; }
	void setSearched(const bool& searched, Portal* parent) {	_searched = searched;
																_searchParent = parent ; }
	
	
public:
	
	/** iVars.*/
	bool							_pathsBuilt;
	std::vector<Portal>				_portals;
	bool							_searched;
	Portal*							_searchParent;
};

#endif  // End of __SECTOR_H__
