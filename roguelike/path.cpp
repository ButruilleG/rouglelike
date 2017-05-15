#include "core.h"
#include "path.h"
#include "dungeondisplay.h"

void path_connect(Tunneler snake, Room dest){
  int x_dist, y_dist, x_next, y_next;
  int has_conn = 0;
/*
  //use a temporary room to represent the snake
  Room temp;
  temp.w = 0;
  temp.h = 0;
*/
  while(has_conn == 0){
    x_dist = dest.x - snake.x;
    y_dist = dest.y - snake.y;

    //turn the current square into a corridor if it isn't room floor
    if(map[snake.y][snake.x] != DUNGEON_ROOM)
      map[snake.y][snake.x] = DUNGEON_CORRIDOR;

    if(snake.x == dest.x && snake.y == dest.y)
      has_conn = 1;

/*  
    //use a temporary room to check for collision
    temp.x = snake.x;
    temp.y = snake.y;

    //if the room has been reached, leave
    if (room_overlap(temp, dest)){
      has_conn = 1;
    }
*/
    //move closer to the destination room, taking rock hardness into account
    x_next = x_dist < abs(x_dist) ? snake.x - 1 : snake.x + 1;
    y_next = y_dist < abs(y_dist) ? snake.y - 1 : snake.y + 1;

    if(abs(x_dist*map[y_next][snake.x]) > abs(y_dist*map[snake.y][x_next])){
      snake.x = x_next;
    }
    else{
      snake.y = y_next;
    }
  }
}
