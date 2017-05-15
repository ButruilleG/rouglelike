#include "binheap.h"
#include "core.h"
#include "dungeonload.h"
#include "dungeoncreate.h"
#include "dungeondisplay.h"
#include "dungeonsave.h"
#include "monster_reader.h"
#include "monsters.h"
#include "mapobject.h"
#include "objectfactory.h"
#include "path.h"
#include "stairs.h"
#include "room.h"
#include "pathfinding.h"
#include "turn.h"
#include "terminalcontrol.h"
#include "utilities.h"

int checkSequence(const char *string, char* arr[], int size);

void newDungeon(Roomslist* rl);

int main(int argc, char *argv[]){
  int loaded = 0;
  level = 0;
  //save files
  char* filepath = strcat(getenv("HOME"), "/.rlg327/");
  char save_filename[50];
  char mons_filename[50];
  char obj_filename[50];
  sprintf(save_filename,"%s%s", filepath,"dungeon");
  sprintf(mons_filename,"%s%s", filepath,"monster_desc.txt");
  sprintf(obj_filename,"%s%s", filepath,"object_desc.txt");
  
  //fills a binheap with the monster archetypes
  std::vector<monster_archetype*>* monster_archetypes = fill_vector_from_filename(mons_filename);

  //fills an object factory with objects
  ObjectFactory::load(obj_filename);

  //For storing map objects
  vector<MapObject*>* MO_vector;
  
  //index of a flag passed into the argument
  int flag_index;
  
  //Roomslist stores the rooms in the dungeon
  //Roomslist* rl = (Roomslist) malloc(sizeof(Roomslist));
  Roomslist* rl = new Roomslist;
  rl->count = 0;

  //initialize the random number generator
  srand(time(NULL));
  
  //if the "--load" flag is present, load the dungeon
  if(checkSequence(LOAD_FLAG, argv, argc)){
    mapLoad(save_filename, rl);
    loaded = 1;
  }

  //determine the number of monsters
  //the default number of monsters is 4
  int num_monsters = 4;
  flag_index = checkSequence(MONSTER_FLAG, argv, argc);
  if(flag_index && flag_index < argc){
    num_monsters = atoi(argv[flag_index]);
  }

  //generate the player
  monster* monster_arr[++num_monsters];
  //the pc is just the first type of monster to be read
  monster* pc = generate_player_monster((*monster_archetypes)[0]);

  //set the initial score from turn.h
  level_score = 0;
  total_score = 0;
  
  monster_list_t monster_list_temp;
  
  while(!(game_state & STATE_QUIT) && !(game_state & STATE_LOST) && !(game_state & STATE_WON)){
    if(!loaded){
      newDungeon(rl);
      reset_map_seen();
    }

    //fills a list of 10~15 Map Objects
    clear_objects_seen();
    MO_vector = create_MapObject_vector(&ObjectFactory::factories, rand()%5 + 10);
    
    //places the objects inside of rooms
    place_mapobjects(MO_vector, rl);
    
    //generate a monster list full of monsters
    generate_monsters(monster_arr, pc, &monster_list_temp, num_monsters, rl, monster_archetypes);

    //place the stairs
    place_stairs(rl);
    
    //starts the ncurses terminal display 
    io_init_terminal();

    //starts turn execution 
    turns_start(&monster_list_temp);
    
    while(!(game_state & STATE_QUIT) && !(game_state & STATE_LOST) && !(game_state & STATE_MOVEON) && !(game_state & STATE_WON)){
      turn_execute(&monster_list_temp,MO_vector);
    }

    //ends turn execution
    turns_end();

    //dungeon should be created from scratch
    loaded = 0;

    //preventing memory leaks
    if(!(game_state & STATE_QUIT || game_state & STATE_LOST || game_state & STATE_WON)){
      delete rl;
      //rl = (Roomslist) malloc(sizeof(Roomslist));
      rl = new Roomslist;
      rl->count = 0;
    }

    free_monster_list(&monster_list_temp);
    
    //resets the player, game state, and level score
    if(game_state & STATE_MOVEON){
      pc->set_next_turn(0);
      game_state = 0;
    }
    
    total_score += level_score;
    level_score = 0;
    
    //clears the vector of map objects
    clear_MapObject_vector(MO_vector);
  }
  
  delete pc;
  
  display_game_state();
  usleep(1000000);

  io_end_terminal();

  // flushes the monster archetypes from the heap, and displays them
  flush_monster_archetypes_from_vector(monster_archetypes);

  
  //deletes the object factories
  ObjectFactory::deleteFactories();
  
  //if the "--save" flag is present, save the dungeon
  if(checkSequence(SAVE_FLAG, argv, argc)){
    mapSave(save_filename, rl);
  }
  
  delete rl;
}

//returns the index of a flag plus one, with 0 being no flag present
int checkSequence(const char *string, char* arr[], int size){
  int i;
  for(i = 0; i < size; i++){
    if(!strcmp(string, arr[i]))
      return i + 1;
  }
  return 0;
}


//generate full dungeon from scratch
void newDungeon(Roomslist *rl){
  int i,j;

  mapGenerate();
  
  //smooth over the map left to right
  mapSmoothLR();
  mapSmoothTB();

  //sets the outer part of the dungeons to be immutable
  setImmutable();
  
  //generate 6 rooms, each at least 1 square away from each other
  while(!rl->count){
    //generate 6 rooms
    for(i = 0; i < 6; i++){
      rl->rooms[i] = room_create();
      rl->count = rl->count + 1;
    }

    //check for conflicts
    for(i = 0; i < rl->count; i++){
      int tooClose = 0;
      for(j = 0; j < rl->count; j++){
	tooClose += room_overlap(rl->rooms[i],rl->rooms[j]);
      }
      if(tooClose > 1){
        rl->count = 0;
        break;
      }
    }
  }

  //place each room on the map
  for(i = 0; i < rl->count; i++){
    room_paint(rl->rooms[i]);
  }
  
  //connect the rooms to each other
  Tunneler snek;
  snek.x = rl->rooms[0].x;
  snek.y = rl->rooms[0].y;
  
  for(i = 1; i < rl->count; i++){
    path_connect(snek, rl->rooms[i]);
  }
  
}
