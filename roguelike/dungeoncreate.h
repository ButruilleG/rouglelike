#ifndef DUNGEONCREATE_H
#define DUNGEONCREATE_H

//generate hardness map
void mapGenerate();
//smooth hardness map
void mapSmoothLR();
void mapSmoothTB();
//sets the border cells immutable
void setImmutable();

#endif
