/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "ateam",
    /* First member's full name */
    "Harry Bovik",
    /* First member's email address */
    "bovik@cs.cmu.edu",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define SIZE(ptr) *(unsigned int*)((char*)ptr - SIZE_T_SIZE)

#define FREELIST_SIZE 20
static list_item freelist[FREELIST_SIZE];
/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
	int i;
	for( i = 0; i < FREELIST_SIZE; i++) {	
		freelist[i].next = &freelist[i];
		freelist[i].prev = &freelist[i];
	}
	return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
	// get size of the block
    int newsize = ALIGN(size + SIZE_T_SIZE);
	newsize = round_up(newsize);

	// get index of the freelist
	int index = get_index(newsize);

	// search the freelist for fit
	void *p = remove_from_head(&freelist[index]); 
	if (p) {
		return p;
	}
	else {
		p = mem_sbrk(newsize);
		if (p == (void *)-1)
			return NULL;
		else {
			*(size_t *)p = newsize;
			return (void *)((char *)p + SIZE_T_SIZE);
		}
	}
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
	// get size of the block
	unsigned int size = SIZE(ptr);

	// get index of the freelist
	int index = get_index(size);

	// insert at the head of the freelist
	insert_at_head(ptr,&freelist[index]);

	return; 
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;
    
    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;
    copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}

int get_index(int size)
{
	int ret = 0;
	size >>= 3;
	while(size != 1) {
		size >>= 1;
		ret++;
	}
	return ret;
}

int round_up(int v)
{
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;
	return v;
}
void insert_at_head(void* ptr, list_item* freelist)
{
	((list_item*)ptr)->prev = freelist;
	((list_item*)ptr)->next = freelist->next;
	freelist->next->prev = (list_item*)ptr;
	freelist->next = (list_item*)ptr;
	return;
}

void* remove_from_head(list_item* freelist) {
	if(freelist->prev == freelist->next)
		return NULL;
	freelist->next->next->prev = freelist;
	list_item* ret = freelist->next;
	freelist->next = freelist->next->next;
	return (void*)ret; 
}









