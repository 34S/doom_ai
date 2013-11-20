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
	inline const std::vector<seg_t>& portals() { return _portals; }
	
public:
	
	/** iVars.*/
	std::vector<seg_t>		_portals;
};

#endif  // End of __BSP_LEAF_H__
