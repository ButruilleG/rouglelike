#include "stairs.h"

typedef struct{
  unsigned int x,y,roomnumber;
  int direction;
} Stair;

void place_stair(Stair s, Roomslist* rl){
  Room temp = rl->rooms[s.roomnumber];
  s.x = temp.x+rand()%temp.w;
  s.y = temp.y+rand()%temp.h;

  map[s.y][s.x] = DUNGEON_STAIRS_UP + s.direction;
}

void place_stairs(Roomslist* rl){
  Stair up,down;
  up.roomnumber = rand()%rl->count;
  up.direction = 0;
  down.roomnumber = rl->count - up.roomnumber - 1;
  down.direction = -1;
  place_stair(up,rl);
  place_stair(down,rl);
}
