#ifndef CORE_H
#define CORE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <endian.h>
#include <unistd.h>
#include <math.h>
#include <ncurses.h>

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>

#define DUNGEON_W 80  /* dungeon width   */
#define DUNGEON_H 21  /* dungeon height  */

#define ROOM_MIN_W 4  /* room min width  */
#define ROOM_MIN_H 3  /* room min height */

#define ROOM_MAX_W 15 /* room max width  */
#define ROOM_MAX_H 6 /* room max height */

/* codes for dungeon objects */
#define DUNGEON_STAIRS_DOWN -4
#define DUNGEON_STAIRS_UP -3
#define DUNGEON_ROOM -2
#define DUNGEON_CORRIDOR -1
#define IMMUTABLE_ROCK 255

/* flags to be passed in */
#define LOAD_FLAG "--load"
#define SAVE_FLAG "--save"
#define MONSTER_FLAG "--nummon"

/* monster charachteristics and statuses*/
/*
Four possible charachteristics:
  Intelligent (rightmost bit)
  Telepathic
  Tunneler
  Erratic

Two possible statuses:
  Dead (rightmost bit)
  Player
*/
#define C_INTELLIGENT 0x1
#define C_TELEPATHIC 0x2
#define C_TUNNELER 0x4
#define C_ERRATIC 0x8
#define C_PASSTHROUGH 0x16

#define S_DEAD 0x1
#define S_PLAYER 0x2

extern int maxhp;
extern int level;
extern int game_state;
extern int map[DUNGEON_H][DUNGEON_W];
extern int map_seen[DUNGEON_H][DUNGEON_W];
extern char message[80];

#endif
