#ifndef ROOM_H
#define ROOM_H

#include "core.h"

#define MAX_ROOMS (DUNGEON_W - 2)*(DUNGEON_H - 2)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
  unsigned int x, y, w, h;
} Room;

typedef struct
{
  Room rooms[MAX_ROOMS];
  int count;
} Roomslist;

//Roomslist* rl;

/* determine if two rooms are adjacent or overlap */
int room_overlap(Room r, Room s);

/* create a new room with random location and size */
Room room_create();

/* paint the room on the dungeon */
void room_paint(Room room);

#ifdef __cplusplus
}
#endif

#endif
