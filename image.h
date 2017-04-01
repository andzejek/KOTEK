#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED
#include "system.h"
typedef struct BMPFILEHEADER {
        short type;
        int fileSize;
        short reserved0;
        short reserved1;
        int dataOffset;
        void show();//moje
    } BMPFILEHEADER;
typedef struct BMPINFOHEADER {
        int hdrSize;
        int width;
        int height;
        short planes;
        short depth;
        int compression;
        int bmpDataSize;
        int hResolution;
        int vResolution;
        int nColors;
        int nImportantColors;
        void show();//moje
    } BMPINFOHEADER;
struct Image{//moze byc klasa ktora juz mam xd
    Byte **R,**G,**B;
    int width;
    int height;
    char *name;
    Image(char *fileName);//do kodera
    Image(int _width,int _height);//do dekodera alokacja
    Image();
    int allocate(int _width,int _height);
    void showChannel(char c,bool scale);
    void showChannelCustom(char c,bool scale);
    int SaveAsBMP(char* _fileName);
    void setName(char *_name);
    void info();
    void clean();
    ~Image();
};
int rowSize(int _width);
int padding(int _width);
#endif // IMAGE_H_INCLUDED
