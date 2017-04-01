#include "kotekEnc.h"
int KOTEKEnc(Byte **data,BITBUFOR *BB,int width,int height){
    Byte *data2=new Byte[width*height*2];
    zeroMemory(data2,width*height*2);
	deltaEnc(data,width,height);
	RLEEnc(data[0],data2,width*height,width*height*2);
	printf("\n");
	int poz,pozv;Byte symbol;//from RLE;
	memcpy(&poz,&data2[0],4);memcpy(&pozv,&data2[4],4);symbol=data2[8];
	for(int i=0;i<9;i++) BB->addByte(data2[i]);
	huffEnc(BB,data2+9,poz);
	huffEnc(BB,data2+poz+9,pozv);
	delete[]data2;
}
int KOTEKEnc(Image *img,char *fileNameOut){
	BITBUFOR bitBufor(img->width*img->height*3);
	KOTEKEnc(img->R,&bitBufor,img->width,img->height);
	KOTEKEnc(img->G,&bitBufor,img->width,img->height);
	KOTEKEnc(img->B,&bitBufor,img->width,img->height);
	FILE *fOut=fopen(fileNameOut,"wb");
    KOTEKHEADER kotekHeader(bitBufor.pos,img->width,img->height);
    kotekHeader.show();
    Byte *header=new Byte[kotekHeader.dataOffset];
    kotekHeader.generate(header);
    fwrite(header,1,kotekHeader.dataOffset,fOut);
    delete[]header;
    fwrite(bitBufor.data,1,bitBufor.pos+1,fOut);
    fclose(fOut);
}
int KOTEKEnc(char *fileNameIn,char *fileNameOut){
    Image *img=new Image(fileNameIn);
    KOTEKEnc(img,fileNameOut);
    img->~Image();
}
int KOTEKDec(Byte **data,BITBUFOR *BB,int width,int height){
    Byte *data2=new Byte[width*height*2];
    zeroMemory(data2,width*height*2);
    int poz,pozv;Byte symbol;//from RLE;
    for(int i=0;i<4;i++) *((Byte*)&poz+i)=BB->getByte();
    for(int i=0;i<4;i++) *((Byte*)&pozv+i)=BB->getByte();
    symbol=BB->getByte();
    memcpy(&data2[0],&poz,4);memcpy(&data2[4],&pozv,4);data2[8]=symbol;
    //for(int i=0;i<20;i++) printf("%4d",data2[i]);
	huffDec(data2+9,BB,poz);
	huffDec(data2+poz+9,BB,pozv);
	printf("\n");
    RLEDec(data2,data[0],poz+pozv+9,width*height*2);
	deltaDec(data,width,height);
	delete[]data2;
}
int KOTEKDec(char *fileNameIn,char *fileNameOut){
    Image *img=new Image;
    KOTEKDec(fileNameIn,img);
    img->SaveAsBMP(fileNameOut);
    img->~Image();
    printf("\n");
    return 0;
}
int KOTEKDec(char *fileNameIn,Image *img){
    printf("KOTEK decoder\n");
    int size=getFileSize(fileNameIn);
    Byte *bufor=new Byte[size];
    loadFile(fileNameIn,bufor,size);
    KOTEKHEADER kotekHeader(bufor);
    kotekHeader.show();
    img->allocate(kotekHeader.width,kotekHeader.height);
    img->info();
    BITBUFOR BB;
    BB.data=&bufor[kotekHeader.dataOffset];
    printf("HUFF decoder...\n");
    KOTEKDec(img->R,&BB,img->width,img->height);
    KOTEKDec(img->G,&BB,img->width,img->height);
    KOTEKDec(img->B,&BB,img->width,img->height);
    delete[]bufor;
    printf("\n");
    return 0;
}
