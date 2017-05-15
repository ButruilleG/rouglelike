#ifndef UTILITIES_H
#define UTILITIES_H

#define INT_MAX 2147483647

#ifdef __cplusplus
extern "C" {
#endif

/* if the value toReturn is zero or smaller, returns one.
   otherwise, returns toReturn */
int floorZero(int toReturn);
int to_unit(int check);


/*
  Resets the map of the dungeon that's been "seen"
*/
void reset_map_seen();
  
#ifdef __cplusplus
}
#endif

#endif
