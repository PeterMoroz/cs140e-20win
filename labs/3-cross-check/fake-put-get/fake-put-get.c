/*
 * Simple tracing memory implementation.  Used for cross-checking.
 * 
 * you will implement a simple fake memory that maps addresses to values.  
 * e.g., an array, linked list, hash table of (<addr>, <val>) tuples such 
 * that after you insert some (<addr>, <val>), you can lookup <val> using 
 * <addr> as a key.
 *
 * simplistic assumptions:
 *  - all loads/stores are 32 bits.
 *  - read returns value of last write, or random() if none.
 *  - load and store prints the addr,val
 * 
 * HINT: I woul suggest a fixed size array, that you do a simple linear scan 
 * to insert, and lookup.
 *
 * mildly formal-ish rules for fake memory:
 *  1. no duplicate entries.  
 *       if: 
 *           (<addr>, <val0>) \in memory
 *           (<addr>, <val1>) \in memory
 *      then <val0> == <val1>
 *  2. addresses are persistent: if you ever write <addr>,<val> to memory, 
 *     then <addr> stays there forever.
 *  3. values are persistent until the next write: if you ever write 
 *     (<addr>,<val> ) to memory, then until the next write, 
 *          if (<addr>, <v>) \in memory, then <v> == <val>.
 */
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "demand.h"
#include "rpi.h"
#include "pi-test.h"

/****************************************************************************
 *          DO NOT CHANGE THIS CODE (your changes go after it)
 *          DO NOT CHANGE THIS CODE (your changes go after it)
 *          DO NOT CHANGE THIS CODE (your changes go after it)
 *          DO NOT CHANGE THIS CODE (your changes go after it)
 *          DO NOT CHANGE THIS CODE (your changes go after it)
 */

typedef struct {
    const volatile void *addr;
    unsigned val;
} mem_t;

typedef struct {
    int op;
    mem_t v;
} log_ent_t;

static mem_t mk_mem(const volatile void *addr, unsigned val) {
    return (mem_t) { .addr = addr, .val = val };
}

// don't change routine so we can compare to everyone.
static void print_write(mem_t *m) {
    printf("\tTRACE:PUT32(%p)=0x%x\n", m->addr, m->val);
}
// don't change routine so we can compare to everyone.
static void print_read(mem_t *m) {
    printf("\tTRACE:GET32(%p)=0x%x\n", m->addr, m->val);
}


#define NUM_OF_MEM_ENTRIES 1024
static mem_t fake_memory[NUM_OF_MEM_ENTRIES];
static size_t num_of_mem_entries = 0;

unsigned get32(const volatile void *addr) {
    size_t i = 0;
    for (; i < num_of_mem_entries; i++) {
        if (fake_memory[i].addr == addr)
            break;
    }

    if (i == num_of_mem_entries) {
        num_of_mem_entries++;
        if (num_of_mem_entries > NUM_OF_MEM_ENTRIES)
            panic("fake memory exhausted\n");
        fake_memory[i] = mk_mem(addr, fake_random());
        return fake_memory[i].val;
    }

    print_read(&fake_memory[i]);
    return fake_memory[i].val;
}

void put32(volatile void *addr, unsigned val) {
    size_t i = 0;
    for (; i < num_of_mem_entries; i++) {
        if (fake_memory[i].addr == addr)
            break;
    }

    if (i == num_of_mem_entries) {
        num_of_mem_entries++;
        if (num_of_mem_entries > NUM_OF_MEM_ENTRIES)
            panic("fake memory exhausted\n");

        fake_memory[i] = mk_mem(addr, val);
    } else {
        fake_memory[i].val = val;
    }

    print_write(&fake_memory[i]);
}

