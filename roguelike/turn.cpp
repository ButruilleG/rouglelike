#include "turn.h"

int level_score;
int total_score;

binheap_t move_queue;

void turns_start(monster_list_t* charachter_list){
  int i;
  
  game_state = 0;
  binheap_init(&move_queue, turn_compare, turn_datum_delete);

  //fills the move queue with indexes referring to monsters
  for(i = 0; i < charachter_list->count; i++){
    monster *curr = charachter_list->list[i];
    if(!is_player(curr))
      curr->set_next_turn(curr->get_next_turn() + 100/curr->roll_speed());
    
    binheap_insert(&move_queue, curr);
  }
}

void turns_end(){
  binheap_delete(&move_queue);
}

void turn_execute(monster_list_t* charachter_list,vector<MapObject*>* objlist){
  monster *curr;
  int command = 0;
  int select;
  
  curr = (monster*) binheap_remove_min(&move_queue);

  if(is_player(curr)){
    //heals the player monster by one point, if the player health is depleted
    if(curr->get_hp() < maxhp){
      curr->set_hp(curr->get_hp() + 1);
    }
    
    while(!command){
      printMap(charachter_list,objlist);
      command = read_input();

      //Monster information screen
      if(command == CODE_M_LOWER){
	display_info_screen(charachter_list,CODE_M_LOWER);
	command = 0;
      }

      //Carried items information screen
      if(command == CODE_I_LOWER){
	display_info_screen(charachter_list,CODE_I_LOWER);
	command = 0;
      }

      //Equipped items information screen
      if(command == CODE_E_LOWER){
	display_info_screen(charachter_list,CODE_E_LOWER);
	command = 0;
      }

      //item inspection information screen
      if(command == CODE_I_UPPER){
	select = item_selection_screen(command, curr);
	
	if(curr->inspect_carry_item(select)){
	  display_item_information(curr->inspect_carry_item(select));
	}
	else{
	  set_message("Not a valid item.",17);
	}
	
	command = 0;
      }

      //wearing an item
      if(command == CODE_W_LOWER){
        select = item_selection_screen(command, curr);

	if(curr->wear_item(select)){
	  set_message("Item worn.",10);
	}
	else{
	  set_message("Item not worn.",14);
	}
	
	command = 0;
      }

      //taking off an item
      if(command == CODE_T_LOWER){
	select = item_selection_screen(command, curr);

	if(curr->unequip(select)){
	  set_message("Item unequipped",16);
	}
	else{
	  set_message("Item not unequipped.",20);
	}
	command = 0;
      }

      //dropping an item
      if(command == CODE_D_LOWER){
	select = item_selection_screen(command, curr);
	ObjectFactory* curr_obj = curr->drop_item(select);
	if(curr_obj){
	  objlist->push_back(create_MapObject(curr_obj));
	  if(drop_object_on_floor((*objlist)[objlist->size() - 1], curr->get_x(), curr->get_y(), objlist)){
	    set_message("Item dropped",12);
	  }
	  else{
	    set_message("No space to drop item",21);
	  }
	}
	else{
	  set_message("No item dropped",15);
	}
      }

      if(command == CODE_X_LOWER){
	select = item_selection_screen(command, curr);
	ObjectFactory* curr_obj = curr->drop_item(select);

	if(curr_obj){
	  set_message("Item expunged.",14);
	}
	else{
	  set_message("No item expunged.",17);
	}

      }
      
      //commands not useable from the current view
      if(command == CODE_UP_ARROW || command == CODE_DOWN_ARROW || command == CODE_ESCAPE){
	command = 0;
      }

      //Must use stair commands while on correct staircase
      if((command == CODE_RIGHT_CHEVRON && map[curr->get_y()][curr->get_x()] == DUNGEON_STAIRS_DOWN)||(command == CODE_LEFT_CHEVRON && map[curr->get_y()][curr->get_x()] == DUNGEON_STAIRS_UP)){
        game_state |= STATE_MOVEON;

	if(command == CODE_RIGHT_CHEVRON)
	  level++;
	else
	  level--;
	
	clear_objects_seen();
      }
    }
  }

  if(command == CODE_Q_LOWER || command == CODE_Q_UPPER){
    game_state |= STATE_QUIT;
  }
  else{
    move_monster(curr, command, charachter_list, objlist);

    curr->set_next_turn(curr->get_next_turn() + 100/curr->roll_speed());
    if(!is_dead(curr))
      binheap_insert(&move_queue, curr);
  }

  update_state(charachter_list);
}

void turn_datum_delete(void *to_del){
  //do nothing
}

int32_t turn_compare(const void *key, const void *with){
  const monster* k = (const monster*) key;
  const monster* w = (const monster*) with;

  return get_monster_next_turn(k) - get_monster_next_turn(w);
}

void update_state(monster_list_t* charachter_list){
  level_score = 0;
  int i;

  for(i = 1; i < charachter_list->count; i++){
    if(is_dead(charachter_list->list[i]))
      level_score++;
  }

  if(is_dead(charachter_list->player)){
    game_state |= STATE_LOST;
  }

  if(level == LAST_LEVEL){
    game_state |= STATE_WON;
  }
  
  char statusmessage[80];
     sprintf(statusmessage,"HP: %04d, level: %03d/%02d",charachter_list->player->get_hp(),level,LAST_LEVEL);
  set_message(statusmessage, 23);
}

void display_game_state(){
  clear();

  printw("%s %d\n","game status:",game_state);

  if(game_state & STATE_WON){
    printw("YOU WIN\n");
  }
  else if(game_state & STATE_LOST){
    printw("YOU LOSE\n");
  }
  
  printw("Score: %d\n", total_score);


  refresh();
}
