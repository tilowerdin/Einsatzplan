/*
 * SwimPlan.h
 *
 *  Created on: Jun 4, 2018
 *      Author: werdin
 */

#ifndef TEST_H_
#define TEST_H_

#include <stddef.h>
#include <map>
#include <string>

using namespace std;

enum Day { Mo, Di, Mi, Do, Fr, Sa, So };
enum Age { Kind, Jugend, AlterSack };

struct Time {
	int hour;
	Day day;
};

struct Slot {
	char* label;
	Time* time;

	void print();
};

struct PoolSlot : Slot {
	int lane;

	void print();
};

struct GymSlot : Slot {
};

template <typename A>
struct SlotArray {
	int count;
	A** slots;

	SlotArray<A>() {
		count = 0;
		slots = NULL;
	}

	void append(SlotArray<A> newSlots);
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

	void print();
};

int stoi(char* str);

map<int,char*> splitAndRemoveComments(string str);

Day toDay (char* dayStr);

SlotArray<PoolSlot> toPoolSlot (map<int, char*> line, char* pool);

SlotArray<GymSlot> toGymSlot (map<int, char*> line, char* label);

PoolSlot* poolSlot(Day day, int hour, int lane, char* pool);

GymSlot* gymSlot(Day day, int hour, char* label);

Group* group(char* name, Age age, int water, int lanes, int gym);

Age toAge(char* str);

string fromAge(Age age);

#endif /* SWIMPLAN_H_ */
