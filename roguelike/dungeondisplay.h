#ifndef DUNGEONDISPLAY_H
#define DUNGEONDISPLAY_H

#include "core.h"
#include "pathfinding.h"
#include "mapobject.h"
#include "monsters.h"
#include "terminalcontrol.h"

/* print the dungeon */
void printMap(monster_list_t*,vector<MapObject*>* objlist);
/* deprecated */
//void print_map_dist();

/* tie each number on the map to a charachter */
char squareConverter(int toConvert);

/* converts distance values to @,0-9,a-z,A-Z */
unsigned char distance_convert(int distance);

/* render a monster based on its charachteristics */
unsigned char charachterConverter(monster *toConvert);

/* return the monster at a location, NULL otherwise*/
monster* character_at_loc(int x, int y, monster_list_t* charachter_list);

/*
  Display various information screens
CODE_M_LOWER - Monsters
CODE_I_LOWER - Player carry items
CODE_E_LOWER - Player equip items
*/
int display_info_screen(monster_list_t* charachter_list, int displaytype);
/*
  Returns the selected item
 */
int item_selection_screen(int command, monster* player);
/*
  Displays the description of an item.
*/
void display_item_information(const ObjectFactory *obj);

#endif
