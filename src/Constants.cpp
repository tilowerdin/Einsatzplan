/*
 * Constants.cpp
 *
 *  Created on: Jun 6, 2018
 *      Author: werdin
 */

#include "../h/Constants.h"

const char* POOLTOKEN = ">Schwimmhalle";
const char* GYMTOKEN = ">Athletik";
const char* NEARTOKEN = ">Nahe";
const char* GROUPTOKEN = ">Gruppe";
const char* COMMENT = "--";

// age
const char* CHILDREN = "Kind";
const char* YOUTH = "Jugend";
const char* OLD = "AlterSack";

const int maxTimeChild = 19;
const int maxTimeYouth = 20;

// reading pool and gym
// lane need to be last since gym does not have a lane
const int COLLANE = 3;
const int COLDAY = 0;
const int COLFROM = 1;
const int COLTO = 2;

// reading groups
const int COLNAME = 0;
const int COLAGE = 1;
const int COLWATER = 2;
const int COLLANES = 3;
const int COLGYM = 4;



