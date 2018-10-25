#include "SemiHomoFun.h"
#include <NTL/RR.h>
#include <sstream>

CThresholdPaillierAgent  mAgent;
CThresholdPaillierServer mMainServer;
CThresholdPaillierServer mAux1Server;
CThresholdPaillierServer mAux2Server;

extern CThresholdPaillierAgent  mAgent;      //user agent
extern CThresholdPaillierServer mMainServer; //main server
extern CThresholdPaillierServer mAux1Server; //1st aux server
extern CThresholdPaillierServer mAux2Server;//2nd aux server

#define ID_MainServer 1
#define ID_Aux1Server 2
#define ID_Aux2Server 3
#define Msk_MainServer (0x01<<(ID_MainServer-1))
#define Msk_Aux1Server (0x01<<(ID_Aux1Server-1))
#define Msk_Aux2Server (0x01<<(ID_Aux2Server-1))
#define FP_BITSIZE       64L
#define FP_PRECISE       30L
#define OUT_PRECISE      6L
#define Max_FloatMsgSize 128

char strErrMsg[Paillier_ErrMsg_Size];

unsigned char* SkipSpaces(unsigned char *PrnStr);

unsigned char* SemiHomoCipher2PrnStr(unsigned char *PrnStr, SemiHomoCipher& c);

unsigned char* PrnStr2SemiHomoCipher(SemiHomoCipher& c, unsigned char *PrnStr);

unsigned char* PrnStr2ZZ(ZZ& z, long& p, unsigned char *PrnStr);

unsigned char* PrnStr2HexStr(unsigned char *HexStr, int &lenHexStr, unsigned char *PrnStr);

unsigned char* HexStr2PrnStr(unsigned char *PrnStr, unsigned char *HexStr, int lenHexStr);

char* DigitalNum2PrnStr(char *PrnStr, ZZ &dn, long dp); //output (dn * 10^{-dp})

void DigitalNum2BinaryNum(ZZ& bn, long bp, ZZ& dn, long dp); //(bn * 2^{-bp}) <-- (dn * 10^{-dp}) [bp must be positive]

void BinaryNum2DigitalNum(ZZ& dn, long dp, ZZ& bn, long bp); //(dn * 10^{-dp}) <-- (bn * 2^{-bp}) [both bp, dp must be positive]

char* SemiHomoGetLastError()
{
	return strErrMsg;
}

using namespace std;

int String2SemiHomoPlain(SemiHomoPlain& p, string &s)
{
	ZZ dn;
	long dp;
	unsigned char *strAscii;

	strAscii = (unsigned char *)s.c_str();

	strAscii = SkipSpaces(strAscii);

	strAscii = PrnStr2ZZ(dn, dp, strAscii);

	if(strAscii) strAscii = SkipSpaces(strAscii);

	if((!strAscii) || (*strAscii))
	{
		sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoPlain: input is not a valid SemiHomoPlain");
		return 0;
	}

	DigitalNum2BinaryNum(p.m, FP_PRECISE, dn, -dp);
	p.prec = FP_PRECISE;

	return 1;
}

int String2SemiHomoPlainVector(vector<SemiHomoPlain> &vp, string &s)
{
	unsigned char *strAscii;
	ZZ dn;
	long dp;
	SemiHomoPlain p;

	vp.clear();

	strAscii = (unsigned char *)s.c_str();

	strAscii = SkipSpaces(strAscii);

	if((*strAscii) != '[')
	{
		sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoPlainVector: input is not a valid SemiHomoPlainVector");
		return 0;
	}
	strAscii = SkipSpaces(strAscii+1);

	while((*strAscii) != ']')
	{
		strAscii = PrnStr2ZZ(dn, dp, strAscii);
		if(!strAscii)
		{
			vp.clear();
			sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoPlainVector: input is not a valid SemiHomoPlainVector");
			return 0;
		}
		DigitalNum2BinaryNum(p.m, FP_PRECISE, dn, -dp);
		p.prec = FP_PRECISE;
		vp.push_back(p);
		strAscii = SkipSpaces(strAscii);
		if((*strAscii) != ',')
		{
			if((*strAscii) == ']') break;
			vp.clear();
			sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoPlainVector: input is not a valid SemiHomoPlainVector");
			return 0;
		}
		strAscii = SkipSpaces(strAscii+1);
	}

	strAscii = SkipSpaces(strAscii+1);

	if(*strAscii)
	{
		vp.clear();
		sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoPlainVector: input is not a valid SemiHomoPlainVector");
		return 0;
	}

	return 1;
}

int SemiHomoPlainVector2String(string &s, vector<SemiHomoPlain> &vp)
{
	vector<SemiHomoPlain>::iterator it;
	char PrnStr[Max_FloatMsgSize];
	ZZ dn;
	long dp = OUT_PRECISE;
	int bFirst = 1;

	s = "[";
	for(it=vp.begin(); it!=vp.end(); it++)
	{
		if(bFirst) bFirst = 0; else s.append(", ");
		BinaryNum2DigitalNum(dn, dp, it->m, it->prec);
		DigitalNum2PrnStr(PrnStr, dn, dp);
		s.append(PrnStr);
	}
	s.append("]");

	return 1;
}

int String2SemiHomoCipherVector(vector<SemiHomoCipher> &vc, string &s)
{
	unsigned char *strAscii;
	SemiHomoCipher c;

	vc.clear();

	strAscii = (unsigned char *)s.c_str();

	strAscii = SkipSpaces(strAscii);

	if((*strAscii) != '[')
	{
		sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoCipherVector: input is not a valid SemiHomoCipherVector");
		return 0;
	}
	strAscii = SkipSpaces(strAscii+1);

	while((*strAscii) != ']')
	{
		strAscii = PrnStr2SemiHomoCipher(c, strAscii);
		if(!strAscii)
		{
			vc.clear();
			sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoCipherVector: input is not a valid SemiHomoCipherVector");
			return 0;
		}
		vc.push_back(c);
		strAscii = SkipSpaces(strAscii);
		if((*strAscii) != ',')
		{
			if((*strAscii) == ']') break;
			vc.clear();
			sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoCipherVector: input is not a valid SemiHomoCipherVector");
			return 0;
		}
		strAscii = SkipSpaces(strAscii+1);
	}

	strAscii = SkipSpaces(strAscii+1);

	if(*strAscii)
	{
		vc.clear();
		sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoCipherVector: input is not a valid SemiHomoCipherVector");
		return 0;
	}

	return 1;
}

int SemiHomoCipherVector2String(string &s, vector<SemiHomoCipher> &vc)
{
	vector<SemiHomoCipher>::iterator it;
	unsigned char PrnCipher[MAX_RSAMODULE_BYTELEN*4+8];
	int bFirst = 1;

	s = "[";
	for(it=vc.begin(); it!=vc.end(); it++)
	{
		if(bFirst) bFirst = 0; else s.append(", ");
		SemiHomoCipher2PrnStr(PrnCipher, *it);
		s.append((char*)PrnCipher);
	}
	s.append("]");

	return 1;
}

int String2SemiHomoPlainPairVector(vector<SemiHomoPlainPair> &vpp, string &s)
{
	unsigned char *strAscii;
	ZZ dn;
	long dp;
	SemiHomoPlainPair pp;

	vpp.clear();

	strAscii = (unsigned char *)s.c_str();

	strAscii = SkipSpaces(strAscii);

	if((*strAscii) != '[')
	{
		sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoPlainPairVector: input is not a valid SemiHomoPlainPairVector");
		return 0;
	}
	strAscii = SkipSpaces(strAscii+1);

	while((*strAscii) != ']')
	{
		if((*strAscii) != '(')
		{
			vpp.clear();
			sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoPlainPairVector: input is not a valid SemiHomoPlainPairVector");
			return 0;
		}
		strAscii = SkipSpaces(strAscii+1);
		strAscii = PrnStr2ZZ(dn, dp, strAscii);
		if(!strAscii)
		{
			vpp.clear();
			sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoPlainPairVector: input is not a valid SemiHomoPlainPairVector");
			return 0;
		}
		DigitalNum2BinaryNum(pp.x.m, FP_PRECISE, dn, -dp);
		pp.x.prec = FP_PRECISE;
		strAscii = SkipSpaces(strAscii);
		if((*strAscii) != ',')
		{
			vpp.clear();
			sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoPlainPairVector: input is not a valid SemiHomoPlainPairVector");
			return 0;
		}
		strAscii = SkipSpaces(strAscii+1);
		strAscii = PrnStr2ZZ(dn, dp, strAscii);
		if(!strAscii)
		{
			vpp.clear();
			sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoPlainPairVector: input is not a valid SemiHomoPlainPairVector");
			return 0;
		}
		DigitalNum2BinaryNum(pp.y.m, FP_PRECISE, dn, -dp);
		pp.y.prec = FP_PRECISE;
		strAscii = SkipSpaces(strAscii);
		if((*strAscii) != ')')
		{
			vpp.clear();
			sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoPlainPairVector: input is not a valid SemiHomoPlainPairVector");
			return 0;
		}
		vpp.push_back(pp);
		strAscii = SkipSpaces(strAscii+1);
		if((*strAscii) != ',')
		{
			if((*strAscii) == ']') break;
			vpp.clear();
			sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoPlainPairVector: input is not a valid SemiHomoPlainPairVector");
			return 0;
		}
		strAscii = SkipSpaces(strAscii+1);
	}

	strAscii = SkipSpaces(strAscii+1);

	if(*strAscii)
	{
		vpp.clear();
		sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoPlainPairVector: input is not a valid SemiHomoPlainPairVector");
		return 0;
	}

	return 1;
}

int SemiHomoPlainPairVector2String(string &s, vector<SemiHomoPlainPair> &vpp)
{
	vector<SemiHomoPlainPair>::iterator it;
	char PrnStr[Max_FloatMsgSize];
	ZZ dn;
	long dp = OUT_PRECISE;
	int bFirst = 1;

	s = "[";
	for(it=vpp.begin(); it!=vpp.end(); it++)
	{
		if(bFirst) bFirst = 0; else s.append(", ");
		s.append("(");
		BinaryNum2DigitalNum(dn, dp, it->x.m, it->x.prec);
		DigitalNum2PrnStr(PrnStr, dn, dp);
		s.append(PrnStr);
		s.append(", ");
		BinaryNum2DigitalNum(dn, dp, it->y.m, it->y.prec);
		DigitalNum2PrnStr(PrnStr, dn, dp);
		s.append(PrnStr);
		s.append(")");
	}
	s.append("]");

	return 1;
}

int String2SemiHomoCipherPairVector(vector<SemiHomoCipherPair> &vcp, string &s)
{
	unsigned char *strAscii;
	SemiHomoCipherPair cp;

	vcp.clear();

	strAscii = (unsigned char *)s.c_str();

	strAscii = SkipSpaces(strAscii);

	if((*strAscii) != '[')
	{
		sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoCipherPairVector: input is not a valid SemiHomoCipherPairVector");
		return 0;
	}
	strAscii = SkipSpaces(strAscii+1);

	while((*strAscii) != ']')
	{
		if((*strAscii) != '(')
		{
			vcp.clear();
			sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoCipherPairVector: input is not a valid SemiHomoCipherPairVector");
			return 0;
		}
		strAscii = SkipSpaces(strAscii+1);
		strAscii = PrnStr2SemiHomoCipher(cp.x, strAscii);
		if(!strAscii)
		{
			vcp.clear();
			sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoCipherPairVector: input is not a valid SemiHomoCipherPairVector");
			return 0;
		}
		strAscii = SkipSpaces(strAscii);
		if((*strAscii) != ',')
		{
			vcp.clear();
			sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoCipherPairVector: input is not a valid SemiHomoCipherPairVector");
			return 0;
		}
		strAscii = SkipSpaces(strAscii+1);
		strAscii = PrnStr2SemiHomoCipher(cp.y, strAscii);
		if(!strAscii)
		{
			vcp.clear();
			sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoCipherPairVector: input is not a valid SemiHomoCipherPairVector");
			return 0;
		}
		strAscii = SkipSpaces(strAscii);
		if((*strAscii) != ')')
		{
			vcp.clear();
			sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoCipherPairVector: input is not a valid SemiHomoCipherPairVector");
			return 0;
		}
		vcp.push_back(cp);
		strAscii = SkipSpaces(strAscii+1);
		if((*strAscii) != ',')
		{
			if((*strAscii) == ']') break;
			vcp.clear();
			sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoCipherPairVector: input is not a valid SemiHomoCipherPairVector");
			return 0;
		}
		strAscii = SkipSpaces(strAscii+1);
	}

	strAscii = SkipSpaces(strAscii+1);

	if(*strAscii)
	{
		vcp.clear();
		sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoCipherPairVector: input is not a valid SemiHomoCipherPairVector");
		return 0;
	}

	return 1;
}

int SemiHomoCipherPairVector2String(string &s, vector<SemiHomoCipherPair> &vcp)
{
	vector<SemiHomoCipherPair>::iterator it;
	unsigned char PrnCipher[MAX_RSAMODULE_BYTELEN*4+8];
	int bFirst = 1;

	s = "[";
	for(it=vcp.begin(); it!=vcp.end(); it++)
	{
		if(bFirst) bFirst = 0; else s.append(", ");
		s.append("(");
		SemiHomoCipher2PrnStr(PrnCipher, it->x);
		s.append((char*)PrnCipher);
		s.append(", ");
		SemiHomoCipher2PrnStr(PrnCipher, it->y);
		s.append((char*)PrnCipher);
		s.append(")");
	}
	s.append("]");

	return 1;
}

int SemiHomoPlain2String(string &s, SemiHomoPlain& p)
{
	char PrnStr[Max_FloatMsgSize];
	ZZ dn;
	long dp = OUT_PRECISE;

	BinaryNum2DigitalNum(dn, dp, p.m, p.prec);
	DigitalNum2PrnStr(PrnStr, dn, dp);

	s = PrnStr;

	return 1;
}

int String2SemiHomoPlainPair(SemiHomoPlainPair& pp, string &s)
{
	ZZ dn;
	long dp;
	unsigned char *strAscii;

	strAscii = (unsigned char *)s.c_str();

	strAscii = SkipSpaces(strAscii);
	if((*strAscii) != '(')
	{
		sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoPlainPair: input is not a valid SemiHomoPlainPair");
		return 0;
	}

	strAscii = SkipSpaces(strAscii+1);
	strAscii = PrnStr2ZZ(dn, dp, strAscii);
	if(!strAscii)
	{
		sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoPlainPair: input is not a valid SemiHomoPlainPair");
		return 0;
	}
	DigitalNum2BinaryNum(pp.x.m, FP_PRECISE, dn, -dp);
	pp.x.prec = FP_PRECISE;

	strAscii = SkipSpaces(strAscii);
	if((*strAscii) != ',')
	{
		sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoPlainPair: input is not a valid SemiHomoPlainPair");
		return 0;
	}

	strAscii = SkipSpaces(strAscii+1);
	strAscii = PrnStr2ZZ(dn, dp, strAscii);
	if(!strAscii)
	{
		sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoPlainPair: input is not a valid SemiHomoPlainPair");
		return 0;
	}
	DigitalNum2BinaryNum(pp.y.m, FP_PRECISE, dn, -dp);
	pp.y.prec = FP_PRECISE;

	strAscii = SkipSpaces(strAscii);
	if((*strAscii) != ')')
	{
		sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoPlainPair: input is not a valid SemiHomoPlainPair");
		return 0;
	}

	strAscii = SkipSpaces(strAscii+1);

	if(*strAscii)
	{
		sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoPlainPair: input is not a valid SemiHomoPlainPair");
		return 0;
	}

	return 1;
}

int SemiHomoPlainPair2String(string &s, SemiHomoPlainPair& pp)
{
	char PrnStr[Max_FloatMsgSize];
	ZZ dn;
	long dp = OUT_PRECISE;

	s = "(";

	BinaryNum2DigitalNum(dn, dp, pp.x.m, pp.x.prec);
	DigitalNum2PrnStr(PrnStr, dn, dp);
	s.append(PrnStr);
	s.append(", ");

	BinaryNum2DigitalNum(dn, dp, pp.y.m, pp.y.prec);
	DigitalNum2PrnStr(PrnStr, dn, dp);
	s.append(PrnStr);

	s.append(")");

	return 1;
}

int String2SemiHomoCipherPair(SemiHomoCipherPair& cp, string &s)
{
	unsigned char *strAscii;

	strAscii = (unsigned char *)s.c_str();

	strAscii = SkipSpaces(strAscii);
	if((*strAscii) != '(')
	{
		sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoCipherPair: input is not a valid SemiHomoCipherPair");
		return 0;
	}

	strAscii = SkipSpaces(strAscii+1);
	strAscii = PrnStr2SemiHomoCipher(cp.x, strAscii);
	if(!strAscii)
	{
		sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoCipherPair: input is not a valid SemiHomoCipherPair");
		return 0;
	}

	strAscii = SkipSpaces(strAscii);
	if((*strAscii) != ',')
	{
		sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoCipherPair: input is not a valid SemiHomoCipherPair");
		return 0;
	}

	strAscii = SkipSpaces(strAscii+1);
	strAscii = PrnStr2SemiHomoCipher(cp.y, strAscii);
	if(!strAscii)
	{
		sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoCipherPair: input is not a valid SemiHomoCipherPair");
		return 0;
	}

	strAscii = SkipSpaces(strAscii);
	if((*strAscii) != ')')
	{
		sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoCipherPair: input is not a valid SemiHomoCipherPair");
		return 0;
	}

	strAscii = SkipSpaces(strAscii+1);

	if(*strAscii)
	{
		sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoCipherPair: input is not a valid SemiHomoCipherPair");
		return 0;
	}

	return 1;
}

int SemiHomoCipherPair2String(string &s, SemiHomoCipherPair& cp)
{
	unsigned char PrnCipher[MAX_RSAMODULE_BYTELEN*4+8];
	
	s = "(";

	SemiHomoCipher2PrnStr(PrnCipher, cp.x);
	s.append((char*)PrnCipher);
	s.append(", ");

	SemiHomoCipher2PrnStr(PrnCipher, cp.y);
	s.append((char*)PrnCipher);

	s.append(")");

	return 1;
}

char* DigitalNum2PrnStr(char *PrnStr, ZZ &dn, long dp)
{
	ZZ q0, q1;
	long r;
	char InvStr[Max_FloatMsgSize], *pStr = InvStr;

	if(dn == 0)
	{
		(*PrnStr++) = '0';
		(*PrnStr) = 0;
		return PrnStr;
	}

	if(dn > 0)
	{
		q0 = dn;
	}
	else
	{
		q0 = -dn;
		(*PrnStr++) = '-';
	}

	while(!(r = DivRem(q1, q0, 10L)))
	{
		dp--;
		if(r = DivRem(q0, q1, 10L))
		{
			q1 = q0;
			break;
		}
		dp--;
	}

	if(dp < 0)
	{
		do{
			(*pStr++) = '0';
			dp++;
		}while(dp);
	}
	else if(dp > 0)
	{
		(*pStr++) = (char)(r + '0');
		while(--dp)
		{
			if(q1 == 0) break;
			r = DivRem(q0, q1, 10L);
			(*pStr++) = (char)(r + '0');
			if(!(--dp))
			{
				q1 = q0;
				break;
			}
			if(q0 == 0)
			{
				q1 = 0;
				break;
			}
			r = DivRem(q1, q0, 10L);
			(*pStr++) = (char)(r + '0');
		}
		if(dp)
		{
			r = 0;
			do{
				(*pStr++) = '0';
			}while(--dp);
		}
		else
		{
			q0 = q1;
			r = DivRem(q1, q0, 10L);
		}
		(*pStr++) = '.';
	}

	(*pStr++) = (char)(r + '0');
	while(q1 != 0)
	{
		r = DivRem(q0, q1, 10L);
		(*pStr++) = (char)(r + '0');
		if(q0 == 0) break;
		r = DivRem(q1, q0, 10L);
		(*pStr++) = (char)(r + '0');
	}

	while((--pStr) != InvStr)
	{
		(*PrnStr++) = (*pStr);
	}
	(*PrnStr++) = (*pStr);
	(*PrnStr) = 0;

	return PrnStr;
}

int String2SemiHomoCipher(SemiHomoCipher& c, string &s)
{
	unsigned char *strAscii;

	strAscii = (unsigned char *)s.c_str();

	strAscii = SkipSpaces(strAscii);

	strAscii = PrnStr2SemiHomoCipher(c, strAscii);

	if(strAscii) strAscii = SkipSpaces(strAscii);

	if((!strAscii) || (*strAscii))
	{
		sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "String2SemiHomoCipher: input is not a valid SemiHomoCipher");
		return 0;
	}

	return 1;
}

int SemiHomoCipher2String(string &s, SemiHomoCipher& c)
{
	unsigned char PrnCipher[MAX_RSAMODULE_BYTELEN*4+8];

	SemiHomoCipher2PrnStr(PrnCipher, c);

	s = (char*)PrnCipher;

	return 1;
}

unsigned char* SkipSpaces(unsigned char *PrnStr)
{
	unsigned char ucVal;

	while(ucVal =(*PrnStr++))
	{
		if(ucVal!=' ' && ucVal!='\t' && ucVal!='\r' && ucVal!='\n' && ucVal!='\v' && ucVal!='\f')
			return PrnStr-1;
	}

	return PrnStr-1;
}

#define Bit_NSign 0x01
#define Bit_Point 0x02
#define Bit_Power 0x04
#define Bit_PSign 0x10
unsigned char* PrnStr2ZZ(ZZ& z, long& p, unsigned char *PrnStr)
{
	int flag = 0, numDigital = 0, numPower = 0;
	long ep = 0;
	unsigned char ucVal;

	ucVal = (*PrnStr++);
	if(ucVal == '-')
	{
		flag |= Bit_NSign;
		z = 0;
	}
	else if(ucVal == '+')
	{
		z = 0;
	}
	else if(ucVal >= '0' && ucVal <= '9')
	{
		z = ((long)(ucVal - '0'));
		numDigital ++;
	}
	else if(ucVal == '.')
	{
		z = 0;
		flag |= Bit_Point;
	}
	else
	{
		return 0;
	}
	p = 0;

	while(ucVal = (*PrnStr++))
	{
		if(ucVal >= '0' && ucVal <= '9')
		{
			z *= 10L; z += ((long)(ucVal - '0'));
			numDigital++;
			if(flag&Bit_Point) --p;
		}
		else if(ucVal == '.')
		{
			if(flag&Bit_Point) break;
			flag |= Bit_Point;
		}
		else if(ucVal == 'e' || ucVal == 'E')
		{
			flag |= Bit_Power;
			break;
		}
		else
		{
			break;
		}
	}

	if(flag&Bit_Power)
	{
		ucVal = (*PrnStr++);
		if(ucVal == '-')
		{
			flag |= Bit_PSign;
		}
		else if(ucVal == '+')
		{
		}
		else if(ucVal >= '0' && ucVal <= '9')
		{
			ep = ((long)(ucVal - '0'));
			numPower++;
		}
		else
		{
			return 0;
		}
		while(ucVal = (*PrnStr++))
		{
			if(ucVal < '0' || ucVal > '9') break;
			ep *= 10L; ep += ((long)(ucVal - '0'));
			numPower++;
		}
		if(!numPower) return 0;
	}

	if(!numDigital) return 0;
	if(flag&Bit_NSign) z = -z;
	if(flag&Bit_PSign) p -= ep; else p += ep;
	return PrnStr - 1;
}

void DigitalNum2BinaryNum(ZZ& bn, long bp, ZZ& dn, long dp)
{
	ZZ zzTmp, zzPow, zzRem;

	if(!dp)
	{
		LeftShift(bn, dn, bp);
	}
	else
	{
		LeftShift(zzTmp, dn, bp);
		if(dp > 0)
		{
			power(zzPow, 10L, dp);
			DivRem(bn, zzRem, zzTmp, zzPow);
			LeftShift(zzTmp, zzRem, 1L);
			if(zzTmp >= zzPow)
				bn += 1;
		}
		else
		{
			power(zzPow, 10L, -dp);
			mul(bn, zzTmp, zzPow);
		}
	}
}

void BinaryNum2DigitalNum(ZZ& dn, long dp, ZZ& bn, long bp)
{
	ZZ zzTmp, zzPow;
	long bitFlag;

	power(zzPow, 10L, dp);
	mul(zzTmp, bn, zzPow);
	bitFlag = bit(zzTmp, bp-1);
	RightShift(dn, zzTmp, bp);
	if(bitFlag)
	{
		if(dn >= 0)
			dn += 1;
		else
			dn -= 1;
	}
}

unsigned char* SemiHomoCipher2PrnStr(unsigned char *PrnStr, SemiHomoCipher& c)
{
	unsigned char HexZZ[MAX_RSAMODULE_BYTELEN];
	long lenHexZZ;
	unsigned char ucVal;

	lenHexZZ = (NumBits(c.c0) + 7) / 8; if(!lenHexZZ) lenHexZZ = 1;
	BytesFromZZ(HexZZ, c.c0, lenHexZZ);
	PrnStr = HexStr2PrnStr(PrnStr, HexZZ, lenHexZZ);
	*PrnStr++ = ':';

	lenHexZZ = (NumBits(c.c1) + 7) / 8; if(!lenHexZZ) lenHexZZ = 1;
	BytesFromZZ(HexZZ, c.c1, lenHexZZ);
	PrnStr = HexStr2PrnStr(PrnStr, HexZZ, lenHexZZ);
	*PrnStr++ = ':';

	ucVal = (c.prec) & 0x0F;
	*PrnStr++ = ucVal+ (ucVal<10 ? '0' : 'A'-10);
	ucVal = (c.prec >> 4) & 0x0F;
	*PrnStr++ = ucVal+ (ucVal<10 ? '0' : 'A'-10);
	ucVal = (c.prec >> 8) & 0x0F;
	*PrnStr++ = ucVal+ (ucVal<10 ? '0' : 'A'-10);
	ucVal = (c.prec >> 12) & 0x0F;
	*PrnStr = ucVal+ (ucVal<10 ? '0' : 'A'-10);
	while(*PrnStr == '0') --PrnStr;
	if(*PrnStr == ':') PrnStr += 2; else PrnStr++;
	*PrnStr = 0;

	return PrnStr;
}

unsigned char* PrnStr2SemiHomoCipher(SemiHomoCipher& c, unsigned char *PrnStr)
{
	unsigned char HexZZ[MAX_RSAMODULE_BYTELEN];
	int lenHexZZ;

	PrnStr = PrnStr2HexStr(HexZZ, lenHexZZ, PrnStr);
	if((!lenHexZZ) || (':' != (*PrnStr))) return 0;
	ZZFromBytes(c.c0, HexZZ, lenHexZZ);

	PrnStr = PrnStr2HexStr(HexZZ, lenHexZZ, PrnStr+1);
	if((!lenHexZZ) || (':' != (*PrnStr))) return 0;
	ZZFromBytes(c.c1, HexZZ, lenHexZZ);

	PrnStr = PrnStr2HexStr(HexZZ, lenHexZZ, PrnStr+1);
	if(lenHexZZ != 1 && lenHexZZ != 2) return 0;

	c.prec = HexZZ[0];
	if(2 == lenHexZZ)
	{
		c.prec += ((unsigned long)(HexZZ[1])) * 256;
	}

	return PrnStr;
}

unsigned char* PrnStr2HexStr(unsigned char *HexStr, int &lenHexStr, unsigned char *PrnStr)
{
	unsigned char ucVal;

	lenHexStr = 0;
	while(ucVal = (*PrnStr++))
	{
		if(ucVal >= '0' && ucVal <= '9')
		{
			HexStr[lenHexStr] = (ucVal - '0');
		}
		else if(ucVal >= 'A' && ucVal <= 'F')
		{
			HexStr[lenHexStr] = (ucVal - 'A' + 10);
		}
		else if(ucVal >= 'a' && ucVal <= 'f')
		{
			HexStr[lenHexStr] = (ucVal - 'a' + 10);
		}
		else
		{
			break;
		}
		ucVal = (*PrnStr++);
		if(ucVal >= '0' && ucVal <= '9')
		{
			HexStr[lenHexStr++] += (ucVal - '0') * 16;
		}
		else if(ucVal >= 'A' && ucVal <= 'F')
		{
			HexStr[lenHexStr++] += (ucVal - 'A' + 10) * 16;
		}
		else if(ucVal >= 'a' && ucVal <= 'f')
		{
			HexStr[lenHexStr++] += (ucVal - 'a' + 10) * 16;
		}
		else
		{
			lenHexStr++;
			break;
		}
	}

	return PrnStr - 1;
}

unsigned char* HexStr2PrnStr(unsigned char *PrnStr, unsigned char *HexStr, int lenHexStr)
{
	unsigned char ucVal, ucTmp;

	do{
		ucVal = *HexStr++;
		ucTmp = ucVal>>4; ucVal &= 0x0F;
		*PrnStr++ = ucVal + ((ucVal < 10) ? '0' : 'A' - 10);
		*PrnStr++ = ucTmp + ((ucTmp < 10) ? '0' : 'A' - 10);
	}while(--lenHexStr);

	return (ucTmp ? PrnStr : PrnStr -1);
}

int SetAgentKeyFromPsw(unsigned char *pswKey, unsigned int pswKeyLen)
{
	ZZ subK[3], subKInv[3], zzN;

	//agent generate paillier key, and calculate subkeys for servers
	if(!mAgent.GenerateKeyFromPsw(pswKey, pswKeyLen))
	{
		strcpy(strErrMsg, mAgent.GetLastError());
		return 0;
	}

	if(!mAgent.GetDecryptSubKey23(subK, subKInv))
	{
		strcpy(strErrMsg, mAgent.GetLastError());
		return 0;
	}

	if(!mAgent.GetPubKey(zzN))
	{
		strcpy(strErrMsg, mAgent.GetLastError());
		return 0;
	}

	//agent should send (subK[ID_x], subKInv[ID_x], zzN) to the corresponding server.

	//after receive the (subK, subKInv, zzN) from agent, each server install them.
	mMainServer.SetDecryptSubKey23(ID_MainServer, subK[ID_MainServer-1], subKInv[ID_MainServer-1]);
	mMainServer.SetPubKey(zzN);

	mAux1Server.SetDecryptSubKey23(ID_Aux1Server, subK[ID_Aux1Server-1], subKInv[ID_Aux1Server-1]);
	mAux1Server.SetPubKey(zzN);

	mAux2Server.SetDecryptSubKey23(ID_Aux2Server, subK[ID_Aux2Server-1], subKInv[ID_Aux2Server-1]);
	mAux2Server.SetPubKey(zzN);

	return 1;
}

int EncryptSemiHomoPlain(SemiHomoCipher& c, SemiHomoPlain& p)
{
	ZZ pz;

	//server will never see plaintext, so encryption occurs only on agent.
	if(!mAgent.NormalZZ2PaillierZZ(pz, p.m))
	{
		strcpy(strErrMsg, mAgent.GetLastError());
		return 0;
	}

	c.prec = p.prec;
	return EncryptRawData(c.c0, c.c1, pz);
}

int DecryptSemiHomoCipher(SemiHomoPlain& p, SemiHomoCipher& c)
{
	ZZ pz;

	//server will never see plaintext, so encryption occurs only on agent.
	if(!DecryptRawData(pz, c.c0, c.c1)) return 0;
	p.prec = c.prec;
	if(!mAgent.PaillierZZ2NormalZZ(p.m, pz))
	{
		strcpy(strErrMsg, mAgent.GetLastError());
		return 0;
	}

	return 1;
}

int EncryptSemiHomoPlainVector(vector<SemiHomoCipher> &vc, vector<SemiHomoPlain> &vp)
{
	vector<SemiHomoPlain>::iterator it;
	SemiHomoCipher c;

	vc.clear();
	for(it=vp.begin(); it!=vp.end(); it++)
	{
		if(!EncryptSemiHomoPlain(c, *it))
		{
			vc.clear();
			return 0;
		}
		vc.push_back(c);
	}

	return 1;
}

int DecryptSemiHomoCipherVector(vector<SemiHomoPlain> &vp, vector<SemiHomoCipher> &vc)
{
	vector<SemiHomoCipher>::iterator it;
	SemiHomoPlain p;

	vp.clear();
	for(it=vc.begin(); it!=vc.end(); it++)
	{
		if(!DecryptSemiHomoCipher(p, *it))
		{
			vp.clear();
			return 0;
		}
		vp.push_back(p);
	}

	return 1;
}

int EncryptSemiHomoPlainPair(SemiHomoCipherPair& cp, SemiHomoPlainPair& pp)
{
	if(!EncryptSemiHomoPlain(cp.x, pp.x)) return 0;

	return EncryptSemiHomoPlain(cp.y, pp.y);
}

int DecryptSemiHomoCipherPair(SemiHomoPlainPair& pp, SemiHomoCipherPair& cp)
{
	if(!DecryptSemiHomoCipher(pp.x, cp.x)) return 0;

	return DecryptSemiHomoCipher(pp.y, cp.y);
}

int EncryptSemiHomoPlainPairVector(vector<SemiHomoCipherPair> &vcp, vector<SemiHomoPlainPair> &vpp)
{
	vector<SemiHomoPlainPair>::iterator it;
	SemiHomoCipherPair cp;

	vcp.clear();
	for(it=vpp.begin(); it!=vpp.end(); it++)
	{
		if(!EncryptSemiHomoPlainPair(cp, *it))
		{
			vcp.clear();
			return 0;
		}
		vcp.push_back(cp);
	}

	return 1;
}

int AddSemiHomoPlain(SemiHomoPlain& pr, SemiHomoPlain& pa, SemiHomoPlain& pb)
{
	if(pa.prec > pb.prec)
	{
		pr.prec = pa.prec;
		LeftShift(pr.m, pb.m, pa.prec - pb.prec);
		pr.m += pa.m;
	}
	else if(pa.prec < pb.prec)
	{
		pr.prec = pb.prec;
		LeftShift(pr.m, pa.m, pb.prec - pa.prec);
		pr.m += pb.m;
	}
	else
	{
		pr.prec = pa.prec;
		add(pr.m, pa.m, pb.m);
	}

	return 1;
}

int NegSemiHomoPlain(SemiHomoPlain& pr, SemiHomoPlain& pa)
{
	pr.prec = pa.prec;
	pr.m = -pa.m;

	return 1;
}

int LeftShiftSemiHomoPlain(SemiHomoPlain& pr, SemiHomoPlain& pa, long shiftNum)
{
	pr.prec = pa.prec;
	LeftShift(pr.m, pa.m, shiftNum);

	return 1;
}

int ScalarSemiHomoPlain(SemiHomoPlain& pr, SemiHomoPlain& pa, ZZ& scalar)
{
	pr.prec = pa.prec;

	mul(pr.m, pa.m, scalar);
	return 1;
}

int DivSemiHomoPlain(SemiHomoPlain& pr, SemiHomoPlain& pa, SemiHomoPlain& pb)
{
	SemiHomoPlain pv;

	if(!InvSemiHomoPlain(pv, pb)) return 0;

	return MulSemiHomoPlain(pr, pa, pv);
}

int InvSemiHomoPlain(SemiHomoPlain& pr, SemiHomoPlain& pa)
{
	long p;
	ZZ pow, rem;

	if(pa.m == 0)
	{
		sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "InvSemiHomoPlain: zero has no inverse");
		return 0;
	}

	p = NumBits(pa.m);
	if(p <= pa.prec)
	{
		p = 2 * pa.prec;
		pr.prec = pa.prec;
		power(pow, 2L, p);
	}
	else
	{
		pr.prec = p;
		p += pa.prec;
		power(pow, 2L, p);
	}

	DivRem(pr.m, rem, pow, pa.m);
	LeftShift(pow, rem, 1L);
	if(pa.m > 0)
	{
		if(pow >= pa.m)
			pr.m += 1;
	}
	else
	{
		if(pow <= pa.m)
			pr.m += 1;
	}

	return 1;
}

int ScalarSemiHomoCipher(SemiHomoCipher& cr, SemiHomoCipher& ca, ZZ& scalar)
{
	cr.prec = ca.prec;

	return HomoScalarOnEncryptedRawData(cr.c0, cr.c1, ca.c0, ca.c1, scalar);
}

int LeftShiftSemiHomoCipher(SemiHomoCipher& cr, SemiHomoCipher& ca, long shiftNum)
{
	cr.prec = ca.prec;

	return HomoLeftShiftOnEncryptedRawData(cr.c0, cr.c1, ca.c0, ca.c1, shiftNum);
}

int MulSemiHomoPlain(SemiHomoPlain& pr, SemiHomoPlain& pa, SemiHomoPlain& pb)
{
	pr.prec = pa.prec + pb.prec;

	mul(pr.m, pa.m, pb.m);

	return 1;
}

int AddSemiHomoCipher(SemiHomoCipher& cr, SemiHomoCipher& ca, SemiHomoCipher& cb)
{
	ZZ t0, t1;

	if(ca.prec > cb.prec)
	{
		cr.prec = ca.prec;
		if(!HomoLeftShiftOnEncryptedRawData(t0, t1, cb.c0, cb.c1, ca.prec - cb.prec)) return 0;
		return HomoAddOnEncryptedRawData(cr.c0, cr.c1, ca.c0, ca.c1, t0, t1);
	}
	else if(ca.prec < cb.prec)
	{
		cr.prec = cb.prec;
		if(!HomoLeftShiftOnEncryptedRawData(t0, t1, ca.c0, ca.c1, cb.prec - ca.prec)) return 0;
		return HomoAddOnEncryptedRawData(cr.c0, cr.c1, cb.c0, cb.c1, t0, t1);
	}
	else
	{
		cr.prec = ca.prec;
		return HomoAddOnEncryptedRawData(cr.c0, cr.c1, ca.c0, ca.c1, cb.c0, cb.c1);
	}
}

int NegSemiHomoCipher(SemiHomoCipher& cr, SemiHomoCipher& ca)
{
	cr.prec = ca.prec;

	return HomoNegOnEncryptedRawData(cr.c0, cr.c1, ca.c0, ca.c1);
}

int MulSemiHomoCipher(SemiHomoCipher& cr, SemiHomoCipher& ca, SemiHomoCipher& cb)
{
	cr.prec = ca.prec + cb.prec;
	return HomoMulOnEncryptedRawData(cr.c0, cr.c1, ca.c0, ca.c1, cb.c0, cb.c1);
}

int SumSemiHomoPlainVector(SemiHomoPlain& pr, vector<SemiHomoPlain> & vp)
{
	vector<SemiHomoPlain>::iterator it;
	SemiHomoPlain ps;
	int nNum;

	pr.prec = 0; pr.m = 0L;
	for(it=vp.begin(), nNum=0; it!=vp.end(); it++, nNum++)
	{
		if(0==nNum)
		{
			ps.m = it->m;
			ps.prec = it->prec;
		}
		else if(nNum&1)
		{
			if(!AddSemiHomoPlain(pr, ps, *it)) return 0;
		}
		else
		{
			if(!AddSemiHomoPlain(ps, pr, *it)) return 0;
		}
	}

	if(nNum&1)
	{
		pr.prec = ps.prec;
		pr.m = ps.m;
	}

	return 1;
}

int AverageSemiHomoPlainVector(SemiHomoPlain& pr, vector<SemiHomoPlain> & vp)
{
	SemiHomoPlain ps;
	ZZ pow, scalar;
	long nNum, r;

	if(!SumSemiHomoPlainVector(ps, vp)) return 0;

	if((nNum = vp.size()) < 2)
	{
		pr.m = ps.m;
		pr.prec = ps.prec;
		return 1;
	}

	power(pow, 2, FP_PRECISE);
	r = DivRem(scalar, pow, nNum);
	if(r+r >= nNum) scalar += 1;
	if(!ScalarSemiHomoPlain(pr, ps, scalar)) return 0;

	pr.prec += FP_PRECISE;
	return 1;
}

int VarianceSemiHomoPlainVector(SemiHomoPlain& pr, vector<SemiHomoPlain> & vp)
{
	vector<SemiHomoPlain>::iterator it;
	SemiHomoPlain ps0, ps1, ps2, ps3, pi;
	long nNum, r;
	ZZ pow, scalar;

	if((nNum = vp.size()) < 2)
	{
		pr.prec = 0; pr.m = 0L;
		return 1;
	}

	it = vp.begin();
	if(!MulSemiHomoPlain(ps3, *it, *it)) return 0;
	ps1.m = it->m; ps1.prec = it->prec;
	it++;
	if(!MulSemiHomoPlain(pi, *it, *it)) return 0;
	if(!AddSemiHomoPlain(ps2, ps3, pi)) return 0;
	if(!AddSemiHomoPlain(ps0, ps1, *it)) return 0;
	it++;

	for(nNum=2; it!=vp.end(); it++, nNum++)
	{
		if(!MulSemiHomoPlain(pi, *it, *it)) return 0;
		if(nNum&1)
		{
			if(!AddSemiHomoPlain(ps2, ps3, pi)) return 0;
			if(!AddSemiHomoPlain(ps0, ps1, *it)) return 0;
		}
		else
		{
			if(!AddSemiHomoPlain(ps3, ps2, pi)) return 0;
			if(!AddSemiHomoPlain(ps1, ps0, *it)) return 0;
		}
	}

	scalar = nNum;
	if(nNum&1)
	{
		if(!ScalarSemiHomoPlain(pr, ps3, scalar)) return 0;
		if(!MulSemiHomoPlain(pi, ps1, ps1)) return 0;
	}
	else
	{
		if(!ScalarSemiHomoPlain(pr, ps2, scalar)) return 0;
		if(!MulSemiHomoPlain(pi, ps0, ps0)) return 0;
	}
	if(!NegSemiHomoPlain(ps0, pi)) return 0;
	if(!AddSemiHomoPlain(ps2, pr, ps0)) return 0;

	power(pow, 2, FP_PRECISE);
	nNum *= nNum;
	r = DivRem(scalar, pow, nNum);
	if(r+r >= nNum) scalar += 1;
	if(!ScalarSemiHomoPlain(pr, ps2, scalar)) return 0;

	pr.prec += FP_PRECISE;

	return 1;
}

int SumSemiHomoCipherVector(SemiHomoCipher& cr, vector<SemiHomoCipher> & vc)
{
	vector<SemiHomoCipher>::iterator it;
	SemiHomoCipher cs;
	int nNum;

	cr.prec = 0; cr.c0 = 1L; cr.c1 = 0L;
	for(it=vc.begin(), nNum=0; it!=vc.end(); it++, nNum++)
	{
		if(0==nNum)
		{
			cs.c0 = it->c0;
			cs.c1 = it->c1;
			cs.prec = it->prec;
		}
		else if(nNum&1)
		{
			if(!AddSemiHomoCipher(cr, cs, *it)) return 0;
		}
		else
		{
			if(!AddSemiHomoCipher(cs, cr, *it)) return 0;
		}
	}

	if(nNum&1)
	{
		cr.prec = cs.prec;
		cr.c0 = cs.c0;
		cr.c1 = cs.c1;
	}

	return 1;
}

int AverageSemiHomoCipherVector(SemiHomoCipher& cr, vector<SemiHomoCipher> & vc)
{
	SemiHomoCipher cs;
	ZZ pow, scalar;
	long nNum, r;

	if(!SumSemiHomoCipherVector(cs, vc)) return 0;

	if((nNum = vc.size()) < 2)
	{
		cr.c0 = cs.c0;
		cr.c1 = cs.c1;
		cr.prec = cs.prec;
		return 1;
	}

	power(pow, 2, FP_PRECISE);
	r = DivRem(scalar, pow, nNum);
	if(r+r >= nNum) scalar += 1;
	if(!ScalarSemiHomoCipher(cr, cs, scalar)) return 0;

	cr.prec += FP_PRECISE;
	return 1;
}

int VarianceSemiHomoCipherVector(SemiHomoCipher& cr, vector<SemiHomoCipher> & vc)
{
	vector<SemiHomoCipher>::iterator it;
	SemiHomoCipher cs0, cs1, cs2, cs3, ci;
	long nNum, r;
	ZZ pow, scalar;

	if((nNum = vc.size()) < 2)
	{
		cr.prec = 0; cr.c0 = 1L; cr.c1 = 0L;
		return 1;
	}

	it = vc.begin();
	if(!MulSemiHomoCipher(cs3, *it, *it)) return 0;
	cs1.c0 = it->c0; cs1.c1 = it->c1; cs1.prec = it->prec;
	it++;
	if(!MulSemiHomoCipher(ci, *it, *it)) return 0;
	if(!AddSemiHomoCipher(cs2, cs3, ci)) return 0;
	if(!AddSemiHomoCipher(cs0, cs1, *it)) return 0;
	it++;

	for(nNum=2; it!=vc.end(); it++, nNum++)
	{
		if(!MulSemiHomoCipher(ci, *it, *it)) return 0;
		if(nNum&1)
		{
			if(!AddSemiHomoCipher(cs2, cs3, ci)) return 0;
			if(!AddSemiHomoCipher(cs0, cs1, *it)) return 0;
		}
		else
		{
			if(!AddSemiHomoCipher(cs3, cs2, ci)) return 0;
			if(!AddSemiHomoCipher(cs1, cs0, *it)) return 0;
		}
	}

	scalar = nNum;
	if(nNum&1)
	{
		if(!ScalarSemiHomoCipher(cr, cs3, scalar)) return 0;
		if(!MulSemiHomoCipher(ci, cs1, cs1)) return 0;
	}
	else
	{
		if(!ScalarSemiHomoCipher(cr, cs2, scalar)) return 0;
		if(!MulSemiHomoCipher(ci, cs0, cs0)) return 0;
	}
	if(!NegSemiHomoCipher(cs0, ci)) return 0;
	if(!AddSemiHomoCipher(cs2, cr, cs0)) return 0;

	power(pow, 2, FP_PRECISE);
	nNum *= nNum;
	r = DivRem(scalar, pow, nNum);
	if(r+r >= nNum) scalar += 1;
	if(!ScalarSemiHomoCipher(cr, cs2, scalar)) return 0;

	cr.prec += FP_PRECISE;

	return 1;
}

int LinearRegressionSemiHomoPlainPairVector(SemiHomoPlain& B, SemiHomoPlain& C, vector<SemiHomoPlainPair> & vpp)
{
	vector<SemiHomoPlainPair>::iterator it;
	SemiHomoPlain xs0, xs1, ys0, ys1, x2s0, x2s1, xys0, xys1, x2, xy, A;
	long nNum;
	ZZ scalar;

	if((nNum = vpp.size()) < 2)
	{
		sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "LinearRegressionSemiHomoPlainPairVector: input must contain at least two elements");
		return 0;
	}

	it = vpp.begin();
	if(!MulSemiHomoPlain(x2s1, it->x, it->x)) return 0;
	if(!MulSemiHomoPlain(xys1, it->x, it->y)) return 0;
	xs1.m = it->x.m; xs1.prec = it->x.prec;
	ys1.m = it->y.m; ys1.prec = it->y.prec;
	it++;
	if(!MulSemiHomoPlain(x2, it->x, it->x)) return 0;
	if(!MulSemiHomoPlain(xy, it->x, it->y)) return 0;
	if(!AddSemiHomoPlain(x2s0, x2s1, x2)) return 0;
	if(!AddSemiHomoPlain(xys0, xys1, xy)) return 0;
	if(!AddSemiHomoPlain(xs0, xs1, it->x)) return 0;
	if(!AddSemiHomoPlain(ys0, ys1, it->y)) return 0;
	it++;

	for(nNum=2; it!=vpp.end(); it++, nNum++)
	{
		if(!MulSemiHomoPlain(x2, it->x, it->x)) return 0;
		if(!MulSemiHomoPlain(xy, it->x, it->y)) return 0;
		if(nNum&1)
		{
			if(!AddSemiHomoPlain(x2s0, x2s1, x2)) return 0;
			if(!AddSemiHomoPlain(xys0, xys1, xy)) return 0;
			if(!AddSemiHomoPlain(xs0, xs1, it->x)) return 0;
			if(!AddSemiHomoPlain(ys0, ys1, it->y)) return 0;
		}
		else
		{
			if(!AddSemiHomoPlain(x2s1, x2s0, x2)) return 0;
			if(!AddSemiHomoPlain(xys1, xys0, xy)) return 0;
			if(!AddSemiHomoPlain(xs1, xs0, it->x)) return 0;
			if(!AddSemiHomoPlain(ys1, ys0, it->y)) return 0;
		}
	}

	scalar = nNum;
	if(nNum&1)
	{
		if(!ScalarSemiHomoPlain(x2s0, x2s1, scalar)) return 0;
		if(!MulSemiHomoPlain(xs0, xs1, xs1)) return 0;
		if(!NegSemiHomoPlain(ys0, xs0)) return 0;
		if(!AddSemiHomoPlain(A, x2s0, ys0)) return 0;

		if(!ScalarSemiHomoPlain(xys0, xys1, scalar)) return 0;
		if(!MulSemiHomoPlain(xs0, xs1, ys1)) return 0;
		if(!NegSemiHomoPlain(ys0, xs0)) return 0;
		if(!AddSemiHomoPlain(xs0, xys0, ys0)) return 0;
		if(!DivSemiHomoPlain(B, xs0, A)) return 0;

		if(!MulSemiHomoPlain(x2s0, x2s1, ys1)) return 0;
		if(!MulSemiHomoPlain(xs0, xs1, xys1)) return 0;
		if(!NegSemiHomoPlain(ys0, xs0)) return 0;
		if(!AddSemiHomoPlain(xs0, x2s0, ys0)) return 0;
		if(!DivSemiHomoPlain(C, xs0, A)) return 0;
	}
	else
	{
		if(!ScalarSemiHomoPlain(x2s1, x2s0, scalar)) return 0;
		if(!MulSemiHomoPlain(xs1, xs0, xs0)) return 0;
		if(!NegSemiHomoPlain(ys1, xs1)) return 0;
		if(!AddSemiHomoPlain(A, x2s1, ys1)) return 0;

		if(!ScalarSemiHomoPlain(xys1, xys0, scalar)) return 0;
		if(!MulSemiHomoPlain(xs1, xs0, ys0)) return 0;
		if(!NegSemiHomoPlain(ys1, xs1)) return 0;
		if(!AddSemiHomoPlain(xs1, xys1, ys1)) return 0;
		if(!DivSemiHomoPlain(B, xs1, A)) return 0;

		if(!MulSemiHomoPlain(x2s1, x2s0, ys0)) return 0;
		if(!MulSemiHomoPlain(xs1, xs0, xys0)) return 0;
		if(!NegSemiHomoPlain(ys1, xs1)) return 0;
		if(!AddSemiHomoPlain(xs1, x2s1, ys1)) return 0;
		if(!DivSemiHomoPlain(C, xs1, A)) return 0;
	}

	return 1;
}

int LinearRegressionSemiHomoCipherPairVector(SemiHomoCipher& B, SemiHomoCipher& C, vector<SemiHomoCipherPair> & vcp)
{
	vector<SemiHomoCipherPair>::iterator it;
	SemiHomoCipher xs0, xs1, ys0, ys1, x2s0, x2s1, xys0, xys1, x2, xy, A;
	long nNum;
	ZZ scalar;

	if((nNum = vcp.size()) < 2)
	{
		sprintf(strErrMsg, "[%s::%d] %s\n", __FILE__, __LINE__, "LinearRegressionSemiHomoCipherPairVector: input must contain at least two elements");
		return 0;
	}

	it = vcp.begin();
	if(!MulSemiHomoCipher(x2s1, it->x, it->x)) return 0;
	if(!MulSemiHomoCipher(xys1, it->x, it->y)) return 0;
	xs1.c0 = it->x.c0; xs1.c1 = it->x.c1; xs1.prec = it->x.prec;
	ys1.c0 = it->y.c0; ys1.c1 = it->y.c1; ys1.prec = it->y.prec;
	it++;
	if(!MulSemiHomoCipher(x2, it->x, it->x)) return 0;
	if(!MulSemiHomoCipher(xy, it->x, it->y)) return 0;
	if(!AddSemiHomoCipher(x2s0, x2s1, x2)) return 0;
	if(!AddSemiHomoCipher(xys0, xys1, xy)) return 0;
	if(!AddSemiHomoCipher(xs0, xs1, it->x)) return 0;
	if(!AddSemiHomoCipher(ys0, ys1, it->y)) return 0;
	it++;

	for(nNum=2; it!=vcp.end(); it++, nNum++)
	{
		if(!MulSemiHomoCipher(x2, it->x, it->x)) return 0;
		if(!MulSemiHomoCipher(xy, it->x, it->y)) return 0;
		if(nNum&1)
		{
			if(!AddSemiHomoCipher(x2s0, x2s1, x2)) return 0;
			if(!AddSemiHomoCipher(xys0, xys1, xy)) return 0;
			if(!AddSemiHomoCipher(xs0, xs1, it->x)) return 0;
			if(!AddSemiHomoCipher(ys0, ys1, it->y)) return 0;
		}
		else
		{
			if(!AddSemiHomoCipher(x2s1, x2s0, x2)) return 0;
			if(!AddSemiHomoCipher(xys1, xys0, xy)) return 0;
			if(!AddSemiHomoCipher(xs1, xs0, it->x)) return 0;
			if(!AddSemiHomoCipher(ys1, ys0, it->y)) return 0;
		}
	}

	scalar = nNum;
	if(nNum&1)
	{
		if(!ScalarSemiHomoCipher(x2s0, x2s1, scalar)) return 0;
		if(!MulSemiHomoCipher(xs0, xs1, xs1)) return 0;
		if(!NegSemiHomoCipher(ys0, xs0)) return 0;
		if(!AddSemiHomoCipher(A, x2s0, ys0)) return 0;

		if(!ScalarSemiHomoCipher(xys0, xys1, scalar)) return 0;
		if(!MulSemiHomoCipher(xs0, xs1, ys1)) return 0;
		if(!NegSemiHomoCipher(ys0, xs0)) return 0;
		if(!AddSemiHomoCipher(xs0, xys0, ys0)) return 0;
		if(!DivSemiHomoCipher(B, xs0, A)) return 0;

		if(!MulSemiHomoCipher(x2s0, x2s1, ys1)) return 0;
		if(!MulSemiHomoCipher(xs0, xs1, xys1)) return 0;
		if(!NegSemiHomoCipher(ys0, xs0)) return 0;
		if(!AddSemiHomoCipher(xs0, x2s0, ys0)) return 0;
		if(!DivSemiHomoCipher(C, xs0, A)) return 0;
	}
	else
	{
		if(!ScalarSemiHomoCipher(x2s1, x2s0, scalar)) return 0;
		if(!MulSemiHomoCipher(xs1, xs0, xs0)) return 0;
		if(!NegSemiHomoCipher(ys1, xs1)) return 0;
		if(!AddSemiHomoCipher(A, x2s1, ys1)) return 0;

		if(!ScalarSemiHomoCipher(xys1, xys0, scalar)) return 0;
		if(!MulSemiHomoCipher(xs1, xs0, ys0)) return 0;
		if(!NegSemiHomoCipher(ys1, xs1)) return 0;
		if(!AddSemiHomoCipher(xs1, xys1, ys1)) return 0;
		if(!DivSemiHomoCipher(B, xs1, A)) return 0;

		if(!MulSemiHomoCipher(x2s1, x2s0, ys0)) return 0;
		if(!MulSemiHomoCipher(xs1, xs0, xys0)) return 0;
		if(!NegSemiHomoCipher(ys1, xs1)) return 0;
		if(!AddSemiHomoCipher(xs1, x2s1, ys1)) return 0;
		if(!DivSemiHomoCipher(C, xs1, A)) return 0;
	}

	return 1;
}

int DecryptSemiHomoCipherPairVector(vector<SemiHomoPlainPair> &vpp, vector<SemiHomoCipherPair> &vcp)
{
	vector<SemiHomoCipherPair>::iterator it;
	SemiHomoPlainPair pp;

	vpp.clear();
	for(it=vcp.begin(); it!=vcp.end(); it++)
	{
		if(!DecryptSemiHomoCipherPair(pp, *it))
		{
			vpp.clear();
			return 0;
		}
		vpp.push_back(pp);
	}

	return 1;
}

int EncryptRawData(ZZ& c0, ZZ& c1, ZZ& m)
{
	//server will never see plaintext, so encryption occurs only on agent.
	if(!mAgent.Encrypt(c0, c1, m))
	{
		strcpy(strErrMsg, mAgent.GetLastError());
		return 0;
	}

	return 1;
}

int DecryptRawData(ZZ& m, ZZ& c0, ZZ& c1)
{
	//server will never see plaintext, so decryption occurs only on agent.
	if(!mAgent.Decrypt(m, c0, c1))
	{
		strcpy(strErrMsg, mAgent.GetLastError());
		return 0;
	}

	return 1;
}

int HomoAddOnEncryptedRawData(ZZ& z0, ZZ& z1, ZZ& x0, ZZ& x1, ZZ& y0, ZZ& y1)
{
	//agent forward (x0, x1) and (y0, y1) to main_server.

	//main_server do the calculation
	if(!mMainServer.HomoAdd(z0, z1, x0, x1, y0, y1))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}

	//main_server return (z0, z1) back to agent

	return 1;
}

int HomoNegOnEncryptedRawData(ZZ& z0, ZZ& z1, ZZ& x0, ZZ& x1)
{
	//agent forward (x0, x1) to main_server.

	//main_server do the calculation
	if(!mMainServer.HomoNeg(z0, z1, x0, x1))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}

	//main_server return (z0, z1) back to agent

	return 1;
}

int HomoScalarOnEncryptedRawData(ZZ& z0, ZZ& z1, ZZ& x0, ZZ& x1, ZZ& scalar)
{
	//agent forward (x0, x1) and scalar to main_server.

	//main_server do the calculation
	if(!mMainServer.HomoScalar(z0, z1, x0, x1, scalar))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}

	//main_server return (z0, z1) back to agent

	return 1;
}

int HomoLeftShiftOnEncryptedRawData(ZZ& z0, ZZ& z1, ZZ& x0, ZZ& x1, long shiftNum)
{
	//agent forward (x0, x1) and shiftNum to main_server.

	//main_server do the calculation
	if(!mMainServer.HomoLeftShift(z0, z1, x0, x1, shiftNum))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}

	//main_server return (z0, z1) back to agent

	return 1;
}

int HomoMulOnEncryptedRawData(ZZ& z0, ZZ& z1, ZZ& x0, ZZ& x1, ZZ& y0, ZZ& y1)
{
	ZZ s0_Main[3], s1_Main[3], fa0_Main, fa1_Main, fb0_Main, fb1_Main, y_Main;
	ZZ s0_Aux1[3], s1_Aux1[3], fa0_Aux1, fa1_Aux1, fb0_Aux1, fb1_Aux1, y_Aux1;
	ZZ s0_Aux2[3], s1_Aux2[3], fa0_Aux2, fa1_Aux2, fb0_Aux2, fb1_Aux2, y_Aux2;

	//agent forward (x0, x1) and (y0, y1) to main_server.

	//main_server forward (x0, x1) and (y0, y1) to aux_server_1.

	//main_server forward (x0, x1) to aux_server_2.

	//main_server/aux_server_1/aux_server_2 collaborate to decrypt_and_share the plaintext of (y0, y1), the plaintext is never seen by any server.
	
	//main_server calculate (y0+y1*N)^{subK} and share the result
	if(!mMainServer.DecryptAndSharePhase0(Msk_MainServer|Msk_Aux1Server, s0_Main, s1_Main, y0, y1))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}
	//main_server will send s0_Main[ID_x], s1_Main[ID_x] to the corresponding server.
	fa0_Main = s0_Main[ID_MainServer-1]; fa1_Main = s1_Main[ID_MainServer-1];
	fa0_Aux1 = s0_Main[ID_Aux1Server-1]; fa1_Aux1 = s1_Main[ID_Aux1Server-1];
	fa0_Aux2 = s0_Main[ID_Aux2Server-1]; fa1_Aux2 = s1_Main[ID_Aux2Server-1];

	//aux_server_1 calculate (y0+y1*N)^{subK} and share the result
	if(!mAux1Server.DecryptAndSharePhase0(Msk_MainServer|Msk_Aux1Server, s0_Aux1, s1_Aux1, y0, y1))
	{
		strcpy(strErrMsg, mAux1Server.GetLastError());
		return 0;
	}
	//aux_server_1 will send s0_Main[ID_x], s1_Main[ID_x] to the corresponding server.
	fb0_Main = s0_Aux1[ID_MainServer-1]; fb1_Main = s1_Aux1[ID_MainServer-1];
	fb0_Aux1 = s0_Aux1[ID_Aux1Server-1]; fb1_Aux1 = s1_Aux1[ID_Aux1Server-1];
	fb0_Aux2 = s0_Aux1[ID_Aux2Server-1]; fb1_Aux2 = s1_Aux1[ID_Aux2Server-1];

	//each server calculate k*fa*fb = x*n + y, share x and publish y
	if(!mMainServer.DecryptAndSharePhase1(s1_Main, y_Main, fa0_Main, fa1_Main, fb0_Main, fb1_Main))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}
	s0_Main[ID_MainServer-1] = s1_Main[ID_MainServer-1];
	s0_Aux1[ID_MainServer-1] = s1_Main[ID_Aux1Server-1];
	s0_Aux2[ID_MainServer-1] = s1_Main[ID_Aux2Server-1];

	if(!mAux1Server.DecryptAndSharePhase1(s1_Aux1, y_Aux1, fa0_Aux1, fa1_Aux1, fb0_Aux1, fb1_Aux1))
	{
		strcpy(strErrMsg, mAux1Server.GetLastError());
		return 0;
	}
	s0_Main[ID_Aux1Server-1] = s1_Aux1[ID_MainServer-1];
	s0_Aux1[ID_Aux1Server-1] = s1_Aux1[ID_Aux1Server-1];
	s0_Aux2[ID_Aux1Server-1] = s1_Aux1[ID_Aux2Server-1];

	if(!mAux2Server.DecryptAndSharePhase1(s1_Aux2, y_Aux2, fa0_Aux2, fa1_Aux2, fb0_Aux2, fb1_Aux2))
	{
		strcpy(strErrMsg, mAux2Server.GetLastError());
		return 0;
	}
	s0_Main[ID_Aux2Server-1] = s1_Aux2[ID_MainServer-1];
	s0_Aux1[ID_Aux2Server-1] = s1_Aux2[ID_Aux1Server-1];
	s0_Aux2[ID_Aux2Server-1] = s1_Aux2[ID_Aux2Server-1];

	//each server get its share of the plaintext of (y0, y1)
	if(!mMainServer.DecryptAndSharePhase2(fa0_Main, s0_Main[ID_MainServer-1], s0_Main[ID_Aux1Server-1], s0_Main[ID_Aux2Server-1], y_Main, y_Aux1, y_Aux2))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}

	if(!mAux1Server.DecryptAndSharePhase2(fa0_Aux1, s0_Aux1[ID_MainServer-1], s0_Aux1[ID_Aux1Server-1], s0_Aux1[ID_Aux2Server-1], y_Main, y_Aux1, y_Aux2))
	{
		strcpy(strErrMsg, mAux1Server.GetLastError());
		return 0;
	}

	if(!mAux2Server.DecryptAndSharePhase2(fa0_Aux2, s0_Aux2[ID_MainServer-1], s0_Aux2[ID_Aux1Server-1], s0_Aux2[ID_Aux2Server-1], y_Main, y_Aux1, y_Aux2))
	{
		strcpy(strErrMsg, mAux2Server.GetLastError());
		return 0;
	}

	//each server calculate (x0+x1*N)^{fa0_xxx} and forward it to the main_server
	if(!mMainServer.HomoScalar(fb0_Main, fb1_Main, x0, x1, fa0_Main))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}

	if(!mAux1Server.HomoScalar(fb0_Aux1, fb1_Aux1, x0, x1, fa0_Aux1))
	{
		strcpy(strErrMsg, mAux1Server.GetLastError());
		return 0;
	}

	if(!mAux2Server.HomoScalar(fb0_Aux2, fb1_Aux2, x0, x1, fa0_Aux2))
	{
		strcpy(strErrMsg, mAux1Server.GetLastError());
		return 0;
	}

	//main_server calculate the ciphertext of the multiplication
	if(!mMainServer.HomoAdd(fa0_Main, fa1_Main, fb0_Main, fb1_Main, fb0_Aux1, fb1_Aux1))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}
	if(!mMainServer.HomoAdd(z0, z1, fa0_Main, fa1_Main, fb0_Aux2, fb1_Aux2))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}

	//main_server return (z0, z1) as the resultant ciphertext back to agent

	return 1;
}

int InvSemiHomoCipher(SemiHomoCipher& cr, SemiHomoCipher& ca)
{
	ZZ y0, y1, u0, u1;
	ZZ s0_Main[3], s1_Main[3], fa0_Main, fa1_Main, fb0_Main, fb1_Main, y_Main;
	ZZ s0_Aux1[3], s1_Aux1[3], fa0_Aux1, fa1_Aux1, fb0_Aux1, fb1_Aux1, y_Aux1;
	ZZ s0_Aux2[3], s1_Aux2[3], fa0_Aux2, fa1_Aux2, fb0_Aux2, fb1_Aux2, y_Aux2;
	long bitPrec;

	//agent forward ca to main_server.

	//main_server forward ca to aux_server_1.

	//main_server/aux_server_1 generate random value, then encrypt it and use it as scalar to HomoScalar, then broadcast the ciphertexts.
	if(!mMainServer.GetRandomZZWithFixBits(y_Main, FP_BITSIZE))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}
	if(!mMainServer.Encrypt(fa0_Main, fa1_Main, y_Main))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}
	if(!mMainServer.HomoScalar(fb0_Main, fb1_Main, ca.c0, ca.c1, y_Main))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}

	if(!mAux1Server.GetRandomZZWithFixBits(y_Aux1, FP_BITSIZE))
	{
		strcpy(strErrMsg, mAux1Server.GetLastError());
		return 0;
	}
	if(!mAux1Server.Encrypt(fa0_Aux1, fa1_Aux1, y_Aux1))
	{
		strcpy(strErrMsg, mAux1Server.GetLastError());
		return 0;
	}
	if(!mAux1Server.HomoScalar(fb0_Aux1, fb1_Aux1, ca.c0, ca.c1, y_Aux1))
	{
		strcpy(strErrMsg, mAux1Server.GetLastError());
		return 0;
	}

	//main_server/aux_server_1 locally calculate the following ciphertexts
	if(!mMainServer.HomoAdd(u0, u1, fa0_Main, fa1_Main, fa0_Aux1, fa1_Aux1))
	{//ciphertext corresponding to share_generated_random_number
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}
	if(!mMainServer.HomoAdd(y0, y1, fb0_Main, fb1_Main, fb0_Aux1, fb1_Aux1))
	{//ciphertext corresponding to (share_generated_random_number * plaintext_of_input)
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}
	//main_server/aux_server_1/aux_server_2 collaborate to decrypt_and_share the plaintext of (y0, y1), i.e., (share_generated_random_number * plaintext_of_input).
	
	//main_server calculate (y0+y1*N)^{subK} and share the result
	if(!mMainServer.DecryptAndSharePhase0(Msk_MainServer|Msk_Aux1Server, s0_Main, s1_Main, y0, y1))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}
	//main_server will send s0_Main[ID_x], s1_Main[ID_x] to the corresponding server.
	fa0_Main = s0_Main[ID_MainServer-1]; fa1_Main = s1_Main[ID_MainServer-1];
	fa0_Aux1 = s0_Main[ID_Aux1Server-1]; fa1_Aux1 = s1_Main[ID_Aux1Server-1];
	fa0_Aux2 = s0_Main[ID_Aux2Server-1]; fa1_Aux2 = s1_Main[ID_Aux2Server-1];

	//aux_server_1 calculate (y0+y1*N)^{subK} and share the result
	if(!mAux1Server.DecryptAndSharePhase0(Msk_MainServer|Msk_Aux1Server, s0_Aux1, s1_Aux1, y0, y1))
	{
		strcpy(strErrMsg, mAux1Server.GetLastError());
		return 0;
	}
	//aux_server_1 will send s0_Main[ID_x], s1_Main[ID_x] to the corresponding server.
	fb0_Main = s0_Aux1[ID_MainServer-1]; fb1_Main = s1_Aux1[ID_MainServer-1];
	fb0_Aux1 = s0_Aux1[ID_Aux1Server-1]; fb1_Aux1 = s1_Aux1[ID_Aux1Server-1];
	fb0_Aux2 = s0_Aux1[ID_Aux2Server-1]; fb1_Aux2 = s1_Aux1[ID_Aux2Server-1];

	//each server calculate k*fa*fb = x*n + y, share x and publish y
	if(!mMainServer.DecryptAndSharePhase1(s1_Main, y_Main, fa0_Main, fa1_Main, fb0_Main, fb1_Main))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}
	s0_Main[ID_MainServer-1] = s1_Main[ID_MainServer-1];
	s0_Aux1[ID_MainServer-1] = s1_Main[ID_Aux1Server-1];
	s0_Aux2[ID_MainServer-1] = s1_Main[ID_Aux2Server-1];

	if(!mAux1Server.DecryptAndSharePhase1(s1_Aux1, y_Aux1, fa0_Aux1, fa1_Aux1, fb0_Aux1, fb1_Aux1))
	{
		strcpy(strErrMsg, mAux1Server.GetLastError());
		return 0;
	}
	s0_Main[ID_Aux1Server-1] = s1_Aux1[ID_MainServer-1];
	s0_Aux1[ID_Aux1Server-1] = s1_Aux1[ID_Aux1Server-1];
	s0_Aux2[ID_Aux1Server-1] = s1_Aux1[ID_Aux2Server-1];

	if(!mAux2Server.DecryptAndSharePhase1(s1_Aux2, y_Aux2, fa0_Aux2, fa1_Aux2, fb0_Aux2, fb1_Aux2))
	{
		strcpy(strErrMsg, mAux2Server.GetLastError());
		return 0;
	}
	s0_Main[ID_Aux2Server-1] = s1_Aux2[ID_MainServer-1];
	s0_Aux1[ID_Aux2Server-1] = s1_Aux2[ID_Aux1Server-1];
	s0_Aux2[ID_Aux2Server-1] = s1_Aux2[ID_Aux2Server-1];

	//each server get its share of the plaintext of (y0, y1)
	if(!mMainServer.DecryptAndSharePhase2(fa0_Main, s0_Main[ID_MainServer-1], s0_Main[ID_Aux1Server-1], s0_Main[ID_Aux2Server-1], y_Main, y_Aux1, y_Aux2))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}

	if(!mAux1Server.DecryptAndSharePhase2(fa0_Aux1, s0_Aux1[ID_MainServer-1], s0_Aux1[ID_Aux1Server-1], s0_Aux1[ID_Aux2Server-1], y_Main, y_Aux1, y_Aux2))
	{
		strcpy(strErrMsg, mAux1Server.GetLastError());
		return 0;
	}

	if(!mAux2Server.DecryptAndSharePhase2(fa0_Aux2, s0_Aux2[ID_MainServer-1], s0_Aux2[ID_Aux1Server-1], s0_Aux2[ID_Aux2Server-1], y_Main, y_Aux1, y_Aux2))
	{
		strcpy(strErrMsg, mAux2Server.GetLastError());
		return 0;
	}

	//each server broadcast the share of (share_generated_random_number * plaintext_of_input)

	//each server can calculate the inverse of (share_generated_random_number * plaintext_of_input)
	fa0_Aux1 += fa0_Aux2; fa0_Main += fa0_Aux1; bitPrec = FP_PRECISE;
	if(!mMainServer.GetInverseOfFixPoint(fb0_Main, fa0_Main, bitPrec))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}

	//main_server calculate the ciphertext of the inverse
	if(!mMainServer.HomoScalar(cr.c0, cr.c1, u0, u1, fb0_Main))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}
	cr.prec = bitPrec - ca.prec;

	//main_server return cr as the resultant ciphertext back to agent

	return 1;
}

int DivSemiHomoCipher(SemiHomoCipher& cr, SemiHomoCipher& ca, SemiHomoCipher& cb)
{
	ZZ y0, y1, u0, u1;
	ZZ s0_Main[3], s1_Main[3], fa0_Main, fa1_Main, fb0_Main, fb1_Main, y_Main;
	ZZ s0_Aux1[3], s1_Aux1[3], fa0_Aux1, fa1_Aux1, fb0_Aux1, fb1_Aux1, y_Aux1;
	ZZ s0_Aux2[3], s1_Aux2[3], fa0_Aux2, fa1_Aux2, fb0_Aux2, fb1_Aux2, y_Aux2;
	long bitPrec;

	//agent forward ca, cb to main_server.

	//main_server forward ca, cb to aux_server_1.

	//main_server/aux_server_1 generate random value, then use it as scalar to HomoScalar applied to ca/cb, then broadcast the ciphertexts.
	if(!mMainServer.GetRandomZZWithFixBits(y_Main, FP_BITSIZE))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}
	if(!mMainServer.HomoScalar(fa0_Main, fa1_Main, ca.c0, ca.c1, y_Main))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}
	if(!mMainServer.HomoScalar(fb0_Main, fb1_Main, cb.c0, cb.c1, y_Main))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}

	if(!mAux1Server.GetRandomZZWithFixBits(y_Aux1, FP_BITSIZE))
	{
		strcpy(strErrMsg, mAux1Server.GetLastError());
		return 0;
	}
	if(!mAux1Server.HomoScalar(fa0_Aux1, fa1_Aux1, ca.c0, ca.c1, y_Aux1))
	{
		strcpy(strErrMsg, mAux1Server.GetLastError());
		return 0;
	}
	if(!mAux1Server.HomoScalar(fb0_Aux1, fb1_Aux1, cb.c0, cb.c1, y_Aux1))
	{
		strcpy(strErrMsg, mAux1Server.GetLastError());
		return 0;
	}

	//main_server/aux_server_1 locally calculate the following ciphertexts
	if(!mMainServer.HomoAdd(u0, u1, fa0_Main, fa1_Main, fa0_Aux1, fa1_Aux1))
	{//ciphertext corresponding to share_generated_random_number
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}
	if(!mMainServer.HomoAdd(y0, y1, fb0_Main, fb1_Main, fb0_Aux1, fb1_Aux1))
	{//ciphertext corresponding to (share_generated_random_number * plaintext_of_input)
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}
	//main_server/aux_server_1/aux_server_2 collaborate to decrypt_and_share the plaintext of (y0, y1), i.e., (share_generated_random_number * plaintext_of_input).
	
	//main_server calculate (y0+y1*N)^{subK} and share the result
	if(!mMainServer.DecryptAndSharePhase0(Msk_MainServer|Msk_Aux1Server, s0_Main, s1_Main, y0, y1))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}
	//main_server will send s0_Main[ID_x], s1_Main[ID_x] to the corresponding server.
	fa0_Main = s0_Main[ID_MainServer-1]; fa1_Main = s1_Main[ID_MainServer-1];
	fa0_Aux1 = s0_Main[ID_Aux1Server-1]; fa1_Aux1 = s1_Main[ID_Aux1Server-1];
	fa0_Aux2 = s0_Main[ID_Aux2Server-1]; fa1_Aux2 = s1_Main[ID_Aux2Server-1];

	//aux_server_1 calculate (y0+y1*N)^{subK} and share the result
	if(!mAux1Server.DecryptAndSharePhase0(Msk_MainServer|Msk_Aux1Server, s0_Aux1, s1_Aux1, y0, y1))
	{
		strcpy(strErrMsg, mAux1Server.GetLastError());
		return 0;
	}
	//aux_server_1 will send s0_Main[ID_x], s1_Main[ID_x] to the corresponding server.
	fb0_Main = s0_Aux1[ID_MainServer-1]; fb1_Main = s1_Aux1[ID_MainServer-1];
	fb0_Aux1 = s0_Aux1[ID_Aux1Server-1]; fb1_Aux1 = s1_Aux1[ID_Aux1Server-1];
	fb0_Aux2 = s0_Aux1[ID_Aux2Server-1]; fb1_Aux2 = s1_Aux1[ID_Aux2Server-1];

	//each server calculate k*fa*fb = x*n + y, share x and publish y
	if(!mMainServer.DecryptAndSharePhase1(s1_Main, y_Main, fa0_Main, fa1_Main, fb0_Main, fb1_Main))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}
	s0_Main[ID_MainServer-1] = s1_Main[ID_MainServer-1];
	s0_Aux1[ID_MainServer-1] = s1_Main[ID_Aux1Server-1];
	s0_Aux2[ID_MainServer-1] = s1_Main[ID_Aux2Server-1];

	if(!mAux1Server.DecryptAndSharePhase1(s1_Aux1, y_Aux1, fa0_Aux1, fa1_Aux1, fb0_Aux1, fb1_Aux1))
	{
		strcpy(strErrMsg, mAux1Server.GetLastError());
		return 0;
	}
	s0_Main[ID_Aux1Server-1] = s1_Aux1[ID_MainServer-1];
	s0_Aux1[ID_Aux1Server-1] = s1_Aux1[ID_Aux1Server-1];
	s0_Aux2[ID_Aux1Server-1] = s1_Aux1[ID_Aux2Server-1];

	if(!mAux2Server.DecryptAndSharePhase1(s1_Aux2, y_Aux2, fa0_Aux2, fa1_Aux2, fb0_Aux2, fb1_Aux2))
	{
		strcpy(strErrMsg, mAux2Server.GetLastError());
		return 0;
	}
	s0_Main[ID_Aux2Server-1] = s1_Aux2[ID_MainServer-1];
	s0_Aux1[ID_Aux2Server-1] = s1_Aux2[ID_Aux1Server-1];
	s0_Aux2[ID_Aux2Server-1] = s1_Aux2[ID_Aux2Server-1];

	//each server get its share of the plaintext of (y0, y1)
	if(!mMainServer.DecryptAndSharePhase2(fa0_Main, s0_Main[ID_MainServer-1], s0_Main[ID_Aux1Server-1], s0_Main[ID_Aux2Server-1], y_Main, y_Aux1, y_Aux2))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}

	if(!mAux1Server.DecryptAndSharePhase2(fa0_Aux1, s0_Aux1[ID_MainServer-1], s0_Aux1[ID_Aux1Server-1], s0_Aux1[ID_Aux2Server-1], y_Main, y_Aux1, y_Aux2))
	{
		strcpy(strErrMsg, mAux1Server.GetLastError());
		return 0;
	}

	if(!mAux2Server.DecryptAndSharePhase2(fa0_Aux2, s0_Aux2[ID_MainServer-1], s0_Aux2[ID_Aux1Server-1], s0_Aux2[ID_Aux2Server-1], y_Main, y_Aux1, y_Aux2))
	{
		strcpy(strErrMsg, mAux2Server.GetLastError());
		return 0;
	}

	//each server broadcast the share of (share_generated_random_number * plaintext_of_input)

	//each server can calculate the inverse of (share_generated_random_number * plaintext_of_input)
	fa0_Aux1 += fa0_Aux2; fa0_Main += fa0_Aux1; bitPrec = FP_PRECISE;
	if(!mMainServer.GetInverseOfFixPoint(fb0_Main, fa0_Main, bitPrec))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}

	//main_server calculate the ciphertext of the inverse
	if(!mMainServer.HomoScalar(cr.c0, cr.c1, u0, u1, fb0_Main))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}
	cr.prec = ca.prec + bitPrec - cb.prec;

	//main_server return cr as the resultant ciphertext back to agent

	return 1;
}

int HomoInvOnEncryptedRawData(ZZ& z0, ZZ& z1, ZZ& x0, ZZ& x1)
{
	ZZ y0, y1, u0, u1;
	ZZ s0_Main[3], s1_Main[3], fa0_Main, fa1_Main, fb0_Main, fb1_Main, y_Main;
	ZZ s0_Aux1[3], s1_Aux1[3], fa0_Aux1, fa1_Aux1, fb0_Aux1, fb1_Aux1, y_Aux1;
	ZZ s0_Aux2[3], s1_Aux2[3], fa0_Aux2, fa1_Aux2, fb0_Aux2, fb1_Aux2, y_Aux2;

	//agent forward (x0, x1) to main_server.

	//main_server forward (x0, x1) to aux_server_1.

	//main_server/aux_server_1 generate random value, then encrypt it and use it as scalar to HomoScalar, then broadcast the ciphertexts.
	if(!mMainServer.GetRandomZZ(y_Main))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}
	if(!mMainServer.Encrypt(fa0_Main, fa1_Main, y_Main))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}
	if(!mMainServer.HomoScalar(fb0_Main, fb1_Main, x0, x1, y_Main))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}

	if(!mAux1Server.GetRandomZZ(y_Aux1))
	{
		strcpy(strErrMsg, mAux1Server.GetLastError());
		return 0;
	}
	if(!mAux1Server.Encrypt(fa0_Aux1, fa1_Aux1, y_Aux1))
	{
		strcpy(strErrMsg, mAux1Server.GetLastError());
		return 0;
	}
	if(!mAux1Server.HomoScalar(fb0_Aux1, fb1_Aux1, x0, x1, y_Aux1))
	{
		strcpy(strErrMsg, mAux1Server.GetLastError());
		return 0;
	}

	//main_server/aux_server_1 locally calculate the following ciphertexts
	if(!mMainServer.HomoAdd(u0, u1, fa0_Main, fa1_Main, fa0_Aux1, fa1_Aux1))
	{//ciphertext corresponding to share_generated_random_number
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}
	if(!mMainServer.HomoAdd(y0, y1, fb0_Main, fb1_Main, fb0_Aux1, fb1_Aux1))
	{//ciphertext corresponding to (share_generated_random_number * plaintext_of_input)
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}
	//main_server/aux_server_1/aux_server_2 collaborate to decrypt_and_share the plaintext of (y0, y1), i.e., (share_generated_random_number * plaintext_of_input).
	
	//main_server calculate (y0+y1*N)^{subK} and share the result
	if(!mMainServer.DecryptAndSharePhase0(Msk_MainServer|Msk_Aux1Server, s0_Main, s1_Main, y0, y1))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}
	//main_server will send s0_Main[ID_x], s1_Main[ID_x] to the corresponding server.
	fa0_Main = s0_Main[ID_MainServer-1]; fa1_Main = s1_Main[ID_MainServer-1];
	fa0_Aux1 = s0_Main[ID_Aux1Server-1]; fa1_Aux1 = s1_Main[ID_Aux1Server-1];
	fa0_Aux2 = s0_Main[ID_Aux2Server-1]; fa1_Aux2 = s1_Main[ID_Aux2Server-1];

	//aux_server_1 calculate (y0+y1*N)^{subK} and share the result
	if(!mAux1Server.DecryptAndSharePhase0(Msk_MainServer|Msk_Aux1Server, s0_Aux1, s1_Aux1, y0, y1))
	{
		strcpy(strErrMsg, mAux1Server.GetLastError());
		return 0;
	}
	//aux_server_1 will send s0_Main[ID_x], s1_Main[ID_x] to the corresponding server.
	fb0_Main = s0_Aux1[ID_MainServer-1]; fb1_Main = s1_Aux1[ID_MainServer-1];
	fb0_Aux1 = s0_Aux1[ID_Aux1Server-1]; fb1_Aux1 = s1_Aux1[ID_Aux1Server-1];
	fb0_Aux2 = s0_Aux1[ID_Aux2Server-1]; fb1_Aux2 = s1_Aux1[ID_Aux2Server-1];

	//each server calculate k*fa*fb = x*n + y, share x and publish y
	if(!mMainServer.DecryptAndSharePhase1(s1_Main, y_Main, fa0_Main, fa1_Main, fb0_Main, fb1_Main))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}
	s0_Main[ID_MainServer-1] = s1_Main[ID_MainServer-1];
	s0_Aux1[ID_MainServer-1] = s1_Main[ID_Aux1Server-1];
	s0_Aux2[ID_MainServer-1] = s1_Main[ID_Aux2Server-1];

	if(!mAux1Server.DecryptAndSharePhase1(s1_Aux1, y_Aux1, fa0_Aux1, fa1_Aux1, fb0_Aux1, fb1_Aux1))
	{
		strcpy(strErrMsg, mAux1Server.GetLastError());
		return 0;
	}
	s0_Main[ID_Aux1Server-1] = s1_Aux1[ID_MainServer-1];
	s0_Aux1[ID_Aux1Server-1] = s1_Aux1[ID_Aux1Server-1];
	s0_Aux2[ID_Aux1Server-1] = s1_Aux1[ID_Aux2Server-1];

	if(!mAux2Server.DecryptAndSharePhase1(s1_Aux2, y_Aux2, fa0_Aux2, fa1_Aux2, fb0_Aux2, fb1_Aux2))
	{
		strcpy(strErrMsg, mAux2Server.GetLastError());
		return 0;
	}
	s0_Main[ID_Aux2Server-1] = s1_Aux2[ID_MainServer-1];
	s0_Aux1[ID_Aux2Server-1] = s1_Aux2[ID_Aux1Server-1];
	s0_Aux2[ID_Aux2Server-1] = s1_Aux2[ID_Aux2Server-1];

	//each server get its share of the plaintext of (y0, y1)
	if(!mMainServer.DecryptAndSharePhase2(fa0_Main, s0_Main[ID_MainServer-1], s0_Main[ID_Aux1Server-1], s0_Main[ID_Aux2Server-1], y_Main, y_Aux1, y_Aux2))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}

	if(!mAux1Server.DecryptAndSharePhase2(fa0_Aux1, s0_Aux1[ID_MainServer-1], s0_Aux1[ID_Aux1Server-1], s0_Aux1[ID_Aux2Server-1], y_Main, y_Aux1, y_Aux2))
	{
		strcpy(strErrMsg, mAux1Server.GetLastError());
		return 0;
	}

	if(!mAux2Server.DecryptAndSharePhase2(fa0_Aux2, s0_Aux2[ID_MainServer-1], s0_Aux2[ID_Aux1Server-1], s0_Aux2[ID_Aux2Server-1], y_Main, y_Aux1, y_Aux2))
	{
		strcpy(strErrMsg, mAux2Server.GetLastError());
		return 0;
	}

	//each server broadcast the share of (share_generated_random_number * plaintext_of_input)

	//each server can calculate the inverse of (share_generated_random_number * plaintext_of_input)
	fa0_Aux1 += fa0_Aux2; fa0_Main += fa0_Aux1;
	if(!mMainServer.GetInverse(fb0_Main, fa0_Main))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}

	//main_server calculate the ciphertext of the inverse
	if(!mMainServer.HomoScalar(z0, z1, u0, u1, fb0_Main))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}

	//main_server return (z0, z1) as the resultant ciphertext back to agent

	return 1;
}

int HomoDivOnEncryptedRawData(ZZ& z0, ZZ& z1, ZZ& x0, ZZ& x1, ZZ& v0, ZZ& v1)
{
	ZZ y0, y1, u0, u1;
	ZZ s0_Main[3], s1_Main[3], fa0_Main, fa1_Main, fb0_Main, fb1_Main, y_Main;
	ZZ s0_Aux1[3], s1_Aux1[3], fa0_Aux1, fa1_Aux1, fb0_Aux1, fb1_Aux1, y_Aux1;
	ZZ s0_Aux2[3], s1_Aux2[3], fa0_Aux2, fa1_Aux2, fb0_Aux2, fb1_Aux2, y_Aux2;

	//agent forward (x0, x1), (v0, v1) to main_server.

	//main_server forward (x0, x1), (v0, v1) to aux_server_1.

	//main_server/aux_server_1 generate random value, then use it as scalar to HomoScalar applied to (x0, x1), then broadcast the ciphertexts.
	if(!mMainServer.GetRandomZZ(y_Main))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}
	if(!mMainServer.HomoScalar(fa0_Main, fa1_Main, x0, x1, y_Main))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}
	if(!mMainServer.HomoScalar(fb0_Main, fb1_Main, v0, v1, y_Main))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}

	if(!mAux1Server.GetRandomZZ(y_Aux1))
	{
		strcpy(strErrMsg, mAux1Server.GetLastError());
		return 0;
	}
	if(!mAux1Server.HomoScalar(fa0_Aux1, fa1_Aux1, x0, x1, y_Aux1))
	{
		strcpy(strErrMsg, mAux1Server.GetLastError());
		return 0;
	}
	if(!mAux1Server.HomoScalar(fb0_Aux1, fb1_Aux1, v0, v1, y_Aux1))
	{
		strcpy(strErrMsg, mAux1Server.GetLastError());
		return 0;
	}

	//main_server/aux_server_1 locally calculate the following ciphertexts
	if(!mMainServer.HomoAdd(u0, u1, fa0_Main, fa1_Main, fa0_Aux1, fa1_Aux1))
	{//ciphertext corresponding to share_generated_random_number
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}
	if(!mMainServer.HomoAdd(y0, y1, fb0_Main, fb1_Main, fb0_Aux1, fb1_Aux1))
	{//ciphertext corresponding to (share_generated_random_number * plaintext_of_input)
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}
	//main_server/aux_server_1/aux_server_2 collaborate to decrypt_and_share the plaintext of (y0, y1), i.e., (share_generated_random_number * plaintext_of_input).
	
	//main_server calculate (y0+y1*N)^{subK} and share the result
	if(!mMainServer.DecryptAndSharePhase0(Msk_MainServer|Msk_Aux1Server, s0_Main, s1_Main, y0, y1))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}
	//main_server will send s0_Main[ID_x], s1_Main[ID_x] to the corresponding server.
	fa0_Main = s0_Main[ID_MainServer-1]; fa1_Main = s1_Main[ID_MainServer-1];
	fa0_Aux1 = s0_Main[ID_Aux1Server-1]; fa1_Aux1 = s1_Main[ID_Aux1Server-1];
	fa0_Aux2 = s0_Main[ID_Aux2Server-1]; fa1_Aux2 = s1_Main[ID_Aux2Server-1];

	//aux_server_1 calculate (y0+y1*N)^{subK} and share the result
	if(!mAux1Server.DecryptAndSharePhase0(Msk_MainServer|Msk_Aux1Server, s0_Aux1, s1_Aux1, y0, y1))
	{
		strcpy(strErrMsg, mAux1Server.GetLastError());
		return 0;
	}
	//aux_server_1 will send s0_Main[ID_x], s1_Main[ID_x] to the corresponding server.
	fb0_Main = s0_Aux1[ID_MainServer-1]; fb1_Main = s1_Aux1[ID_MainServer-1];
	fb0_Aux1 = s0_Aux1[ID_Aux1Server-1]; fb1_Aux1 = s1_Aux1[ID_Aux1Server-1];
	fb0_Aux2 = s0_Aux1[ID_Aux2Server-1]; fb1_Aux2 = s1_Aux1[ID_Aux2Server-1];

	//each server calculate k*fa*fb = x*n + y, share x and publish y
	if(!mMainServer.DecryptAndSharePhase1(s1_Main, y_Main, fa0_Main, fa1_Main, fb0_Main, fb1_Main))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}
	s0_Main[ID_MainServer-1] = s1_Main[ID_MainServer-1];
	s0_Aux1[ID_MainServer-1] = s1_Main[ID_Aux1Server-1];
	s0_Aux2[ID_MainServer-1] = s1_Main[ID_Aux2Server-1];

	if(!mAux1Server.DecryptAndSharePhase1(s1_Aux1, y_Aux1, fa0_Aux1, fa1_Aux1, fb0_Aux1, fb1_Aux1))
	{
		strcpy(strErrMsg, mAux1Server.GetLastError());
		return 0;
	}
	s0_Main[ID_Aux1Server-1] = s1_Aux1[ID_MainServer-1];
	s0_Aux1[ID_Aux1Server-1] = s1_Aux1[ID_Aux1Server-1];
	s0_Aux2[ID_Aux1Server-1] = s1_Aux1[ID_Aux2Server-1];

	if(!mAux2Server.DecryptAndSharePhase1(s1_Aux2, y_Aux2, fa0_Aux2, fa1_Aux2, fb0_Aux2, fb1_Aux2))
	{
		strcpy(strErrMsg, mAux2Server.GetLastError());
		return 0;
	}
	s0_Main[ID_Aux2Server-1] = s1_Aux2[ID_MainServer-1];
	s0_Aux1[ID_Aux2Server-1] = s1_Aux2[ID_Aux1Server-1];
	s0_Aux2[ID_Aux2Server-1] = s1_Aux2[ID_Aux2Server-1];

	//each server get its share of the plaintext of (y0, y1)
	if(!mMainServer.DecryptAndSharePhase2(fa0_Main, s0_Main[ID_MainServer-1], s0_Main[ID_Aux1Server-1], s0_Main[ID_Aux2Server-1], y_Main, y_Aux1, y_Aux2))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}

	if(!mAux1Server.DecryptAndSharePhase2(fa0_Aux1, s0_Aux1[ID_MainServer-1], s0_Aux1[ID_Aux1Server-1], s0_Aux1[ID_Aux2Server-1], y_Main, y_Aux1, y_Aux2))
	{
		strcpy(strErrMsg, mAux1Server.GetLastError());
		return 0;
	}

	if(!mAux2Server.DecryptAndSharePhase2(fa0_Aux2, s0_Aux2[ID_MainServer-1], s0_Aux2[ID_Aux1Server-1], s0_Aux2[ID_Aux2Server-1], y_Main, y_Aux1, y_Aux2))
	{
		strcpy(strErrMsg, mAux2Server.GetLastError());
		return 0;
	}

	//each server broadcast the share of (share_generated_random_number * plaintext_of_input)

	//each server can calculate the inverse of (share_generated_random_number * plaintext_of_input)
	fa0_Aux1 += fa0_Aux2; fa0_Main += fa0_Aux1;
	if(!mMainServer.GetInverse(fb0_Main, fa0_Main))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}

	//main_server calculate the ciphertext of the inverse
	if(!mMainServer.HomoScalar(z0, z1, u0, u1, fb0_Main))
	{
		strcpy(strErrMsg, mMainServer.GetLastError());
		return 0;
	}

	//main_server return (z0, z1) as the resultant ciphertext back to agent

	return 1;
}