#ifndef MONSTERS_H
#define MONSTERS_H

#include "core.h"
#include "room.h"
#include "utilities.h"
#include "pathfinding.h"
#include "terminalcontrol.h"

/* just the monster class, and it's getter/setter functions */
#include "monster.h"
/* includes the monster archetype class */
#include "monster_reader.h"

/* monster difficulty settings */
#define monster_min_difficulty 20
#define monster_difficulty_level_multiplier 8

typedef struct monster_list{
  /* count is the # of initialized monsters in the list (other than player), size is the total size of the array list */
  int count, size, monsters_killed;
  /*
    A pointer to the pointer of the first monster in an array of monsters
  */
  monster **list;
  /*
    A pointer to the player charachter of the list
  */
  monster *player;
} monster_list_t;

/* radius in which monsters can see*/
#define VISION_RANGE 15
#define VISION_RANGE_PLAYER 3

/* the main list of charachters */
//extern monster_list_t *charachter_list;

/* generates a monster. Should only be used if a map is already generated */
monster* generate_player_monster(monster_archetype*);
monster* generate_monster_below_difficulty(std::vector<monster_archetype*>*, int difficulty);
monster* generate_monster_from_template(monster_archetype*);

/*
  Fills a charachterList with monsters, given that the first in the list is the player.
  The monsters should be generated using a list of archetypes
*/
void generate_monsters(monster **monster_arr, monster *pc, monster_list_t *monsters, int num_monsters, Roomslist* rl, std::vector<monster_archetype*>* monster_archetype_list);
void generate_monster_location(int index, monster_list_t *monsters, Roomslist * rl);
/*
  Calls free() on every monster generated for charachter_list
*/
monster *free_monster_list(monster_list_t *monsters);

/* sets the status of a monster or player */
void set_player(monster *to_set);
void kill_monster(monster *to_kill, monster_list_t *monsters);

/* determines status of player/monster */
int is_dead(monster *mob);
int is_player(monster *mob);

/* returns whether the monsters a and b intersect */
int check_intersects(monster *a, monster *b);
/* returns the number of monsters to_check intersects with in check_against */
int check_all_intersects(int index, monster_list_t *check_against);

/*
  Moves a monster according to its traits.
  At final location, kills any occupants.
  Returns whether the monster moved
*/
int move_monster(monster *mob, int command, monster_list_t *monsters,  vector<MapObject*>* objlist);

/*
  Specific instructions for how to move a monster.
  Only intended for use inside move_monster()
*/
int move_straight(monster *mob, monster *target, monster_list_t *monsters);
int move_erratic(monster *mob, monster_list_t *monsters);
int move_intelligent(monster *mob, monster *target, monster_list_t *monsters);
int move_player(int command, monster_list_t *monsters, vector<MapObject*>* objlist);

/*
  Determines if any other monsters share a location with mob. If so, kills them.
  Returns the number of competitors killed
*/
int eliminate_competitors(monster *mob, monster_list_t *monsters);

//TODO
/*
  Determines whether the monster interacts with something at it's movement end
  Returns the number of monsters killed
*/
int process_movement(monster *mob, monster_list_t *monsters, int x_mod, int y_mod);
/*
  Peacefully swaps the locations of two monsters
 */
void swap_positions(monster* mover, monster* target);
/*
  A monster attacks another using its items.
  Returns 1 if the target dies, 0 otherwise
 */
int attack_monster(monster* mover, monster* target, monster_list_t* moblist);
/*
  if there's something at this location, pick it up and remove it from
 */
int item_pickup(monster* mob, vector<MapObject*>* objlist);

#endif
