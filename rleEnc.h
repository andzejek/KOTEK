#ifndef RLEENC_H
#define RLEENC_H
#include "system.h"

int RLEEnc(Byte *in,Byte *out,int sizeIn,int sizeOut);
int RLEDec(Byte *in,Byte *out,int sizeIn,int sizeOut);
#endif // RLEENC_H
