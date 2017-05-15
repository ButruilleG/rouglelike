#include "monster.h"

dice* barehanded;

int monster::get_x(){
  return x;
}

int monster::get_y(){
  return y;
}

int monster::get_speed(){
  return speed;
}

int monster::roll_speed(){
  int total = get_speed();

  for(int i = 0; i < EQUIPSLOTS; i ++){
    if(equip[i]){
      if(equip[i]->dspeed){
	total += equip[i]->dspeed->roll();
      }
    }
  }
  
  return total;
}

int monster::get_hp(){
  return hp;
}

int monster::get_damage(){
  return damage_dice->roll();
}

int monster::roll_damage(){
  int total = 0;

  for(int i = 0; i < EQUIPSLOTS; i ++){
    if(equip[i]){
      if(equip[i]->ddam){
	total += equip[i]->ddam->roll();
      }
    }
  }

  if(!equip[0]){
    total += get_damage();
  }
  
  return total;
}

int monster::get_color(){
  return color;
}

int monster::get_next_turn(){
  return next_turn;
}

char monster::get_symb(){
  return symb;
}

const char* monster::get_name(){
  return name;
}

uint8_t monster::get_charachteristics(){
  return charachteristics;
}

uint8_t monster::get_status(){
  return status;
}

void monster::set_x(int i){
  x = i;
}

void monster::set_y(int i){
  y = i;
}

void monster::set_speed(int i){
  speed = i;
}

void monster::set_hp(int i){
  hp = i;
}

void monster::set_color(int i){
  color = i;
}

void monster::set_next_turn(int i){
  next_turn = i;
}

void monster::set_damage_dice(dice* dd){
  damage_dice = dd;
}

void monster::set_symb(char c){
  symb = c;
}

void monster::set_name(const char* s){
  name = s;
}

void monster::set_charachteristics(uint8_t c){
  charachteristics = c;
}

void monster::set_status(uint8_t c){
  status = c;
}

int get_monster_next_turn(const monster* mob){
  monster* mob2 = const_cast<monster*>(mob);
  return mob2->get_next_turn();
}

/*
  getter/setter functions to use on class monster by c files
  will be removed 1.07
*/
/*
void delete_monster(monster* mob){
  delete mob;
}

int get_monster_x(monster* mob){return mob->get_x();}
int get_monster_y(monster* mob){return mob->get_y();}
int get_monster_speed(monster* mob){return mob->get_speed();}


uint8_t get_monster_charachteristics(monster* mob){return mob->get_charachteristics();}
uint8_t get_monster_status(monster* mob){return mob->get_status();}

void set_monster_x(monster* mob, int i){mob->set_x(i);}
void set_monster_y(monster* mob, int i){mob->set_y(i);}
void set_monster_speed(monster* mob, int i){mob->set_speed(i);}
void set_monster_next_turn(monster* mob, int i){mob->set_next_turn(i);}

void set_monster_charachteristics(monster* mob, uint8_t c){mob->set_charachteristics(c);}
void set_monster_status(monster* mob, uint8_t c){mob->set_status(c);}
*/

/* item related funtions */

int monster::add_item(ObjectFactory* fac){
  for(int i = 0; i < CARRYSLOTS; i++){
    if(!carry[i]){
      carry[i] = fac;
      
      return i;
    }
  }

  return -1;
}

ObjectFactory* monster::drop_item(int loc){
  if(loc > CARRYSLOTS || loc < 0)
    return NULL;
  
  ObjectFactory* curr = carry[loc];
  carry[loc] = NULL;

  return curr;
}

int monster::wear_item(int loc){
  itemslot sl;
  ObjectFactory* hold;

  if(loc > CARRYSLOTS || loc < 0 || !carry[loc])
    return 0;

  hold = carry[loc];

  if(hold->type == "WEAPON")
    sl = WEAPON;
  else if(hold->type == "OFFHAND")
    sl = OFFHAND;
  else if(hold->type == "RANGED")
    sl = RANGED;
  else if(hold->type == "ARMOR")
    sl = ARMOR;
  else if(hold->type == "HELMET")
    sl = HELMET;
  else if(hold->type == "CLOAK")
    sl = CLOAK;
  else if(hold->type == "GLOVES")
    sl = GLOVES;
  else if(hold->type == "BOOTS")
    sl = BOOTS;
  else if(hold->type == "AMULET")
    sl = AMULET;
  else if(hold->type == "LIGHT")
    sl = LIGHT;
  else if(hold->type == "RING"){
    sl = RING1;
    if(equip[sl])
      sl = RING2;
  }
  else{ //object type is not supported
    return 0;
  }

  carry[loc] = equip[sl];
  equip[sl] = hold;
  
  return 1;
}

int monster::unequip(int loc){
  if(loc > EQUIPSLOTS || loc < 0)
    return 0;

  for(int i = 0; i < CARRYSLOTS; i++){
    if(!carry[i]){
      carry[i] = equip[loc];
      equip[loc] = NULL;
      return 1;
    }
  }

  return 0;
}

const ObjectFactory* monster::inspect_carry_item(int loc){
  if(loc > CARRYSLOTS || loc < 0)
    return NULL;

  return carry[loc];
}

const ObjectFactory* monster::inspect_equip_item(int loc){
  if(loc > EQUIPSLOTS || loc < 0)
    return NULL;

  return equip[loc];
}
