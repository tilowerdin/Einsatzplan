/*
 * Structures.cpp
 *
 *  Created on: Jun 6, 2018
 *      Author: werdin
 */

#include "../h/Structures.h"

#include <string.h>
#include <stdlib.h>

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

