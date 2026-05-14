This is an exploration into using the memory arena approach to memory management with C.

This library allows you to create a memory area from which
memory areas are allocated. When the memory areas allocated
are no longer needed, the entire arena area can be freed.

  - ArenaNewArena() allocate a new arena memory area using malloc()
  - ArenaFreeArena() free an arena memory area using free()
  - ArenaAllocItem() provide a pointer to a section of the arena
  - ArenaCloneArena() clone an arena to create a temporary book mark

The purpose of the ArenaCloneArena() function is to create a copy of
an arena management data when a new scope is being entered so that when
the scope is exited, any arena memory allocations done during that scope
are rolled back as if they did not happen.

See the test program in Source_02.c for an example.


See the following articles

https://www.dgtlgrove.com/p/untangling-lifetimes-the-arena-allocator

https://alonsozamorano.me/how-to-actually-use-arenas-and-program-in-c/
