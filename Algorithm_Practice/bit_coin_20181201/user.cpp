//http://www.jungol.co.kr/xpert/viewp.php?bo_table=sm_prac&id=3188&alcate=&mtitle=%EC%8B%A4%EC%A0%84%EB%AC%B8%EC%A0%9C&al=002&altitle=&alname=&altag=&page=1

// 9:50 시작
// 11:40 put, get hash 완성


// inputs:
//1
//2
//AB078ACD0123 32 7900072B
//000000001A2B 26 AB078ACD
//1 5
//00000042000000001A2B02030005050003AB078ACD01230303001505010002000F
//AB078ACD 3
//AB078ACD 5
//AB078ACD 2
//AB078ACD 1
//7900072B 3


// outputs:
//26
//259
//15
//0
//21

//=== user.cpp ===
#ifndef NULL
#define NULL 0
#endif // NULL

#include <stdio.h>

extern unsigned int hexToDeci(unsigned char*s, unsigned int len);
extern unsigned int getBlockID(unsigned char*str, int stIdx, int length);

const int LM = (int)1e6 + 5;

#define MAX_TRANS	16
#define MAX_HASH_CNT	65536
struct Block
{
	unsigned int parentID;
	unsigned int blockSeed;
	unsigned int transactioN;
	unsigned int exchangeID;
	unsigned int volume;
	unsigned int 	transInfo[MAX_TRANS];

	int  hitCount;
	Block * child;
};

struct BitCoinSystem 
{
	Block * hashTable[MAX_HASH_CNT];

	//unsigned int getHash(unsigned int );
	//Block * find(unsigned int blockID)
	//{
	//}

	Block * put(Block blk)
	{
		unsigned int hash = blk.blockSeed % MAX_HASH_CNT;	//getHash(blockID)
		unsigned int i = hash;

		do {
			if (hashTable[i] == NULL) {
				hashTable[i] = new Block();
				hashTable[i]->blockSeed = blk.blockSeed;
				hashTable[i]->parentID = blk.parentID;
				hashTable[i]->transactioN = blk.transactioN;
				hashTable[i]->exchangeID = blk.exchangeID;
				hashTable[i]->volume = blk.volume;

				hashTable[i]->hitCount++;
				return hashTable[i];
			}
			else if (hashTable[i]->blockSeed == blk.blockSeed) {
				hashTable[i]->hitCount++;

				return hashTable[i];
			}
		} while ((++i % MAX_HASH_CNT) != hash);

		printf("HASH FULL!");
		return NULL;
	}

	Block * get(unsigned int blockID)
	{
		unsigned int hash = blockID % MAX_HASH_CNT;	//getHash(blockID)
		unsigned int i = hash;

		do {
			if (hashTable[i]->blockSeed == blockID) {
				return hashTable[i];
			}
		} while ((++i % MAX_HASH_CNT) != hash);

		printf("HASH FULL!");
		return NULL;
	}

	void init() 
	{
		unsigned int i;
		for (i = 0; i < MAX_HASH_CNT; i++) {
			hashTable[i] = NULL;
		}
	}
};

BitCoinSystem bc;


void processBlockImage(int serverCnt, unsigned char image[][LM]) {

	bc.init();

	int i, j, k;
	int offset;
	unsigned int img_len;
	Block blk;

	offset = 0;
	for (i = 0; i < serverCnt; i++) {
		img_len = hexToDeci(image[i], 4*2);
		offset += 4 * 2;

		// 이미지 안의 각 블록에 대해서
		for (j = 0; j < img_len; j++) {
			// 1. 블럭 정보 구성
			//   a) 파싱
			//   b) 블록 테이블 구성
			blk.parentID = hexToDeci(image[i + offset], 4*2);
			offset += 4 * 2;

			blk.blockSeed = hexToDeci(image[i + offset], 2 * 2);
			offset += 2 * 2;

			blk.transactioN = hexToDeci(image[i + offset], 1 * 2);
			offset += 1 * 2;

			blk.exchangeID = hexToDeci(image[i + offset], 1 * 2);
			offset += 1 * 2;

			blk.volume = hexToDeci(image[i + offset], 2 * 2);
			offset += 2 * 2;

			for (k = 0; k < blk.transactioN; k++) {
				blk.transInfo[k] = hexToDeci(image[i + offset], 4);
				offset += 4;
			}

			bc.put(blk);
		}
	}

	// 모든 블럭 정보가 구성된 후
	// 2. 블럭 간 관계 구성
	//   : 트리

}

int getSumVolume(unsigned int ID, int exchangeID) {

	// 블럭 트리를 순회하면서 값 계산



	return 0;
}


