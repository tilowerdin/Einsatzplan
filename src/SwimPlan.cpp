//============================================================================
// Name        : SwimPlan.cpp
// Author      : Tilo Werdin
// Version     :
// Copyright   : it's mine
// Description : Hello World in C++, Ansi-style
//============================================================================


#include "../h/SwimPlan.h"
#include "../h/Constants.h"

#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>


Day toDay (char* dayStr)
{

	if (strcmp(dayStr, "Mo") == 0)
	{
		return Mo;
	}
	else if (strcmp(dayStr, "Di") == 0)
	{
		return Di;
	}
	else if (strcmp(dayStr, "Mi") == 0)
	{
		return Mi;
	}
	else if (strcmp(dayStr, "Do") == 0)
	{
		return Do;
	}
	else if (strcmp(dayStr, "Fr") == 0)
	{
		return Fr;
	}
	else if (strcmp(dayStr, "Sa") == 0)
	{
		return Sa;
	}
	else if (strcmp(dayStr, "So") == 0)
	{
		return So;
	}
	else
	{
		throw "not a Day ";
	}
}

string fromDay(Day day) {
	switch (day) {
		case Mo:
			return "Mo";
		case Di:
			return "Di";
		case Mi:
			return "Mi";
		case Do:
			return "Do";
		case Fr:
			return "Fr";
		case Sa:
			return "Sa";
		case So:
			return "So";
		default:
			return "unknown";
	}
}

SlotArray<PoolSlot> toPoolSlot (map<int, char*> line, char* pool) {

	Day day = toDay(line[COLDAY]);

	int from = atoi(line[COLFROM]);
	int to = atoi(line[COLTO]);
	int lane = atoi(line[COLLANE]);


	SlotArray<PoolSlot> result;
	result.count = to - from;
	result.slots = (PoolSlot**) calloc((to-from), sizeof(PoolSlot*));

	for (int i = from; i < to; i++) {
		PoolSlot* slot = poolSlot(day, i, lane, pool);
		result.slots[i-from] = slot;
	}

	return result;
}

SlotArray<GymSlot> toGymSlot (map<int, char*> line, char* label) {
    Day day = toDay(line[COLDAY]);
    int from = atoi(line[COLFROM]);
    int to = atoi(line[COLTO]);

    SlotArray<GymSlot> result;
    result.count = to-from;
    result.slots = (GymSlot**) calloc((to-from), sizeof(GymSlot*));

    for(int i = from; i < to; i++) {
    	GymSlot* slot = gymSlot(day,i,label);
    	result.slots[i-from] = slot;
    }

    return result;
}


PoolSlot* poolSlot(Day day, int hour, int lane, char* pool) {
	PoolSlot* slot = (PoolSlot*) malloc(sizeof(PoolSlot));
	slot -> lane = lane;
	slot -> label = pool;
	Time* time = (Time*) malloc(sizeof(Time));
	time -> day = day;
	time -> hour = hour;
	slot -> time = time;
	return slot;
}

GymSlot* gymSlot(Day day, int hour, char* label) {
	GymSlot* slot = (GymSlot*) malloc(sizeof(GymSlot));
	slot -> label = label;
	Time* time = (Time*) malloc(sizeof(Time));
	time -> day = day;
	time -> hour = hour;
	slot -> time = time;
	return slot;
}


map<int,char*> splitAndRemoveComments(string str) {
	char* s = (char*) calloc(32, sizeof(char));
	int sizeOfS = 0;
	map<int,char*> result;
	int count = 0;
	char c;
	for (unsigned int i = 0; i < str.length(); ++i)
	{
		c = str.at(i);
		switch (c) {
		case ' ':
			if (s[0] != '\0')
			{
				if (strcmp(s, COMMENT) == 0) {
					return result;
				}
			    result[count] = s;
			    s = (char*) calloc(32,sizeof(char));
			    sizeOfS = 0;
			    count++;
			}
		    break;
		default:
			//s = (char*) realloc(s, (sizeOfS+1) * sizeof(char));
			s[sizeOfS] = c;
			sizeOfS++;
		}
	}
	result[count] = s;
	return result;
}



template <typename A>
void SlotArray<A>::append(SlotArray<A> newSlots) {
	if (count == 0) {
		count = newSlots.count;
		slots = newSlots.slots;
		return;
	}

	count += newSlots.count;
	slots = (A**) realloc(slots, count * sizeof(A));


	for (int i = 0; i < newSlots.count; i++) {
		slots[count - newSlots.count + i] = newSlots.slots[i];
	}
}

void Slot::print() {
	cout << label << " "
		 << fromDay(time -> day) << " "
		 << time -> hour << endl;
}

void PoolSlot::print() {
	cout << label << " "
		 << fromDay(time -> day) << " "
		 << time -> hour << " "
		 << lane << endl;
}

Group* group(char* name, Age age, int water, int lanes, int gym) {
	Group* res = (Group*) malloc(sizeof(Group));
	res -> name = name;
	res -> age = age;
	res -> amountWater = water;
	res -> parallelLanes = lanes;
	res -> amountGym = gym;
	return res;
}

Age toAge(char* str) {
	if (strcmp(str,CHILDREN) == 0) {
		return Kind;
	}
	else if (strcmp(str,YOUTH) == 0) {
		return Jugend;
	}
	else if (strcmp(str,OLD) == 0) {
		return AlterSack;
	}
	else
		throw "not an age";
}

string fromAge(Age age) {
	switch (age) {
		case Kind:
			return CHILDREN;
		case Jugend:
			return YOUTH;
		case AlterSack:
			return OLD;
		default:
			throw "not an age";
	}
}

void Group::print() {
	cout << name << " "
		 << fromAge(age) << " "
		 << amountWater << " "
		 << parallelLanes << " "
		 << amountGym << endl;
}

int main() {

	string line;
	ifstream file ("testFiles/testData1");
	map<int,char*> m;

	int state;

	char* building;

	SlotArray<PoolSlot> pools;
	SlotArray<PoolSlot> lineRes;

	SlotArray<GymSlot> gyms;
	SlotArray<GymSlot> lineGyms;

	// wie verwalten wir gebäude, die nahe bei einander sind?
	map<string,string> nearBuildings;

	Group** groups = (Group**) calloc(64,sizeof(Group*));
	int groupCount = 0;
	Group* g;

	int lineNumber = 0;

	try {
	if (file.is_open())
	{
		while ( getline(file,line))
		{
			lineNumber++;
			m = splitAndRemoveComments(line);
			if (m.size() <= 0)
				continue;

			if (strcmp(m[0], POOLTOKEN) == 0)
			{ // starting new pool section
				state = POOLSTATE;
				building = m[1];
			}
			else if (strcmp(m[0],GYMTOKEN) == 0)
			{ // starting a new gym section
				state = GYMSTATE;
				building = m[1];
			}
			else if (strcmp(m[0],NEARTOKEN) == 0)
			{ // starting a near section
				state = NEARSTATE;
			}
			else if (strcmp(m[0],GROUPTOKEN) == 0)
			{ // starting a group section
				state = GROUPSTATE;
			}
			else
			{ // reading train time

				switch (state) {
				case POOLSTATE: // reading lane
					lineRes = toPoolSlot(m,building);
					pools.append(lineRes);
					break;
				case GYMSTATE: // reading gym
					lineGyms = toGymSlot(m,building);
					gyms.append(lineGyms);
					break;
				case NEARSTATE: // reading near
					nearBuildings[m[0]] = m[1];
					nearBuildings[m[1]] = m[0];
					break;
				case GROUPSTATE:
					g = group( m[COLNAME]
							 , toAge(m[COLAGE])
							 , atoi(m[COLWATER])
							 , atoi(m[COLLANES])
							 , atoi(m[COLGYM]));
					groups[groupCount] = g;
					groupCount++;
					break;
				default:
					break;
				}
			}
		}
		file.close();
	}
	else
	{
		cout << "file not open" << endl;
	}
	} catch (char const* e) {
		cerr << e << endl;
	}

	for (int i = 0; i < gyms.count; i++) {
		gyms.slots[i] -> print();
	}

	for (int i = 0; i < pools.count; i++) {
		pools.slots[i] -> print();
	}

	cout << nearBuildings.size() << endl;

	for(int i = 0; i < groupCount; i++) {
		groups[i] -> print();
	}

	return 0;
}
