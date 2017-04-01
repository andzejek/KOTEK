#include "delta.h"
int deltaEnc(Byte **data,int width,int height){
    Byte **in=allocMatrix2D(width,height);
    memcpy(&in[0][0],&data[0][0],width*height);
    Byte **out=data;
    out[0][0]=in[0][0];
    for(int j=0;j<height-1;j++) out[0][j+1]=in[0][j]-in[0][j+1];
    for(int i=1;i<width-1;i++)
        for(int j=0;j<height;j++){
                out[i+1][j]=in[i][j]-in[i+1][j];
        }
    deleteMatrix2D(in);
    return 0;
}
int deltaDec(Byte **data,int width,int height){
    Byte **in=allocMatrix2D(width,height);
    memcpy(&in[0][0],&data[0][0],width*height);
    Byte **out=data;
    out[0][0]=in[0][0];
    for(int j=0;j<height-1;j++) out[0][j+1]=out[0][j]-in[0][j+1];
    for(int i=1;i<width-1;i++)
        for(int j=0;j<height;j++){
                out[i+1][j]=out[i][j]-in[i+1][j];
        }
    deleteMatrix2D(in);
    return 0;
}
