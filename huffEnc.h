#ifndef HUFFENC_H_INCLUDED
#define HUFFENC_H_INCLUDED
#include "system.h"
#include "image.h"
#include "BITBUFOR.h"
//#include <vector>
//#include <algorithm>//freq int[256],codes Code[256]
struct Node{
    Node *left,*right;
    int freq;
    Byte byte;
    Node(Node *l,Node *r,Byte b)
    {
        Node(l,r,0,b);
    }
    Node()
    {
        right=NULL;
        left=NULL;
        freq=0;
        byte=0;
    }
    Node(Node *l,Node *r,int f,Byte b)
    {
        freq=f;
        byte=b;
        left=l;
        right=r;
    }
};
struct Code{
        int wartosc;
        char kod[256];
        int wystapienia;
        int dlugosc;
        int kot;
        Code()
        {
            wartosc=0;
            wystapienia=0;
            dlugosc=0;
            kot=0;
            zeroMemory(kod,256);
        }
    };
void getFreq(Byte *data,int size,int *freq);
void showFreq(int *freq);
int getSize(int *freq,Code *code);
Node* makeTree(int *freq);
void killTree(Node *node);
void showTree(Node *node);
void showCodes(Code *codes);
void mapCodes(Code *codes,Node *w);
int getSize(Byte *data,int size);
float mean(Byte *data,int datasize);

int huffEnc(BITBUFOR *BB,Byte *data,int size);
int huffDec(Byte *data,BITBUFOR *BB,int size);
#endif // HUFFENC_H_INCLUDED
