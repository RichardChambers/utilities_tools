

#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC

#include <stdlib.h>

#include <crtdbg.h>

#include <memory.h>
#include <assert.h>

#include "Source_01.h"

static const unsigned long ARENA_SIGNATURE = 0x1234FEDC;

static 	unsigned long  ulIdentifier = 1;


static ArenaPtr ArenaNewArena(size_t nSize) {
	Arena a = { 0 };
	ArenaPtr p = malloc(sizeof(Arena) + nSize * sizeof(unsigned char*));

	assert(p);

	a.pArenaStart = p;
	a.pArenaCurrent = (unsigned char*)(p + 1);
	a.pArenaEnd = a.pArenaCurrent + nSize;
	a.pArenaSave = NULL;
	a.chainNext = NULL;
	a.ulIdentifier = ulIdentifier++;
	a.ulSignature = ARENA_SIGNATURE;
	*p = a;

	assert(a.pArenaStart < (void *)a.pArenaCurrent);
	assert(a.pArenaCurrent <= a.pArenaEnd);

	return p;
}

static void ArenaFreeArena(ArenaPtr x) {
	assert(x);
	assert(x->ulSignature == ARENA_SIGNATURE);
	assert(x->pArenaStart);
	assert(x->pArenaCurrent);
	assert(x->pArenaEnd);
	assert(x->pArenaStart < (void*)x->pArenaCurrent);
	assert(x->pArenaCurrent <= x->pArenaEnd);
	assert(x->chainNext == NULL);

	free(x->pArenaStart);
	*x = (Arena){ 0 };
}

static Arena ArenaCloneArena(ArenaPtr x) {
	assert(x);
	assert(x->ulSignature == ARENA_SIGNATURE);
	assert(x->pArenaStart);
	assert(x->pArenaCurrent);
	assert(x->pArenaEnd);
	assert(x->pArenaStart < (void*)x->pArenaCurrent);
	assert(x->pArenaCurrent <= x->pArenaEnd);

	Arena a = *x;
	a.pArenaSave = NULL;

	return a;
}


static void* ArenaAllocItem(ArenaPtr x, size_t nSize) {
	void* p = NULL;

	assert(x);
	assert(x->ulSignature == ARENA_SIGNATURE);
	assert(x->pArenaStart);
	assert(x->pArenaCurrent);
	assert(x->pArenaEnd);
	assert(x->pArenaStart < (void*)x->pArenaCurrent);
	assert(x->pArenaCurrent <= x->pArenaEnd);
	assert(x->pArenaCurrent + nSize <= x->pArenaEnd);

	if (x->pArenaCurrent + nSize <= x->pArenaEnd) {
		p = x->pArenaCurrent;
		x->pArenaCurrent = x->pArenaCurrent + nSize;
	}

	return p;
}

static void ArenaAddToChain(ArenaPtr x) {
	Arena* p = arenaFactory.chainHead;

	assert(x);
	assert(x->ulSignature == ARENA_SIGNATURE);
	assert(x->pArenaStart);
	assert(x->pArenaCurrent);
	assert(x->pArenaEnd);
	assert(x->pArenaStart < (void*)x->pArenaCurrent);
	assert(x->pArenaCurrent <= x->pArenaEnd);

	if (p == NULL) {
		arenaFactory.chainHead = x->pArenaStart;
		x->chainNext = NULL;
	}
	else {
		for (; ; p = p->chainNext) {
			if (p == x->pArenaStart) break;
			if (p->chainNext == NULL) {
				p->chainNext = x->pArenaStart;
				x->chainNext = NULL;
				break;
			}
		}
	}
}

static void * ArenaRemoveFromChain(ArenaPtr x) {
	Arena* p = arenaFactory.chainHead;

	assert(x);
	assert(x->ulSignature == ARENA_SIGNATURE);
	assert(x->pArenaStart);
	assert(x->pArenaCurrent);
	assert(x->pArenaEnd);
	assert(x->pArenaStart < (void*)x->pArenaCurrent);
	assert(x->pArenaCurrent <= x->pArenaEnd);

	if (p == NULL) {
		// if there is nothing on the chain then this arena should not be in a chain.
		assert(x->chainNext == NULL);
		return NULL;
	}
	else if (p == x->pArenaStart) {
		// if this item is the beginning of the chain, remove it.
		// the last item in the chain should have NULL in its chainNext pointer.
		arenaFactory.chainHead = x->chainNext;
		x->chainNext = NULL;
		return p;
	}
	else {
		Arena* pPrev = p;
		for (p = p->chainNext; p; p = p->chainNext) {
			if (p == x->pArenaStart) {
				pPrev->chainNext = p->chainNext;
				p->chainNext = NULL;
				assert(p->ulSignature == ARENA_SIGNATURE);
				return p;
			}
		}
	}

	return NULL;
}

struct ArenaWalkData ArenaWalkAndCheckChain(void) {
	Arena* p = arenaFactory.chainHead;
	struct ArenaWalkData walk = { 0 };

	while (p) {
		walk.nTotalSize += p->pArenaEnd - (unsigned char *)p->pArenaStart - sizeof(Arena);
		walk.nInUseSize += p->pArenaCurrent - (unsigned char*)p->pArenaStart - sizeof(Arena);
		walk.iCount++;
		p = p->chainNext;
	}

	return walk;
}

ArenaObject arenaFactory = {
	NULL,
	ArenaNewArena,
	ArenaFreeArena,
	ArenaCloneArena,
	ArenaAllocItem,
	ArenaAddToChain,
	ArenaRemoveFromChain
};



#if 0
// test harness

static int test_01() {
	ArenaPtr myArena;

	myArena = arenaFactory.ArenaNewArena(sizeof(int) * 20);

	ArenaAddToChain(myArena);

	int* pInt = arenaFactory.ArenaAllocItem(myArena, sizeof(int) * 30);
	assert(pInt == NULL);

	pInt = arenaFactory.ArenaAllocItem(myArena, sizeof(int) * 20);
	assert(pInt);

	struct ArenaWalkData walk = ArenaWalkAndCheckChain();

	return 0;
}

static void testx(Arena ar) {
	ArenaPtr myArena = &ar;
	unsigned long* a = NULL, * b = NULL, * c = NULL, * d = NULL;

	a = arenaFactory.ArenaAllocItem(myArena, sizeof(unsigned long));
	b = arenaFactory.ArenaAllocItem(myArena, sizeof(unsigned long));
	c = arenaFactory.ArenaAllocItem(myArena, sizeof(unsigned long));
	d = arenaFactory.ArenaAllocItem(myArena, sizeof(unsigned long));
}

int main_source_03() {
	ArenaPtr myArena;
	
	myArena = arenaFactory.ArenaNewArena(4096);

	ArenaAddToChain(myArena);

	test_01();

	unsigned long* a = NULL, * b = NULL, * c = NULL, * d = NULL;

	a = arenaFactory.ArenaAllocItem(myArena, sizeof(unsigned long));
	b = arenaFactory.ArenaAllocItem(myArena, sizeof(unsigned long));
	c = arenaFactory.ArenaAllocItem(myArena, sizeof(unsigned long));
	d = arenaFactory.ArenaAllocItem(myArena, sizeof(unsigned long));

//	unsigned char* bad = ArenaAllocItem(myArena, 4096);

	{
		// clone the current arena to create a bench mark and then use
		// the clone for temporary storage which is then discarded
		// once we exit the scope of these braces.
		Arena ar = arenaFactory.ArenaCloneArena(myArena);
		ArenaPtr arPtr = &ar;

		unsigned long* a = NULL, * b = NULL, * c = NULL, * d = NULL;

		a = arenaFactory.ArenaAllocItem(arPtr, sizeof(unsigned long));
		b = arenaFactory.ArenaAllocItem(arPtr, sizeof(unsigned long));
		c = arenaFactory.ArenaAllocItem(arPtr, sizeof(unsigned long));
		d = arenaFactory.ArenaAllocItem(arPtr, sizeof(unsigned long));

		struct ArenaWalkData walk = ArenaWalkAndCheckChain();

		testx(ar);
	}

	testx(arenaFactory.ArenaCloneArena(myArena));

	struct ArenaWalkData walk = ArenaWalkAndCheckChain();

	arenaFactory.ArenaFreeArena(ArenaRemoveFromChain(myArena));

	walk = ArenaWalkAndCheckChain();

	return 0;
}

#endif
