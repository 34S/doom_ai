/**
 *  BSPLeaf.h
 *  doom-ai
 *
 *  Created by Trevor Richardson on 11/18/13.
 *  Copyright 2013 Trevor Richardson. All rights reserved.
 */

#ifndef __BSP_LEAF_H__
#define __BSP_LEAF_H__

// System headers
#include <vector>

// DOOM headers
#include "p_local.h"
#include "r_defs.h"

class BSPLeaf;

struct Portal {
	BSPLeaf* leaf;
	seg_t* seg;
};

/** TODO: class description. */
class BSPLeaf
{
public:
    
    /** Default constructor. */
    BSPLeaf();
    
	/** Constructor. */
    BSPLeaf(int bspNum);
	
    /** Destructor. */
    ~BSPLeaf();
	
	/** Accessor for the border segments containing portals to other convex polys. */
	inline const std::vector<seg_t>& lineSegments() { return _lineSegs; }
	
	/** Accessor for the possible paths from this leaf. */
	inline void addPortal(BSPLeaf* path, seg_t* seg) { _portals.push_back(Portal{path, seg}); }
	inline const std::vector<Portal>& portals() { return _portals; }
	
	/** Accessor for whether this leaf has has its paths built. */
	inline const bool& pathsBuilt() { return _pathsBuilt; }
	void setPathsBuilt(const bool& pathsBuilt) { _pathsBuilt = pathsBuilt; }
	
	/** Accessor for this leafs subsector number. */
	inline const subsector_t* subsector() { return _subsector; }
	
	
public:
	
	/** iVars.*/
	std::vector<seg_t>				_lineSegs;
	bool							_pathsBuilt;
	std::vector<Portal>				_portals;
	subsector_t*					_subsector;
};

#endif  // End of __BSP_LEAF_H__
