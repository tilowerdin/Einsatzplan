/*
 * Structures.cpp
 *
 *  Created on: Jun 6, 2018
 *      Author: werdin
 */

#include "../h/Structures.h"

#include <string.h>
#include <stdlib.h>

bool trainerAvailable(char*, MyArray<Group> groups, Time* time) {
	for (int i = 0; i < groups.count; i++) {
		for (int j = 0; j < groups.arr[i] -> pools.count; j++) {
			if (time->equals(*(groups.arr[i] -> pools.arr[j] -> time))) {
				return false;
			}
		}
		for (int j = 0; j < groups.arr[i] -> gyms.count; j++) {
			if (time->equals(*(groups.arr[i] -> gyms.arr[j] -> time))) {
				return false;
			}
		}
	}
	return true;
}

bool Time::equals(Time time) {
	return hour == time.hour && day == time.day;
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

bool Group::add(PoolSlot* slot, map<string,MyArray<Group> > trainer, map<string,pair<int, Day*> > excludeDays) {
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

	// check that the trainer did not exlude this day
	pair<int, Day*> days = excludeDays[string(name)];
	for (int i = 0; i < days.first; i++) {
		if (days.second[i] == slot -> time -> day)
			return false;
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

	// check pause for groups that have an amount of two times training
	// only check the pause if there already is a session saved
	if (amountWater == 2 && pools.count > 0) {
		Day d1 = pools.arr[0] -> time -> day;
		Day d2 = slot -> time -> day;
		if (abs(d1 - d2) <= DAYSPAUSE     // at least DAYSPAUSE pause between sessions in this week
			|| abs((d1 + DAYSPAUSE) % 7 - (d2 + DAYSPAUSE) % 7 ) <= DAYSPAUSE) // pause over the week end
			return false;
	}

	// check that this trainer does not already have an event at this time
	MyArray<Group> groups = trainer[string(name)];
	if (groups.count > 1 && !trainerAvailable(name, groups, slot -> time)) {
		return false;
	}

	// check that if the trainer already has some event at that day that they are directly lined up.
	if (groups.count > 1) {
		bool equalDay = false;
		bool nearTime = false;
		for (int i = 0; i < groups.count; i++) {
			MyArray<PoolSlot> pools = groups.arr[i]->pools;
			for(int j = 0; j < pools.count; j++) {
				if(pools.arr[j] -> time -> day == slot -> time -> day) {
					equalDay = true;
					if (abs(pools.arr[j] -> time -> hour - slot -> time -> hour) <= 1
						&& strcmp(slot -> label, pools.arr[j] -> label) == 0) {
						nearTime = true;
					}
				}
			}
			MyArray<GymSlot> gyms = groups.arr[i] -> gyms;
			for(int j = 0; j < gyms.count; j++) {
				if(gyms.arr[j] -> time -> day == slot -> time -> day) {
					equalDay = true;
					if (abs(gyms.arr[j] -> time -> hour - slot -> time -> hour) <= 1
						&& strcmp(slot -> label, gyms.arr[j] -> label) == 0) {
						nearTime = true;
					}
				}
			}
		}
		if (equalDay && !nearTime)
			return false;
	}

	// TODO: do we need to check more?
	// i think now we can add the slot
	pools.arr[pools.count] = slot;
	pools.count++;
	return true;
}

bool Group::add(GymSlot* slot, map<string,string> nearBuildings, map<string,MyArray<Group> > trainer) {
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

	// check amount gym
	if (amountGym <= gyms.count)
		// if the group doesn't need any slots anymore
		return false;

	// check days
	// at this point we need to check that the new slot is not at a day
	// we already have a training session
	for (int i = 0; i < gyms.count; i++) {
		if(gyms.arr[i] -> time -> day == slot -> time -> day){
			return false;
		}
	}

	// check that there is a water session near this gym session
	bool possible = false;
	for (int i = 0; i < pools.count; i++) {
		if(nearBuildings[slot -> label].compare(pools.arr[i] -> label) == 0
		   && slot -> time -> day == pools.arr[i] -> time -> day
		   && abs(slot -> time -> hour - pools.arr[i] -> time -> hour) <= 1) {
			possible = true;
		}
	}
	if(!possible)
		return false;

	// check if trainer is available
	if(trainer[string(name)].count > 1 && !trainerAvailable(name, trainer[name], slot -> time))
		return false;

	// TODO: do we need to check more?
	// i think now we can add the slot
	gyms.arr[gyms.count] = slot;
	gyms.count++;
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

void Group::remove(GymSlot* slot) {
	if (gyms.arr[gyms.count-1] == slot) {
		gyms.arr[gyms.count-1] = NULL;
		gyms.count--;
	}
	else
		throw "that's not possible";
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

	for (int i = 0; i < gyms.count; i++) {
		res += gyms.arr[i] -> toString();
	}

	return res;
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

