/* heap.h :: Binaarkuhi kasutades füüsilise andmestruktuurina dünaamilist massiivi.
 * Autor: Kristjan Kaitsa 
 * Programmeerimine C-keeles (MTAT.03.219) @ 2012 A.D. */
#pragma once
#include "dArray.h"
#define TRUE 1
#define FALSE 0
// Seadistus:
#define INIT_ARRAY_SIZE 16		// Algne põhja suurus
//#define MAXHEAP

typedef struct tagHeapNode {
	unsigned short int priority;
	void *data;
} heapNode;

__inline dArray* heap_new(void);
void* heap_pop(dArray *arr);
void heap_push(dArray *arr, unsigned short int priority, void *data);
unsigned char heap_isEmpty(dArray *arr);
void heap_destroyWithContent(dArray *arr);

__inline dArray* heap_new(void) {
	return dArray_new(INIT_ARRAY_SIZE);
}
