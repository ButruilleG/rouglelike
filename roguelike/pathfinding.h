#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "core.h"
#include "binheap.h"
#include "utilities.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct pointnode pointnode_t;

int32_t pathfinding_compare(const void *key, const void *with);
void pathfinding_datum_delete(void *to_delete);

/* "immutable flag" is the hardness value considered immutable */
void run_djinkstras(int source_x, int source_y, int immutable_flag);
void fill_arr(int source_x, int source_y);
void fill_heap();
void check_neighbors(pointnode_t *n);

typedef struct pointnode{
  int x,y;
  int in_heap;
  uint32_t dist;
  pointnode_t *prev;
} pointnode_t;

/* converts the hardness for use in finding distances (ranges as specified in 1.03) */
int hardness_convert(int hardness);

extern int immutable_min;

extern pointnode_t pathnodes[DUNGEON_H][DUNGEON_W];
extern binheap_t distance_heap;
extern binheap_node_t *binarr[DUNGEON_H][DUNGEON_W];

#ifdef __cplusplus
}
#endif

#endif
