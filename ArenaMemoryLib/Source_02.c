

#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC

#include <stdlib.h>

#include <crtdbg.h>

#include <memory.h>
#include <assert.h>

#include "Source_01.h"

// test harness

static int test_01() {
	ArenaPtr myArena;

	myArena = arenaFactory.ArenaNewArena(sizeof(int) * 20);

	arenaFactory.ArenaAddToChain(myArena);

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

	arenaFactory.ArenaAddToChain(myArena);

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

	arenaFactory.ArenaFreeArena(arenaFactory.ArenaRemoveFromChain(myArena));

	walk = ArenaWalkAndCheckChain();

	return 0;
}

