#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED
#include <stdio.h>
#include <string.h>
#include <math.h>
#define Byte unsigned char
void zeroMemory(void *data,int size);
///(de)allocations
Byte** allocMatrix2D(int dim1, int dim2);
void deleteMatrix2D(Byte**& matrix2d);
int** allocMatrix2DI(int dim1, int dim2);
void deleteMatrix2DI(int**& matrix2d);
///file support
int getFileSize(char* fileName);
int loadFile(char* fileName,Byte *bufor,int buforSize);
int saveDataToFile(Byte *data,int dataSize,char *fileName);
int compareFile(char *fileName1,char* fileName2);
#endif // SYSTEM_H_INCLUDED
