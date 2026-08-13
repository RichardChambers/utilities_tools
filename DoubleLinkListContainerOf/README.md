I ran across a Reddit post which mentioned using a macro, `container_of()`, that when
given an address of a `struct` member, can provide the address of the container, or the beginning of the `struct` object, that contains
the data whose address is given to the macro.

It appears this is used in the Linux kernel to implement linked lists of various sorts
according to the Reddit post.

https://www.reddit.com/r/C_Programming/comments/11xdyr8/comment/jd2q0sn/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button

### Windows API `CONTAINING_RECORD` macro

For an explanation of the `CONTAINING_RECORD()` macro that is part of the Microsoft Windows API see the Microsoft learning documentation [CONTAINING_RECORD macro in ntdef.h](https://learn.microsoft.com/en-us/windows/win32/api/ntdef/nf-ntdef-containing_record). It is from the Windows 2000 days and is a bit different though it does the same thing.

## The Source Code

The source code in this area provides a doubly linked list of users. The example shows adding users to the front as well as the back of the
linked list. Each user item added has its own memory area using the `malloc()` call. This memory needs to be freed so I include an example
of traversing a list and freeing each item to release its memory.

The object that contains the list management head or header which points to the first and last list items is interesting in its implementation.
An empty list just points to the head object itself. This makes finding the end of a list when traversing either forward or backward easy by
comparing the next/previous pointer value to the address of the header variable.


## The Basic Idea

When I first read the post, my first reaction was "What sorcery is this!!" however once I spent some
time working with it, things became a bit clearer as the post was a bit misleading to me.

If I have a collection of struct objects are in a linked list, each of the structs has two
sections: (1) the data section and the list management section.

For example you may have a struct like the following:

```
struct User {
    int user_id;          // id number for this user
    char username[50];    // the name of this user
    struct list_head list; // the link data that points to the next link in the collection
};
```

And the link data struct is two pointers, one pointing to the previous item in the list and the
other pointing to the next item in the list.

```
// The generic list node structure
struct list_head {
    struct list_head* next;
    struct list_head* prev;
};
```

Our goal here that if we have a pointer to a `struct User` object which points to the `list` member that
contains the list management data, how can we generate a pointer to the beginning of the `struct User` 
object that the `list` member is in, its container?

The answer is an ingenious macro that takes the pointer to the `struct list_head` that is contained in the `struct User` and
derives the pointer to the beginning of the `struct User` object.

### The `container_of()` macro

There are two variations of this macros depending on whether your C compiler is C23 compliant offering the `type()` operator or not.
Here's the version that should be compatible with all modern C compilers.

```
#define container_of(ptr, type, member) \
  ((type*)((char*)(1 ? (ptr) : &((type*)0)->member) - offsetof(type, member)))
```

This macro uses the tertiary operator, the `(1 ? (ptr) : &((type*)0)->member)` and I did not understand why as it seemed unneeded.
So I asked Google Gemini and it told me the following:

> This macro finds the starting address of a structure instance when you only have a pointer to one of its internal fields.
> 
> It allows C programs—most famously the Linux kernel—to implement object-oriented style inheritance and generic containers like linked lists.
>  - What it does: This uses the ternary operator ? : as a compile-time trick.
>  - The Mechanism: Because the condition is 1, the expression always evaluates to `ptr` at runtime. However, the compiler must still ensure that both possible outcomes of the ternary operator have compatible pointer types.
>  - The Result: It forces the compiler to throw a warning or error if the pointer `ptr` does not match the data type of member inside the struct type.
  
The beauty of this approach is that you can use a set of generic functions to traverse any collection using this protocol with the same set of
functions and macros and then to then obtain the data of a particular object in the collection by using the `container_of()` macro on the traversal
pointer to obtain a pointer to the entire object of both data and the list management area.
