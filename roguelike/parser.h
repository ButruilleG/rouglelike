#ifndef PARSER_H
#define PARSER_H

#include "dice.h"
#include "objectfactory.h"
#include <iostream>

using namespace std;

class Parser
{
	public:
		// get int associated with the given color string
		static int parseColor(string &);

		// see also Dice::parseDice
		static dice *parseDice(string &);

		// return 1 if there are parsing errors
		static int parseDiceField(stringstream &ss, string &field);

		// get first char from string, return -1 if s.length > 1
		static int parseSymb(string &s);

		// get object type index
		static int parseType(string &s);

		// trim leading spaces of stringstream
		static int trim(stringstream &);
		
		// trim leading spaces of string
		static string &trim(string &);
};

#endif
