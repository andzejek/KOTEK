#include "kotekEnc.h"

int main(int argc,char *argv[]){
    printf("argc=%d\n",argc);
    for(int i=0;i<argc;i++) printf("argv[%d]=%s\n",i,argv[i]);
    if(argc>3){
    	if(!strcmp(argv[1],"-c")) KOTEKEnc(argv[2],argv[3]);
    	if(!strcmp(argv[1],"-d")) KOTEKDec(argv[2],argv[3]);
    }
    //KOTEKEnc("test2.bmp","tescior2.KOTEK");
    //KOTEKDec("tescior2.KOTEK","tesciorZDE2.bmp");
    //cmpBmpEx("test2.bmp","tesciorZDE2.bmp");
    //cmpBmp("test2.bmp","tesciorZDE2.bmp");
    return 0;
}
