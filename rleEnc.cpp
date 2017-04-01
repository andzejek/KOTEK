#include "rleEnc.h"
#include "huffEnc.h"
int getFrequencesFromData(int *frequence,Byte *data,int dataSize){
    memset(&frequence[0],0,256*4);
    for(int i=0;i<dataSize;i++) frequence[data[i]]++;
}
void showFrequences(int *frequence){
    printf("freq=%10d %10d",frequence[0],frequence[255]);
}
int RLEGetCompressedSize(Byte *data){//ByFirstBytes
    int poz,pozv;
    memcpy(&poz,&data[0],4);
    memcpy(&pozv,&data[4],4);
    return poz+pozv+9;
}
int RLEGetApproximateDecompressedSize(Byte *data){
    int poz,pozv;
    memcpy(&poz,&data[0],4);
    memcpy(&pozv,&data[4],4);
    Byte *rlev=&data[9+poz];
    int size=0;
    for(int i=0;i<pozv;i++) size+=(rlev[i]-1);
    size+=(poz-pozv);
    return size;
}
void RLEError(int err){
    if(err>0){printf("is OK\n");return;}
    printf("ERROR:");
    switch(err)
    {
    case -1: printf("(FATAL)NULL_PTR!:in\n");break;
    case -2: printf("(FATAL)NULL_PTR!:out\n");break;
    case -3: printf("sizeOut<(sizeIn*257/256+9)\n");break;
    case -4: printf("(FATAL)compressed size>sizeOut!\n");break;
    case -10: printf("(FATAL)NULL_PTR!:in\n");break;
    case -11: printf("(FATAL)NULL_PTR!:out\n");break;
    case -12: printf("(FATAL)sizeIn is to small?!\n");break;
    case -13: printf("(FATAL)sizeOut is to small?!\n");break;
        default:printf("Undefined ERROR\n");
    }
}
int RLEEnc(Byte *in,Byte *out,int sizeIn,int sizeOut){
    if(in==0) return -1;
    if(out==0) return -2;
    if(sizeOut<(ceil(sizeIn*257.f/256)+9)) return -3;

    ///huffman
    int freq[256];
    Code codes[256];
    getFreq(in,sizeIn,freq);
    Node *tree=makeTree(freq);
    mapCodes(codes,tree);
    //showCodes(codes);
    //showFreq(freq);
    killTree(tree);
    ///huffman end
    ///entropia
    float entropia[256];
    for(int i=0;i<256;i++)
    {
        if(freq[i]!=0)
        {
        double p=(float)freq[i]/sizeIn;
        entropia[i]=-log2(p);
        //printf("liczba bitow na symbol[%3d]=%f\n",i,-log2(p));
        }
        else
        entropia[i]=0;
    }
    //for(int i=0;i<256;i++) entropia[i]*=(44-entropia[i])/22;
    ///entropia end
    #define ESCAPE ((counter-1)*codes[znak].dlugosc<11)
    //#define ESCAPE ((counter-1)*entropia[znak]<9)
    //#define ESCAPE (counter<4)
    int frequences[256];
    getFrequencesFromData(frequences,in,sizeIn);





    int symbol;
    int min=frequences[0];
    for(int i=1;i<256;i++) if(min>frequences[i]){ min=frequences[i];symbol=i;}

    Byte* rle=out+9;//9 piwerwsze bajty to rozmiar tablicyznakow i tablicy wystapien
    Byte* rlev=new Byte[int(ceil(sizeIn*257.f/512))];
    int znak=in[0];
    int poz=0;
    int pozv=0;
    int counter=1;
    for(int i=1;i<sizeIn;i++)
        {
        if(znak==in[i]){
            counter++;
        }else{
            if(counter>1){

                if(ESCAPE) {
                    if(znak!=symbol) for(int i=0;i<counter;i++) rle[poz++]=znak;
                    else
                    {
                        while(counter>255){rle[poz++]=symbol;rlev[pozv++]=255;counter-=255;rle[poz++]=znak;}
                        rle[poz++]=symbol;rlev[pozv++]=counter;rle[poz++]=znak;
                    }
                }else{
                while(counter>255){rle[poz++]=symbol;rlev[pozv++]=255;counter-=255;rle[poz++]=znak;}
                rle[poz++]=symbol;rlev[pozv++]=counter;rle[poz++]=znak;
                }
            }else{
                if(znak!=symbol) rle[poz++]=znak;
                else{
                        rle[poz++]=symbol;rlev[pozv++]=counter;rle[poz++]=znak;
                }
            }
            znak=in[i];
            counter=1;
        }
    }
    if(counter>1){
                if(ESCAPE) {
                    if(znak!=symbol) for(int i=0;i<counter;i++) rle[poz++]=znak;
                    else
                    {
                        while(counter>255){rle[poz++]=symbol;rlev[pozv++]=255;counter-=255;rle[poz++]=znak;}
                        rle[poz++]=symbol;rlev[pozv++]=counter;rle[poz++]=znak;
                    }
                }else{
                    while(counter>255){rle[poz++]=symbol;rlev[pozv++]=255;counter-=255;rle[poz++]=znak;}
                    rle[poz++]=symbol;rlev[pozv++]=counter;rle[poz++]=znak;
                }
            }else{
                if(znak!=symbol) rle[poz++]=znak;
                else{
                    rle[poz++]=symbol;rlev[pozv++]=counter;rle[poz++]=znak;
                }
            }
    int size=9+poz+pozv;
    if(size>sizeOut) return -4;
    memcpy(&out[0],&poz,4);
    memcpy(&out[4],&pozv,4);
    memcpy(&out[8],&symbol,1);
    memcpy(&out[9+poz],rlev,pozv);
    return size;
}
int RLEDec(Byte *in,Byte *out,int sizeIn,int sizeOut){
    if(in==0) return -10;
    if(out==0) return -11;
    int poz,pozv;
    Byte symbol;
    memcpy(&poz,&in[0],4);
    memcpy(&pozv,&in[4],4);
    memcpy(&symbol,&in[8],1);
    Byte *rle=&in[9];
    Byte *rlev=&in[9+poz];
    if(sizeIn<RLEGetCompressedSize(in)) return -12;
    int size=RLEGetApproximateDecompressedSize(in);
    if(size>sizeOut) return -13;

    for(int i=0,i2=0,i3=0;i<poz;){
        if(rle[i]==symbol)
        {
                for(int k=0;k<(rlev[i2]);k++){
                    *(&out[0]+i3++)=rle[i+1];
                }
                i+=2;
                i2++;
        }else{
            *(&out[0]+i3++)=rle[i++];
        }
    }
    return size;
}
