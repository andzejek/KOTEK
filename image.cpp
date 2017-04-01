#include "image.h"
//#include <windows.h>
void Image::clean(){
    memset(&R[0][0],0,width*height);
    memset(&G[0][0],0,width*height);
    memset(&B[0][0],0,width*height);
}
void Image::info(){
    printf("Image:%s\n",name);
    printf("wymiary:%5d X %5d\n",width,height);
}
void Image::setName(char *_name){
    sprintf(name,"%s",_name);
}
Image::Image(int _width,int _height){
    R=NULL  ;G=NULL;    B=NULL;
    int width=0;int height=0;
    name=new char[256];
    sprintf(name,"BLANK");
    allocate(_width,_height);
}
Image::Image(){
    R=NULL; G=NULL; B=NULL;
    int width=0;int height=0;
    name=new char[256];
    sprintf(name,"BLANK");
}
Image::~Image(){
    if(R!=0)deleteMatrix2D(R);
    if(G!=0)deleteMatrix2D(G);
    if(B!=0)deleteMatrix2D(B);
    if(name!=0) delete[]name;
}
Image::allocate(int _width,int _height){
    width=_width;
    height=_height;
    if(R==0) R=allocMatrix2D(_width,_height);
    else
    {
        deleteMatrix2D(R);
        R=allocMatrix2D(_width,_height);
    }
    if(G==0) G=allocMatrix2D(_width,_height);
    else
    {
        deleteMatrix2D(G);
        G=allocMatrix2D(_width,_height);
    }
    if(B==0) B=allocMatrix2D(_width,_height);
    else
    {
        deleteMatrix2D(B);
        B=allocMatrix2D(_width,_height);
    }
}
Image::Image(char *fileName){
    memset((this),0,sizeof(Image));
    int size=getFileSize(fileName);
    if(size==0) {

            printf("podany plik nie istnieje!");
            return;
    }
    //printf("%d",size);
    Byte *BUFOR=new Byte[size];
    loadFile(fileName,BUFOR,size);
    if(BUFOR[0]=='B'&&BUFOR[1]=='M')
    {
    BMPFILEHEADER FileInfo;
    BMPINFOHEADER PictureInfo;
    memcpy(&FileInfo.type, &BUFOR[0], 2);
    memcpy(&FileInfo.fileSize, &BUFOR[2], 4);
    memcpy(&FileInfo.reserved0, &BUFOR[6], 2);
    memcpy(&FileInfo.reserved1, &BUFOR[8], 2);
    memcpy(&FileInfo.dataOffset, &BUFOR[10], 4);
    memcpy(&PictureInfo.hdrSize,&BUFOR[14],4);
    memcpy(&PictureInfo.width,&BUFOR[18],4);
    memcpy(&PictureInfo.height,&BUFOR[22],4);
    memcpy(&PictureInfo.planes,&BUFOR[26],2);
    memcpy(&PictureInfo.depth,&BUFOR[28],2);
    memcpy(&PictureInfo.compression,&BUFOR[30],4);
    memcpy(&PictureInfo.bmpDataSize,&BUFOR[34],4);
    memcpy(&PictureInfo.hResolution,&BUFOR[38],4);
    memcpy(&PictureInfo.vResolution,&BUFOR[42],4);
    memcpy(&PictureInfo.nColors,&BUFOR[46],4);
    memcpy(&PictureInfo.nImportantColors,&BUFOR[50],4);
    if(PictureInfo.depth!=24){
        printf("Nie obslugiwana glebia kolorow (tylko 24bit)\n");
        delete[]BUFOR;
        return ;
    }
    width=PictureInfo.width;
    height=PictureInfo.height;
    int rowSize = PictureInfo.width * 3;
    size_t padding = rowSize % 4;
    if(padding != 0) padding = 4 - padding;
    rowSize += padding;
    allocate(width,height);
    Byte *temp=BUFOR+54;
    for (int iy = 0; iy <height; ++iy)
    {
        for(int ix = 0; ix < width; ++ix)
        {
            R[ix][iy]=temp[ix*3+2];
            G[ix][iy]=temp[ix*3+1];
            B[ix][iy]=temp[ix*3];

        }
        temp+=rowSize;
    }
    Byte temp2[256];
    memset(temp2,0,256);
    for(int i=0;i<strlen(fileName);i++) {
            if(fileName[i]=='.') {name=new char[i+1];break;}
            else temp2[i]=fileName[i];
    }
    sprintf(name,"%s",temp2);
    }
    else if(BUFOR[0]=='P'&&BUFOR[1]=='6')
    {
        sscanf((const char*)&BUFOR[2],"%4d",&width);
        sscanf((const char*)&BUFOR[7],"%4d",&height);
        printf("szerokosc=%d",width);
        printf("wysokosc=%d",height);
        allocate(width,height);
        Byte *temp=&BUFOR[14];
        for (int iy = height-1; iy >=0; iy--)
        {
            for(int ix = 0; ix < width; ++ix)
            {
                R[ix][iy]=temp[ix*3];
                G[ix][iy]=temp[ix*3+1];
                B[ix][iy]=temp[ix*3+2];

            }
            temp+=width*3;
        }
    }
    else return;
    delete[]BUFOR;
}
/*void Image::showChannel(char c,bool scale){
    BYTE **T;
    if(c=='R') T=R;
    if(c=='G') T=G;
    if(c=='B') T=B;
    HDC hdc=GetDC(0);
    if(scale==false){
        for(int ix=0;ix<width;ix++)
        for(int iy=0;iy<height;iy++){
            SetPixel(hdc,ix,height-iy-1,RGB(T[ix][iy],T[ix][iy],T[ix][iy]));
        }
    }
    else
    {
        float scaleX,scaleY;
        int screenX=GetSystemMetrics(SM_CXSCREEN);
        int screenY=GetSystemMetrics(SM_CYSCREEN);
        scaleX=(float)width/screenX;
        scaleY=(float)height/screenY;
        for(int ix=0;ix<screenX;ix++)
        for(int iy=0;iy<screenY;iy++){
            SetPixel(hdc,ix,screenY-iy-1,RGB(T[(int)(ix*scaleX)][(int)(iy*scaleY)],T[(int)(ix*scaleX)][(int)(iy*scaleY)],T[(int)(ix*scaleX)][(int)(iy*scaleY)]));
        }
    }
    ReleaseDC(0,hdc);
}
void Image::showChannelCustom(char c,bool scale){
    BYTE **T;
    int screenX=GetSystemMetrics(SM_CXSCREEN);
    int screenY=GetSystemMetrics(SM_CYSCREEN);
    if(c=='R') T=R;
    if(c=='G') T=G;
    if(c=='B') T=B;
    HDC hdc=GetDC(0);
    if(scale==false){
            for(int i=0;i<40;i++)
            {
        for(int ix=0;ix<width;ix++)
        for(int iy=0;iy<height;iy++){
                if(T[ix][iy]==(i)) SetPixel(hdc,ix,screenY-iy-1,RGB(0,255,0));
                else SetPixel(hdc,ix,screenY-iy-1,RGB(0,0,0));
        }
            while(!GetAsyncKeyState('A'));
            }
    }
    else
    {
        float scaleX,scaleY;

        scaleX=(float)width/screenX;
        scaleY=(float)height/screenY;
        for(int ix=0;ix<screenX;ix++)
        for(int iy=0;iy<screenY;iy++){
            SetPixel(hdc,ix,screenY-iy-1,RGB(T[(int)(ix*scaleX)][(int)(iy*scaleY)],T[(int)(ix*scaleX)][(int)(iy*scaleY)],T[(int)(ix*scaleX)][(int)(iy*scaleY)]));
        }
    }
    ReleaseDC(0,hdc);
}*/
Image::SaveAsBMP(char* fileName){
    FILE *fOut=fopen(fileName,"wb");
    BMPFILEHEADER FileInfo;
    memset(&FileInfo,0,sizeof(BMPFILEHEADER));
    FileInfo.type           =19778;
    FileInfo.fileSize       =rowSize(width)*height+54;
    FileInfo.reserved0      =0;
    FileInfo.reserved1      =0;
    FileInfo.dataOffset     =54;
    fwrite(&FileInfo.type,1,2,fOut);
    fwrite(&FileInfo.fileSize,1,4,fOut);
    fwrite(&FileInfo.reserved0,1,2,fOut);
    fwrite(&FileInfo.reserved1,1,2,fOut);
    fwrite(&FileInfo.dataOffset,1,4,fOut);
    BMPINFOHEADER PictureInfo;
    memset(&PictureInfo,0,sizeof(BMPINFOHEADER));
    PictureInfo.hdrSize         =40;
    PictureInfo.width           =width;
    PictureInfo.height          =height;
    PictureInfo.planes          =1;
    PictureInfo.depth           =24;
    PictureInfo.compression     =0;
    PictureInfo.bmpDataSize     =rowSize(width)*height;
    PictureInfo.hResolution     =3780;
    PictureInfo.vResolution     =3780;
    PictureInfo.nColors         =0;
    PictureInfo.nImportantColors=0;
    fwrite(&PictureInfo.hdrSize,1,4,fOut);
    fwrite(&PictureInfo.width,1,4,fOut);
    fwrite(&PictureInfo.height,1,4,fOut);
    fwrite(&PictureInfo.planes,1,2,fOut);
    fwrite(&PictureInfo.depth,1,2,fOut);
    fwrite(&PictureInfo.compression,1,4,fOut);
    fwrite(&PictureInfo.bmpDataSize,1,4,fOut);
    fwrite(&PictureInfo.hResolution,1,4,fOut);
    fwrite(&PictureInfo.vResolution,1,4,fOut);
    fwrite(&PictureInfo.nColors,1,4,fOut);
    fwrite(&PictureInfo.nImportantColors,1,4,fOut);
    Byte *bufor=new Byte[rowSize(width)*height];
    Byte *t=bufor;
    for (int iy = 0; iy <height; iy++)
    {
        for(int ix = 0; ix < width; ix++)
        {
            t[ix*3+2]=R[ix][iy];
            t[ix*3+1]=G[ix][iy];
            t[ix*3]=B[ix][iy];

        }
        t+=rowSize(width);
    }
    fwrite(bufor,1,rowSize(width)*height,fOut);
    fclose(fOut);
    printf("Zapisano plik:%s\n",fileName);
    delete []bufor;
}
int padding(int _width){
    int rowSize = _width * 3;
    size_t padding = rowSize % 4;
    return padding;
}
int rowSize(int _width){
    int rowSize = _width * 3;
    size_t padding = rowSize % 4;
    if(padding != 0) padding = 4 - padding;
    return rowSize + padding;
}
void BMPFILEHEADER::show(){
        printf("BMPFILEHEADER:\n");
        printf("type=               %d\n",type);
        printf("fileSize=           %d\n",fileSize);
        printf("reserved0=          %d\n",reserved0);
        printf("reserved1=          %d\n",reserved1);
        printf("dataOffset=         %d\n",dataOffset);
}
void BMPINFOHEADER::show(){
        printf("BMPINFOHEADER:\n");
        printf("hdrSize=            %10d\n",hdrSize);
        printf("width=              %10d\n",width);
        printf("height=             %10d\n",height);
        printf("planes=             %10d\n",planes);
        printf("depth=              %10d\n",depth);
        printf("compression=        %10d\n",compression);
        printf("bmpDataSize=        %10d\n",bmpDataSize);
        printf("hResolution=        %10d\n",hResolution);
        printf("vResolution=        %10d\n",vResolution);
        printf("nColors=            %10d\n",nColors);
        printf("nImportantColors=   %10d\n",nImportantColors);
}
