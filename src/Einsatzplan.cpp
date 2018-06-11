//============================================================================
// Name        : SwimPlan.cpp
// Author      : Tilo Werdin
// Version     :
// Copyright   : it's mine
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "../h/Einsatzplan.h"

#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../h/Structures.h"

struct stat st = {0};

map<string,string> nearBuildings;
map<string,MyArray<Group> > trainer;
map<string,pair<int, Day*> > excludeTrainer;
MyArray<char> onlyOneTrainer;

MyArray<PoolSlot> toPoolSlot (map<int, char*> line, char* pool) {

	Day day = toDay(line[COLDAY]);

	double from = atof(line[COLFROM]);
	double to = atof(line[COLTO]);
	int lane = atoi(line[COLLANE]);

	MyArray<PoolSlot> result;
	double timeDiff = to - from;
	int minus = (1 - HOURPART) / HOURPART;
	int total = timeDiff / HOURPART;
	result.count = round(total - minus);
	result.arr = (PoolSlot**) calloc(result.count, sizeof(PoolSlot*));

	for (int i = 0; i < result.count; i++) {
		PoolSlot* slot = poolSlot(day, from + i*HOURPART, lane, pool);
		result.arr[i] = slot;
	}

	return result;
}

MyArray<GymSlot> toGymSlot (map<int, char*> line, char* label) {
    Day day = toDay(line[COLDAY]);
    int from = atof(line[COLFROM]);
    int to = atof(line[COLTO]);

    MyArray<GymSlot> result;
    result.count = round((to-from) / HOURPART - (1 - HOURPART) / HOURPART);
    result.arr = (GymSlot**) calloc(result.count, sizeof(GymSlot*));

    for(int i = 0; i < result.count; i++) {
    	GymSlot* slot = gymSlot(day,from + i*HOURPART,label);
    	result.arr[i] = slot;
    }

    return result;
}

PoolSlot* poolSlot(Day day, double hour, int lane, char* pool) {
	PoolSlot* slot = (PoolSlot*) malloc(sizeof(PoolSlot));
	slot -> lane = lane;
	slot -> label = pool;
	Time* time = (Time*) malloc(sizeof(Time));
	time -> day = day;
	time -> hour = hour;
	slot -> time = time;
	return slot;
}

GymSlot* gymSlot(Day day, double hour, char* label) {
	GymSlot* slot = (GymSlot*) malloc(sizeof(GymSlot));
	slot -> label = label;
	Time* time = (Time*) malloc(sizeof(Time));
	time -> day = day;
	time -> hour = hour;
	slot -> time = time;
	return slot;
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

bool inRange(PoolSlot* s1, PoolSlot* s2) {
	bool res = strcmp(s1 -> label, s2 -> label) == 0;
	res &= s1 -> lane == s2 -> lane;
	res &= s1 -> time -> day == s2 -> time -> day;
	res &= abs(s1 -> time -> hour - s2 -> time -> hour) < 1;
	return res;
}

bool inRange(GymSlot* s1, GymSlot* s2) {
	bool res = strcmp(s1 -> label, s2 -> label) == 0;
	res &= s1 -> time -> day == s2 -> time -> day;
	res &= abs(s1 -> time -> hour - s2 -> time -> hour) < 1;
	return res;
}

void dfs(MyArray<Group> groups, bool* taken, MyArray<PoolSlot> pools, MyArray<GymSlot> gyms) {
	if (finish(true, taken, pools.count, groups)) {
		dfs(groups, falseArray(gyms.count),gyms);
		return;
	}

	Group* group;

	for(int i = 0; i < groups.count; i++) {
		Group* g = groups.arr[i];
		if (g -> amountWater * g -> parallelLanes > g -> pools.count) {
			group = g;
			break;
		}
	}

	for (int i = 0; i < pools.count; i++) {
		if (taken[i])
			continue;

		if (group -> add(pools.arr[i], trainer, excludeTrainer, onlyOneTrainer)) {
			int countSlotsToCheck = 2 / HOURPART - 1;
			// take all indices that overlap with this slot
			int takenIndices[countSlotsToCheck];
			for (int j = 0; j < countSlotsToCheck; j++)
			{
				takenIndices[j] = -1;
			}
			// check slots before and after this slot
			for (int j = -(1/HOURPART - 1); j < 1 / HOURPART; j++) {
				if (i + j >= 0 && i + j < pools.count && !taken[i+j] && inRange(pools.arr[i], pools.arr[i+j])){
					int pos = round(1 / HOURPART - 1);
					takenIndices[j + pos] = i+j;
					taken[i+j] = true;
				}
			}

			dfs(groups, taken, pools, gyms);

			for(int j = 0; j < countSlotsToCheck; j++){
				if (takenIndices[j] != -1) {
					taken[takenIndices[j]] = false;
				}
			}

			group -> remove(pools.arr[i]);
		}
	}
}

bool finish(bool onlyWater, bool* taken, int count, MyArray<Group> groups) {
	bool finish1 = true;

	for (int i = 0; i < count; i++) {
		finish1 &= taken[i];
	}

	bool finish2 = true;

	for (int i = 0; i < groups.count; i++) {
		finish2 &= ( groups.arr[i] -> amountWater
				   * groups.arr[i] -> parallelLanes
				   == groups.arr[i] -> pools.count);
		if (!onlyWater)
			finish2 &= ( groups.arr[i] -> amountGym
			           == groups.arr[i] -> gyms.count);
	}

	return finish1 || finish2;
}

void dfs(MyArray<Group> groups, bool* taken, MyArray<GymSlot> gyms) {
	if(finish(false, taken, gyms.count, groups)) {
		printSolutionToFile(groups);
		return;
	}

	Group* group;

	for (int i = 0; i < groups.count; i++) {
		if (groups.arr[i] -> amountGym > groups.arr[i] -> gyms.count) {
			group = groups.arr[i];
			break;
		}
	}

	// search for a session for this group
	for (int i = 0; i < gyms.count; i++) {
		if (taken[i])
			continue;

		if (group -> add(gyms.arr[i], nearBuildings, trainer)) {
			int countSlotsToCheck = 2 / HOURPART - 1;
			// take all indices that overlap with this slot
			int takenIndices[countSlotsToCheck];
			for (int j = 0; j < countSlotsToCheck; j++)
			{
				takenIndices[j] = -1;
			}
			// check slots before and after this slot
			for (int j = -(1/HOURPART - 1); j < 1 / HOURPART; j++) {
				if (i + j >= 0 && i + j < gyms.count && !taken[i+j] && inRange(gyms.arr[i], gyms.arr[i+j])){
					int pos = round(1 / HOURPART - 1);
					takenIndices[j + pos] = i+j;
					taken[i+j] = true;
				}
			}

			dfs(groups, taken, gyms);

			for(int j = 0; j < countSlotsToCheck; j++){
				if (takenIndices[j] != -1) {
					taken[takenIndices[j]] = false;
				}
			}

			group -> remove(gyms.arr[i]);
		}
	}
}

bool* falseArray(int count) {
	bool* res = (bool*) calloc(count, sizeof(bool));
	return res;
}

int sols = 0;
char* outputDir = "out";

void printSolutionToFile(MyArray<Group> groups) {
	sols++;

	cout << "found a solution" << endl;

	if (sols > MAXSOLUTIONS){
//		return;
		throw "finished";
	}

	char filename[16];
	sprintf(filename, "%s/sol%d", outputDir,sols);
	ofstream output (filename);


	for(int i = 0; i < groups.count; i++) {
		output << groups.arr[i] -> toString();
	}

	output.close();

}

const string usage = "prog input [output]";

void exclude(map<int,char*> m) {
	pair<int, Day*> days;
	days.first = m.size()-1;
	days.second = (Day*) calloc(m.size()-1, sizeof(Day));
    for (unsigned int i = 1; i < m.size(); i++) {
		days.second[i-1] = toDay(m[i]);
	}
	excludeTrainer[string(m[0])] = days;
}

void sort(MyArray<Group> toSort) {
	Group* currentBest;
	int index;
	int bestValue;

	// sort by complication
	for(int i = 0; i < toSort.count-1; i++)
	{
		currentBest = toSort.arr[i];
		bestValue = currentBest -> toValue();
		index = i;
		for (int j = i+1; j < toSort.count; j++)
		{
			if (bestValue < toSort.arr[j] -> toValue())
			{
				currentBest = toSort.arr[j];
				bestValue = currentBest -> toValue();
				index = j;
			}
		}
		toSort.arr[index] = toSort.arr[i];
		toSort.arr[i] = currentBest;
	}

	int value;
	int j;
	// sort by name
	for(int i = 0; i < toSort.count; i++) {
		currentBest = toSort.arr[i];
		value = currentBest->toValue();
		index = i;
		j = i+1;
		while (j < toSort.count && toSort.arr[j] -> toValue() == value)
		{
			if (strcmp(currentBest -> name, toSort.arr[j] -> name) > 0)
			{
				currentBest = toSort.arr[j];
				value = currentBest -> toValue();
				index = j;
			}
			j++;
		}
		toSort.arr[index] = toSort.arr[i];
		toSort.arr[i] = currentBest;
	}
}

int main(int argc, char* argv[]) {

	if (argc < 2) {
		cerr << usage << endl;
		return 0;
	}

	char* filename = "testFiles/testData2";

	filename = argv[1];

	if (argc > 2) {
		int i = 0;
		bool lastSlash = false;
		while(argv[2][i] != '\0') {
			if(argv[2][i] == '/') {
				lastSlash = true;
			}
			i++;
		}
		if (lastSlash)
			argv[2][i-1] = '\0';
		outputDir = argv[2];
	}

	if(stat(outputDir, &st) == -1){
		#ifdef _WIN32
		mkdir(outputDir);
		#else
		mkdir(outputDir, 0775);
		#endif
	}


	string line;
	ifstream file (filename);
	map<int,char*> m;

	int state;

	char* building;

	MyArray<PoolSlot> pools;
	MyArray<PoolSlot> lineRes;

	MyArray<GymSlot> gyms;
	MyArray<GymSlot> lineGyms;

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
				else if (strcmp(m[0],EXCLUDETOKEN) == 0)
				{ // starting a exclude day for trainers section
					state = EXCLUDESTATE;
				}
				else if (strcmp(m[0],ONLYONETOKEN) == 0)
				{
					state = ONLYONESTATE;
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
						trainer[string(m[COLNAME])].add(g);
						break;
					case EXCLUDESTATE:
						exclude(m);
						break;
					case ONLYONESTATE:
						onlyOneTrainer.add(m[0]);
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

		sort(gs);

		for (int i = 0; i < gs.count; i++)
			cout << gs.arr[i]->toString() << endl;

		dfs(gs, falseArray(pools.count), pools, gyms);

		if (sols > 0)
			cout << "found " << sols << " solutions" << endl;
		else
			cout << "not found" << endl;

	} catch (char const* e) {
		cerr << e << endl;
	}

	return 0;
}
