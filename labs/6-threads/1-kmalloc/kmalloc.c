#include "rpi.h"

// symbol created by libpi/memmap, placed at the end
// of all the code/data in a pi binary file.
extern char __heap_start__;

// track if initialized.
static int init_p;

static char *heap_ptr = NULL;

static unsigned mod(unsigned m, unsigned n) {
    if (m == n)
        return 0;
    if (m < n)
        return m;
    while (m > n)
        m -= n;
    return m;
}

/*
 * Return a memory block of at least size <nbytes>
 * Notes:
 *  - There is no free, so is trivial: should be just 
 *    a few lines of code.
 *  - The returned pointer should always be 4-byte aligned.  
 *    Easiest way is to make sure the heap pointer starts 4-byte
 *    and you always round up the number of bytes.  Make sure
 *    you put an assertion in.  
 */
void *kmalloc(unsigned nbytes) {
    demand(init_p, calling before initialized);
    if (nbytes < 4)
        nbytes = 4;
    else if (nbytes % 4 != 0)
        nbytes += (4 - (nbytes % 4));
    char *ptr = heap_ptr;
    heap_ptr += nbytes;
    return ptr;
}

// address of returned pointer should be a multiple of
// alignment.
void *kmalloc_aligned(unsigned nbytes, unsigned alignment) {
    demand(init_p, calling before initialized);

    if(alignment <= 4)
        return kmalloc(nbytes);
    demand(alignment % 4 == 0, weird alignment: not a multiple of 4!);
    if (nbytes < alignment)
        nbytes = alignment;
    else
    {
        unsigned r = mod(nbytes, alignment);
        if (r != 0)
            nbytes += alignment - r;
    }

    unsigned r = mod((unsigned)heap_ptr, alignment);
    if (r != 0)
        heap_ptr += alignment - r;
    char *ptr = heap_ptr;
    heap_ptr += nbytes;
    return ptr;
}

/*
 * One-time initialization, called before kmalloc 
 * to setup heap. 
 *    - should be just a few lines of code.
 *    - sets heap pointer to the location of 
 *      __heap_start__.   print this to make sure
 *      it makes sense!
 */
void kmalloc_init(void) {
    if(init_p)
        return;
    init_p = 1;
    heap_ptr = &__heap_start__;
}

/* 
 * free all allocated memory: reset the heap 
 * pointer back to the beginning.
 */
void kfree_all(void) {
    heap_ptr = &__heap_start__;
}

// return pointer to the first free byte.
// for the current implementation: the address <addr> of any
// allocated block satisfies: 
//    assert(<addr> < kmalloc_heap_ptr());
// 
void *kmalloc_heap_ptr(void) {
    return heap_ptr;
}
