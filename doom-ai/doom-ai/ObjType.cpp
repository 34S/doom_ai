//
//  ObjType.cpp
//  doom-ai
//
//  Created by Colin Ray on 12/8/13.
//  Copyright (c) 2013 Trevor Richardson. All rights reserved.
//

#include "ObjType.h"

#include <iostream>

bool isMonster(mobjtype_t type) {
    switch (type) {
        case MT_POSSESSED:
        case MT_TROOP:
        case MT_SERGEANT:
        case MT_BRUISER:
            return true;
        default:
            return false;
    }
}

//MT_PLAYER,
//MT_POSSESSED,
//MT_SHOTGUY,
//MT_VILE,
//MT_FIRE,
//MT_UNDEAD,
//MT_TRACER,
//MT_SMOKE,
//MT_FATSO,
//MT_FATSHOT,
//MT_CHAINGUY,
//MT_TROOP,
//MT_SERGEANT,
//MT_SHADOWS,
//MT_HEAD,
//MT_BRUISER,
//MT_BRUISERSHOT,
//MT_KNIGHT,
//MT_SKULL,
//MT_SPIDER,
//MT_BABY,
//MT_CYBORG,
//MT_PAIN,
//MT_WOLFSS,
//MT_KEEN,
//MT_BOSSBRAIN,
//MT_BOSSSPIT,
//MT_BOSSTARGET,
//MT_SPAWNSHOT,
//MT_SPAWNFIRE,
//MT_BARREL,
//MT_TROOPSHOT,
//MT_HEADSHOT,
//MT_ROCKET,
//MT_PLASMA,
//MT_BFG,
//MT_ARACHPLAZ,
//MT_PUFF,
//MT_BLOOD,
//MT_TFOG,
//MT_IFOG,
//MT_TELEPORTMAN,
//MT_EXTRABFG,
