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

	bool equals(Time time) {
		return hour == time.hour && day == time.day;
	}
};

struct Slot {
	char* label;
	Time* time;

	string toString();
};

struct PoolSlot : Slot {
	int lane;

	string toString();
};

struct GymSlot : Slot {
};

template <typename A>
struct MyArray {
	int count;
	A** arr;

	MyArray<A>() {
		count = 0;
		arr = NULL;
	}

	void append(MyArray<A> newSlots);
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
	MyArray<GymSlot> gyms;
	MyArray<PoolSlot> pools;

	string toString();

	bool add(PoolSlot* slot);
	bool add(GymSlot* slot);

	void remove(PoolSlot* slot);

};

int stoi(char* str);

map<int,char*> splitAndRemoveComments(string str);

Day toDay (char* dayStr);

MyArray<PoolSlot> toPoolSlot (map<int, char*> line, char* pool);

MyArray<GymSlot> toGymSlot (map<int, char*> line, char* label);

PoolSlot* poolSlot(Day day, int hour, int lane, char* pool);

GymSlot* gymSlot(Day day, int hour, char* label);

Group* group(char* name, Age age, int water, int lanes, int gym);

Age toAge(char* str);

string fromAge(Age age);

void dfs(int from, bool* taken, MyArray<PoolSlot> pools, MyArray<Group> groups);

bool finish(bool* taken, int count, MyArray<Group> groups);

bool* falseArray(int count);

void printSolutionToFile(MyArray<Group> groups);

#endif /* SWIMPLAN_H_ */
