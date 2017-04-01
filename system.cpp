#include "system.h"

///(de)allocations
void zeroMemory(void *data,int size){
    memset(data,0,size);
}
Byte** allocMatrix2D(int dim1, int dim2) {
    Byte** matrix2d = new Byte*[dim1];
    Byte*  dumm     = new Byte[dim1*dim2];
    for ( int i = 0; i < dim1; i++ ) matrix2d[i] = dumm + i*dim2;
    //ZeroMemory(dumm,dim1*dim2);
    return matrix2d;
}
void deleteMatrix2D(Byte**& matrix2d) {
    delete [] matrix2d[0];
    delete [] matrix2d;
    matrix2d = 0;
}
int** allocMatrix2DI(int dim1, int dim2) {
    int** matrix2d = new int*[dim1];
    int*  dumm     = new int[dim1*dim2];
    for ( int i = 0; i < dim1; i++ ) matrix2d[i] = dumm + i*dim2;
    //ZeroMemory(dumm,dim1*dim2);
    return matrix2d;
}
void deleteMatrix2DI(int**& matrix2d) {
    delete [] matrix2d[0];
    delete [] matrix2d;
    matrix2d = 0;
}
///file support
int getFileSize(char* fileName){
    long size=0;
    FILE *f = fopen(fileName, "rb");
    if (f){
        fseek(f, 0, SEEK_END);
        size = ftell(f);
        fclose(f);
    }
    return size;
}
int loadFile(char* fileName,Byte *bufor,int buforSize){
    if(bufor==0) return 0;
    long size=getFileSize(fileName);
    FILE *f = fopen(fileName, "rb");
    if(size>buforSize) {
            fread(&bufor[0],1,buforSize,f);
            return buforSize;
    }
    fread(&bufor[0],1,size,f);
    fclose(f);
    return size;
}
int saveDataToFile(Byte *data,int dataSize,char *fileName){
    FILE *f = fopen(fileName, "wb");
    fwrite(&data[0],1,dataSize,f);
    fclose(f);
}
int compareFile(char *fileName1,char* fileName2){
    int size1=getFileSize(fileName1),size2=getFileSize(fileName2);
    if(size1!=size2) return 1;
    Byte *bufor1=new Byte[size1];
    Byte *bufor2=new Byte[size2];
    loadFile(fileName1,bufor1,size1);
    loadFile(fileName2,bufor2,size2);
    for(int i=0;i<size1;i++)
    {
        if(bufor1[i]!=bufor2[i]) return 1;
    }
    return 0;
}
