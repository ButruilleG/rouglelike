#ifndef TURN_H
#define TURN_H

#include "core.h"
#include "binheap.h"
#include "monsters.h"
#include "dungeondisplay.h"
#include "terminalcontrol.h"

/* starts the turn action queue */
void turns_start(monster_list_t*);
/* frees the turn action queue */
void turns_end();
/* executes a single turn */
void turn_execute(monster_list_t*,vector<MapObject*>*);

/* the current state of the game */
#define STATE_QUIT 0x1
#define STATE_LOST 0x2
#define STATE_MOVEON 0x4
#define STATE_WON 0x8

#define LAST_LEVEL 25

/* number of monsters killed in the current dungeon*/
extern int level_score;
extern int total_score;

/* finds the game state */
void update_state(monster_list_t*);
void display_game_state();

/* a queue of movements */
extern binheap_t move_queue;

void turn_datum_delete(void *to_del);
/* compares the next turn times of monster key and monster with*/
int32_t turn_compare(const void *key, const void *with);

#endif
