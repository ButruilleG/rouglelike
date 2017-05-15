#include "dungeonload.h"

int converterLoad(unsigned char toConvert);

void mapLoad(char *filename, Roomslist* rl){
  FILE *save = fopen(filename, "r");
  if(!save){
    fprintf(stderr, "Couldn't open file\n");
  }
  else{
    //loop indexes
    int i, j;
    int num;

    unsigned char c = 'r';
    unsigned char room_data[4];

    Room temp;

    //reads the semantic file-type marker
    for(i = 0; i < 6; i++){
      fread(&c, sizeof(c), 1, save);
      //printf("%c ",c);
      //printf("%d\n",c);
    }

    //reads the file version marker
    for(i = 6; i < 10; i++){
      fread(&c, sizeof(c), 1, save);
      //printf("%c ",c);
      //printf("%d\n",c);
    }

    //reads the size of the file
    for(i = 10; i < 14; i += 4){
      fread(&num, sizeof(num), 1, save);
      //printf("%d ",num);
      //printf("%d\n",num);
    }

    //reads the rest of the dungeon
    for(i = 0; i < DUNGEON_H; i++){
      for(j = 0; j < DUNGEON_W; j++){
	fread(&c, sizeof(c), 1, save);
	map[i][j] = converterLoad(c);
      }
    }

    //adds rooms to the dungeon
    int read = 1;
    while(read == 1){
      read = fread(&room_data, sizeof(room_data), 1, save);
      //printf("%d",read);
      temp.x = room_data[0];
      temp.w = room_data[1];
      temp.y = room_data[2];
      temp.h = room_data[3];

      rl->rooms[rl->count] = temp;
      rl->count++;
      room_paint(temp);
    }

    fclose(save);
  }
}

int converterLoad(unsigned char toConvert){
  switch(toConvert){
  case 0:
    return DUNGEON_CORRIDOR;
    break;
  default:
    return toConvert;
    break;
  }
}
