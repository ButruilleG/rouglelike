#include "monsters.h"

//monster_list_t *charachter_list;

monster* generate_player_monster(monster_archetype* mob){
  monster *temp = generate_monster_from_template(mob);
  maxhp = temp->get_hp();
  set_player(temp);
  temp->set_next_turn(-1);
  return temp;
}

monster* generate_monster_from_template(monster_archetype* mob){
  monster *temp = new monster;
  temp->set_charachteristics(mob->charachteristics);
  temp->set_status(0);
  temp->set_speed(mob->speed_die->roll());
  temp->set_x(0);
  temp->set_y(0);
  temp->set_next_turn(0);
  
  /* new for 1.07 */
  temp->set_hp(mob->hp_die->roll());
  temp->set_color(mob->color);
  temp->set_damage_dice(mob->damage_die);
  temp->set_symb(mob->symb);
  temp->set_name(mob->name);
  
  return temp;
}

void generate_monsters(monster **monster_arr, monster *pc, monster_list_t *monsters, int num_monsters, Roomslist* rl, std::vector<monster_archetype*>* monster_archetype_list){
  int i;
  
  monsters->count = 0;
  monsters->size = num_monsters;
  monsters->monsters_killed = 0;
  monsters->list = monster_arr;
  monsters->player = pc;
  monster_arr[0] = monsters->player;

  generate_monster_location(0, monsters, rl);

  //adding the rest of the required monsters
  for(i = 1; i < monsters->size; i++){
    monster *temp;

    //only generates monsters of appropriate difficulty
    if(level < 0)
      temp = generate_monster_below_difficulty(monster_archetype_list, monster_min_difficulty);
    else{
      temp = generate_monster_below_difficulty(monster_archetype_list, monster_min_difficulty + level*monster_difficulty_level_multiplier);
    }
    
    monster_arr[i] = temp;
    generate_monster_location(i, monsters, rl);
  }
}

monster* generate_monster_below_difficulty(std::vector<monster_archetype*>* monster_archetype_list, int difficulty){
  int ran;

  /* will only try to generate a monster of appropriate difficulty a set number of times. Will gradually increase the allowable difficulty */
  for(int i = 0; i < monster_min_difficulty; i++){
    ran = rand()%monster_archetype_list->size();
    if(((*(monster_archetype_list))[ran])->get_difficulty() < difficulty + i * 2)
      return generate_monster_from_template((*monster_archetype_list)[ran]);
  }
  
  return generate_monster_from_template((*monster_archetype_list)[ran]);
}

void generate_monster_location(int index, monster_list_t *monsters, Roomslist* rl){
  monster *to_set = monsters->list[index];

  do{
    //if the monster tunnels, generate it anywhere
    if(to_set->get_charachteristics() & C_TUNNELER && !(is_player(to_set))){
      to_set->set_x(rand()%(DUNGEON_W - 2) + 1);
      to_set->set_y(rand()%(DUNGEON_H - 2) + 1);
    }
    //if the monster has to move through open space, generate it in a room
    else{
      Room init = rl->rooms[rand()%rl->count];
    
      to_set->set_x(init.x + rand()%init.w);
      to_set->set_y(init.y + rand()%init.h);
    }
  }
  while(check_all_intersects(index, monsters));

  monsters->count++;
}

monster *free_monster_list(monster_list_t *monsters){
  int i;
  for(i = 1; i < monsters->count; i++){
    delete monsters->list[i];
  }
  return monsters->list[0];
}

void set_player(monster *to_set){
  to_set->set_status(to_set->get_status() | 0x2);
  to_set->set_speed(10);
  to_set->set_color(COLOR_WHITE);
  to_set->set_symb('@');
  to_set->set_name("Player");
}

void kill_monster(monster *to_kill, monster_list_t *monsters){
  to_kill->set_status(to_kill->get_status() | 1);

  monsters->monsters_killed++;
}

int is_dead(monster *mob){
  return (mob->get_status() & S_DEAD);
}

int is_player(monster *mob){
  return (mob->get_status() & S_PLAYER);
}

int check_intersects(monster *a, monster *b){
  return !((a->get_x() - b->get_x())|(a->get_y() - b->get_y()));
}

int check_all_intersects(int index, monster_list_t *monsters){
  monster *to_check = monsters->list[index];

  int i;
  int num = 0;
  monster *curr;
  
  for(i = 0; i < monsters->count; i++){
    curr = monsters->list[i];
    if(check_intersects(to_check, curr)){
      num++;
    }
  }
  return num;
}

int move_straight(monster *mob, monster *target, monster_list_t *monsters){
  int next_x, next_y;
  int dx = target->get_x() - mob->get_x();
  int dy = target->get_y() - mob->get_y();
  int dist = sqrt(dx*dx + dy*dy);
  int hardness;

  //if the monsters isn't telepathic and the target is too far away
  if(!(mob->get_charachteristics() & C_TELEPATHIC) && dist > VISION_RANGE){
    return 0;
  }

  if(abs(dx) > abs(dy)){
    next_x = mob->get_x() + to_unit(dx);
    next_y = mob->get_y();
   }
  else{
    next_x = mob->get_x();
    next_y = mob->get_y() + to_unit(dy);
  }

  hardness = map[next_y][next_x];

  //if the monster can tunnel, do so
  if(mob->get_charachteristics() & C_TUNNELER){
    //if the next square is rock, subtract at most 85 from its hardness
    if(hardness > 0){
      hardness -= 85;

      if(hardness < 1){
	hardness = DUNGEON_CORRIDOR;
      }

      map[next_y][next_x] = hardness;

    }
  }

  //if the next area is now some sort of floor
  if(hardness < 0){
    //mob->set_x(next_x);
    //mob->set_y(next_y);
    process_movement(mob, monsters, next_x - mob->get_x(), next_y - mob->get_y());
  }
  else{
    return 0;
  }


  //eliminate_competitors(mob, monsters);

  return 1;
}

int move_erratic(monster *mob, monster_list_t *monsters){
  int x_next,y_next;
  int hardness;

  //if the monster tunnels (and by definition) is not a player
  if(mob->get_charachteristics() & C_TUNNELER && !(is_player(mob))){
    //randomly chose next location
    do{
      x_next = rand()%3 - 1;
      y_next = rand()%3 - 1;
      hardness = map[mob->get_y() + y_next][mob->get_x() + x_next];
    }
    while(hardness == IMMUTABLE_ROCK);
    
    //if the next square is rock, subtract at most 85 from its hardness
    if(hardness > 0){
      hardness -= 85;

      if(hardness < 1){
	hardness = DUNGEON_CORRIDOR;
      }

      map[mob->get_y() + y_next][mob->get_x() + x_next] = hardness;
    }

    //if the current value of the next square is a room or corridor, move there
    if(hardness < 0){
      process_movement(mob, monsters, x_next, y_next);

      //mob->set_x(mob->get_x() + x_next);
      //mob->set_y(mob->get_y() + y_next);
    }
  }
  else{
    //randomly choose next location
    do{
      x_next = rand()%3 - 1;
      y_next = rand()%3 - 1;
      hardness = map[mob->get_y() + y_next][mob->get_x() + x_next];
    }
    while((hardness != DUNGEON_ROOM && hardness != DUNGEON_CORRIDOR) || hardness == IMMUTABLE_ROCK);
    //assign the monster to its new location
    process_movement(mob, monsters, x_next, y_next);

    //mob->set_x(mob->get_x() + x_next);
    //mob->set_y(mob->get_y() + y_next);
  }

  //if the monster erratically decided not to move
  if(x_next == 0 && y_next == 0)
    return 0;

  //eliminate_competitors(mob, monsters);

  return 1;
}

int move_intelligent(monster *mob, monster *target, monster_list_t *monsters){
  int dx = target->get_x() - mob->get_x();
  int dy = target->get_y() - mob->get_y();
  int dist = sqrt(dx*dx + dy*dy);
  int hardness;

  //if the monsters isn't telepathic and the target is too far away
  if(!(mob->get_status() & C_TELEPATHIC) && dist > VISION_RANGE){
    return 0;
  }
  
  if(mob->get_charachteristics() & C_TUNNELER){
    run_djinkstras(target->get_x(), target->get_y(), IMMUTABLE_ROCK);
  }
  else{
    run_djinkstras(target->get_x(), target->get_y(), 0);
  }

  int x_store = 0;
  int y_store = 0;
  int i,j;
  dist = IMMUTABLE_ROCK;

  for(i = -1; i < 2; i++){
    for(j = -1; j < 2; j++){
      int dist_curr = pathnodes[mob->get_y() + j][mob->get_x() + i].dist;

      if(dist_curr < dist){
	dist = dist_curr;
	x_store = i;
	y_store = j;
      }
    }
  }

  if(x_store == 0 && y_store == 0)
    return 0;

  hardness = map[mob->get_y()+y_store][mob->get_x() + x_store];

  //if the next square is rock, subtract at most 85 from its hardness
  if(hardness > 0){
    hardness -= 85;

    if(hardness < 1){
      hardness = DUNGEON_CORRIDOR;
    }

    map[mob->get_y() + y_store][mob->get_x() + x_store] = hardness;
  }

  //if the current value of the next square is smaller than or equal to zero
  if(hardness < 0){
        process_movement(mob, monsters, x_store, y_store);

    //mob->set_x(mob->get_x() + x_store);
    //mob->set_y(mob->get_y() + y_store);


    //eliminate_competitors(mob, monsters);

    return 1;
  }

  return 0;
}

int move_player(int command, monster_list_t *monsters, vector<MapObject*>* objlist){
  int hardness;
  int x_store = 0;
  int y_store = 0;

  switch(command){
  case CODE_7:
  case CODE_Y_LOWER:
    x_store--;
    y_store--;
    break;
  case CODE_8:
  case CODE_K_LOWER:
    y_store--;
    break;
  case CODE_9:
  case CODE_U_LOWER:
    x_store++;
    y_store--;
    break;
  case CODE_6:
  case CODE_L_LOWER:
    x_store++;
    break;
  case CODE_3:
  case CODE_N_LOWER:
    x_store++;
    y_store++;
    break;
  case CODE_2:
  case CODE_J_LOWER:
    y_store++;
    break;
  case CODE_1:
  case CODE_B_LOWER:
    x_store--;
    y_store++;
    break;
  case CODE_4:
  case CODE_H_LOWER:
    x_store--;
    break;
  case CODE_M_LOWER:
  case CODE_SPACE:
    break;
  default:
    break;
  }
  
  hardness = map[monsters->player->get_y() + y_store][monsters->player->get_x() + x_store];
  
  //if the current value of the next square is a room or corridor, move there
  if(hardness < 0){
    process_movement(monsters->player, monsters, x_store, y_store);

    //monsters->player->set_x(monsters->player->get_x() + x_store);
    //monsters->player->set_y(monsters->player->get_y() + y_store);


    //eliminate_competitors(monsters->player, monsters);
    item_pickup(monsters->player, objlist);
    
    return 1;
  }
  
  return 0;
}

int move_monster(monster *mob, int command, monster_list_t *monsters, vector<MapObject*>* objlist){
  //if the monster is already dead, don't calculate anything
  if(is_dead(mob))
    return 0;
  
  /*
    if the monster is erratic every 1/2 times it moves erratically
    if the monster is a player, it moves erratically every time
  */
  if(is_player(mob)){
    return move_player(command, monsters, objlist);
  }
  else if(mob->get_charachteristics() & C_ERRATIC && rand() & 1){
    return move_erratic(mob, monsters);
  }
  else{
    /* if the monster is intelligent */
    if(mob->get_charachteristics() & C_INTELLIGENT){
      return move_intelligent(mob, monsters->player,monsters);
    }
    else{
      return move_straight(mob, monsters->player, monsters);
    }
  }
}

int eliminate_competitors(monster *mob, monster_list_t *monsters){
  int num = 0;
  monster *to_comp;
  
  for(int i = 0; i < monsters->count; i++){
    to_comp = monsters->list[i];

    /*
      if the monsters pointed to by mob and to_comp aren't the same,
      kill to_comp
    */
    if(mob !=  to_comp && check_intersects(mob, to_comp)){
      kill_monster(to_comp, monsters);
      num++;
    }
  }

  return num;
}

int process_movement(monster *mob, monster_list_t *monsters, int x_mod, int y_mod){
  int num = 0;
  int met = 0;
  monster *next;

  /* temporarily moves the monster forward */
  mob->set_x(mob->get_x() + x_mod);
  mob->set_y(mob->get_y() + y_mod);
  
  for(int i = 0; i < monsters->count; i++){
    next = monsters->list[i];

    /*
      if the monsters pointed to by mob and to_comp aren't the same,
    */
    if(mob !=  next && check_intersects(mob, next)){
      //if one is a player, mob attacks next
      if(is_player(mob) ^ is_player(next)){
	met++;
	num += attack_monster(mob, next, monsters);
      }
      //if both are monsters, swap mob and next
      else{
	mob->set_x(mob->get_x() - x_mod);
	mob->set_y(mob->get_y() - y_mod);
	swap_positions(mob,next);
	return num;
      }
    }
  }

  /* moves the monster back if it hasn't killed everythin in its way */
  
  if(num != met){
    mob->set_x(mob->get_x() - x_mod);
    mob->set_y(mob->get_y() - y_mod);
  }
  
  return num;
}

void swap_positions(monster* mover, monster* target){
  int x,y;
  
  x = mover->get_x();
  y = mover->get_y();

  mover->set_x(target->get_x());
  mover->set_y(target->get_y());

  target->set_x(x);
  target->set_y(y);
}

int attack_monster(monster* mover, monster* target, monster_list_t* moblist){
  target->set_hp(target->get_hp() - mover->roll_damage());

  if(target->get_hp() < 0){
    kill_monster(target, moblist);
    return 1;
  }

  return 0;
}

int item_pickup(monster* mob, vector<MapObject*>* objlist){
  MapObject* map_obj = objects_seen[mob->get_y()][mob->get_x()];
  
  if(map_obj){
    //if the object is succesfully added, remove it from the map.
    if(mob->add_item(map_obj->base_class) + 1){
      objects_seen[mob->get_y()][mob->get_x()] = NULL;
      objlist->erase(objlist->begin() + index_of_obj(map_obj,objlist));

      delete map_obj;
    }
  }
  
  return 0;
}
