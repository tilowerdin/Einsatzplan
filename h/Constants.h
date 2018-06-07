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

#define MAXSOLUTIONS 10
#define DAYSPAUSE 2

// token to be searched for in the input file
extern const char* POOLTOKEN;
extern const char* GYMTOKEN;
extern const char* NEARTOKEN;
extern const char* GROUPTOKEN;
extern const char* COMMENT;
extern const char* EXCLUDETOKEN;
extern const char* ONLYONETOKEN;

// age
extern const char* CHILDREN;
extern const char* YOUTH;
extern const char* OLD;

extern const int maxTimeChild;
extern const int maxTimeYouth;

// reading pool and gym
// lane need to be last since gym does not have a lane
extern const int COLLANE;
extern const int COLDAY;
extern const int COLFROM;
extern const int COLTO;

// reading groups
extern const int COLNAME;
extern const int COLAGE;
extern const int COLWATER;
extern const int COLLANES;
extern const int COLGYM;

// states while reading the input file
#define POOLSTATE 1
#define GYMSTATE 2
#define NEARSTATE 3
#define GROUPSTATE 4
#define EXCLUDESTATE 5
#define ONLYONESTATE 6

#endif /* CONSTANTS_H_ */
