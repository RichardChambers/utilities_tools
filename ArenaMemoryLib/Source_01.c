// A function library for the simple Arena approach to memory
// management with the C programming language.
//
// This library allows you to create a memory area from which
// memory areas are allocated. When the memory areas allocated
// are no longer needed, the entire arena area can be freed.
//   - ArenaNewArena() allocate a new arena memory area using malloc()
//   - ArenaFreeArena() free an arena memory area using free()
//   - ArenaAllocItem() provide a pointer to a section of the arena
//   - ArenaCloneArena() clone an arena to create a temporary book mark
//
// The purpose of the ArenaCloneArena() function is to create a copy of
// an arena management data when a new scope is being entered so that when
// the scope is exited, any arena memory allocations done during that scope
// are rolled back as if they did not happen.
// 
// See the test program in Source_02.c for an example.

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

// The following function is a provided as part of testing
// the Arena memory library. It walks the chain of arenas
// to generate a few statistics.
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

// global object containing function pointers to the above
// functions which are file level scope using static.
// This allows us to have only a single global visible rather
// than the several globals of the functions.
ArenaObject arenaFactory = {
	NULL,
	ArenaNewArena,
	ArenaFreeArena,
	ArenaCloneArena,
	ArenaAllocItem,
	ArenaAddToChain,
	ArenaRemoveFromChain
};

