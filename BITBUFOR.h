#ifndef BITBUFOR_H
#define BITBUFOR_H
#include "system.h"
struct BITBUFOR{
    Byte *data;
    int size;
    Byte bitIndex;
    int pos;
    BITBUFOR(int _size)
    {
        size=_size;
        data=0;
        bitIndex=0;
        pos=0;
        data=new Byte[_size];
        memset(data,0,_size);
    }
    BITBUFOR()
    {
        size=0;
        data=0;
        pos=0;
        bitIndex=0;
    }
    inline void addBit(Byte bit){
        if(bit)
        {
            data[pos]|=(1<<(7-bitIndex));///7-bitIndex lub bitIndex!!!
        }
        bitIndex++;
        if(bitIndex==8){pos++;bitIndex=0;}
    }
    inline void addByte(Byte byte){
        for(int i=0;i<8;i++)
        {
            Byte x=(byte & (1 << i)) != 0;
            addBit(x);
        }

    }
    inline Byte getBit(){
        Byte bit=(data[pos] & (1 << (7-bitIndex))) != 0;///7-bitIndex lub bitIndex!!!
        bitIndex++;
        if(bitIndex==8){pos++;bitIndex=0;}
        return bit;
    }
    inline Byte getByte(){
        Byte x=0;
        for(int i=0;i<8;i++)
        {
            Byte b=getBit();
            if(b)
            {
            x|=(1<<i);
            }

        }
        return x;
    }
    /*Byte getNBytes(Byte n)
    {
        short s;
        memcpy(&s,&data[pos],2);
        s=s>>(16-bitIndex-n);
        short t=-128;
        t=t>>(16-n);
        return s&t;
        bitIndex+=n;
        if(bitIndex>=8) bitIndex-=8;
    }*/

    ~BITBUFOR()
    {
        delete[]data;
    }
    void show(int _length)
    {
        for(int i=0;i<_length;i++)
        {
            printf("%4d",data[i]);
        }
    }
    void clear()
    {
        memset(data,0,size);
    }
};
#endif // BITBUFOR_H
