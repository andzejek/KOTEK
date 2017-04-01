#ifndef KOTEK_H_INCLUDED
#define KOTEK_H_INCLUDED
#include "system.h"
#include "BITBUFOR.h"
#include "image.h"
#include "delta.h"
#include "rleEnc.h"
#include "huffEnc.h"
int KOTEKEnc(Image *img,char *_fileNameOut);
int KOTEKEnc(char *_fileNameIn,char *_fileNameOut);
int KOTEKEnc(Byte **data,BITBUFOR *BB,int width,int height);

int KOTEKDec(char *fileNameIn,Image *img);
int KOTEKDec(char *_fileNameIn,char *_fileNameOut);
int KOTEKDec(Byte **data,BITBUFOR *BB,int width,int height);
typedef struct KOTEKHEADER{
    char type[5];
    int fileSize;
    Byte version;
    int width;
    int height;
    int dataOffset;
    KOTEKHEADER(int _fileSize,int _width,int _height){
        type[0]='K';type[1]='O';type[2]='T';type[3]='E';type[4]='K';
        version=1;
        fileSize=_fileSize;
        width=_width;
        height=_height;
        dataOffset=22;
    }
    KOTEKHEADER(Byte *data){
        memcpy(type,&data[0],5);
        memcpy(&fileSize,&data[5],4);
        memcpy(&version,&data[9],1);
        memcpy(&width,&data[10],4);
        memcpy(&height,&data[14],4);
        memcpy(&dataOffset,&data[18],4);
    }
    void show(){
        printf("KOTEKHEADER:\n");
        printf("type=       "); for(int i=0;i<5;i++) printf("%c",type[i]);printf("\n");
        printf("fileSize=   %10d\n",fileSize+dataOffset);
        printf("version=    %10d\n",version);
        printf("width=      %10d\n",width);
        printf("height=     %10d\n",height);
        printf("dataOffset= %10d\n",dataOffset);
    }
    void generate(Byte* bufor){
        memcpy(&bufor[0],type,5);
        memcpy(&bufor[5],&fileSize,4);
        memcpy(&bufor[9],&version,1);
        memcpy(&bufor[10],&width,4);
        memcpy(&bufor[14],&height,4);
        memcpy(&bufor[18],&dataOffset,4);
    }
} KOTEKHEADER;
#endif // KOTEK_H_INCLUDED
