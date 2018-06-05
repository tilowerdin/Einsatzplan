/*
 * SwimPlan.h
 *
 *  Created on: Jun 4, 2018
 *      Author: werdin
 */

#ifndef SWIMPLAN_H_
#define SWIMPLAN_H_

#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <list>
#include <stdlib.h>
#include <string.h>

using namespace std;

enum Day { Mo, Di, Mi, Do, Fr, Sa, So };
enum Age { Kind, Jugend, AlterSack };

struct Time {
	int hour;
	Day day;
};

struct PoolSlot {
	char* pool;
	int lane;
	Time* time;
};

struct GymSlot {
	int gym;
	Time* time;
};

struct Near {
	int gym;
	int pool;
};

struct Group {
	char* name;
	Age age;
	int amountWater;
	int parallelLanes;
	int amountGym;
};

int stoi(char* str);

map<int,char*> splitAndRemoveComments(string str);

Day toDay (char* dayStr);

pair<int, PoolSlot**> toPoolSlot (map<int, char*> line);

PoolSlot* poolSlot(Day day, int hour, int lane, char* pool);

pair<int, PoolSlot**> append(pair<int, PoolSlot**> first, pair<int, PoolSlot**> second);

void printPool(PoolSlot* poolSlot);

#endif /* SWIMPLAN_H_ */
