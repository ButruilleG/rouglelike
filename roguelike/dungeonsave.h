#ifndef DUNGEONSAVE_H
#define DUNGEONSAVE_H

#include "core.h"
#include "room.h"
#include "utilities.h"

/* saves the dungeon to the file at "filename" */
void mapSave(char *filename, Roomslist* rl);

#endif
