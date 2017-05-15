#include "pathfinding.h"

int immutable_min;

pointnode_t pathnodes[DUNGEON_H][DUNGEON_W];
binheap_t distance_heap;
binheap_node_t *binarr[DUNGEON_H][DUNGEON_W];

void run_djinkstras(int source_x, int source_y, int immutable_flag){
  immutable_min = immutable_flag;
  fill_arr(source_x,source_y);
  fill_heap();

  while(!binheap_is_empty(&distance_heap)){
    //printf("removing min. left in heap: %d\n",distance_heap.size);
    pointnode_t *curr_n = (pointnode_t*) binheap_remove_min(&distance_heap);
    curr_n->in_heap = 1;
    //printf("done removing. Checking neighbors.\n");
    check_neighbors(curr_n);
    //printf("done checking.\n");
  }

  binheap_delete(&distance_heap);
}

/* assigns the proper values to the pointnodes in pathnodes; the source node has zero distance */
void fill_arr(int source_x, int source_y){
  int y_pointer, x_pointer;
  pointnode_t *temp;

  for(y_pointer = 0; y_pointer < DUNGEON_H; y_pointer++){
    for(x_pointer = 0; x_pointer < DUNGEON_W; x_pointer++){
      temp = &pathnodes[y_pointer][x_pointer];
      temp->x = x_pointer;
      temp->y = y_pointer;
      temp->dist = INT_MAX;
      temp->prev = NULL;
    }
  }

  pathnodes[source_y][source_x].dist = 0;
}

/* fills a binary heap with pathnodes */
void fill_heap(){
  binheap_init(&distance_heap, pathfinding_compare, pathfinding_datum_delete);

  int y_pointer, x_pointer;

  for(y_pointer = 0; y_pointer < DUNGEON_H; y_pointer++){
    for(x_pointer = 0; x_pointer < DUNGEON_W; x_pointer++){
      if(map[y_pointer][x_pointer] < immutable_min)
	pathnodes[y_pointer][x_pointer].in_heap = 0;
        binarr[y_pointer][x_pointer] = binheap_insert(&distance_heap, &(pathnodes[y_pointer][x_pointer]));
    }
  }
}

/* takes a pointer in a binheap to a pointer to a pointnode to the distance of the pointnode */
int32_t pathfinding_compare(const void *key, const void *with){
  const pointnode_t *kp = (const pointnode_t*) key;
  const pointnode_t *wp = (const pointnode_t*) with;

  return (kp->dist) - (wp->dist);
}

void pathfinding_datum_delete(void *to_delete){
//do nothing
}

void check_neighbors(pointnode_t *n){
  int x = n->x;
  int y = n->y;
  uint32_t dist = n->dist;
  uint32_t alt;

  int i,j;

  pointnode_t *curr;

  for(i = -1; i < 2; i++){
    for(j = -1; j < 2; j++){
      //if x+i and y+j are within the boundraries of the map, and below immutable min, that area is a neighbor
      if((x+i) > 0 && (x+i) < DUNGEON_W - 1 && (y+j) > 0 && (y+j) < DUNGEON_H && map[y+j][x+i] < immutable_min && (i != 0 || j != 0)){
	alt = dist + hardness_convert(map[y+j][x+i]);
	curr = &pathnodes[y+j][x+i];
	//if the curr node is still in the heap, and the new distance is less than the old one
	if(curr->in_heap == 0 && alt < curr->dist){
	  curr->dist = alt;
	  curr->prev = n;
	  binheap_decrease_key(&distance_heap, binarr[y+j][x+i]);
	}
      }
    }
  }
}

int hardness_convert(int hardness){
  if(hardness < 85)
    return 1;
  if(hardness < 171)
    return 2;
  return 3;
}
