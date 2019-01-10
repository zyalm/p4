// Kenny Jin jjin59@wisc.edu
// Ying Zhang yzhang865@wisc.edu

#ifndef TREE
#define TREE

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#define BLACK false
#define RED true
#define LEAF NULL

typedef struct node Node;

struct node {

	void *addr;
	size_t len;
	Node *left;
	Node *right;
	Node *parent;
	bool freed; // True indicates the memory range has been freed, false means allocated
	bool color; // True indicates the color of the node is red

};

typedef struct rangeTree {

	//int num_nodes;
	Node *root;

} RangeTree;

typedef struct nodeList {

	int num_nodes;
	Node ** nodeArray;

} NodeList;

Node * parent(Node *n);

Node * grandparent(Node *n);

Node * sibling(Node *n);

Node * uncle(Node *n);

void rotate_left(Node *n);

void rotate_right(Node *n);

void replace_node(Node* n, Node* child);

Node* delete_one_child(Node* n);

void delete_case1(Node* n);

void delete_case2(Node* n);

void delete_case3(Node* n);

void delete_case4(Node* n);

void delete_case5(Node* n);

void delete_case6(Node* n);


// This function is a constructor of Node
Node * createNode(void *addr, size_t len);

void destroyNode(Node *node);

void setLeftNode(Node *node, Node *left);

void setRightNode(Node *node, Node *right);

//void setParentNode(Node *node, Node *parent);

void setFreed(Node *node, bool freed);

void setColor(Node *node, bool color);

NodeList * createNodeList();

void destroyNodeList(NodeList *nl);

void add_NodeList(NodeList *nl, Node *node);

// This function will insert the tuple (ptr, size) into the Tree structure.
Node * recordTuple(Node *root, void *ptr, size_t size, bool freed);

// This function determines if the address range specified by ptr and size
// is fully within a range in the range tree
bool addrRangeInTree(Node *node);

// This function gets the node from the tree according to the address ptr
// If node is not in the tree, return NULL
// If ptr is NULL, return NULL
Node * getNode(Node *root, void *ptr);

// This function gets the nodes from the tree according to the range
// The range is specified by ptr and size
// All the nodes that are overlapped with the range is returned, 
// regardless of the node's status (freed, alloc'd)
NodeList * getOverlappedNodes(Node *root, void *ptr, size_t size);

// This function determines if the range in the node has overlapped with
// the range specifed by ptr and size
bool nodeOverlapped(Node *node, void *ptr, size_t size);

// This function determines if the address range is allocated.
// The address range is specifed by the node
// If node is NULL, false is returned.
bool addrRangeAllocated(Node *node);

// This function determines if the address range is freed.
// The address range is specifed by the node
// If node is NULL, false is returned.
bool addrRangeFreed(Node *node);

// This function determines if the address specified by ptr is in the tree.
// The tree contains tuples, (ptri, sizei). This is to find if any of the ptri is equal to ptr.
// The function does not care whether the memory range specifed by ptr is freed or not.
bool startAddrInTree(Node *node, void *ptr);

// This function determines if the address specified by ptr is in the tree.
// The tree contains tuples, (ptri, sizei). This is to find if any of the ptri is equal to ptr.
// The function does not care whether the memory range specifed by ptr is freed or not.
bool startAddrInTree(Node *node, void *ptr);

// This function removes the node contains ptr from the range tree.
// This function will only be used when the range of ptr is already freed
// and there is an overlap between the freed region and the newly allocated region
Node * removeTuple(Node *node);

Node * findRoot(Node *nodeInTree);

Node * findMax(Node *root);

Node *KeepTreeBalance(Node *root, Node *newNode);

Node *leftRotate(Node *node, Node *root);

Node *rightRotate(Node *node, Node *root);

int compareTuples(Node *n1, Node *n2);

#endif