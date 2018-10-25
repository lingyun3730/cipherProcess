#ifndef Macro_ThresholdPaillierServer_H
#define Macro_ThresholdPaillierServer_H 1

#include "PaillierPublic.h"

class CThresholdPaillierServer : public CPaillierPublic
{
public:
	CThresholdPaillierServer(void);
	~CThresholdPaillierServer(void);
public:
	int SetDecryptSubKey23(int id, ZZ& subK, ZZ& subKInv);
	int GetMyID(int &id);
	int DecryptAndSharePhase0(int setmask, ZZ s0[3], ZZ s1[3], ZZ& c0, ZZ& c1);
	int DecryptAndSharePhase1(ZZ x[3], ZZ& y, ZZ& fa0, ZZ& fa1, ZZ& fb0, ZZ& fb1);
	int DecryptAndSharePhase2(ZZ& s, ZZ& g1, ZZ& g2, ZZ& g3, ZZ& y1, ZZ& y2, ZZ& y3);
protected:
	int boolSubKeyOK;
	int nID;
	ZZ zzSubK;
	ZZ zzSubKInv;
};

#endif//#ifndef Macro_ThresholdPaillierServer_H
