
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

// usage: container_of(listNode, struct User, listPtr)
// where:    listNode is pointer of type struct User that contains a struct list_head
//           listPtr is a pointer of type struct list_head * in another struct User object
#define container_of(ptr, type, member) \
  ((type*)((char*)(1 ? (ptr) : &((type*)0)->member) - offsetof(type, member)))


// The generic list node structure
struct list_head {
    struct list_head* next;      // address of the next item in the list
    struct list_head* prev;      // address of the previous item in the list
};

struct User {
    int user_id;
    char username[50];
    struct list_head list; // The embedded list hook
};

// Helper macro to initialize a list head at runtime
#define INIT_LIST_HEAD(ptr) do { \
    (ptr)->next = (ptr);         \
    (ptr)->prev = (ptr);         \
} while (0)


// Internal helper to insert a node between two known nodes
// If an empty list and we adding the first item then prev == next == head.
// Add to front of list with __list_add(newItem, head, head->next);
//     inserts between head and head->next
// Add to back of list with __list_add(newItem, head->prev->next, head->prev);
//     inserts between head->prev and head
static inline void __list_add(struct list_head* new_node,
    struct list_head* prev,
    struct list_head* next) {
    next->prev = new_node;
    new_node->next = next;
    new_node->prev = prev;
    prev->next = new_node;
}


// Add a new entry immediately after the specified head (push to front)
// typically the call will be list_add_front(newItem, head) in order to add
// a new item to the beginning of the list.
void list_add_front(struct list_head* new_node, struct list_head* head) {
    // add the new item between the head and the first item of the list.
    // remember that initialized head with an empty list points to itself
    // as both next member and last member.
    __list_add(new_node, head, head->next);
}

// Add a new entry immediately before the specified head (push to back)
// typically the call will be list_add_back(newItem, head) in order to add
// a new item to the end of the list.
void list_add_back(struct list_head* new_node, struct list_head* head) {
    // add a new item between the last item of the list and the head
    // remember that initialized head with an empty list points to itself
    // as both next member and last member.
    __list_add(new_node, head->prev, head->prev->next);
}


// Remove an entry from the list by bridging its neighbors
// we constrain this to a struct list_head only to allow it to
// be used with any object.
void list_del(struct list_head* entry) {
    // a few elementary checks that our data structure is still in
    // some kind of a dependable state.
    if (entry && entry->next && entry->prev) {
        if (entry->next != entry && entry->prev != entry) {
            entry->next->prev = entry->prev;
            entry->prev->next = entry->next;
            entry->next = NULL; // Optional: clear to catch bugs
            entry->prev = NULL;
        }
    }
}

// The struct User specific version of the item delete to provide
// a return of the item previous to the item being deleted to allow
// for deleting when traversing to be done easily.
struct User* user_list_del(struct User* cursor) {
    struct User* cursor2 = NULL;

    if (cursor) {
        cursor2 = container_of(cursor->list.prev, struct User, list);
        list_del(&cursor->list);
        free(cursor);
    }

    return cursor2;
}

// Iterates over a list of a given type
// was using typeof() but that is in the C23 standard
#define list_for_each_entry_forwards(pos, t, head, member) \
    for (pos = container_of((head)->next, t, member); \
         &pos->member != (head); \
         pos = container_of(pos->member.next, t, member))

#define list_for_each_entry_backwards(pos, t, head, member) \
    for (pos = container_of((head)->prev, t, member); \
         &pos->member != (head); \
         pos = container_of(pos->member.prev, t, member))


int main_01 (void) {
    // 1. Declare and initialize the anchor "head" of our list
    struct list_head user_list;
    INIT_LIST_HEAD(&user_list);

    list_del(&user_list);      // test that our function will not delete an empty list.
    assert(user_list.next == &user_list);
    assert(user_list.prev == &user_list);

    // 2. Create and allocate concrete user instances
    struct User* user = NULL;

#if 1
    user = malloc(sizeof(struct User));
    user->user_id = 101;
    snprintf(user->username, sizeof(user->username), "Alice");
    list_add_front(&user->list, &user_list);

#if 1
    // Test that the safe guards we've put in provide some protection of
    // messing up our list during deletes.
    // The asserts() test that the original initialization is still in place.
    list_del(&user->list);
    assert(user_list.next == &user_list);
    assert(user_list.prev == &user_list);

    list_del(&user_list);
    assert(user_list.next == &user_list);
    assert(user_list.prev == &user_list);

    user->user_id = 101;
    snprintf(user->username, sizeof(user->username), "Alice");
    list_add_front(&user->list, &user_list);
#endif

    user = malloc(sizeof(struct User));
    user->user_id = 102;
    snprintf(user->username, sizeof(user->username), "Bob");
    list_add_front(&user->list, &user_list);
#endif

#if 1
    user = malloc(sizeof(struct User));
    user->user_id = 201;
    snprintf(user->username, sizeof(user->username), "Alicex");
    list_add_back(&user->list, &user_list);

    user = malloc(sizeof(struct User));
    user->user_id = 202;
    snprintf(user->username, sizeof(user->username), "Bobx");
    list_add_back(&user->list, &user_list);
#endif

#if 1
    user = malloc(sizeof(struct User));
    user->user_id = 401;
    snprintf(user->username, sizeof(user->username), "Alicey");
    list_add_front(&user->list, &user_list);

    user = malloc(sizeof(struct User));
    user->user_id = 402;
    snprintf(user->username, sizeof(user->username), "Boby");
    list_add_front(&user->list, &user_list);
#endif


    // 4. Iterate over the entries seamlessly
    struct User* cursor;
    printf("Iterating through user list:\n");
    printf("  Forwards\n");
    list_for_each_entry_forwards(cursor, struct User, &user_list, list) {
        printf("    User ID: %d, Name: %s\n", cursor->user_id, cursor->username);
    }

    printf("  Backwards\n");
    list_for_each_entry_backwards(cursor, struct User, &user_list, list) {
        printf("    User ID: %d, Name: %s\n", cursor->user_id, cursor->username);
    }

    // Clean up memory
    printf("Free memory\n");
    list_for_each_entry_forwards(cursor, struct User, &user_list, list) {
        cursor = user_list_del(cursor);   // delete the current item and back up to the previous item.
    }

    printf("  Forwards\n");
    list_for_each_entry_forwards(cursor, struct User, &user_list, list) {
        printf("    User ID: %d, Name: %s\n", cursor->user_id, cursor->username);
    }

    return 0;
}


