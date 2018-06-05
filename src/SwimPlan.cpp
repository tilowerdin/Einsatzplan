//============================================================================
// Name        : SwimPlan.cpp
// Author      : Tilo Werdin
// Version     :
// Copyright   : it's mine
// Description : Hello World in C++, Ansi-style
//============================================================================


#include "../h/SwimPlan.h"
#include "../h/Constants.h"

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

pair<int, PoolSlot**> toPoolSlot (map<int, char*> line, char* pool) {

	Day day = toDay(line[COLDAY]);

	int from = atoi(line[COLFROM]);
	int to = atoi(line[COLTO]);
	int lane = atoi(line[COLLANE]);


	pair<int, PoolSlot**> result(to-from, (PoolSlot**) calloc((to-from), sizeof(PoolSlot*)));

	for (int i = from; i < to; i++) {
		PoolSlot* slot = poolSlot(day, i, lane, pool);
		result.second[i-from] = slot;
	}

	return result;
}

pair<int, GymSlot**> toGymSlot (map<int, char*> line, char* label) {
    Day day = toDay(line[COLDAY]);
    int from = atoi(line[COLFROM]);
    int to = atoi(line[COLTO]);

    pair<int, GymSlot**> result(to-from, (GymSlot**) calloc((to-from), sizeof(GymSlot*)));

    for(int i = from; i < to; i++) {
    	GymSlot* slot = gymSlot(day,i,label);
    	result.second[i-from] = slot;
    }

    return result;
}


PoolSlot* poolSlot(Day day, int hour, int lane, char* pool) {
	PoolSlot* slot = (PoolSlot*) malloc(sizeof(PoolSlot));
	slot -> lane = lane;
	slot -> pool = pool;
	Time* time = (Time*) malloc(sizeof(Time));
	time -> day = day;
	time -> hour = hour;
	slot -> time = time;
	return slot;
}

GymSlot* gymSlot(Day day, int hour, char* label) {
	GymSlot* slot = (GymSlot*) malloc(sizeof(GymSlot));
	slot -> gym = label;
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

void printPool(PoolSlot* poolSlot) {
	cout << poolSlot -> pool << " "
	     << fromDay(poolSlot -> time -> day) << " "
		 << poolSlot -> time -> hour << " "
		 << poolSlot -> lane << endl;
}

void printGym(GymSlot* gymSlot) {
	cout << gymSlot -> gym << " "
	     << fromDay(gymSlot -> time -> day) << " "
		 << gymSlot -> time -> hour << endl;
}

pair<int, PoolSlot**> append(pair<int, PoolSlot**> first, pair<int, PoolSlot**> second) {
	if (first.first == 0)
		return second;

	first.first += second.first;
	first.second = (PoolSlot**) realloc(first.second, first.first * sizeof (PoolSlot*));

	for (int i = 0; i < second.first; i++) {
		first.second[first.first - second.first + i] = second.second[i];
	}

	// free second

	return first;
}

pair<int, GymSlot**> append(pair<int, GymSlot**> first, pair<int, GymSlot**> second) {
	if (first.first == 0)
		return second;

	first.first += second.first;
	first.second = (GymSlot**) realloc(first.second, first.first * sizeof (GymSlot*));

	for (int i = 0; i < second.first; i++) {
		first.second[first.first - second.first + i] = second.second[i];
	}

	// free second

	return first;
}


int main() {

	string line;
	ifstream file ("testFiles/testData1");
	map<int,char*> m;

	int state;

	char* building;

	pair<int, PoolSlot**> pools(0,NULL);
	pair<int, PoolSlot**> lineRes(0,NULL);

	pair<int, GymSlot**> gyms(0,NULL);
	pair<int, GymSlot**> lineGyms(0,NULL);
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
			else
			{ // reading train time

				switch (state) {
				case POOLSTATE: // reading lane
					lineRes = toPoolSlot(m,building);
					pools = append(pools, lineRes);
					break;
				case GYMSTATE: // reading
					lineGyms = toGymSlot(m,building);
					gyms = append(gyms, lineGyms);
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

	for (int i = 0; i < gyms.first; i++) {
		printGym(gyms.second[i]);
	}

	for (int i = 0; i < pools.first; i++) {
		printPool(pools.second[i]);
	}

	return 0;
}
