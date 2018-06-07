/*
 * Structures.h
 *
 *  Created on: Jun 6, 2018
 *      Author: werdin
 */

#ifndef STRUCTURES_H_
#define STRUCTURES_H_

#include "../h/Conversion.h"
#include "../h/Constants.h"

#include <stddef.h>
#include <string>
#include <stdlib.h>

using namespace std;

enum Day { Mo, Di, Mi, Do, Fr, Sa, So };
enum Age { Kind, Jugend, AlterSack };

template <typename A>
struct MyArray {
	int count;
	A** arr;

	MyArray<A>() {
		count = 0;
		arr = NULL;
	}

	void append(MyArray<A> newSlots) {
		if (count == 0) {
			count = newSlots.count;
			arr = newSlots.arr;
			return;
		}

		count += newSlots.count;
		arr = (A**) realloc(arr, count * sizeof(A*));


		for (int i = 0; i < newSlots.count; i++) {
			arr[count - newSlots.count + i] = newSlots.arr[i];
		}
	}

	void add(A* a) {
		if (count == 0) {
			count = 1;
			arr = (A**) calloc(1, sizeof(A*));
			arr[0] = a;
			return;
		}

		count++;
		arr = (A**) realloc(arr, count * sizeof(A*));
		arr[count-1] = a;
	}
};

struct Time {
	int hour;
	Day day;

	bool equals(Time time);
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

	bool add(PoolSlot* slot, map<string,MyArray<Group> > trainer);
	bool add(GymSlot* slot, map<string,string> nearBuildings, map<string,MyArray<Group> > trainer);

	void remove(PoolSlot* slot);
	void remove(GymSlot* slot);
};

string fromAge(Age age);

string fromDay(Day day);

Age toAge(char* str);

Day toDay (char* dayStr);



#endif /* STRUCTURES_H_ */
