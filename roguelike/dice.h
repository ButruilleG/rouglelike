#ifndef DICE_H
#define DICE_H

#include "core.h"

using namespace std;

class dice{
 public:
  int base, num, faces;

 dice(int base, int num, int faces) :
  base(base),
    num(num),
    faces(faces) {}
  ~dice();

  static dice *parseDice(string &s);
  static dice *parseDice(const char* s);
  
  int roll();
  int getmax();

  inline int min() { return base + num; }
  inline int max() { return base + num * faces; }
};

ostream& operator<<(ostream& os, dice &Dice);

#endif
