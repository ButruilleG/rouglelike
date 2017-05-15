#include "room.h"

/* determine if two rooms are adjacent or overlap */
int room_overlap(Room r, Room s){
  if (r.x+r.w < s.x || r.x > s.x+s.w)
    return 0;
  if (r.y+r.h < s.y || r.y > s.y+s.h)
    return 0;

  return 1;
}

/* create a new room with random location and size */
Room room_create(){
  Room filler;
  filler.x = rand()%(DUNGEON_W - 2 - ROOM_MAX_W) + 1;
  filler.y = rand()%(DUNGEON_H - 2 - ROOM_MAX_H) + 1;
  filler.w = rand()%(ROOM_MAX_W - ROOM_MIN_W + 1) + ROOM_MIN_W;
  filler.h = rand()%(ROOM_MAX_H - ROOM_MIN_H + 1) + ROOM_MIN_H;
  return filler;
}

/* paint the room on the dungeon */
void room_paint(Room room){
  unsigned int r, c;
  for (r=room.y;r<room.y+room.h;r++) {
    for (c=room.x;c<room.x+room.w;c++) {
      map[r][c] = DUNGEON_ROOM;
    }
  }
}
