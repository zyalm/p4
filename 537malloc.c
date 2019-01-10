// Kenny Jin jjin59@wisc.edu
// Ying Zhang yzhang865@wisc.edu

#include "537malloc.h"

// In addition to actually allocating the memory by calling malloc(), 
// this function will record a tuple (addri, leni), for the memory that you allocate in the heap.
void *malloc537(size_t size){

	void *ptr;
	if (size == 0) {
		fprintf(stderr, "%s\n", "Memory size is 0.");
		//return NULL;
	}
	ptr = malloc(size);
	if (ptr == NULL) {
		return NULL;
	}
	NodeList *overlapped = getOverlappedNodes(rangeTree.root, ptr, size);

	// If the new malloc does not use the previously freed regions
	// Just insert the node to the tree
	if (overlapped -> num_nodes == 0) {
		destroyNodeList(overlapped);
		rangeTree.root = recordTuple(rangeTree.root, ptr, size, false); // Insertion
		return ptr;
	}

	// Sanity check
	// All the overlapped node should be freed, otherwise there is an error
	for (int i = 0; i < overlapped -> num_nodes; i++) {
		if (!overlapped -> nodeArray[i] -> freed) {
			fprintf(stderr, "%s\n", "Sanity check failed, the overlapped node is not freed");
			exit(-1);
		}
	}

	void * firstNodeStartAddr = overlapped -> nodeArray[0] -> addr;

	// Remove all the overlapped freed nodes
	for (int i = 0; i < overlapped -> num_nodes; i++) {
		rangeTree.root = removeTuple(overlapped -> nodeArray[i]); // Deletion
		//destroyNode(overlapped -> nodeArray[i]);
	}
	destroyNodeList(overlapped);
	// If the ptr is in the middle of a freed block,
	// we need to add the first half of the freed block back to tree
	if (firstNodeStartAddr < ptr) {
		rangeTree.root = recordTuple(rangeTree.root, firstNodeStartAddr, ptr - firstNodeStartAddr, true);
	}

	rangeTree.root = recordTuple(rangeTree.root, ptr, size, false); // Insertion
	return ptr;
}

// This function will first check to make sure that freeing the memory specified by ptr makes sense,
// then will call free() to do the actual free. Some of the error conditions are checked
void free537(void *ptr) {

	if (ptr == NULL) {
		fprintf(stderr, "%s\n", "Trying to free NULL pointer");
		exit(-1);
	}

	Node *node = getNode(rangeTree.root, ptr);

    // Trying to free memory that has not been allocated with malloc537().
	if (!addrRangeInTree(node)) {
		fprintf(stderr, "The memory address %p has not been allocated with malloc537().\n", ptr);
		exit(-1);
	}

    // Freeing memory that is not the first byte of the range of memory that was allocated.
	if (!startAddrInTree(node, ptr)) {
		fprintf(stderr, "The memory address %p is not the first byte of the range of memory that was allocated.\n", ptr);
		exit(-1);
	}

    // Freeing memory that was previously freed (double free). 
    // How to deal with freed memory address is tricky
    if (addrRangeFreed(node)) {
    	fprintf(stderr, "The memory address %p was previously freed.\n", ptr);
		exit(-1);
    }

	node -> freed = true;
    free(ptr);
}

// If ptr is NULL,then this follows the specification of malloc537() above. 
// If size is zero and ptr is not NULL,then this follows the specification 
// of free537() above. Otherwise, in addition to changing the memory allocation 
// by calling realloc(), this function will first check to see if there was a 
// tuple for the (addr = ptr, and removes that tuple, then adds a new one where 
// addr is the return value from realloc() and len is size
void *realloc537(void *ptr, size_t size){
	void *newptr;
	if (ptr == NULL) {
		newptr = malloc537(size);
		return newptr;
	}
	else if (size == 0) {
		free537(ptr);
		return NULL;
	}

	Node *node = getNode(rangeTree.root, ptr);

	// Freeing memory that has not been allocated with malloc537().
	if (!addrRangeInTree(node)) {
		fprintf(stderr, "The memory address %p has not been allocated with malloc537().\n", ptr);
		exit(-1);
	}

    // Freeing memory that is not the first byte of the range of memory that was allocated.
	if (!startAddrInTree(node, ptr)) {
		fprintf(stderr, "The memory address %p is not the first byte of the range of memory that was allocated.\n", ptr);
		exit(-1);
	}

    // Freeing memory that was previously freed (double free). 
    // How to deal with freed memory address is tricky
    if (addrRangeFreed(node)) {
    	fprintf(stderr, "The memory address %p was previously freed.\n", ptr);
		exit(-1);
    }

	rangeTree.root = removeTuple(node);
	// if ((newptr = realloc(ptr, size)) == NULL) {
	// 	fprintf(stderr, "%s\n", "Failed to reallocate memory.");
	// 	exit(-1);
	// }
	newptr = realloc(ptr, size);

	if (newptr == NULL) {
		return NULL;
	}

	NodeList *overlapped = getOverlappedNodes(rangeTree.root, newptr, size);

	// If the new malloc does not use the previously freed regions
	// Just insert the node to the tree
	if (overlapped -> num_nodes == 0) {
		rangeTree.root = recordTuple(rangeTree.root, newptr, size, false); // Insertion
		destroyNodeList(overlapped);
		return newptr;
	}

	// Sanity check
	// All the overlapped node should be freed, otherwise there is an error
	for (int i = 0; i < overlapped -> num_nodes; i++) {
		if (!overlapped -> nodeArray[i] -> freed) {
			fprintf(stderr, "%s\n", "Sanity check failed, the overlapped node is not freed");
			exit(-1);
		}
	}

	void * firstNodeStartAddr = overlapped -> nodeArray[0] -> addr;

	// Remove all the overlapped freed nodes
	for (int i = 0; i < overlapped -> num_nodes; i++) {
		rangeTree.root = removeTuple(overlapped -> nodeArray[i]); // Deletion
		//destroyNode(overlapped -> nodeArray[i]);
	}
	destroyNodeList(overlapped);
	// If the ptr is in the middle of a freed block,
	// we need to add the first half of the freed block back to tree
	if (firstNodeStartAddr < newptr) {
		rangeTree.root = recordTuple(rangeTree.root, firstNodeStartAddr, newptr - firstNodeStartAddr, true);
	}

	rangeTree.root = recordTuple(rangeTree.root, newptr, size, false);
	return newptr;
}

// This function checks to see the address range specified by address ptr and length size 
// are fully within a range allocated by malloc537() and memory not yet freed by free537(). 
// When an error is detected, then print out a detailed and informative error message and 
// exit the program (with a -1 status). 
void memcheck537(void *ptr, size_t size){
	NodeList *overlapped = getOverlappedNodes(rangeTree.root, ptr, size);
	if (overlapped -> num_nodes != 1) {
		fprintf(stderr, "the address range specified by address %p and length %zu are not fully within a range allocated by malloc537().\n", ptr, size);
		exit(-1);
	}
	Node *node = overlapped -> nodeArray[0];
	destroyNodeList(overlapped);
	if ((node -> addr) > ptr || ((node -> addr) + (node -> len)) < (ptr + size)) {
		fprintf(stderr, "the address range specified by address %p and length %zu are not fully within a range allocated by malloc537().\n", ptr, size);
		exit(-1);
	}
	if (addrRangeFreed(node)) {
		fprintf(stderr, "the address range specified by address %p and length %zu are already freed.\n", ptr, size);
		exit(-1);
	}
}














