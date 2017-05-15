#include "mapobject.h"

MapObject* objects_seen[DUNGEON_H][DUNGEON_W];

MapObject* create_MapObject(ObjectFactory* fac){
  if(!fac)
    return NULL;
  
  MapObject* obj = new MapObject();
  obj->x = 0;
  obj->y = 0;
  //obj->color = Parser::parseColor(fac->color);
  obj->color = parse_string_into_color(fac->color.c_str());
  obj->symb = ObjectFactory::SYMB[Parser::parseType(fac->type)];
  obj->base_class = fac;
  return obj;
}

void clear_MapObject_vector(vector<MapObject*>* v){
  while(!v->empty()){
    if((*v)[v->size() - 1])
      delete (*v)[v->size() - 1];
    
    v->pop_back();
  }

  delete v;
}

MapObject* create_MapObject_below_difficulty(vector<ObjectFactory*>*vf, int difficulty){
  int ran;
  
  for(int i = 0; i < item_min_difficulty; i++){
    ran = rand()%vf->size();
    if((*vf)[ran]->get_difficulty() < difficulty + i)
      return create_MapObject((*vf)[ran]);
  }
  return create_MapObject((*vf)[ran]);
}


vector<MapObject*>* create_MapObject_vector(vector<ObjectFactory*>* vf, unsigned int num){
  vector<MapObject*>* vm = new vector<MapObject*>();
  for(unsigned int i = 0; i < num; i++){
    if(level < 0)
      vm->push_back(create_MapObject_below_difficulty(vf, item_min_difficulty));
    else
      vm->push_back(create_MapObject_below_difficulty(vf, item_min_difficulty + level*item_difficulty_level_multiplier));
  }
  
  return vm;
}

void place_mapobjects(vector<MapObject*>* v, Roomslist* rl){
  int i = 0;

  while(i < (int) v->size()){
    Room init = rl->rooms[rand()%rl->count];

    (*v)[i]->x = init.x + rand()%init.w;
    (*v)[i]->y = init.y + rand()%init.h;
    
    //check against the previous vectors to make sure it doesn't intersect
    for(int j = 0; j < i; j++){
      if((*v)[i]&& j - i && (*v)[i]->x == (*v)[j]->x && (*v)[i]->y == (*v)[j]->y){
	i--;
	break;
      }
    }

    i++;
  }
}

MapObject* obj_at_loc(int x, int y, vector<MapObject*>* v){
  for(unsigned int i = 0; i < v->size(); i++){
    if((*v)[i] && (*v)[i]->y == y && (*v)[i]->x == x ){
      return (*v)[i];
    }
  }

  return NULL;
}

int index_of_obj(MapObject* mob,vector<MapObject*>* objlist){
  for(unsigned int i = 0; i < objlist->size(); i++){
    if((*objlist)[i] == mob)
      return i;
  }
  return -1;
}

void clear_objects_seen(){
  for(int y = 0; y < DUNGEON_H; y++){
    for(int x = 0; x < DUNGEON_H; x++){
      objects_seen[y][x] = NULL;
    }
  }
}

int drop_object_on_floor(MapObject* obj, int currx, int curry, vector<MapObject*>* objlist){
  for(int y = -1; y < 2; y++){
    for(int x = -1; x < 2; x++){
      if(map[curry + y][currx + x] < 0 && (x || y) && !obj_at_loc(currx + x, curry + y, objlist)){
	obj->y = curry + y;
	obj->x = currx + x;
	objects_seen[curry + y][currx + x] = obj;

	return 1;
      }
    }
  }

  return 0;
}
