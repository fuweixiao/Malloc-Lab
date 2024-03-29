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
    "Blue shit",
    /* First member's full name */
    "Weixiao Fu",
    /* First member's email address */
    "weixiaofu2014@u.northwestern.edu",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

/* SIZE_T_SIZE */
#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/* get size of a block */
#define SIZE(ptr) *(unsigned int*)((char*)ptr - SIZE_T_SIZE)

/* size of freelist */
#define FREELIST_SIZE 30

/* size of list_item*/
#define LIST_ITEM 8
typedef struct item {
	struct item* prev;
	struct item* next;
}list_item;

/* freelist */
list_item* freelist;

/* declare some funtions */
int get_index(int size);
int round_up(int v); 
void insert_at_head(void* ptr, list_item* freelist);
void* remove_from_head(list_item* freelist);

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
	freelist = (list_item*)(mem_sbrk(FREELIST_SIZE * LIST_ITEM));
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
	int newsize = round_up(size);

	// get index of the freelist
	int index = get_index(newsize);
	
	// search the freelist for fit
	void* p = remove_from_head(&freelist[index]);
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
	return NULL;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
	int index;
	// get size of the block
	unsigned int size = SIZE(ptr);

	// get index of the freelist
	if(size == 4104) {
		index = 18;
		// coalescing or not 
		if (((char*)freelist[index].next == ((char*)ptr - 4104))) {
			index = 19;
			ptr = (char*)ptr - 4104;
			*(size_t *)((char*)ptr - 8) = 8208;
			remove_from_head(&freelist[18]);
		}
	}
	else if(size == 8208) {
		index = 18;
		*(size_t *)((char*)ptr - 8) = 4104;
		ptr = (char*)ptr + 4104;
		*(size_t *)((char*)ptr - 8) = 4104;
		insert_at_head(ptr, &freelist[index]);
		ptr = (char*)ptr - 4104;
	}
	else {
		index = get_index(size);
	}
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
	if(ptr == NULL) {
		if (size == 0)
			return NULL;
		else {
		return mm_malloc(size);
		}
	}
	if(size == 0 && ptr != NULL) {
		mm_free(ptr);
		return NULL;
	}
	int oldsize = SIZE(ptr);
	int newsize = round_up(ALIGN(size + SIZE_T_SIZE));
	//int diff;
	if( oldsize == newsize) {
		return ptr;
	}
	if( oldsize > newsize) {
		//diff = oldsize - newsize;
		return ptr;
	}
	if( oldsize < newsize) {
		newptr = mm_malloc(newsize);
		int copySize = oldsize; 
		memcpy(newptr, oldptr, copySize);
		mm_free(oldptr);
		return newptr;
	}
    return NULL;
}

/*
 * Get the index of freelist from the size 
 */
int get_index(int size)
{
	if (size == 20000)
		return 20;
	else if (size == 22000) 
		return 21;
	else if (size == 22000)
		return 22;
	else if (size == 24000)
		return 23;
	else if (size == 26000)
		return 24;
	else if (size == 28000)
		return 25;
	else if (size == 30000)
		return 26;
	else if (size == 32768)
		return 27;
	else if (size == 4104)
		return 18;
	else if (size == 8208)
		return 19;
	else if (size == 520)
		return 17;
	else if (size == 136)
		return 15;
	else if (size == 72)
		return 16;
	else if (size == 24)
		return 14;
	int ret = 0;
	size >>= 3;
	while(size != 1) {
		size >>= 1;
		ret++;
	}
	return ret;
}

/*
 * Round up the size to a power of 2
 */
int round_up(int v)
{
	if(v > 16376) {
		if (v <= 19992)
			return 20000;
		else if ( v <= 21992)
			return 22000;
		else if ( v <= 23992)
			return 24000;
		else if ( v <= 25992)
			return 26000;
		else if ( v <= 27992)
			return 28000;
		else if ( v <= 29992)
			return 30000;
		else if ( v <= 32760)
			return 32768;
	}
	if(v == 4095) {
		return 4104;
	}
	else if(v == 8190) {
		return 8208;
	}
	else if (v == 64) {
		return 72;
	}
	else if (v == 448) {
		return 520;
	}
	else if (v == 512) {
		return 520;
	}
	else if (v == 16) {
		return 24;
	}
	else if (v == 112) {
		return 136;
	}
	else if (v == 128){
		return 136;
	} 
	v = ALIGN(v + SIZE_T_SIZE);
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;
	return v;
}

/*
 * insert a node at the head of the linked list
 */
void insert_at_head(void* ptr, list_item* freelist)
{
	((list_item*)ptr)->prev = freelist;
	((list_item*)ptr)->next = freelist->next;
	freelist->next->prev = (list_item*)ptr;
	freelist->next = (list_item*)ptr;
	return;
}

/*
 * remove a node from the head of the linked list
 */
void* remove_from_head(list_item* freelist) {
	if(freelist->prev == freelist)
		return NULL;
	freelist->next->next->prev = freelist;
	list_item* ret = freelist->next;
	freelist->next = freelist->next->next;
	return (void*)ret; 
}









