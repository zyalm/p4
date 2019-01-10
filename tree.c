// Kenny Jin jjin59@wisc.edu
// Ying Zhang yzhang865@wisc.edu

#include "tree.h"

#define SIZE 1024

// https://en.wikipedia.org/wiki/Red%E2%80%93black_tree

// This funcion gets the parent of a node
Node * parent(Node *n) {
	return n -> parent;
}

// This funcion gets the grandparent of a node
Node * grandparent(Node *n) {
	Node * p = parent(n);
	if (p == NULL) {
		return NULL; // No parent means no grandparent
	}
	return parent(p); // NULL if parent is root
}

// This funcion gets the sibling of a node
Node * sibling(Node *n) {
	Node * p = parent(n);
	if (p == NULL) {
		return NULL; // No parent means no sibling
	}
	if (n == p -> left) {
		return p -> right;
	} else {
		return p -> left;
	}
}

// This funcion gets the uncle of a node
Node * uncle(Node *n) {
	Node * p = parent(n);
	Node * g = grandparent(n);
	if (g == NULL) {
		return NULL; // No grandparent means no uncle
	}
	return sibling(p);
}

// Perform the left rotation of red-black tree
void rotate_left(Node *n) {
	Node * nnew = n -> right;
	Node * p = parent(n);
	assert(nnew != LEAF);
	n -> right = nnew -> left;
	nnew -> left = n;
	n -> parent = nnew;
	if (n -> right != NULL) {
		n -> right -> parent = n;
	}
	if (p != NULL) {
		if (n == p -> left) {
			p -> left = nnew;
		}
		else if (n == p -> right) {
			p -> right = nnew;
		}
	}
	nnew -> parent = p;
}

// Perform the right rotation of the red-black tree
void rotate_right(Node *n){
	Node *nnew = n -> left;
	Node *p = parent(n);
	assert(nnew != LEAF);
	n -> left = nnew -> right;
	nnew -> right = n;
	n -> parent = nnew;
	if (n -> left != NULL) {
		n -> left -> parent = n;
	}
	if (p != NULL) {
		if (n == p -> left) {
			p -> left = nnew;
		}
		else if (n == p -> right) {
			p -> right = nnew;
		}
	}
	nnew -> parent = p;
}

// Replace one node in the tree with the child
void replace_node(Node* n, Node* child) {
	assert(n != NULL);
	if (child != NULL) {
		child->parent = n->parent;
	}
	if (n -> parent == NULL) {
		return;
	}
    if (n == n->parent->left)
        n->parent->left = child;
    else
        n->parent->right = child;
}

// Delete node n from the tree.
// Precondition: n has at most one non-leaf child.
Node* delete_one_child(Node* n)
{
	Node *nodeInTree; // This is a node in tree to help find root
	assert(n != NULL);
	Node *child;
	if (n -> right != NULL) {
		child = n -> right;
	}
	else if (n -> left != NULL) {
		child = n -> left;
	}
	else {
		child = NULL;
	}
	if (child == NULL) {
		// When child is NULL, the only node that we are sure will be in tree
		// after the removal of n is the parent node of n
		// if the parent is NULL, n itself is root then if we remove it, 
		// no nodes will be left in tree, tree.root = NULL
		nodeInTree = parent(n);
		if (n -> color == RED) {
			replace_node(n, child);
		} else {
			delete_case1(n);
			replace_node(n, child);	
		}
	}
	else {
		// When child is not NULL, 
		// the only node that we are sure will be in tree
		// after the removal of n is the child node of n
		// We cannot use parent this time as n might be root
		// and parent might be NULL
		nodeInTree = child;
		replace_node(n, child);
 		if (n->color == BLACK) {
  			if (child->color == RED)
   				child->color = BLACK;
  			else
   				delete_case1(child);
 		}
	}
 	free(n);
 	return nodeInTree;
}

void delete_case1(Node* n)
{
	if (n->parent != NULL)
  		delete_case2(n);
}

void delete_case2(Node* n)
{
 	Node* s = sibling(n);
 	assert(s != NULL);
 	if (s->color == RED) {
 		n->parent->color = RED;
  		s->color = BLACK;
  		if (n == n->parent->left)
   			rotate_left(n->parent);
  		else
   			rotate_right(n->parent);
 	}
 	delete_case3(n);
}

void delete_case3(Node* n)
{
	Node* s = sibling(n);
	assert(s != NULL);
 	if ((n->parent->color == BLACK) &&
     		(s->color == BLACK) &&
     			((s -> left == NULL)||(s->left->color == BLACK)) &&
     				((s -> right == NULL)||(s->right->color == BLACK))) {
  		s->color = RED;
  		delete_case1(n->parent);
 	} 
 	else
  		delete_case4(n);
}

void delete_case4(Node* n)
{
 	Node* s = sibling(n);
 	assert(s != NULL);
 	if ((n->parent->color == RED) &&
     		(s->color == BLACK) &&
     			((s -> left == NULL) || (s->left->color == BLACK)) &&
     				((s -> right == NULL) || (s->right->color == BLACK))) {
  		s->color = RED;
  		n->parent->color = BLACK;
 	} 
 	else
  		delete_case5(n);
}

void delete_case5(Node* n)
{
 	Node* s = sibling(n);
 	assert(s != NULL);
	if  (s->color == BLACK) { 
  		if ((n == n->parent->left) &&
      			((s -> right == NULL) || s->right->color == BLACK) &&
      				((s -> left != NULL) && s->left->color == RED)) {
   			s->color = RED;
   			s->left->color = BLACK;
   			rotate_right(s);
  		} else if ((n == n->parent->right) &&
             			((s -> left == NULL) || s->left->color == BLACK) &&
             				((s -> right != NULL) && s->right->color == RED)) {/* this last test is trivial too due to cases 2-4. */
   			s->color = RED;
  			s->right->color = BLACK;
   			rotate_left(s);
  		}
 	}
 	delete_case6(n);
}

void delete_case6(Node* n)
{
 	Node* s = sibling(n);
 	assert(s != NULL);
 	s->color = n->parent->color;
 	n->parent->color = BLACK;
 	if (n == n->parent->left) {
 		assert(s -> right != NULL);
 		assert(s -> right -> color == RED);
  		s->right->color = BLACK;
  		rotate_left(n->parent);
 	} else {
 		assert(s -> left != NULL);
 		assert(s -> left -> color == RED);
  		s->left->color = BLACK;
  		rotate_right(n->parent);
 	}
}

// This function is a constructor of Node
Node * createNode(void *addr, size_t len) {
	Node *node;
	if ((node = malloc(sizeof(Node))) == NULL) {
		fprintf(stderr, "%s\n", "Malloc for node failed");
		exit(-1);
	}
	node -> addr = addr;
	node -> len = len;
	node -> left = NULL;
	node -> right = NULL;
	node -> parent = NULL;
	node -> freed = false;
	node -> color = BLACK;
	return node;
}

// This function frees a node
void destroyNode(Node *node) {
	if (node != NULL) {
		free(node);
	}
}

// Set left node of the node
void setLeftNode(Node *node, Node *left) {
	node -> left = left;
	left -> parent = node;
}

// Set right node of the node
void setRightNode(Node *node, Node *right) {
	node -> right = right;
	right -> parent = node;
}

// Set the freed status of a node
void setFreed(Node *node, bool freed) {
	node -> freed = freed;
}

// Set color of a node
void setColor(Node *node, bool color) {
	node -> color = color;
}

// Create a node list. Used for store search results.
NodeList * createNodeList() {
	NodeList *nl;
	if ((nl = malloc(sizeof(NodeList))) == NULL) {
		fprintf(stderr, "%s\n", "Malloc for node list failed");
		exit(-1);
	}
	nl -> num_nodes = 0;
	if ((nl -> nodeArray = malloc(SIZE * sizeof(Node*))) == NULL) {
		fprintf(stderr, "%s\n", "Malloc for node array failed");
		exit(-1);
	}
	return nl;
}

// Free a node list.
void destroyNodeList(NodeList *nl) {
	if (nl != NULL) {
		if (nl -> nodeArray != NULL) {
			free(nl -> nodeArray);
		}
		free(nl);
	}
}

// Add node to nodelist
void add_NodeList(NodeList *nl, Node *node) {
	if (nl == NULL) {
		fprintf(stderr, "%s\n", "NodeList is NULL");
		exit(-1);
	}
	if (node == NULL) {
		fprintf(stderr, "%s\n", "Node is NULL");
		exit(-1);
	}
	nl -> nodeArray[nl -> num_nodes] = node;
	nl -> num_nodes += 1;
}



// This function will insert the tuple (ptr, size) into the Tree structure. 
// This will also decide the inserted node is freed or not
Node * recordTuple(Node *root, void *ptr, size_t size, bool freed) {
	Node *newNode = createNode(ptr, size);
	newNode -> freed = freed;
	if(root == NULL) {
		setColor(newNode, false);
		root = newNode;
	} else{
		Node *traverseNode;
		Node *tempRoot = root;
		// Keep traversing until adding newnode to correct position
		while(tempRoot != NULL) {
			traverseNode = tempRoot;
			// When two nodes are overlapped
			if(compareTuples(newNode, tempRoot) == 0) {
				fprintf(stderr, "%s\n", "Error, adding two overlapped nodes");
				exit(-1);
			}
			if(compareTuples(newNode, tempRoot) == 1) {
				tempRoot = tempRoot->right;			
			} else if(compareTuples(newNode, tempRoot) == -1) {
				tempRoot = tempRoot->left;
			}
		}
		newNode->parent = traverseNode;		
		if(compareTuples(newNode, traverseNode) == 1) {
			traverseNode->right = newNode;
		}
		if(compareTuples(newNode, traverseNode) == -1) {
			traverseNode->left = newNode;
		}
		setColor(newNode, true);
		root = KeepTreeBalance(root,newNode);
	}

	return root;
}

// Keep the red black tree balanced after insertion
Node *KeepTreeBalance(Node *root, Node *newNode) {
		while(newNode != root && newNode->parent->color == true) {
			Node *uncleNode;
			// Find uncleNode
			if(newNode->parent == newNode->parent->parent->left) {
				uncleNode = newNode->parent->parent->right;
			} else{
				uncleNode = newNode->parent->parent->left;
			}

			// ….a) If x’s uncle is RED (Grand parent must have been black from property 4)
			// ……..(i) Change color of parent and uncle as BLACK.
			// ……..(ii) color of grand parent as RED.
			// ……..(iii) Change x = x’s grandparent, repeat steps 2 and 3 for new x
			if(uncleNode != NULL && uncleNode->color == true && uncleNode->parent->color == false) {
				uncleNode->color = false;
				newNode->parent->color = false;
				uncleNode->parent->color = true;
				newNode = newNode->parent->parent;

				if(newNode == root && newNode->color == true){
					setColor(root, false);
				}
				
			}
			// ….b) If x’s uncle is BLACK, then there can be four configurations for x, 
			// x’s parent (p) and x’s grandparent (g) (This is similar to AVL Tree)
			else if(uncleNode == NULL || uncleNode->color == false) {
				// ……..i) Left Left Case (p is left child of g and x is left child of p)
				//			1) right Rotate grandparent
				// 			2) swap colors of newnode's parent and grandparent
				if(newNode->parent == newNode->parent->parent->left && newNode == newNode->parent->left) {
//					rightRotate(newNode->parent->parent, root); //RightRotate
					bool swapParentColor = newNode->parent->color;
					newNode->parent->color = newNode->parent->parent->color;
					newNode->parent->parent->color = swapParentColor;
					root = rightRotate(newNode->parent->parent, root);
				}
				// ……..ii) Left Right Case (p is left child of g and x is right child of p)
				// 			1) left Rotate parent
				//			2) Apply left left case
				else if(newNode->parent == newNode->parent->parent->left && newNode == newNode->parent->right) {
//					leftRotate(newNode->parent, root);
					
					// bool swapParentColor = newNode->parent->color;
					// newNode->parent->color = newNode->parent->parent->color;
					// newNode->parent->parent->color = swapParentColor;
					newNode->parent->parent->color = RED;
					newNode->color = BLACK;
					root = leftRotate(newNode->parent, root);
					root = rightRotate(newNode->parent, root); //RightRotate

				}
				// ……..iii) Right Right Case (Mirror of case a)
				//			1) Left rotate grandparent
				//			2) swap color of grandparent and parent 
				else if(newNode->parent == newNode->parent->parent->right && newNode == newNode->parent->right) {
//					leftRotate(newnNode->parent->parent, root);
					bool swapParentColor = newNode->parent->color;
					newNode->parent->color = newNode->parent->parent->color;
					newNode->parent->parent->color = swapParentColor;
					root = leftRotate(newNode->parent->parent, root);
				}
				// ……..iv) Right Left Case (Mirror of case c)
				// 			1) right Rotate parent
				//			2) Apply right right case
				else if(newNode->parent == newNode->parent->parent->right && newNode == newNode->parent->left) {
					newNode->parent->parent->color = RED;
					newNode->color = BLACK;
					root = rightRotate(newNode->parent, root); //RightRotate
                    root = leftRotate(newNode->parent->parent, root);

				}
			}
		} 
		

		return root;
}

// left rotate for insertion as it returns new root
Node *leftRotate(Node *node, Node *root) {
	Node *temp = node->right;
	node->right = temp->left;
	if(node->right != NULL) {
		node->right->parent = node;
	}
	temp->parent = node->parent;
	if(node->parent == NULL) {
		root = temp;
	} else if(node == node->parent->left) {
		node->parent->left = temp;
	} else {
		node->parent->right = temp;
	}
	temp->left = node;
	node->parent = temp;

	return root;
}

// right rotate for insertion as it returns new root
Node *rightRotate(Node *node, Node *root) {
	Node *temp = node->left;
	node->left = temp->right;
	if(temp->right != NULL) {
		temp->right->parent = node;
	}
	temp->parent = node->parent;
	if(temp->parent == NULL) {
		root = temp;
	}else if(node == node->parent->left) {
		node->parent->left = temp;
	} else {
		node->parent->right = temp;
	}
	temp->right = node;
	node->parent = temp;

	return root;
}

// This function will compare two nodes. Return true means n1 is bigger than n2, otherwise return false.
// For example, node n1 is [1,2); node n2 is [3,4), in this case, return false. 
int compareTuples(Node *n1, Node *n2) {
	if(nodeOverlapped(n1, n2->addr, n2->len)) {
		return 0;
	}
	void * n1startAddr = n1->addr;
	void * n1endAddr = n1startAddr + (n1->len);
	void * n2lowerBound = n2->addr;
//	void * n2upperBound = n2lowerBound + (n2->len);

	if(n1endAddr > n2lowerBound) {
		return 1;
	}
	return -1;
}

// This function determines if the address range specified by ptr and size
// is fully within a range in the range tree
bool addrRangeInTree(Node *node) {
	//Node * node = getNode(ptr, size);
	if (node != NULL) {
		return true;
	}
	return false;
}

// This function gets the node from the tree according to the address ptr
// If node is not in the tree, return NULL
// If ptr is NULL, return NULL
Node * getNode(Node *root, void *ptr) {
	NodeList * nodelist = getOverlappedNodes(root, ptr, (size_t)0);
	if (nodelist -> num_nodes == 0) {
		destroyNodeList(nodelist);
		return NULL;
	}
	assert(nodelist -> num_nodes == 1);
	Node *node = nodelist -> nodeArray[0];
	destroyNodeList(nodelist);
	return node;
}

//https://www.geeksforgeeks.org/print-bst-keys-in-the-given-range/
void overLappedHelper(NodeList *nl, Node *root, void *ptr, size_t size) {
	 /* base case */
   if ( NULL == root ) 
      return; 
   /* Since the desired o/p is sorted, recurse for left subtree first 
      If root->data is greater than k1, then only we can get o/p keys 
      in left subtree */
   if (ptr < root -> addr)
   {
   	overLappedHelper(nl, root->left, ptr, size); 
   }
   /* if root's data lies in range, then add root to list */
   if (nodeOverlapped(root, ptr, size)) 
     add_NodeList(nl, root);
  
  /* If root->data is smaller than k2, then only we can get o/p keys 
      in right subtree */
   if ((root -> addr + root -> len) < (ptr + size))
     overLappedHelper(nl, root->right, ptr, size); 
} 


// This function gets the nodes from the tree according to the range
// The range is specified by ptr and size
// All the nodes that are overlapped with the range is returned, 
// regardless of the node's status (freed, alloc'd)
// The nodes in the returned nodelist will not be NULL
// www.geeksforgeeks.org/print-bst-keys-in-given-range-o1-space/
// This is a modification of Morris Traversal
NodeList * getOverlappedNodes(Node *root, void *ptr, size_t size) {
	NodeList * nodelist = createNodeList();
	// if (root == NULL) {
	// 	fprintf(stderr, "%s\n", "Root is NULL");
	// 	exit(-1);
	// }
	overLappedHelper(nodelist, root, ptr, size);

	return nodelist;
}

// This function determines if the range in the node has overlapped with
// the range specifed by ptr and size
// If overlapped, return true, otherwise. return false
bool nodeOverlapped(Node *node, void *ptr, size_t size) {
	void * startAddr = node -> addr;
	void * endAddr = startAddr + (node -> len);
	// Sanity check, end addr should not be smaller than start addr
	if (endAddr < startAddr) {
		fprintf(stderr, "%s\n", "Error in end address");
		exit(-1);
	}
	void * lowerBound = ptr;
	void * upperBound = ptr + size;
	// Sanity check, upperBound should not be smaller than lowerBound
	if (upperBound < lowerBound) {
		fprintf(stderr, "%s\n", "Error in end upperBound");
		exit(-1);
	}
	// Deal with the situation where size == 0
	if ((endAddr == startAddr) && (upperBound != lowerBound)) {
		if (endAddr >= lowerBound && endAddr < upperBound) {
			return true;
		}
		return false;
	}
	// Deal with the situation where size == 0
	if ((endAddr != startAddr) && (upperBound == lowerBound)) {
		if (upperBound >= startAddr && upperBound < endAddr) {
			return true;
		}
		return false;
	}
	// Deal with the situation where size == 0
	if ((endAddr == startAddr) && (upperBound == lowerBound)) {
		if (endAddr == upperBound) {
			return true;
		}
		return false;
	}
	if (startAddr < upperBound && startAddr >= lowerBound) {
		return true;
	}
	if (endAddr > lowerBound && endAddr <= upperBound) {
		return true;
	}
	// Corresponds to start, lower, upper, end (The sequence)
	if (startAddr <= lowerBound && endAddr >= upperBound) {
		return true;
	}
	return false;
}

// Return a true or false for whether the range is alloc'd
bool addrRangeAllocated(Node *node) {
	//Node * node = getNode(ptr, size);
	if (node == NULL) {
		return false;
	}
	return !(node -> freed);
}

// Return a true or false for whether the range is freed
bool addrRangeFreed(Node *node) {
	if (node == NULL) {
		return false;
	}
	return node -> freed;
}

// This function determines if the address specified by ptr is in the tree.
// The tree contains tuples, (ptri, sizei). This is to find if any of the ptri is equal to ptr.
// The function does not care whether the memory range specifed by ptr is freed or not.
bool startAddrInTree(Node *node, void *ptr) {
	//Node * node = getNode(ptr, 1);
	if (node == NULL) {
		return false;
	}
	if (node -> addr == ptr) {
		return true;
	}
	return false;

}

// This function removes the node contains ptr from the range tree.
// This function will only be used when the range of ptr is already freed
// and there is an overlap between the freed region and the newly allocated region
Node * removeTuple(Node *node) {
	assert(node != NULL);
	Node *curr = node;
	if (curr -> left != NULL && curr -> right != NULL) {
		Node *temp = findMax(curr -> left);
		curr -> addr = temp -> addr;
		curr -> len = temp -> len;
		curr -> freed = temp -> freed;
		curr = temp;
	}
	Node * nodeInTree = delete_one_child(curr);
	Node * root = findRoot(nodeInTree);
	return root;
}

// This will return the root node of the tree
// Any node in the tree will help
// This function assumes Tree is constructed correctly
// and there is no loop in the tree (A -> parent == B, B -> parent  == A)
Node * findRoot(Node *nodeInTree) {
	Node *curr;
	curr = nodeInTree;
	if (curr == NULL) {
		return NULL;
	}
	while(curr -> parent != NULL) {
		curr = curr -> parent;
	}
	return curr;
}

// Find the max node in the (sub)tree represented by root
Node * findMax(Node *root) {
	Node *curr;
	curr = root;
	if (curr == NULL) {
		return NULL;
	}
	while (curr -> right != NULL) {
		curr = curr -> right;
	}
	return curr;
}

