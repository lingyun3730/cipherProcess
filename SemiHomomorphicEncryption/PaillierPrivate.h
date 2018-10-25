#ifndef Macro_PaillierPrivate_H
#define Macro_PaillierPrivate_H 1

#include "PaillierPublic.h"

class CPaillierPrivate : public CPaillierPublic
{
public:
	CPaillierPrivate(void);
	~CPaillierPrivate(void);
public:
	int GenerateKeyFromPsw(unsigned char *pswKey, unsigned int pswKeyLen, unsigned int rsaModuleByteLen = 128);
	int GenerateKeyAfterPQ(ZZ& p, ZZ& q);
	int GetPriKey(ZZ& p, ZZ& q);
#if(UseN2 == 1)
	int DecryptII(ZZ& m, ZZ& c);
#endif//#if(UseN2 == 1)
	int Decrypt(ZZ& m, ZZ& c0, ZZ& c1);
protected:
	int boolPriKeyOK;
	ZZ zzP;
	ZZ zzQ;
	ZZ zzK;
	ZZ zzKInv;
};

#endif//#ifndef Macro_PaillierPrivate_H

