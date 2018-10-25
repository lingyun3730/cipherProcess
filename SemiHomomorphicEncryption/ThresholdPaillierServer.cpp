#include "ThresholdPaillierServer.h"

CThresholdPaillierServer::CThresholdPaillierServer(void)
{
	boolSubKeyOK = 0;
}

CThresholdPaillierServer::~CThresholdPaillierServer(void)
{
}

int CThresholdPaillierServer::SetDecryptSubKey23(int id, ZZ& subK, ZZ& subKInv)
{
	nID = id;
	zzSubK = subK;
	zzSubKInv = subKInv;
	boolSubKeyOK = 1;

	return 1;
}

int CThresholdPaillierServer::GetMyID(int &id)
{
	if(!boolSubKeyOK)
	{
		Paillier_SetErrorMsg("CThresholdPaillierServer: use GetMyID before subKey setup");
		return 0;
	}

	id = nID;

	return 1;
}

int CThresholdPaillierServer::DecryptAndSharePhase0(int setmask, ZZ s0[3], ZZ s1[3], ZZ& c0, ZZ& c1)
{
	ZZ x0, x1, r0, r1, k;

	if(!boolSubKeyOK)
	{
		Paillier_SetErrorMsg("CThresholdPaillierServer: use DecryptAndSharePhase0 before subKey setup");
		return 0;
	}

	if(!boolPubKeyOK)
	{
		Paillier_SetErrorMsg("CThresholdPaillierServer: use DecryptAndSharePhase0 before PubKey setup");
		return 0;
	}

	switch(setmask)
	{
	case 3:
		if(1==nID)
		{
			add(k, zzSubK, zzSubK);
			HomoScalar(x0, x1, c0, c1, k);
		}
		else if(2==nID)
		{
			HomoScalar(r0, r1, c0, c1, zzSubK);
			if(!HomoNeg(x0, x1, r0, r1)) return 0;
		}
		else
		{
			Paillier_SetErrorMsg("CThresholdPaillierServer: this DecryptAndSharePhase0 is not for me");
			return 0;
		}
		break;
	case 5:
		if(1==nID)
		{
			add(k, zzSubK, zzSubK); k += zzSubK;
			HomoScalar(x0, x1, c0, c1, k);
		}
		else if(3==nID)
		{
			HomoScalar(r0, r1, c0, c1, zzSubK);
			if(!HomoNeg(x0, x1, r0, r1)) return 0;
		}
		else
		{
			Paillier_SetErrorMsg("CThresholdPaillierServer: this DecryptAndSharePhase0 is not for me");
			return 0;
		}
		break;
	case 6:
		if(2==nID)
		{
			add(k, zzSubK, zzSubK); k += zzSubK;
			HomoScalar(r0, r1, c0, c1, k);
			if(!HomoNeg(x0, x1, r0, r1)) return 0;
		}
		else if(3==nID)
		{
			add(k, zzSubK, zzSubK);
			HomoScalar(x0, x1, c0, c1, k);
		}
		else
		{
			Paillier_SetErrorMsg("CThresholdPaillierServer: this DecryptAndSharePhase0 is not for me");
			return 0;
		}
		break;
	default:
		Paillier_SetErrorMsg("CThresholdPaillierServer: invoke DecryptAndSharePhase0 with invalid set mask");
		return 0;
	}

	RandomBnd(r0, zzN);
	RandomBnd(r1, zzN);

	add(s1[0], x1, r1);
	add(s0[0], x0, r0);
	if(s0[0] >= zzN)
	{
		s0[0] -= zzN;
		s1[0] += 1;
	}
	if(s1[0] >= zzN) s1[0] -= zzN;

	add(s1[1], s1[0], r1);
	add(s0[1], s0[0], r0);
	if(s0[1] >= zzN)
	{
		s0[1] -= zzN;
		s1[1] += 1;
	}
	if(s1[1] >= zzN) s1[1] -= zzN;

	add(s1[2], s1[1], r1);
	add(s0[2], s0[1], r0);
	if(s0[2] >= zzN)
	{
		s0[2] -= zzN;
		s1[2] += 1;
	}
	if(s1[2] >= zzN) s1[2] -= zzN;

	return 1;
}

int CThresholdPaillierServer::DecryptAndSharePhase1(ZZ x[3], ZZ& y, ZZ& fa0, ZZ& fa1, ZZ& fb0, ZZ& fb1)
{
	ZZ z0, z1, r1;

	if(!boolSubKeyOK)
	{
		Paillier_SetErrorMsg("CThresholdPaillierServer: use DecryptAndSharePhase1 before subKey setup");
		return 0;
	}

	if(!HomoAdd(z0, z1, fa0, fa1, fb0, fb1)) return 0;

	switch(nID)
	{
	case 1:
		add(r1, z1, z1); if(r1 >= zzN) r1 -= zzN;
		z1 += r1; if(z1 >= zzN) z1 -= zzN;
		add(r1, z0, z0); if(r1 >= zzN) {r1 -= zzN; z1 += 1;}
		z0 += r1; if(z0 >= zzN) {z0 -= zzN; z1 += 1;}
		if(z1 >= zzN) z1 -= zzN;
		break;
	case 2:
		add(r1, z1, z1); if(r1 >= zzN) r1 -= zzN;
		z1 += r1; if(z1 >= zzN) z1 -= zzN;
		add(r1, z0, z0); if(r1 >= zzN) {r1 -= zzN; z1 += 1;}
		z0 += r1; if(z0 >= zzN) {z0 -= zzN; z1 += 1;}
		if(z1 >= zzN) z1 -= zzN;
		if(z0 == 0)
		{
			if(z1 != 0) z1 = zzN - z1;
		}
		else
		{
			if(z1 == 0)
			{
				sub(z1, zzN, 1);
			}
			else
			{
				add(r1, z1, 1);
				sub(z1, zzN, r1);
			}
			z0 = zzN - z0;
		}
		break;
	case 3:
		break;
	default:
		Paillier_SetErrorMsg("CThresholdPaillierServer: invalid id");
		return 0;
	}

	y = z0;
	RandomBnd(r1, zzN);
	z1 += r1; if(z1 >= zzN) z1 -= zzN;
	x[0] = z1;
	z1 += r1; if(z1 >= zzN) z1 -= zzN;
	x[1] = z1;
	z1 += r1; if(z1 >= zzN) z1 -= zzN;
	x[2] = z1;

	return 1;
}

int CThresholdPaillierServer::DecryptAndSharePhase2(ZZ& s, ZZ& g1, ZZ& g2, ZZ& g3, ZZ& y1, ZZ& y2, ZZ& y3)
{
	ZZ q, r, t;

	if(!boolSubKeyOK)
	{
		Paillier_SetErrorMsg("CThresholdPaillierServer: use DecryptAndSharePhase2 before subKey setup");
		return 0;
	}

	add(t, y1, y2);
	t += y3;

	DivRem(q, r, t, zzN);
	if(r != 1)
	{
		Paillier_SetErrorMsg("CThresholdPaillierServer: the original ciphertext is not a valid one");
		return 0;
	}

	q += g1; if(q >= zzN) q-= zzN;
	add(r, g2, g3); if(r >= zzN) r -= zzN;
	q += r; if(q >= zzN) q -= zzN;

	switch(nID)
	{
	case 1:
		add(r, q, q); if(r >= zzN) r -= zzN;
		q += r; if(q >= zzN) q -= zzN;
		break;
	case 2:
		add(r, q, q); if(r >= zzN) r -= zzN;
		r += q; if(r >= zzN) r -= zzN;
		if(r == 0) q = 0; else sub(q, zzN, r);
		break;
	case 3:
		break;
	default:
		Paillier_SetErrorMsg("CThresholdPaillierServer: invalid id");
		return 0;
	}

	MulMod(s, q, zzSubKInv, zzN);

	return 1;
}