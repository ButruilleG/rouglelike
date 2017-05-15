#ifndef MONSTER_READER_H
#define MONSTER_READER_H

/* This class reads information from the monster_desc.txt file */

#include "core.h"
#include "binheap.h"
#include "dice.h"

/* the colors supported by the monsters */
#define COLOR_BLACK 0
#define COLOR_RED 1
#define COLOR_GREEN 2
#define COLOR_YELLOW 3
#define COLOR_BLUE 4
#define COLOR_MAGENTA 5
#define COLOR_CYAN 6
#define COLOR_WHITE 7

/* Each line of a description is 77 chars wide */
#define DESCRIPTION_WIDTH 77
class monster_archetype{
 public:
  char name[DESCRIPTION_WIDTH];
  
  /* a vector of char*'s. */
  std::vector<std::string>* desc;
  int desc_size_in_chars;

  int color;
  char symb;

  dice* speed_die;
  dice* hp_die;
  dice* damage_die;
  uint8_t charachteristics;

  int get_difficulty();

  monster_archetype();
  ~monster_archetype();
};

/* populates an binheap of monster "archetypes" */
std::vector<monster_archetype*>* fill_vector_from_filename(char*);
/*
  Prints, to the console, all the monster archetypes found in the heap.
  Also frees all the data associated with each monster
*/
void flush_monster_archetypes_from_vector(std::vector<monster_archetype*>*);

/*
  flags for beginning and ending files and monster archetypes
*/
#define SEMANTIC_BEGIN_FLAG "RLG327 MONSTER DESCRIPTION 1"
#define BEGIN_MONSTER_FLAG "BEGIN MONSTER"
#define END_MONSTER_FLAG "END"

/* an array of the accepted monster flags */
#define MR_FLAGS_NUM 8
extern const char* monster_reader_flags[MR_FLAGS_NUM];
#define MR_CHARACHTERISTIC_FLAGS_NUM 5
extern const char* monster_reader_charachteristic_flags[MR_CHARACHTERISTIC_FLAGS_NUM];

/* returns which flag a string is. -1 otherise */
int which_flag(const char*);
int which_charachteristic_flag(const char*);

/*
  given a stream pointer, monster archetype reference, and flag,
  sets a charachteristic for a monster
*/
void handle_monster_modification(int, std::ifstream*, monster_archetype*, const char* );

/* prints a monster's description */
void print_monster_archetype_description(monster_archetype*);

/*
  parses between a char* and an integer for ncurses color display
*/
int parse_string_into_color(const char*);
const char* parse_color_into_string(int);

/*
  parses between a string of tags and abilities for a monster
*/
void parse_string_into_abilities(const char*, monster_archetype*);
/*
  parses between a tag and a specific ability
*/
void parse_tag_into_ability(const char*, monster_archetype*);
/*
  prints the abilities of a monster as tags
*/
void print_monster_archetype_abilities(monster_archetype*);

/* modifies a set of dice to match a string, in (base value)+(num dice)d(die faces) format*/
void parse_string_into_dice(dice**,const char*);

#endif
