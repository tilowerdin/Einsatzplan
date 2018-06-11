/*
 * SwimPlan.h
 *
 *  Created on: Jun 4, 2018
 *      Author: werdin
 */

#ifndef TEST_H_
#define TEST_H_

#include "Structures.h"
#include "Constants.h"
#include "Conversion.h"

#include <stddef.h>
#include <map>
#include <string>

using namespace std;


MyArray<PoolSlot> toPoolSlot (map<int, char*> line, char* pool);

MyArray<GymSlot> toGymSlot (map<int, char*> line, char* label);

PoolSlot* poolSlot(Day day, double hour, int lane, char* pool);

GymSlot* gymSlot(Day day, double hour, char* label);

Group* group(char* name, Age age, int water, int lanes, int gym);

void dfs(MyArray<Group> groups, bool* taken, MyArray<PoolSlot> pools, MyArray<GymSlot> gyms);

bool finish(bool onlyWater,bool* taken, int count, MyArray<Group> groups);

void dfs(MyArray<Group> groups, bool* taken, MyArray<GymSlot> gyms);

bool* falseArray(int count);

void printSolutionToFile(MyArray<Group> groups);

#endif /* SWIMPLAN_H_ */
