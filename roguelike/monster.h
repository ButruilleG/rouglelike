#ifndef MONSTER_H
#define MONSTER_H

#include "core.h"
#include "dice.h"
#include "monster_reader.h"
#include "objectfactory.h"
#include "mapobject.h"

enum itemslot{
  WEAPON,
  OFFHAND,
  RANGED,
  ARMOR,
  HELMET,
  CLOAK,
  GLOVES,
  BOOTS,
  AMULET,
  LIGHT,
  RING1,
  RING2
};

#define EQUIPSLOTS 12
#define CARRYSLOTS 10

class monster{
 private:
  int x,y;
  uint8_t charachteristics, status;
  int next_turn;

  /*
    new information from monster_reader
  */
  const char* name;

  int color;
  char symb;

  int hp, speed;
  dice* damage_dice;

  //moved from public to private for 1.09
  int get_damage();
 public:
  /* for holding equipment information */
  /* Equipped items*/
  ObjectFactory** equip;
  /* Carried items*/
  ObjectFactory** carry;

  monster(){
    equip = new ObjectFactory*[EQUIPSLOTS];
    for(int i = 0; i < EQUIPSLOTS;i++){
      equip[i] = NULL;
    }
    carry = new ObjectFactory*[CARRYSLOTS];
    for(int i = 0; i < CARRYSLOTS;i++){
      carry[i] = NULL;
    }
  }

  ~monster(){
    delete equip;
    delete carry;
  }
  
  /* getter methods */
  int get_x(), get_y(), get_speed(), get_hp(), get_color(), get_next_turn();
  char get_symb();
  const char* get_name();
  uint8_t get_charachteristics(), get_status();

  /* setter methods */
  void set_x(int), set_y(int), set_speed(int), set_hp(int), set_color(int), set_next_turn(int);
  void set_damage_dice(dice*);
  void set_symb(char);
  void set_name(const char*);
  void set_charachteristics(uint8_t), set_status(uint8_t);

  /* rolling methods (include equipment bonuses) */
  int roll_speed();
  int roll_damage();
  
  /* item-related functions*/
  /* Adds an item to the first available carry slot. Returns the slot the item was added to, -1 otherwise */
  int add_item(ObjectFactory*);
  /*
    drops an item corresponding to the int carry slot on the floor
    returns a pointer to the item dropped
  */
  ObjectFactory* drop_item(int);
  /*
    causes the item at the int carry slot to be worn
    returns whether the item was succesfully worn
  */
  int wear_item(int);
  /*
    Causes the item at the int equip slot to be carried instead
    Does nothing if there's no carry space
    returns whether the item was succesfully removed
  */
  int unequip(int);
  /* returns the item at a carry slot int, so information on it can be used */
  const ObjectFactory* inspect_carry_item(int);
  /* returns the item at a carry slot int, so information on it can be used */
  const ObjectFactory* inspect_equip_item(int);
};

/* necessary for comparison */
int get_monster_next_turn(const monster*);

/*
#ifdef __cplusplus
extern "C" {
#endif
*/
/*
//deletes a monster
void delete_monster(monster*);

//return information about the monster to non-c++ programs
int get_monster_x(monster*), get_monster_y(monster*), get_monster_speed(monster*), ;
uint8_t get_monster_charachteristics(monster*), get_monster_status(monster*);

//sets the monster's attributes for non-c++ programs
void set_monster_x(monster*,int), set_monster_y(monster*,int), set_monster_speed(monster*,int), set_monster_next_turn(monster*,int);
void set_monster_charachteristics(monster*,uint8_t), set_monster_status(monster*,uint8_t);
*/
/*
#ifdef __cplusplus
}
#endif
*/
#endif
