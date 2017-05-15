#ifndef PATH_H
#define PATH_H

#include "room.h"

typedef struct{
  unsigned int x, y;
} Tunneler;

/* paint a path from a room to another */
void path_connect(Tunneler snake, Room destination);

#endif
