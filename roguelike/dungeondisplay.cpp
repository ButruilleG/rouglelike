#include "dungeondisplay.h"

//prints out the map
void printMap(monster_list_t* charachter_list,vector<MapObject*>* objlist){
  int i,j;
  monster* curr;
  unsigned char c;
  monster* pc = charachter_list->player;
  
  clear();
  printw(message);

  for(i = 0; i < DUNGEON_H; i++){
    for(j = 0; j < DUNGEON_W; j++){
      curr = character_at_loc(j,i, charachter_list);
      
      if(sqrt((i-pc->get_y())*(i-pc->get_y())+(j-pc->get_x())*(j-pc->get_x())) < VISION_RANGE_PLAYER){
	map_seen[i][j] = map[i][j];
	objects_seen[i][j] = obj_at_loc(j,i,objlist);

	if(curr){
	  c = charachterConverter(curr);
	  if(c == (unsigned char) curr->get_symb()){
	    attron(COLOR_PAIR(curr->get_color()));
	    mvaddch(1+i,j,c);
	    attroff(COLOR_PAIR(curr->get_color()));

	    //if(objects_seen[i][j] && c == '@')
	    //set_message(objects_seen[i][j]->color,objects_seen[i][j]->symb);
	  }
	  else{	
	    if(objects_seen[i][j]){
	      attron(COLOR_PAIR(objects_seen[i][j]->color));
	      //mvaddch(1+i,j,objects_seen[i][j]->symb);
	      attroff(COLOR_PAIR(objects_seen[i][j]->color));
	    }
	    else{
	      attron(COLOR_PAIR(COLOR_WHITE));
	      mvaddch(1+i,j,c);
	      attroff(COLOR_PAIR(COLOR_WHITE));
	    }
	  }
	}
      }
      else{
	curr = NULL;
	if(index_of_obj(objects_seen[i][j], objlist) == -1)
	  objects_seen[i][j] = NULL;
      }

      if(!curr){		
	if(objects_seen[i][j]){
	  attron(COLOR_PAIR(objects_seen[i][j]->color));
	  mvaddch(1+i,j,objects_seen[i][j]->symb);
	  attroff(COLOR_PAIR(objects_seen[i][j]->color));
	}
	else{
	  attron(COLOR_PAIR(COLOR_WHITE));
	  mvaddch(1+i,j,squareConverter(map_seen[i][j]));
	  attroff(COLOR_PAIR(COLOR_WHITE));
	}
      }
    }
  }

  refresh();
}

//renders a map square as a string
char squareConverter(int toConvert){
  switch(toConvert){
  case DUNGEON_STAIRS_DOWN:
    return '>';//stairs down
    break;
  case DUNGEON_STAIRS_UP:
    return '<';//stairs up
    break;
  case DUNGEON_CORRIDOR:
    return '#'; //path
    break;
  case DUNGEON_ROOM:
    return '.'; //room
    break;
  default:
    return ' '; //rock
  }
}

unsigned char charachterConverter(monster *toConvert){
  if(is_dead(toConvert))
    return squareConverter(map[toConvert->get_y()][toConvert->get_x()]);

  return (unsigned char) toConvert->get_symb();
  
  /*
  if(is_player(toConvert)){
    return '@';
  }
  else{
    unsigned char mob_type = toConvert->get_charachteristics() & 0xf;
    if(mob_type < 10)
      return mob_type + 48;

    mob_type -= 10;
    
    return mob_type + 97;
  }
  */
}

monster* character_at_loc(int x, int y, monster_list_t *charachter_list){
  int i;
  monster *curr;
  for(i = 0; i < charachter_list->count; i++){
    curr = charachter_list->list[i];

    if(curr->get_x() == x && curr->get_y() == y && !(is_dead(curr))){
      return curr;
    }
  }

  return NULL;
}
/*
void print_map_dist(){
  int i,j;
  int curr;

  for(i = 0; i < DUNGEON_H; i++){
    for(j = 0; j < DUNGEON_W; j++){
      curr = pathnodes[i][j].dist;
      if(curr > 61){
        mvaddch(1+i,j,squareConverter(map[i][j]));
      }
      else{
        if(curr < 0 || map[i][j] >= immutable_min){
          mvaddch(1+i,j,' ');
        }
        else{
          mvaddch(1+i,j,distance_convert(curr));
        }
      }
    }
  }
}
*/

unsigned char distance_convert(int distance){
  if(distance < 10)
    return (unsigned char) (distance + 48);

  distance -= 10;

  if(distance < 26)
    return (unsigned char) (distance + 97);

  distance -= 26;

  return (unsigned char) (distance + 65);
}

int display_info_screen(monster_list_t* charachter_list, int displaytype){
  int command = 0;
  int start = 0;

  while(1){
    clear();

    int count = 0;
    monster *mob;

    if(displaytype == CODE_M_LOWER){
      for(int i = 1; i + start < charachter_list->count && count < TERMINAL_HEIGHT - 1; i++){
	mob = charachter_list->list[i + start];
	if(!is_dead(mob)){
	  attron(COLOR_PAIR(mob->get_color()));
	  printw("%s: %c, relative x: %d, relative y: %d\n", mob->get_name(), charachterConverter(mob), charachter_list->player->get_x() - mob->get_x(), charachter_list->player->get_y() - mob->get_y());
	  attroff(COLOR_PAIR(mob->get_color()));
	  count++;
	}
      }
      printw(message);
    }
    else{
      mob = charachter_list->player;
      printw("press any key to leave\n");

      if(displaytype == CODE_I_LOWER){
	for(int i = 0; i < CARRYSLOTS; i++){
	  const ObjectFactory* curr = mob->inspect_carry_item(i);

	  if(curr){
	    printw("Item %d: ",i);
	    attron(COLOR_PAIR(parse_string_into_color(curr->color.c_str())));
	    printw("%s\n",curr->name.c_str());
	    attroff(COLOR_PAIR(parse_string_into_color(curr->color.c_str())));
	  }
	}
      }
      
      if(displaytype == CODE_E_LOWER){
	for(int i = 0; i < EQUIPSLOTS; i++){
	  const ObjectFactory* curr = mob->inspect_equip_item(i);

	  if(curr){
	    printw("Item %c: ",(char) i + 97);
	    attron(COLOR_PAIR(parse_string_into_color(curr->color.c_str())));
	    printw("%s\n",curr->name.c_str());
	    attroff(COLOR_PAIR(parse_string_into_color(curr->color.c_str())));
	  }
	}
      }

      getch();
      return 1;
    }
    
    refresh();

    command = read_input();
    if(command == CODE_UP_ARROW && start > 0){
      start--;
    }
    else if(command == CODE_DOWN_ARROW && start < charachter_list->count - 23){
      start++;
    }
    else if(command == CODE_ESCAPE){
      return 1;
    }
    else if(command == CODE_Q_LOWER || command == CODE_Q_UPPER){
      io_end_terminal();
      exit(1);
    }
  }
}

int item_selection_screen(int command, monster* player){
  int select;
  // 0 is modifying equip, 1 is modifying carry, -1 is incorrect command
  int equip_modifier;
  clear();

  if(command == CODE_W_LOWER || command == CODE_I_UPPER || CODE_D_LOWER){
    equip_modifier = 1;
  }
  else if(command == CODE_T_LOWER){
    equip_modifier = 0;
  }
  else{
    equip_modifier = -1;
  }
  
  if(equip_modifier == 1){
    set_message("please input carry slot (0-9)",29);
    printw(message);

    refresh();

    select = read_numerical_input();
  }
  else if(equip_modifier == 0){
    set_message("please input equip slot (a-l)",29);
    printw(message);

    refresh();

    select = read_alphabetical_input();
  }
  else{
    set_message("Not a valid command",29);
    return -1;
  }
  

  if((equip_modifier == 1 && select > -1 && select < 10) || (equip_modifier == 0 && select > -1 && select < 12)){
    return select;
  }
  else{
    return -1;
  }
}

void display_item_information(const ObjectFactory *obj){
  clear();
  printw("Press any key to exit.\n");
  printw("Item: %s\n",obj->name.c_str());
  printw("Description: ");
  printw("%s",obj->desc.c_str());
  refresh();

  getch();
}
