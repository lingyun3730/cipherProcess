#ifndef Macro_PaillierPublic_H
#define Macro_PaillierPublic_H 1

#include <stdio.h>
#include <NTL/ZZ.h>
using namespace NTL;

#pragma warning(disable: 4996)

#define MAX_RSAMODULE_BYTELEN 512

#define Paillier_ErrMsg_Size 256
#define Paillier_SetErrorMsg(msg) sprintf(msgError, "[%s::%d] %s\n", __FILE__, __LINE__, msg);

#define UseN2 0

class CPaillierPublic
{
public:
	CPaillierPublic(void);
	~CPaillierPublic(void);
public:
	char *GetLastError();
	int SetPubKey(ZZ& n);
	int GetPubKey(ZZ& n);
#if(UseN2 == 1)
	int EncryptII(ZZ& c, ZZ& m, ZZ& r);
	int EncryptII(ZZ& c, ZZ& m);
#endif//#if(UseN2 == 1)
	int Encrypt(ZZ& c0, ZZ&c1, ZZ& m, ZZ& r);
	int Encrypt(ZZ& c0, ZZ&c1, ZZ& m);
	int HomoAdd(ZZ& z0, ZZ& z1, ZZ& x0, ZZ& x1, ZZ& y0, ZZ& y1);
	int HomoNeg(ZZ& z0, ZZ& z1, ZZ& x0, ZZ& x1);
	int HomoScalar(ZZ& z0, ZZ& z1, ZZ& x0, ZZ& x1, ZZ& scalar);
	int HomoLeftShift(ZZ& z0, ZZ& z1, ZZ& x0, ZZ& x1, long shiftNum);

	int GetRandomZZ(ZZ& r);
	int GetRandomZZWithFixBits(ZZ& r, long bitNum);
	int GetInverse(ZZ& v, ZZ& m);
	int GetInverseOfFixPoint(ZZ& v, ZZ&m, long& bitPrec);
	int NormalZZ2PaillierZZ(ZZ& pz, ZZ& nz);
	int PaillierZZ2NormalZZ(ZZ& nz, ZZ& pz);
protected:
	int FastPowerModN2(ZZ& x0, ZZ& x1, const ZZ& a, const ZZ& e); //x0 + x1*zzN = a^e % zzN^2
protected:
	int boolPubKeyOK;
	char msgError[Paillier_ErrMsg_Size];
	ZZ zzN;
	ZZ zzHalfN;
#if(UseN2 == 1)
	ZZ zzN2;
#endif//#if(UseN2 == 1)
};

#endif//#ifndef Macro_PaillierPublic_H
