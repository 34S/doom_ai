/**
 *  Sector.cpp
 *  doom-ai
 *
 *  Created by Trevor Richardson on 11/18/13.
 *  Copyright 2013 Trevor Richardson. All rights reserved.
 */

// System headers
#include <iostream>

// Local headers
#include "Sector.h"


#ifdef __APPLE__
#pragma mark -
#pragma mark Initialization
#endif

Sector::Sector()
{
	_pathsBuilt = false;
	_searched = false;
}

Sector::~Sector()
{
    ;
}