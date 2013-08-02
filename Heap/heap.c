/* heap.c :: Binaarkuhi kasutades f��silise andmestruktuurina d�naamilist massiivi.
 * Autor: Kristjan Kaitsa 
 * Programmeerimine C-keeles (MTAT.03.219) @ 2012 A.D. */
#include "heap.h"

// Funktsioonid kuhja elemendi indeksi j�rgi tema esivanema ning j�rglaste arvutamiseks

__inline unsigned int getIndexOfParent(dArray* arr, unsigned int index) {
	return (index - 1)/2;
}
__inline unsigned int getIndexOfLeftChild(dArray* arr, unsigned int index) {
	return 2*index+1;
}
__inline unsigned int getIndexOfRightChild(dArray* arr, unsigned int index) {
	return 2*index+2;
}

/* Korrastab kuhja liikudes alt �lesse.
 * Vajalik p�rast kuhja lisamist. */
__inline void heap_trickleUp(dArray *arr, unsigned int index) {
	heapNode* self;
	heapNode* parent;
	unsigned int indexOfParent;

	if (index == 0) return;
	indexOfParent = getIndexOfParent(arr, index);
	self = (heapNode*) arr->data[index];
	parent = (heapNode*) arr->data[indexOfParent];

#ifdef MAXHEAP
	if (self->priority > parent->priority)
#else
	if (self->priority < parent->priority)
#endif
		dArray_swap(arr, index, getIndexOfParent(arr, index));

	heap_trickleUp(arr, indexOfParent);
}

// Kontrollib, kas antud indeks eksisteerib (massiiv on piisavalt suur).
__inline unsigned char indexExists(dArray *arr, unsigned int index) {
	if (index < arr->size)
		return TRUE;
	else
		return FALSE;
}

// Tagastab vastavalt kuhja t��bile (minimaalne/maksimaalne kuhi) kahest indeksist parema v��rtuse (prioriteediga) indeksi.
__inline unsigned int getIndexWithBestPriority(dArray *arr, unsigned int index1, unsigned int index2) {
#ifdef MAXHEAP
	if ((((heapNode*)arr->data[index2])->priority) > (((heapNode*)arr->data[index1])->priority))
#else
	if ((((heapNode*)arr->data[index2])->priority) < (((heapNode*)arr->data[index1])->priority))
#endif
		return index2;
	else
		return index1;
}

// Tagastab indeksist endast ning tema vahetutest j�rglastest parima prioriteediga indeksi.
__inline unsigned int nextChild(dArray *arr, unsigned int index) {
	heapNode* self;
	unsigned int indexOfLeftChild;
	unsigned int indexOfRightChild;
	unsigned char leftChildExists;
	unsigned char rightChildExists;

	self = (heapNode*) arr->data[index];
	indexOfLeftChild = getIndexOfLeftChild(arr, index);
	indexOfRightChild = getIndexOfRightChild(arr, index);
	leftChildExists = indexExists(arr, indexOfLeftChild);
	rightChildExists = indexExists(arr, indexOfRightChild);

	if ((!leftChildExists) && (!leftChildExists))
		return index;
	else if (!leftChildExists)
		return getIndexWithBestPriority(arr, index, indexOfRightChild);
	else if (!rightChildExists)
		return getIndexWithBestPriority(arr, index, indexOfLeftChild);
	else
		return getIndexWithBestPriority(arr, index,
			getIndexWithBestPriority(arr, indexOfLeftChild, indexOfRightChild));
}

// Korrastab kuhja liikudes �levalt alla: vajalik p�rast kuhjast kustutamist.
__inline void heap_trickleDown(dArray *arr, unsigned int index) {
	unsigned int lNextChild;

	lNextChild = nextChild(arr, index);
	if (lNextChild != index) {
		dArray_swap(arr, index, lNextChild);
		heap_trickleDown(arr, lNextChild);
	}
}

// Kuhja elemendi lisamine.
void heap_push(dArray *arr, unsigned short int priority, void *data) {
	heapNode *node = (heapNode*) malloc(sizeof(heapNode));

	node->priority = priority;
	node->data = data;
	
	heap_trickleUp(arr, dArray_append(arr, node));
}

// Kuhjast pealmise (parima prioriteediga) elemendi v�tmine.
void* heap_pop(dArray *arr) {
	heapNode *node;
	void *data;

	if (arr->size <= 0) return NULL;
	dArray_swap(arr, 0, arr->size - 1);
	node = (heapNode*)dArray_popLast(arr);
	data = node->data;
	heap_trickleDown(arr, 0);
	free(node);

	return data;
}

// Tagastab, kas kuhi on t�hi.
unsigned char heap_isEmpty(dArray *arr) {
	if (arr->size == 0)
		return TRUE;
	else
		return FALSE;
}

/* H�vitab kuhja, selle alusstruktuuriks oleva massiivi ning kuhja elementide poolt
 * viidatud m�lu.
 * Hoiatus! Programmi k�itumine etteaimamatu (t�en�oliselt crash), kui kuhjas on korduvaid
 * elemente v�i elemendid viitavad staatiliselt eraldatud m�lule jne.  */
void heap_destroyWithContent(dArray *arr) {
	int i;
	heapNode *node;

	for (i = 0; i < arr->size; i++) {
		node = (heapNode*)arr->data[i];
		free(node->data);
		free(node);
	}
	free(arr->data);
	free(arr);
}
