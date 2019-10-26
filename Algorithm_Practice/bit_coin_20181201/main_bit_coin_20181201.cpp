//=== main.cpp ===
#ifndef _CRT_SECURE_NO_WARNINGS 
#define _CRT_SECURE_NO_WARNINGS 
#endif 

#include   <stdio.h>

const int LIMIT = (int)1e6 + 5;
const int DSIZE = 1  << 16;

extern void processBlockImage(int sn, unsigned char img[][LIMIT]);
extern int getSumVolume(unsigned int ID, int exID);

static int HN;
static int SN;
static int QN;
static unsigned char img[5][LIMIT];

static struct Data {
	long long hval;
	unsigned int len, ID;

	unsigned int checker(long long hv, unsigned int nlen) {
		if (len != nlen || hval != hv) return   -1;
		return ID;
	}

	void setD(long long nh, unsigned int nl, unsigned int nid) {
		hval = nh, len = nl, ID = nid;
	}
} dataList[DSIZE + 5];

/// 16진법 문자열을 10진 정수로 변환하여 반환한다.
unsigned int hexToDeci(unsigned char*s, unsigned int len) {
	unsigned int i, d = 0;
	for (i = 0; i < len; ++i)
		d = d * 16 + s[i] - (s[i]  <'A' ? 48 : 55);
	return d;
}

/// 이미지 포인터, 블록의 시작 index, 블록의 길이를 전달받아
/// 블록의 해시 ID를 반환한다.
unsigned int getBlockID(unsigned char*str, int stIdx, int length) {

	unsigned int a = hexToDeci(str + stIdx, 8);
	unsigned int b = hexToDeci(str + stIdx + 8, 4);

	long long hv = ((long long) a  << 16) + b;
	return dataList[b].checker(hv, length);
}

static void scanData() {
	scanf("%d", &HN);
	int i;
	long long hval;
	unsigned int a, b, len, ID;

	for (i = 0; i < HN; ++i) {
		scanf("%8X%4X %u %X", &a, &b, &len, &ID);
		hval = ((long long)a  << 16) + b;
		dataList[b].setD(hval, len, ID);
	}

	scanf("%d %d", &SN, &QN);
	for (int i = 0; i < SN; ++i)
		scanf("%s", img[i]);
}

static void run() {

	processBlockImage(SN, img);

	unsigned int hashID;
	int i, exID, userRet;

	for (i = 0; i < QN; ++i) {
		scanf("%X %d", &hashID, &exID);
		userRet = getSumVolume(hashID, exID);
		printf("%d\n", userRet);
	}
}

int main() {
	int tc = 1;

	scanf("%d", &tc);
	for (int i = 1; i <= tc; ++i) {
		scanData();
		run();
	}
	return 0;
}

