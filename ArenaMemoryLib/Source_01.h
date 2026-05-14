#pragma once

#include <malloc.h>

typedef struct Arena {
	void* pArenaStart;
	unsigned char* pArenaEnd;
	unsigned char* pArenaCurrent;
	unsigned char* pArenaSave;
	struct Arena*  chainNext;
	unsigned long  ulIdentifier;
	unsigned long  ulSignature;
} Arena, *ArenaPtr;


typedef struct ArenaObject {
	Arena* chainHead;
	ArenaPtr (*ArenaNewArena)(size_t nSize);
	void (*ArenaFreeArena)(ArenaPtr x);
	Arena (*ArenaCloneArena)(ArenaPtr x);
	void* (*ArenaAllocItem)(ArenaPtr x, size_t nSize);
	void (*ArenaAddToChain)(ArenaPtr x);
	void* (*ArenaRemoveFromChain)(ArenaPtr x);
} ArenaObject;

extern ArenaObject arenaFactory;

struct ArenaWalkData {
	size_t  nTotalSize;    // total memory for arena allocation.
	size_t  nInUseSize;    // amount of total memory that is currently allocated.
	int     iCount;        // count of number of arenas on the chain.
};

extern struct ArenaWalkData ArenaWalkAndCheckChain(void);
