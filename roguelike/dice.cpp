#include "dice.h"

ostream& operator<<(ostream& os, dice &Dice)
{
	os << Dice.base << "+"
	   << Dice.num << "d"
	   << Dice.faces;
	return os;
}

int dice::roll(){
  int total = 0;
  int i;
  for(i = 0; i < num; i++){
    total += rand()%faces + 1;
  }
  return total + base;
}

int dice::getmax(){
  return num*faces + base;
}

dice::~dice(){
}

dice *dice::parseDice(string &s)
{
	int base = -1, num = -1, faces = -1;
	if (sscanf(s.c_str(), "%d+%dd%d", &base, &num, &faces) < 3)
		return NULL;
	else
		return new dice(base, num, faces);
}

dice *dice::parseDice(const char* s){
	int base = -1, num = -1, faces = -1;
	if (sscanf(s, "%d+%dd%d", &base, &num, &faces) < 3)
		return NULL;
	else
		return new dice(base, num, faces);
}
