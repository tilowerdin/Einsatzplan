/*
 * Conversion.cpp
 *
 *  Created on: Jun 6, 2018
 *      Author: werdin
 */

#include "../h/Conversion.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>


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

