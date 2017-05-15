#ifndef MAPOBJECT_H
#define MAPOBJECT_H

#include "core.h"
#include "objectfactory.h"
#include "room.h"
#include "monster_reader.h"

/* item difficulty settings */
#define item_min_difficulty 10
#define item_difficulty_level_multiplier 10

class MapObject{
 public:
  int x,y,color;
  char symb;
  ObjectFactory* base_class;
};

MapObject* create_MapObject(ObjectFactory*);
MapObject* create_MapObject_below_difficulty(vector<ObjectFactory*>*, int difficulty);

vector<MapObject*>* create_MapObject_vector(vector<ObjectFactory*>*, unsigned int);
void clear_MapObject_vector(vector<MapObject*>*);
void place_mapobjects(vector<MapObject*>*, Roomslist*);

MapObject* obj_at_loc(int x,int y,vector<MapObject*>* objlist);
int index_of_obj(MapObject*,vector<MapObject*>*);

extern MapObject* objects_seen[DUNGEON_H][DUNGEON_W];
void clear_objects_seen();

/*
  drops the map object on the nearest empty floor space.
  returns whether the object was dropped.
*/
int drop_object_on_floor(MapObject* obj, int currx, int curry, vector<MapObject*>* objlist);

#endif
