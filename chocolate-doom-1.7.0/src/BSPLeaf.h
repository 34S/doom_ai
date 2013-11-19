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
#include "r_defs.h"

/** TODO: class description. */
class BSPLeaf
{
public:
    
    /** Default constructor. */
    BSPLeaf();
    
	/** Constructor. */
    BSPLeaf(node_t* nodes, int bspNum);
	
    /** Destructor. */
    ~BSPLeaf();
	
	/** Adds a line segment to this convex polygon. */
	void addLineSegment(maplinedef_t line) { _borderSegments.push_back(line); }
	
	/** Accessor for the border segments containing portals to other convex polys. */
	std::vector<maplinedef_t> portals();
	
public:
	
	/** iVars.*/
	std::vector<maplinedef_t> _borderSegments;
};

#endif  // End of __BSP_LEAF_H__
