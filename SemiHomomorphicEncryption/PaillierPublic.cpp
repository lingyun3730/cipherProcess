#include "PaillierPublic.h"

CPaillierPublic::CPaillierPublic(void)
{
	boolPubKeyOK = 0;
	msgError[0] = 0;
}

CPaillierPublic::~CPaillierPublic(void)
{
}

char* CPaillierPublic::GetLastError()
{
	return msgError;
}

int CPaillierPublic::SetPubKey(ZZ& n)
{
	zzN = n;
	div(zzHalfN, zzN, 2);
#if(UseN2 == 1)
	mul(zzN2, zzN, zzN);
#endif//#if(UseN2 == 1)
	boolPubKeyOK = 1;

	return 1;
}

int CPaillierPublic::GetPubKey(ZZ& n)
{
	if(!boolPubKeyOK)
	{
		Paillier_SetErrorMsg("CPaillierPublic: use GetPubKey before PubKey setup");
		return 0;
	}

	n = zzN;
	return 1;
}

#define CheckGCDrN 0
int CPaillierPublic::Encrypt(ZZ& c0, ZZ&c1, ZZ& m, ZZ& r)
{
	ZZ zzTmp;

	if(!boolPubKeyOK)
	{
		Paillier_SetErrorMsg("CPaillierPublic: use Encrypt2 before PubKey setup");
		return 0;
	}

	if((m < 0) || (m >= zzN))
	{
		Paillier_SetErrorMsg("CPaillierPublic: encrypt with invalid message");
		return 0;
	}

	if((r < 0) || (r >= zzN))
	{
		Paillier_SetErrorMsg("CPaillierPublic: encrypt with invalid random");
		return 0;
	}

#if(CheckGCDrN==1)
	GCD(zzTmp, r, zzN);
	if(zzTmp != 1)
	{
		zzTmp = r;
		do{
			zzTmp += 1;
		}while(GCD(zzTmp, zzN) != 1);
		FastPowerModN2(c0, c1, zzTmp, zzN);
	}
	else
	{
		FastPowerModN2(c0, c1, r, zzN);
	}
#else
	FastPowerModN2(c0, c1, r, zzN);
#endif

	mul(zzTmp, c0, m);
	zzTmp += c1;
	rem(c1, zzTmp, zzN);

	return 1;
}

#if(UseN2 == 1)
int CPaillierPublic::EncryptII(ZZ& c, ZZ& m, ZZ& r)
{
	ZZ zzTmp;

	if(!boolPubKeyOK)
	{
		Paillier_SetErrorMsg("CPaillierPublic: use Encrypt before PubKey setup");
		return 0;
	}

	if((m < 0) || (m >= zzN))
	{
		Paillier_SetErrorMsg("CPaillierPublic: encrypt with invalid message");
		return 0;
	}

	if((r < 0) || (r >= zzN))
	{
		Paillier_SetErrorMsg("CPaillierPublic: encrypt with invalid random");
		return 0;
	}

#if(CheckGCDrN==1)
	GCD(zzTmp, r, zzN);
	if(zzTmp != 1)
	{
		zzTmp = r;
		do{
			zzTmp += 1;
		}while(GCD(zzTmp, zzN) != 1);
		zzTmp = PowerMod(zzTmp, zzN, zzN2);
	}
	else
	{
		PowerMod(zzTmp, r, zzN, zzN2);
	}
#else
	PowerMod(zzTmp, r, zzN, zzN2);
#endif
	
	mul(c, m, zzN);
	c += 1;
	MulMod(c, c, zzTmp, zzN2);

	return 1;
}
#endif//#if(UseN2 == 1)

#if(UseN2 == 1)
int CPaillierPublic::EncryptII(ZZ& c, ZZ& m)
{
	ZZ r;

	if(!boolPubKeyOK)
	{
		Paillier_SetErrorMsg("CPaillierPublic: use Encrypt before PubKey setup");
		return 0;
	}

	RandomBnd(r, zzN);

	return EncryptII(c, m, r);
}
#endif//#if(UseN2 == 1)

int CPaillierPublic::Encrypt(ZZ& c0, ZZ&c1, ZZ& m)
{
	ZZ r;

	if(!boolPubKeyOK)
	{
		Paillier_SetErrorMsg("CPaillierPublic: use Encrypt2 before PubKey setup");
		return 0;
	}

	RandomBnd(r, zzN);

	return Encrypt(c0, c1, m, r);
}

int CPaillierPublic::HomoAdd(ZZ& z0, ZZ& z1, ZZ& x0, ZZ& x1, ZZ& y0, ZZ& y1)
{
	ZZ zzTmp1;

	if(!boolPubKeyOK)
	{
		Paillier_SetErrorMsg("CPaillierPublic: use HomoAdd before PubKey setup");
		return 0;
	}

	MulMod(zzTmp1, x1, y0, zzN);
	MulMod(z1, x0, y1, zzN);
	z1 += zzTmp1; if(z1 >= zzN) z1 -= zzN;
	mul(z0, x0, y0);
	DivRem(zzTmp1, z0, z0, zzN);
	z1 += zzTmp1; if(z1 >= zzN) z1 -= zzN;

	return 1;
}

int CPaillierPublic::HomoNeg(ZZ& z0, ZZ& z1, ZZ& x0, ZZ& x1)
{
	ZZ d, zzTmp1;

	if(!boolPubKeyOK)
	{
		Paillier_SetErrorMsg("CPaillierPublic: use HomoAdd before PubKey setup");
		return 0;
	}

	XGCD(d, zzTmp1, z0, zzN, x0);

	if(d != 1)
	{
		Paillier_SetErrorMsg("CPaillierPublic: HomoInv, the input has no inverse");
		return 0;
	}

	if(z0 < 0)
	{
		z0 += zzN;
		zzTmp1 = x0 - zzTmp1;
	}
	else
	{
		zzTmp1 = -zzTmp1;
	}
	MulMod(d, z0, x1, zzN);
	d += zzTmp1; if(d >= zzN) d -= zzN;
	if(d == 0)
	{
		z1=0;
	}
	else
	{
		sub(zzTmp1, zzN, d);
		MulMod(z1, zzTmp1, z0, zzN);
	}

	return 1;
}

int CPaillierPublic::HomoScalar(ZZ& z0, ZZ& z1, ZZ& x0, ZZ& x1, ZZ& scalar)
{
	ZZ zzX0, zzX1, zzY1;

	if(!boolPubKeyOK)
	{
		Paillier_SetErrorMsg("CPaillierPublic: use HomoScalar before PubKey setup");
		return 0;
	}

	if(scalar == 0)
	{
		z0 = 1;
		z1 = 0;
		return 1;
	}

	sub(zzY1, scalar, 1);
	FastPowerModN2(zzX0, zzX1, x0, zzY1);
	MulMod(zzY1, scalar, x1, zzN);
	MulMod(z1, x0, zzX1, zzN);
	MulMod(zzX1, zzX0, zzY1, zzN);
	z1 += zzX1; if(z1 >= zzN) z1 -= zzN;
	mul(zzY1, zzX0, x0);
	DivRem(zzX0, z0, zzY1, zzN);
	z1 += zzX0; if(z1 >= zzN) z1 -= zzN;

	return 1;
}

int CPaillierPublic::HomoLeftShift(ZZ& z0, ZZ& z1, ZZ& x0, ZZ& x1, long shiftNum)
{
	ZZ zzTmp, zzTmp1;

	if(!boolPubKeyOK)
	{
		Paillier_SetErrorMsg("CPaillierPublic: use FastPowerMod2 before PubKey setup");
		return 0;
	}

	z0 = x0; z1 = x1;
	while((--shiftNum)>= 0)
	{
		mul(z1, z1, z0);
		rem(z1, z1, zzN);
		z1 += z1; if(z1 >= zzN) z1 -= zzN;
		sqr(zzTmp, z0);
		DivRem(zzTmp1, z0, zzTmp, zzN);
		z1 += zzTmp1; if(z1 >= zzN) z1 -= zzN;
	}

	return 1;
}

int CPaillierPublic::GetRandomZZ(ZZ& r)
{
	if(!boolPubKeyOK)
	{
		Paillier_SetErrorMsg("CPaillierPublic: use GetRandomZZ before PubKey setup");
		return 0;
	}

	RandomBnd(r, zzN);
	return 1;
}

int CPaillierPublic::GetRandomZZWithFixBits(ZZ& r, long bitNum)
{
	if(!boolPubKeyOK)
	{
		Paillier_SetErrorMsg("CPaillierPublic: use GetRandomZZWithFixBits before PubKey setup");
		return 0;
	}

	RandomBits(r, bitNum);

	if(RandomBnd(2))
	{
		r = zzN - r;
		if(r >= zzN) r -= zzN;
	}

	return 1;
}

int CPaillierPublic::GetInverse(ZZ& v, ZZ& m)
{
	if(!boolPubKeyOK)
	{
		Paillier_SetErrorMsg("CPaillierPublic: use GetInverse before PubKey setup");
		return 0;
	}

	if(InvModStatus(v, m%zzN, zzN))
	{
		Paillier_SetErrorMsg("CPaillierPublic: the input has no inverse");
		return 0;
	}

	return 1;
}

int CPaillierPublic::GetInverseOfFixPoint(ZZ& v, ZZ&m, long& bitPrec)
{
	ZZ nz, pow, r;
	long bitNum;

	if(!boolPubKeyOK)
	{
		Paillier_SetErrorMsg("CPaillierPublic: use GetInverseOfFixPoint before PubKey setup");
		return 0;
	}

	rem(nz, m, zzN);
	if(nz <= zzHalfN)
	{
		if(0==(bitNum = NumBits(nz)))
		{
			Paillier_SetErrorMsg("CPaillierPublic::GetInverseOfFixPoint input is zero thus has no inverse");
			return 0;
		}
		bitPrec += bitNum;
		power(pow, 2L, bitPrec);
		DivRem(v, r, pow, nz);
		if(r + r >= nz) v += 1;
	}
	else
	{
		sub(v, zzN, nz);
		if(0==(bitNum = NumBits(v)))
		{
			Paillier_SetErrorMsg("CPaillierPublic::GetInverseOfFixPoint input is zero thus has no inverse");
			return 0;
		}
		bitPrec += bitNum;
		power(pow, 2L, bitPrec);
		DivRem(nz, r, pow, v);
		if(r + r >= v) nz += 1;
		sub(v, zzN, nz);
	}

	return 1;
}

int CPaillierPublic::NormalZZ2PaillierZZ(ZZ& pz, ZZ& nz)
{
	if(!boolPubKeyOK)
	{
		Paillier_SetErrorMsg("CPaillierPublic: use NormalNumber2PaillierNumber before PubKey setup");
		return 0;
	}

	pz = nz;
	if(pz >= 0)
	{
		if(pz > zzHalfN)
		{
			Paillier_SetErrorMsg("CPaillierPublic: input is out of range");
			return 0;
		}
	}
	else
	{
		pz += zzN;
		if(pz <= zzHalfN)
		{
			Paillier_SetErrorMsg("CPaillierPublic: input is out of range");
			return 0;
		}
	}

	return 1;
}

int CPaillierPublic::PaillierZZ2NormalZZ(ZZ& nz, ZZ& pz)
{
	if(!boolPubKeyOK)
	{
		Paillier_SetErrorMsg("CPaillierPublic: use NormalNumber2PaillierNumber before PubKey setup");
		return 0;
	}

	if(pz <= zzHalfN) nz = pz; else sub(nz, pz, zzN);

	return 1;
}

int CPaillierPublic::FastPowerModN2(ZZ& x0, ZZ& x1, const ZZ& a, const ZZ& e)
{
	long curBit;
	ZZ zzTmp, zzTmp1;

	if(!boolPubKeyOK)
	{
		Paillier_SetErrorMsg("CPaillierPublic: use FastPowerMod2 before PubKey setup");
		return 0;
	}

	if((curBit = NumBits(e)) == 0)
	{
		x0 = 1;
		x1 = 0;
		return 1;
	}

	x0 = a; x1 = 0;
	--curBit;
	while((--curBit)>= 0)
	{
		mul(x1, x1, x0);
		rem(x1, x1, zzN);
		x1 += x1; if(x1 >= zzN) x1 -= zzN;
		sqr(zzTmp, x0);
		DivRem(zzTmp1, x0, zzTmp, zzN);
		x1 += zzTmp1; if(x1 >= zzN) x1 -= zzN;
		if(bit(e, curBit))
		{
			MulMod(zzTmp1, x1, a, zzN);
			mul(zzTmp, x0, a);
			DivRem(x1, x0, zzTmp, zzN);
			x1 += zzTmp1; if(x1 >= zzN) x1 -= zzN;
		}
	}

	return 1;
}
