#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

int Parser::parseColor(string &s)
{
	stringstream ss(s);
	string c;
	ss >> c; // one color
	if (c=="BLACK"  ) return COLOR_BLACK;
	if (c=="RED"    ) return COLOR_RED;
	if (c=="GREEN"  ) return COLOR_GREEN;
	if (c=="YELLOW" ) return COLOR_YELLOW;
	if (c=="BLUE"   ) return COLOR_BLUE;
	if (c=="MAGENTA") return COLOR_MAGENTA;
	if (c=="CYAN"   ) return COLOR_CYAN;
	if (c=="WHITE"  ) return COLOR_WHITE;
	return -1;
}

dice *Parser::parseDice(string &s) {
	return dice::parseDice(s);
}

int Parser::parseDiceField(stringstream &ss, string &field)
{
	if (!field.empty()) {
		return 1;
	}

	trim(ss);
	getline(ss, field);

	dice *Dice = parseDice(field);
	if (NULL==Dice) {
		return 1;
	} else {
		delete Dice;
	}
	return 0;
}

int Parser::parseSymb(string &s)
{
	if (1==s.length()) return s.c_str()[0];
	return -1;
}

int Parser::parseType(string &s)
{
	for (int i = 0; i < 20; i++)
		if (s==ObjectFactory::TYPE[i])
			return i;
	return -1;
}

int Parser::trim(stringstream &ss)
{
	while (ss.peek()==' ')
		ss.get();
	
	return 0;
}

string &Parser::trim(string &s)
{
	stringstream ss(s);
	trim(ss);
	s = ss.str();
	return s;
}

