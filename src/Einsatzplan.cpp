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

void dfs(int from, bool* taken, MyArray<PoolSlot> pools, MyArray<Group> groups, MyArray<GymSlot> gyms) {
	if (finish(true, taken, pools.count, groups)) {
		dfs(0, falseArray(gyms.count),gyms,groups);
		return;
	}

	for (int i = from; i < pools.count; i++) {
		if (taken[i])
			continue;

		for(int j = 0; j < groups.count; j++) {
			if (groups.arr[j] -> add(pools.arr[i])) {
				taken[i] = true;

				// before going deeper try to find parallelLanes -1
				// other lanes -> makes it a lot faster
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
					dfs(i, taken, pools, groups, gyms);

				//backtracking
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
// TODO add remove GymSlot to Group
void dfs(int from, bool* taken, MyArray<GymSlot> gyms, MyArray<Group> groups) {
	if(finish(false, taken, gyms.count, groups)) {
		printSolutionToFile(groups);
		return;
	}

	for (int i = from; i < gyms.count; i++) {
		if (taken[i])
			continue;

		for(int j = 0; j < groups.count; j++) {
			if (groups.arr[j] -> add(gyms.arr[i])) {
				taken[i] = true;

				dfs(i, taken, gyms, groups);

				taken[i] = false;
				groups.arr[j] -> remove(gyms.arr[i]);
			}
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

	if (sols > MAXSOLUTIONS)
		throw "finished";

	char filename[16];
	sprintf(filename, "%s/sol%d", outputDir,sols);
	ofstream output (filename);


	for(int i = 0; i < groups.count; i++) {
		output << groups.arr[i] -> toString();
	}

	output.close();

}

const string usage = "prog input [output]";

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

		dfs(0, falseArray(pools.count), pools, gs, gyms);

		if (sols > 0)
			cout << "found" << endl;
		else
			cout << "not found" << endl;

	} catch (char const* e) {
		cerr << e << endl;
	}

	return 0;
}
