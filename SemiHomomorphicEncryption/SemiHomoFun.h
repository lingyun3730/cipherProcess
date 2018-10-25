#ifndef Macro_SemiHomoFun_H
#define Macro_SemiHomoFun_H 1

#include <string.h>
#include <string>
#include <vector>

using namespace std;

#include "ThresholdPaillierAgent.h"
#include "ThresholdPaillierServer.h"

typedef struct
{
	ZZ c0;
	ZZ c1;
	long prec;//precise
}SemiHomoCipher;

typedef struct
{
	SemiHomoCipher x;
	SemiHomoCipher y;
}SemiHomoCipherPair;

typedef struct
{
	ZZ m;
	long prec;//precise
}SemiHomoPlain;

typedef struct
{
	SemiHomoPlain x;
	SemiHomoPlain y;
}SemiHomoPlainPair;

char* SemiHomoGetLastError();

int String2SemiHomoPlain(SemiHomoPlain& p, string &s);

int SemiHomoPlain2String(string &s, SemiHomoPlain& p);

int String2SemiHomoPlainVector(vector<SemiHomoPlain> &vp, string &s);

int SemiHomoPlainVector2String(string &s, vector<SemiHomoPlain> &vp);

int String2SemiHomoPlainPair(SemiHomoPlainPair& pp, string &s);

int SemiHomoPlainPair2String(string &s, SemiHomoPlainPair& pp);

int String2SemiHomoPlainPairVector(vector<SemiHomoPlainPair> &vpp, string &s);

int SemiHomoPlainPairVector2String(string &s, vector<SemiHomoPlainPair> &vpp);

int String2SemiHomoCipher(SemiHomoCipher& c, string &s);

int SemiHomoCipher2String(string &s, SemiHomoCipher& c);

int String2SemiHomoCipherVector(vector<SemiHomoCipher> &vc, string &s);

int SemiHomoCipherVector2String(string &s, vector<SemiHomoCipher> &vc);

int String2SemiHomoCipherPair(SemiHomoCipherPair& cp, string &s);

int SemiHomoCipherPair2String(string &s, SemiHomoCipherPair& cp);

int String2SemiHomoCipherPairVector(vector<SemiHomoCipherPair> &vcp, string &s);

int SemiHomoCipherPairVector2String(string &s, vector<SemiHomoCipherPair> &vcp);

int SetAgentKeyFromPsw(unsigned char *pswKey, unsigned int pswKeyLen);

/*    p[i]: plaintext
      c[o]: ciphertext */
int EncryptSemiHomoPlain(SemiHomoCipher& c, SemiHomoPlain& p);

/*    c[i]: ciphertext
      p[o]: plaintext */
int DecryptSemiHomoCipher(SemiHomoPlain& p, SemiHomoCipher& c);

/*    vp[i]: plaintext
      vc[o]: ciphertext */
int EncryptSemiHomoPlainVector(vector<SemiHomoCipher> &vc, vector<SemiHomoPlain> &vp);

/*    vc[i]: ciphertext
      vp[o]: plaintext */
int DecryptSemiHomoCipherVector(vector<SemiHomoPlain> &vp, vector<SemiHomoCipher> &vc);

/*    pp[i]: plaintext
      cp[o]: ciphertext */
int EncryptSemiHomoPlainPair(SemiHomoCipherPair& cp, SemiHomoPlainPair& pp);

/*    cp[i]: ciphertext
      pp[o]: plaintext */
int DecryptSemiHomoCipherPair(SemiHomoPlainPair& pp, SemiHomoCipherPair& cp);

/*    vpp[i]: plaintext
      vcp[o]: ciphertext */
int EncryptSemiHomoPlainPairVector(vector<SemiHomoCipherPair> &vcp, vector<SemiHomoPlainPair> &vpp);

/*    vcp[i]: ciphertext
      vpp[o]: plaintext */
int DecryptSemiHomoCipherPairVector(vector<SemiHomoPlainPair> &vpp, vector<SemiHomoCipherPair> &vcp);

/*    pa[i]: plain input a
      pb[i]: plain input b
	  pr[o]: plain output r = a + b */
int AddSemiHomoPlain(SemiHomoPlain& pr, SemiHomoPlain& pa, SemiHomoPlain& pb);

/*    pa[i]: plain input a
	  pr[o]: plain output r = -a */
int NegSemiHomoPlain(SemiHomoPlain& pr, SemiHomoPlain& pa);

/*    pa[i]: plain input a
	  pr[o]: plain output r = a * 2^shiftNum */
int LeftShiftSemiHomoPlain(SemiHomoPlain& pr, SemiHomoPlain& pa, long shiftNum);

/*    pa[i]: plain input a
  scalar[i]: scalar input, non-negative integer
	  pr[o]: plain output r = a * scalar */
int ScalarSemiHomoPlain(SemiHomoPlain& pr, SemiHomoPlain& pa, ZZ& scalar);

/*    pa[i]: plain input a
      pb[i]: plain input b
	  pr[o]: plain output r = a * b */
int MulSemiHomoPlain(SemiHomoPlain& pr, SemiHomoPlain& pa, SemiHomoPlain& pb);

/*    pa[i]: plain input a
	  pr[o]: plain output r = 1/a */
int InvSemiHomoPlain(SemiHomoPlain& pr, SemiHomoPlain& pa);

/*    pa[i]: plain input a
      pb[i]: plain input b
	  pr[o]: plain output r = a/b */
int DivSemiHomoPlain(SemiHomoPlain& pr, SemiHomoPlain& pa, SemiHomoPlain& pb);

/*    vp[i]: plain input list {a[1], ... , a[n]}
	  pr[o]: plain output r = sum(a[i]) */
int SumSemiHomoPlainVector(SemiHomoPlain& pr, vector<SemiHomoPlain> & vp);

/*    vp[i]: plain input list {a[1], ... , a[n]}
	  pr[o]: plain output r = sum(a[i])/n */
int AverageSemiHomoPlainVector(SemiHomoPlain& pr, vector<SemiHomoPlain> & vp);

/*    vp[i]: plain input list {a[1], ... , a[n]}
	  pr[o]: plain output r = (n*sum(a[i]^2)-(sum(a[i]))^2)/n^2 */
int VarianceSemiHomoPlainVector(SemiHomoPlain& pr, vector<SemiHomoPlain> & vp);

/*    vpp[i]: plain input list {p[1], ... , p[n]}
      B,C[o]: plain output: the line is y = B * x + C */
int LinearRegressionSemiHomoPlainPairVector(SemiHomoPlain& B, SemiHomoPlain& C, vector<SemiHomoPlainPair> & vpp);

/*    ca[i]: cipher input a
      cb[i]: cipher input b
	  cr[o]: cipher output r = a + b */
int AddSemiHomoCipher(SemiHomoCipher& cr, SemiHomoCipher& ca, SemiHomoCipher& cb);

/*    ca[i]: cipher input a
	  cr[o]: cipher output r = -a */
int NegSemiHomoCipher(SemiHomoCipher& cr, SemiHomoCipher& ca);

/*    ca[i]: cipher input a
	  cr[o]: cipher output r = a * 2^shiftNum */
int LeftShiftSemiHomoCipher(SemiHomoCipher& cr, SemiHomoCipher& ca, long shiftNum);

/*    ca[i]: cipher input a
  scalar[i]: scalar input, non-negative integer
	  cr[o]: cipher output r = a * scalar */
int ScalarSemiHomoCipher(SemiHomoCipher& cr, SemiHomoCipher& ca, ZZ& scalar);

/*    ca[i]: cipher input a
      cb[i]: cipher input b
	  cr[o]: cipher output r = a * b */
int MulSemiHomoCipher(SemiHomoCipher& cr, SemiHomoCipher& ca, SemiHomoCipher& cb);

/*    ca[i]: cipher input a
	  cr[o]: cipher output r = 1/a */
int InvSemiHomoCipher(SemiHomoCipher& cr, SemiHomoCipher& ca);

/*    ca[i]: cipher input a
      cb[i]: cipher input b
	  cr[o]: cipher output r = a/b */
int DivSemiHomoCipher(SemiHomoCipher& cr, SemiHomoCipher& ca, SemiHomoCipher& cb);

/*    vc[i]: cipher input list {a[1], ... , a[n]}
	  cr[o]: cipher output r = sum(a[i]) */
int SumSemiHomoCipherVector(SemiHomoCipher& cr, vector<SemiHomoCipher> & vc);

/*    vc[i]: cipher input list {a[1], ... , a[n]}
	  cr[o]: cipher output r = sum(a[i])/n */
int AverageSemiHomoCipherVector(SemiHomoCipher& cr, vector<SemiHomoCipher> & vc);

/*    vc[i]: cipher input list {a[1], ... , a[n]}
	  cr[o]: cipher output r = (n*sum(a[i]^2)-(sum(a[i]))^2)/n^2 */
int VarianceSemiHomoCipherVector(SemiHomoCipher& cr, vector<SemiHomoCipher> & vc);

/*    vcp[i]: cipher input list {p[1], ... , p[n]}
      B,C[o]: cipher output: the line is y = B * x + C */
int LinearRegressionSemiHomoCipherPairVector(SemiHomoCipher& B, SemiHomoCipher& C, vector<SemiHomoCipherPair> & vcp);

/*        m[i]: plaintext
   (c0, c1)[o]: ciphertext */
int EncryptRawData(ZZ& c0, ZZ& c1, ZZ& m);

/* (c0, c1)[i]: ciphertext
          m[o]: plaintext  */
int DecryptRawData(ZZ& m, ZZ& c0, ZZ& c1);

/* (x0, x1)[i]: ciphertext corresponding to message a
   (y0, y1)[i]: ciphertext corresponding to message b
   (z0, z1)[o]: ciphertext corresponding to message a+b  */
int HomoAddOnEncryptedRawData(ZZ& z0, ZZ& z1, ZZ& x0, ZZ& x1, ZZ& y0, ZZ& y1);

/* (x0, x1)[i]: ciphertext corresponding to message a
   (z0, z1)[o]: ciphertext corresponding to message -a  */
int HomoNegOnEncryptedRawData(ZZ& z0, ZZ& z1, ZZ& x0, ZZ& x1);

/* (x0, x1)[i]: ciphertext corresponding to message a
     scalar[i]: the scalar, nonnegative integer
   (z0, z1)[o]: ciphertext corresponding to message a*scalar  */
int HomoScalarOnEncryptedRawData(ZZ& z0, ZZ& z1, ZZ& x0, ZZ& x1, ZZ& scalar);

/* (x0, x1)[i]: ciphertext corresponding to message a
   shiftNum[i]: number of left_shift, positive
   (z0, z1)[o]: ciphertext corresponding to message a*2^shiftNum  */
int HomoLeftShiftOnEncryptedRawData(ZZ& z0, ZZ& z1, ZZ& x0, ZZ& x1, long shiftNum);

/* (x0, x1)[i]: ciphertext corresponding to message a
   (y0, y1)[i]: ciphertext corresponding to message b
   (z0, z1)[o]: ciphertext corresponding to message a*b  */
int HomoMulOnEncryptedRawData(ZZ& z0, ZZ& z1, ZZ& x0, ZZ& x1, ZZ& y0, ZZ& y1);

/* (x0, x1)[i]: ciphertext corresponding to message a
   (z0, z1)[o]: ciphertext corresponding to message 1/a  */
int HomoInvOnEncryptedRawData(ZZ& z0, ZZ& z1, ZZ& x0, ZZ& x1);

/* (x0, x1)[i]: ciphertext corresponding to message a
   (v0, v1)[i]: ciphertext corresponding to message b
   (z0, z1)[o]: ciphertext corresponding to message a/b  */
int HomoDivOnEncryptedRawData(ZZ& z0, ZZ& z1, ZZ& x0, ZZ& x1, ZZ& v0, ZZ& v1);

#endif//#ifndef Macro_SemiHomoFun_H
