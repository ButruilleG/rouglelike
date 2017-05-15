#include "core.h"
#include "utilities.h"
#include "binheap.h"

int floorZero(int toReturn){
  if(toReturn <= 0)
    return 1;

  return toReturn;
}

int to_unit(int check){
  if(check > 0)
    return 1;
  else if(check < 0)
    return -1;

  return 0;
}

void reset_map_seen(){
  int i,j;
  for(i = 0; i < DUNGEON_H; i++){
    for(j = 0; j < DUNGEON_W; j++){
      map_seen[i][j] = 0;
    }
  }
}
