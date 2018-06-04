/*
 * SwimPlan.h
 *
 *  Created on: Jun 4, 2018
 *      Author: werdin
 */

#ifndef SWIMPLAN_H_
#define SWIMPLAN_H_

#include <string>

using namespace std;

enum Day { Mo, Di, Mi, Do, Fr, Sa, So };
enum Age { Kind, Jugend, AlterSack };

struct Time {
	int hour;
	Day day;
};

struct PoolSlot {
	int pool;
	int lane;
	Time time;
};

struct GymSlot {
	int gym;
	Time time;
};

struct Near {
	int gym;
	int pool;
};

struct Group {
	string name;
	Age age;
	int amountWater;
	int parallelLanes;
	int amountGym;
};

#endif /* SWIMPLAN_H_ */
