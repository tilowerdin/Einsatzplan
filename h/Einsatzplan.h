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

PoolSlot* poolSlot(Day day, int hour, int lane, char* pool);

GymSlot* gymSlot(Day day, int hour, char* label);

Group* group(char* name, Age age, int water, int lanes, int gym);

void dfs(int from, bool* taken, MyArray<PoolSlot> pools, MyArray<Group> groups, MyArray<GymSlot> gyms);

bool finish(bool onlyWater,bool* taken, int count, MyArray<Group> groups);

void dfs(int from, bool* taken, MyArray<GymSlot> gyms, MyArray<Group> groups);

bool* falseArray(int count);

void printSolutionToFile(MyArray<Group> groups);

#endif /* SWIMPLAN_H_ */
