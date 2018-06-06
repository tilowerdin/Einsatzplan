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
#include <cmath>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

struct stat st = {0};


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

MyArray<PoolSlot> toPoolSlot (map<int, char*> line, char* pool) {

	Day day = toDay(line[COLDAY]);

	int from = atoi(line[COLFROM]);
	int to = atoi(line[COLTO]);
	int lane = atoi(line[COLLANE]);


	MyArray<PoolSlot> result;
	result.count = to - from;
	result.arr = (PoolSlot**) calloc((to-from), sizeof(PoolSlot*));

	for (int i = from; i < to; i++) {
		PoolSlot* slot = poolSlot(day, i, lane, pool);
		result.arr[i-from] = slot;
	}

	return result;
}

MyArray<GymSlot> toGymSlot (map<int, char*> line, char* label) {
    Day day = toDay(line[COLDAY]);
    int from = atoi(line[COLFROM]);
    int to = atoi(line[COLTO]);

    MyArray<GymSlot> result;
    result.count = to-from;
    result.arr = (GymSlot**) calloc((to-from), sizeof(GymSlot*));

    for(int i = from; i < to; i++) {
    	GymSlot* slot = gymSlot(day,i,label);
    	result.arr[i-from] = slot;
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
void MyArray<A>::append(MyArray<A> newSlots) {
	if (count == 0) {
		count = newSlots.count;
		arr = newSlots.arr;
		return;
	}

	count += newSlots.count;
	arr = (A**) realloc(arr, count * sizeof(A));


	for (int i = 0; i < newSlots.count; i++) {
		arr[count - newSlots.count + i] = newSlots.arr[i];
	}
}

char* itoa(int i) {
	int digits;
	if (i == 0)
		digits = 1;
	else
		digits = log10(i) + 1;

	char* str = (char*) calloc(digits+1, sizeof(char));
	for (int j = 0; j < digits; j++) {
		int a = digits -j-1;
		int b = pow(10, a);
		int c = i / b;
		int d = c % 10;
		switch (d) {
		case 0:
			str[j] = '0';
			break;
		case 1:
			str[j] = '1';
			break;
		case 2:
			str[j] = '2';
			break;
		case 3:
			str[j] = '3';
			break;
		case 4:
			str[j] = '4';
			break;
		case 5:
			str[j] = '5';
			break;
		case 6:
			str[j] = '6';
			break;
		case 7:
			str[j] = '7';
			break;
		case 8:
			str[j] = '8';
			break;
		case 9:
			str[j] = '9';
			break;
		default:
			throw "error";
		}
	}

	return str;
}

string Slot::toString() {
	string res = string(label==NULL ? "" : label) + " "
		 + fromDay(time -> day) + " "
		 + string(itoa(time -> hour)) + "\n";

	return res;
}

string PoolSlot::toString() {
	string res = string(label==NULL ? "" : label) + " "
		 + fromDay(time -> day) + " "
		 + string(itoa(time -> hour)) + " "
		 + string(itoa(lane)) + "\n";
	return res;
}

Group* group(char* name, Age age, int water, int lanes, int gym) {
	Group* res = (Group*) malloc(sizeof(Group));
	res -> name = name;
	res -> age = age;
	res -> amountWater = water;
	res -> parallelLanes = lanes;
	res -> amountGym = gym;

	res -> gyms.count = 0;
	res -> pools.count = 0;

    res -> gyms.arr = (GymSlot**) calloc(gym, sizeof(GymSlot*));
	res -> pools.arr = (PoolSlot**) calloc(water*lanes, sizeof(PoolSlot*));

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

string Group::toString() {
	string res = string(name == NULL ? "" : name) + " "
		 + fromAge(age) + " "
		 + string(itoa(amountWater)) + " "
		 + string(itoa(parallelLanes)) + " "
		 + string(itoa(amountGym)) + "\n";

	for (int i = 0; i < pools.count; i++) {
		res += pools.arr[i] -> toString();
	}

	return res;
}

bool Group::add(PoolSlot* slot) {
	// check age constraint
	switch (age) {
	case Kind: // children must not train after maxTimeChild
		if (slot -> time -> hour >= maxTimeChild)
			return false;
		break;
	case Jugend: // youth must not train after maxTimeYouth
		if (slot -> time -> hour >= maxTimeYouth)
			return false;
		break;
	case AlterSack: // old people train whenever they want
		break;
	default:
		break;
	}

	// check amount water
	if (amountWater*parallelLanes <= pools.count)
		// if the group doesn't need any slots anymore
		return false;

	// check lanes
	if (parallelLanes > 1) {
		// if we need more than one lane at a time we want to check if
		// there is a slot that is alone and needs some other slots
		// in parallel. Therefore we hold a slot to compare with and
		// search for parallel slots

		PoolSlot* comparePoolSlot = NULL;
		PoolSlot* currentPoolSlot = NULL;
		int count = 0; // count the currently found parallel slots

		for (int i = 0; i < pools.count; i++) {
			currentPoolSlot = pools.arr[i];

			// if there is no compareSlot we are either at the beginning
			// or just ended a day where there were enough lanes at one
			// time. Then we want to use the current slot to compare with
			// the next ones
			if (comparePoolSlot == NULL) {
				comparePoolSlot = currentPoolSlot;
				count = 1;
			}

			// if we have a compareSlot we want to count the parallelLines
			// that we already have
			else if (comparePoolSlot -> time -> equals(
					 *(currentPoolSlot -> time))) {
				count++;

				// if we reach the number of parallel lanes then we need
				// to check that the slot we want to add to our group
				// is not another parallel lane because this training
				// session already has enough parallel lanes
				if (count == parallelLanes) {
					if (comparePoolSlot -> time -> equals(*(slot -> time))) {
						return false;
					}
					// start a new comparison.
					comparePoolSlot = NULL;
					count = 0;
				}
			}
			else
			{
				// this case should not happen since every time we add
				// a slot it should satisfy the rule above and so the
				// currentSlot should either have the same time as the
				// compareSlot or the compareSlot should be NULL
				throw "should not happen";
			}
		}

		// at this point count must be less than parallelLanes.
		// it is zero if all slots that are already in the list have enough
		// parallel slots
		// greater than zero if some slot needs a partner, so we compare
		// the compareSlot to the slot passed to the function if the time
		// matches and the lanes are besides each other.
		// TODO: for parallelLanes more than 3 check that they are besides
		// each other
		if (count > 0
			&& strcmp(comparePoolSlot -> label, slot -> label) == 0
			&& comparePoolSlot -> time -> equals(*(slot -> time))
			&& abs(comparePoolSlot -> lane - slot -> lane) <= 1) {
			// if these requirements are fulfilled we keep the slot
			// and return true
			pools.arr[pools.count] = slot;
			pools.count++;
			return true;
		} else if (count > 0) {
			return false;
		}
	}

	// check days
	// at this point we need to check that the new slot is not at a day
	// we already have a training session
	for (int i = 0; i < pools.count; i++) {
		if(pools.arr[i] -> time -> day == slot -> time -> day){
			return false;
		}
	}

	// TODO: do we need to check more?
	// i think now we can add the slot
	pools.arr[pools.count] = slot;
	pools.count++;
	return true;
}

void Group::remove(PoolSlot* slot) {
	if (pools.arr[pools.count-1] == slot) {
		pools.arr[pools.count-1] = NULL;
		pools.count--;
	}
	else
		throw "that's not possible";
}

void dfs(int from, bool* taken, MyArray<PoolSlot> pools, MyArray<Group> groups) {
	if (finish(taken, pools.count, groups)) {
		printSolutionToFile(groups);
		return;
	}

	for (int i = from; i < pools.count; i++) {
		if (taken[i])
			continue;

		for(int j = 0; j < groups.count; j++) {
			if (groups.arr[j] -> add(pools.arr[i])) {
				taken[i] = true;

				// before going deeper try to find parallelLanes -1
				// other lanes
				int takenIndices[groups.arr[j]->parallelLanes];
				for(int n = 0; n < groups.arr[j]->parallelLanes; n++) {
					takenIndices[n] = -1;
				}
				takenIndices[0] = i;

				for (int k = 1; k < groups.arr[j]->parallelLanes; k++) {
					for (int m = from; m < pools.count; m++) {
						if (taken[m])
							continue;

						if (groups.arr[j] -> add(pools.arr[m])) {
							takenIndices[k] = m;
							taken[m] = true;
							break;
						}
					}
					if (takenIndices[k] == -1)
						break;
				}

				// if we were able to take parallelLanes many lanes
				if (takenIndices[groups.arr[j]->parallelLanes-1] > -1)
					dfs(i, taken, pools, groups);

				for (int k = groups.arr[j]->parallelLanes-1; k >= 0; k--) {
					if (takenIndices[k] != -1) {
						taken[takenIndices[k]] = false;
						groups.arr[j] -> remove(pools.arr[takenIndices[k]]);
					}
				}
			}
		}
	}
}

bool finish(bool* taken, int count, MyArray<Group> groups) {
	bool finish1 = true;

	for (int i = 0; i < count; i++) {
		finish1 &= taken[i];
	}

	bool finish2 = true;

	for (int i = 0; i < groups.count; i++) {
		finish2 &= ( groups.arr[i] -> amountWater
				   * groups.arr[i] -> parallelLanes
				   == groups.arr[i] -> pools.count);
	}

	return finish1 || finish2;
}

bool* falseArray(int count) {
	bool* res = (bool*) calloc(count, sizeof(bool));
	return res;
}

int sols = 0;

void printSolutionToFile(MyArray<Group> groups) {
	sols++;

	if (sols > MAXSOLUTIONS)
		throw "finished";

	char filename[16];
	sprintf(filename, "sol/sol%d", sols);
	ofstream output (filename);


	for(int i = 0; i < groups.count; i++) {
		output << groups.arr[i] -> toString();
	}

	output.close();

}

int main() {



	if(stat("sol/", &st) == -1)
		mkdir("sol/", 0775);

//	ofstream file1("sol/file");
//	file1 << "content" << endl;
//	file1.close();
//
//	return 0;
	string line;
	ifstream file ("testFiles/testData2");
	map<int,char*> m;

	int state;

	char* building;

	MyArray<PoolSlot> pools;
	MyArray<PoolSlot> lineRes;

	MyArray<GymSlot> gyms;
	MyArray<GymSlot> lineGyms;

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

		MyArray<Group> gs;
		gs.count = groupCount;
		gs.arr = groups;

		dfs(0, falseArray(pools.count), pools, gs);

		if (sols > 0)
			cout << "found" << endl;
		else
			cout << "not found" << endl;

	} catch (char const* e) {
		cerr << e << endl;
	}

	return 0;
}
