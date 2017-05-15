#include "monster_reader.h"

const char* monster_reader_flags[MR_FLAGS_NUM] =
  {"NAME","DESC","SYMB","COLOR","SPEED","ABIL","HP","DAM"};
const char* monster_reader_charachteristic_flags[MR_CHARACHTERISTIC_FLAGS_NUM] =
  {"SMART","TELE","TUNNEL","ERRATIC","PASS"};

using namespace std;

monster_archetype::monster_archetype(){
  speed_die = hp_die = damage_die = NULL;
}

monster_archetype::~monster_archetype(){
  if(speed_die) delete speed_die;
  if(hp_die) delete hp_die;
  if(damage_die) delete damage_die;
}

int monster_archetype::get_difficulty(){
  int d = 0;
  
  if(speed_die)
    d += speed_die->roll();
  if(hp_die)
    d += hp_die->roll();
  if(damage_die)
    d += damage_die->roll();
  return d;
}

vector<monster_archetype*>* fill_vector_from_filename(char* filename){  
  //opens file
  ifstream f;
  f.open(filename, ios::in);

  //check if file has been properly opened
  if(!f.is_open())
    return NULL;
  
  //checks for presence of the semantic header
  string semantic = SEMANTIC_BEGIN_FLAG;
  string curr;
  getline(f, curr);
  
  if(curr.compare(semantic)){
    return NULL;
  }

  //create a monster_archetype vector
  vector<monster_archetype*>* monster_archetypes = new vector<monster_archetype*>();
  monster_archetype* mobtype;
  //for holding exactly one string
  char tag[80];
  char word[80];
  //for holding an integer flag
  int flag = -1;
  
  //until the end of a file is found...
  while(!f.eof()){
    //find the tag for beginning a monster
    while(getline(f, curr)){
      if(!curr.compare(BEGIN_MONSTER_FLAG)){
	//create a new monster to work with
	mobtype = new monster_archetype();
	//add the monster to the vector
	if(mobtype){
	  monster_archetypes->push_back(mobtype);
	}
	break;
      }
    }

    //until the end of the monster is found...
    while(getline(f, curr)){
      if(!curr.compare(END_MONSTER_FLAG)){
	break;
      }
      
      //if there is a word on this line
      if(sscanf(curr.c_str(),"%[^' '] %[^\n]",tag,word)){
	flag = which_flag(tag);
	//check if this is a keyword
	if(flag + 1){
	  //pass the stream pointer, vector, and keyword to something that can handle them
	  handle_monster_modification(flag, &f, mobtype, word);
	  //the stream pointer now points to the new keyword
	}
      }
    }
  }
  
  f.close();
  return monster_archetypes;
}

void flush_monster_archetypes_from_vector(vector<monster_archetype*>* v){
  //if the vector pointer is null, just quit
  if(!v)
    return;
  
  for(uint8_t i = 0; i < v->size(); i++){
    monster_archetype* temp = (*v)[i];
    //printf("%s\n",temp->name); //name

    //print_monster_archetype_description(temp); //description

    //remove the description
    temp->desc->clear();
    delete temp->desc;

    //printf("%c\n",temp->symb);

    //printf("%s\n",parse_color_into_string(temp->color));

    //printf("%d+%dd%d\n",temp->speed_die->base,temp->speed_die->num, temp->speed_die->faces); //speed die

    //print_monster_archetype_abilities(temp);
    
    //printf("%d+%dd%d\n",temp->hp_die->base,temp->hp_die->num, temp->hp_die->faces); //HP die

    //printf("%d+%dd%d\n",temp->damage_die->base,temp->damage_die->num, temp->damage_die->faces); //damage die
    
    //printf("\n");
    delete temp;
  }
  
  //delete the vector
  v->clear();
  delete v;
}

int which_flag(const char* in){
  int i;
  for(i = 0; i < MR_FLAGS_NUM; i++){
    if(!strcmp(monster_reader_flags[i],in)){
      //printf("%s matches\n",in);
      return i;
    }
    else{
      //printf("%s doesn't match\n",in);
    }
  }
  return -1;
}
  
int which_charachteristic_flag(const char* in){
  int i;
  for(i = 0; i < MR_CHARACHTERISTIC_FLAGS_NUM; i++){
    if(!strcmp(monster_reader_charachteristic_flags[i],in)){
      return i;
    }
  }
  return -1;
}

void handle_monster_modification(int flag, ifstream* f, monster_archetype* mobtype, const char* in){
  string curr = "";
  
  switch(flag){
  case 0: //name
    strcpy(mobtype->name,in);
    break;
  case 1: //description
    //create the desc vector
    mobtype->desc = new vector<string>();
    
    while(getline(*f, curr)){
      if(!f->eof() && curr.compare(".")){
	mobtype->desc->push_back(curr);
      }
      else{
	break;
      }
    }
    break;
  case 2: //symbol
    sscanf(in,"%c",&(mobtype->symb));
    break;
  case 3: //color
    mobtype->color = parse_string_into_color(in);
    break;
  case 4: //speed
    parse_string_into_dice(&mobtype->speed_die,in);
    break;
  case 5: //ability
    parse_string_into_abilities(in, mobtype);
    break;
  case 6: //HP
    parse_string_into_dice(&mobtype->hp_die,in);
    break;
  case 7: //damage
    parse_string_into_dice(&mobtype->damage_die,in);
    break;
  default:
    break;
  }
}

void print_monster_archetype_description(monster_archetype* mob){
    for(uint8_t j = 0; j < mob->desc->size(); j++){
      printf("%s\n",(*(mob->desc))[j].c_str());
    }
}

int parse_string_into_color(const char* in){
  char curr[DESCRIPTION_WIDTH + 1];
  sscanf(in,"%s",curr);

  if(!strcmp(curr, "BLACK"))
    return COLOR_BLACK;
  
  if(!strcmp(curr, "RED"))
    return COLOR_RED;
  
  if(!strcmp(curr, "GREEN"))
    return COLOR_GREEN;
  
  if(!strcmp(curr, "YELLOW"))
    return COLOR_YELLOW;
  
  if(!strcmp(curr, "BLUE"))
    return COLOR_BLUE;
  
  if(!strcmp(curr, "MAGENTA"))
    return COLOR_MAGENTA;
  
  if(!strcmp(curr, "CYAN"))
    return COLOR_CYAN;
  
  if(!strcmp(curr, "WHITE"))
    return COLOR_WHITE;

  return -1;
}

const char* parse_color_into_string(int i){
  switch(i){
  case COLOR_BLACK:
    return "BLACK";
    break;
  case COLOR_RED:
    return "RED";
    break;
  case COLOR_GREEN:
    return "GREEN";
    break;
  case COLOR_YELLOW:
    return "YELLOW";
    break;
  case COLOR_BLUE:
    return "BLUE";
    break;
  case COLOR_MAGENTA:
    return "MAGNETA";
    break;
  case COLOR_CYAN:
    return "CYAN";
    break;
  case COLOR_WHITE:
    return "WHITE";
    break;
  default:
    break;
  }

  return NULL;
}

void parse_string_into_abilities(const char* in, monster_archetype* mob){
  int num_tags;
  char ab1[DESCRIPTION_WIDTH];
  char ab2[DESCRIPTION_WIDTH];
  char ab3[DESCRIPTION_WIDTH];
  char ab4[DESCRIPTION_WIDTH];
  char ab5[DESCRIPTION_WIDTH];
  char* arr[5] = {ab1,ab2,ab3,ab4,ab5};
  
  num_tags = sscanf(in,"%s %s %s %s %s",ab1,ab2,ab3,ab4,ab5);

  for(int i = 0; i < num_tags; i++){
    parse_tag_into_ability(arr[i],mob);
  }
}

void parse_tag_into_ability(const char* in, monster_archetype* mob){
  int i = which_charachteristic_flag(in);

  if(i+1)
    mob->charachteristics |= (2<<i)/2;
}

void print_monster_archetype_abilities(monster_archetype* mob){
  for(int i = 0; i < MR_CHARACHTERISTIC_FLAGS_NUM; i++){
    if(mob->charachteristics & (2<<i)/2)
      printf("%s ",monster_reader_charachteristic_flags[i]);
  }

  printf("\n");
}

void parse_string_into_dice(dice** d,const char* s){
  //sscanf(s,"%d+%dd%d",&(d->base),&(d->num),&(d->faces));
  *d = dice::parseDice(s);
}

