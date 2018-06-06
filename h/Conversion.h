/*
 * Conversion.h
 *
 *  Created on: Jun 6, 2018
 *      Author: werdin
 */

#ifndef CONVERSION_H_
#define CONVERSION_H_

#include "../h/Constants.h"

#include <map>
#include <string>

using namespace std;

char* itoa(int i);

map<int,char*> splitAndRemoveComments(string str);

#endif /* CONVERSION_H_ */
