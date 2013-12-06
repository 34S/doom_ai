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
	
	/** Accessor for the lines from this sector. */
	inline void addLine(line_t* line) { _lines.push_back(line); }
	inline const std::vector<line_t*>& lines() { return _lines; }
		
	/** Accessor for setting the doom sector that this sector corresponds to. */
	inline void setDoomSector(sector_t* sector) { _doomSector = sector; }
	inline sector_t* doomSector() { return _doomSector; }
	
	/** Accessor for whether this leaf has been searched. */
	inline const bool& searched() { return _searched; }
	inline const std::shared_ptr<Portal> searchParent() { return _searchParent; }
	void setSearched(const bool& searched, std::shared_ptr<Portal> parent) {_searched = searched;
																		_searchParent = parent ; }
	void setSearchChild(std::shared_ptr<Portal> child) { _searchChild = child; }
	inline const std::shared_ptr<Portal> searchChild() { return _searchChild; }
	
	
public:
	
	/** iVars.*/
	sector_t*						_doomSector;
	std::vector<line_t*>				_lines;
	bool							_pathsBuilt;
	std::vector<Portal>				_portals;
	bool							_searched;
	std::shared_ptr<Portal>			_searchChild;
	std::shared_ptr<Portal>			_searchParent;
};

#endif  // End of __SECTOR_H__
