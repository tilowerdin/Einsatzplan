/*
 * Constants.h
 *
 *  Created on: Jun 4, 2018
 *      Author: werdin
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <string>

using namespace std;

const int MAXSOLUTIONS = 10;


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


const int POOLSTATE = 1;
const int GYMSTATE = 2;
const int NEARSTATE = 3;
const int GROUPSTATE = 4;

#endif /* CONSTANTS_H_ */
