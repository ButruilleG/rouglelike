#include "core.h"
#include "utilities.h"
#include "dungeoncreate.h"

//initialize "map" with a random number 0-9 in all entries
void mapGenerate(){
  int i,j;

  for(i = 0; i < DUNGEON_H; i++){
    for(j = 0; j < DUNGEON_W; j++){
      map[i][j] = rand()%255 + 1;
    }
  }
}

//smooth over the map left to right
void mapSmoothLR(){
  int i,j;
  int curr, next;
  for(i = 0; i < DUNGEON_H; i++){
    for(j = 0; j < DUNGEON_W - 1; j++){
      curr = map[i][j];
      next = map[i][j+1];
      map[i][j] = floorZero((curr+(curr+next)/2)/2);
      map[i][j+1] = floorZero((next+(curr+next)/2)/2);
    }
  }
}

//smooth over the map top to bottom
void mapSmoothTB(){
  int i,j;
  int curr,next;
  for(i = 0; i < DUNGEON_W; i++){
    for(j = 0; j < DUNGEON_H - 1; j++){
      curr = map[j][i];
      next = map[j+1][i];
      map[j][i] = floorZero((curr+(curr+next)/2)/2);
      map[j+1][i] = floorZero((next+(curr+next)/2)/2);
    }
  }
}

void setImmutable(){
  int i;
  for(i = 0; i < DUNGEON_W; i++){
    map[0][i] = IMMUTABLE_ROCK;
  }
  for(i = 0; i < DUNGEON_W; i++){
    map[DUNGEON_H - 1][i] = IMMUTABLE_ROCK;
  }
  for(i = 0; i < DUNGEON_H; i++){
    map[i][0] = IMMUTABLE_ROCK;
  }
  for(i = 0; i < DUNGEON_H; i++){
    map[i][DUNGEON_W - 1] = IMMUTABLE_ROCK;
  }
}
