#include "dungeonsave.h"

unsigned char converterSave(int toConvert);

void mapSave(char *filename, Roomslist* rl){
  FILE *save = fopen(filename, "w");

  if(!save){
    fprintf(stderr, "Couldn't open file\n");
  }
  else{
    //loop indexes
    int i, j;
    //int num;

    unsigned char c = '\0';
    //unsigned char room_data[4];

    //file type marker
    char marker[] = {'R','L','G','3','2','7'};
    //file version
    int version = 0;
    //file size
    int filesize = 0;
  
    //print the file-type marker
    fwrite(marker, sizeof(marker), 1, save);
    //print the file version
    fwrite(&version, sizeof(version), 1,save);
    //print the filesize (filler)
    //filesize = htobe32(filesize);
    fwrite(&filesize, sizeof(filesize), 1,save);

    filesize = 14;

    //print the hardness map of the dunegeon
    for(i = 0; i < DUNGEON_H; i++){
      for(j = 0; j < DUNGEON_W; j++){
	c = converterSave(map[i][j]);
	fwrite(&c, sizeof(c), 1, save);
      }
    }

    filesize = 1694;

    /*
    //decompose the dungeon's rooms into a set of 1x1 subrooms and print them
    for(i = 1; i < DUNGEON_H - 1; i++){
      for(j = 1; j < DUNGEON_W - 1; j++){
	if(map[i][j] == DUNGEON_ROOM){
	  c = j;
	  fwrite(&c, sizeof(c), 1, save);
	  c = 1;
	  fwrite(&c, sizeof(c), 1, save);
	  c = i;
	  fwrite(&c, sizeof(c), 1, save);
	  c = 1;
	  fwrite(&c, sizeof(c), 1, save);
	  filesize += 4;
	}
      }
    }
    */

    //saves rooms from the Roomslist
    for(i = 0; i < rl->count; i++){
      Room temp  = rl->rooms[i];
      c = temp.x;
      fwrite(&c, sizeof(c), 1, save);
      c = temp.w;
      fwrite(&c, sizeof(c), 1, save);
      c = temp.y;
      fwrite(&c, sizeof(c), 1, save);
      c = temp.h;
      fwrite(&c, sizeof(c), 1, save);
      filesize += 4;
    }
    //print the filesize (filler)
    fseek(save, 10, SEEK_SET);
    filesize = htobe32(filesize);
    fwrite(&filesize, sizeof(filesize), 1,save);

    fclose(save);
  }
}

unsigned char converterSave(int toConvert){
  if(toConvert <= 0)
    return 0;

  if(toConvert > 0 || toConvert < IMMUTABLE_ROCK)
    return (unsigned char) toConvert;
  
  return IMMUTABLE_ROCK;
}
