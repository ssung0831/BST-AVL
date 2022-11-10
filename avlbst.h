#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);


    // Add helper functions here
		void insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* c);
		void removeFix(AVLNode<Key, Value>* n, int diff);
		void rotateLeft(AVLNode<Key, Value>* n);
		void rotateRight(AVLNode<Key, Value>* n);
		AVLNode<Key, Value>* predecessor(AVLNode<Key, Value>* current);

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // // TODO
    //If empty tree => set n as root, b(n) = 0, done!
    if(this -> root_ == NULL){
        AVLNode<Key, Value>* r = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
        r -> setBalance(0);
        this -> root_ = r;
				return;
    }
    
		//Else insert n
		//walk the tree to a leaf, p
		AVLNode<Key, Value>* walker = static_cast<AVLNode<Key, Value>*> (this -> root_);
    AVLNode<Key, Value>* potentialParent;
    
    while(walker != NULL){

			//go left if value is less than the node
			if (new_item.first < walker -> getKey()){
				if(walker -> getLeft() == NULL){ //we are at leaf node
					potentialParent = walker;
					break;
				}
				walker = walker-> getLeft();
			}
			//go right if value is greater than the node
			else if(new_item.first > walker -> getKey()){
				if(walker -> getRight() == NULL){ //we are at a leaf node
					potentialParent = walker;
					break;
				}
				walker = walker -> getRight();
			}
			//if the value is the same as the node
			else if (new_item.first == walker -> getKey()){
				walker -> setValue(new_item.second);
				return;
			}
  	}
    
		//insert the new node as the leaf node's child and set balance to 0
		AVLNode<Key, Value>* insertedNode = new AVLNode<Key, Value>(new_item.first, new_item.second, potentialParent);
		insertedNode -> setBalance(0);

		if(potentialParent -> getKey() > new_item.first){
			potentialParent -> setLeft(insertedNode);
			insertedNode -> setParent(potentialParent);
		}
		else{
			potentialParent -> setRight(insertedNode);
			insertedNode -> setParent(potentialParent);
		}

		//If b(p) was -1, then b(p) = 0. Done!
		if(potentialParent -> getBalance() == -1){
			potentialParent -> setBalance(0);
			return;
		}
		//If b(p) was +1, then b(p) = 0. Done!
		else if (potentialParent -> getBalance() == 1){
			potentialParent -> setBalance(0);
			return;
		}
		//If b(p) was 0, then update b(p) and call insertFix(p, n) 
		else if (potentialParent -> getBalance() == 0){
			if(insertedNode == potentialParent -> getLeft()){
				potentialParent -> setBalance(-1);
			}
			else{
				potentialParent -> setBalance(1);
			}
			insertFix(potentialParent, insertedNode);
			return;
		}
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* c){

	AVLNode<Key, Value>* g = p -> getParent();
	
	//If p is null or parent(p) is null, return
	if(p == NULL || g == NULL){
		return;
	}

	//p is left child of g 
	if(g -> getLeft() == p){
		g -> setBalance(g -> getBalance() - 1);

		if(g -> getBalance() == 0){
			return;
		}
		
		if (g -> getBalance() == -1){
			insertFix(g, p); //recurse
			return;
		}
		else{ //b(g) == -2
			/*zig-zig*/ 
			if(c == p -> getLeft()){
				rotateRight(g);
				p -> setBalance(0);
				g -> setBalance(0);
			}
			else{ //zig-zag
				rotateLeft(p);
				rotateRight(g);

				if(c -> getBalance() == -1){
					p -> setBalance(0);
					g -> setBalance(1);
					c -> setBalance(0);
				}
				else if (c -> getBalance() == 0){
					p -> setBalance(0);
					g -> setBalance(0);
					c -> setBalance(0);
				}
				else{ //b(n) == +1
					p -> setBalance(-1);
					g -> setBalance(0);
					c -> setBalance(0);
				}
			}
		}
	}
	//if p is right child of g, swap left/right, +/-
	else{
		g -> setBalance(g -> getBalance() + 1);

		if(g -> getBalance() == 0){
			return;
		}

		if (g -> getBalance() == 1){
			insertFix(g, p); //recurse
			return;
		}
		else{ //b(g) == 2
		/*zig-zig*/ 
			if(c == p -> getRight()){
				rotateLeft(g);
				p -> setBalance(0);
				g -> setBalance(0);
			}
			else{ //zig-zag
				rotateRight(p);
				rotateLeft(g);

				if(c -> getBalance() == 1){
					p -> setBalance(0);
					g -> setBalance(-1);
					c -> setBalance(0);
				}
				else if (c -> getBalance() == 0){
					p -> setBalance(0);
					g -> setBalance(0);
					c -> setBalance(0);
				}
				else{ //b(n) == -1
					p -> setBalance(1);
					g -> setBalance(0);
					c -> setBalance(0);
				}
			}		
		}
	}
}


template<class Key, class Value>
//Define a left rotation as taking a right child, making it the parent 
//and making the original parent the new left child
void AVLTree<Key, Value>:: rotateLeft(AVLNode<Key, Value>* n){
	AVLNode<Key, Value>* p = n -> getParent();
	AVLNode<Key, Value>* c = n -> getRight();

	//this makes the right child the parent
	if(p == NULL){
		this -> root_ = c;
		c -> setParent(p);
	}
	else if(p -> getLeft() == n){
		p -> setLeft(c);
		c -> setParent(p);
	}
	else if (p -> getRight() == n){
		p -> setRight(c);
		c -> setParent(p);
	}

	AVLNode<Key, Value>* next = c -> getLeft();
	//this makes the original node the new left child
	c -> setLeft(n);
	n -> setParent(c);
	n -> setRight(next);
	//attach on the reset
	if(next != NULL){
		next -> setParent(n);
	}

}

template<class Key, class Value>
//Define a right rotation as taking a left child, making it the parent 
//and making the original parent the new right child
void AVLTree<Key, Value>:: rotateRight(AVLNode<Key, Value>* n){
	AVLNode<Key, Value>* p = n -> getParent();
	AVLNode<Key, Value>* c = n -> getLeft();

	//this makes the left child the parent
	if(p == NULL){
		this -> root_ = c;
		c -> setParent(p);
	}
	else if(p -> getLeft() == n){
		p -> setLeft(c);
		c -> setParent(p);
	}
	else if (p -> getRight() == n){
		p -> setRight(c);
		c -> setParent(p);
	}

	AVLNode<Key, Value>* next = c -> getRight();
	//this makes the original node the new right child
	c -> setRight(n);
	n -> setParent(c);
	n -> setLeft(next);
	//attach on the reset
	if(next != NULL){
		next -> setParent(n);
	}
}


/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
	//Find node, n, to remove by walking the tree
	AVLNode<Key, Value>* n = static_cast<AVLNode<Key, Value>*> (this -> internalFind(key));

	if(n == NULL){
		delete n;
		return;
	}

	//If n has 2 children, swap positions with in-order predecessor) and perform the next step
	if(n -> getLeft() != NULL && n -> getRight() != NULL){
		AVLNode<Key, Value>* pred = predecessor(n);
		nodeSwap(n, pred);
	}

	AVLNode<Key, Value>* p = n -> getParent();
	int diff = 0; //diff will be the amount added to updated the balance of p

	if(p != NULL){
		if(p -> getLeft() == n){
			diff = 1;
		}
		else{
			diff = -1;
		}
	}

	//n either has 1 or 0 children
	AVLNode<Key, Value>* c = NULL;
	if(n -> getLeft() != NULL){
		c = n -> getLeft();
	}
	else if (n -> getRight() != NULL){
		c = n -> getRight();
	}

	delete n;

	//update pointers

	if(c != NULL){
		c -> setParent(p);
	}
	
	if (p == NULL){
		this -> root_ = c;
	}
	else{
		if(p -> getLeft() == n){
			p -> setLeft(c);
		}
		else{
			p -> setRight(c);
		}
	}

	removeFix(p, diff);

}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* n, int diff){
	if(n == NULL){
		return;
	}

	//Compute next recursive call's arguments now before altering the tree
	AVLNode<Key, Value>* p = n -> getParent();
	
	int nDiff = 0;
	if(p != NULL){
		if (p -> getLeft() == n){
			nDiff = 1;
		}
		else{
			nDiff = -1;
		}
	}

	if(diff == -1){
		if(n -> getBalance() + diff == -2){
			AVLNode<Key, Value>* c = n -> getLeft(); //the taller of the children
			//zig-zag case
			if(c -> getBalance() == -1){
				rotateRight(n);
				n -> setBalance(0);
				c -> setBalance(0);
				removeFix(p, nDiff);
			}
			//zig-zag case
			else if (c -> getBalance() == 0){
				rotateRight(n);
				n -> setBalance(-1);
				c -> setBalance(1);
				return;
			}
			//zig-zag case
			else if (c -> getBalance() == 1){
				AVLNode<Key, Value>* g = c -> getRight();
				rotateLeft(c);
				rotateRight(n);
				if(g -> getBalance() == 1){
					n -> setBalance(0);
					c -> setBalance(-1);
					g -> setBalance(0);
				}
				else if (g -> getBalance() == 0){
					n -> setBalance(0);
					c -> setBalance(0);
					g -> setBalance(0);
				}
				else if(g -> getBalance() == -1){
					n -> setBalance(1);
					c -> setBalance(0);
					g -> setBalance(0);
				}

				removeFix(p, nDiff);
			}
		}
		else if (n -> getBalance() + diff == -1){
			n -> setBalance(-1);
			return;
		}
		else if (n -> getBalance() + diff == 0){
			n -> setBalance(0);
			removeFix(p, nDiff);
		}
	}
	//Perform the check for the mirror case where b(n) + diff == +2, flipping left/right and -1/+1
	else if(diff == 1){
		if(n -> getBalance() + diff == 2){
			AVLNode<Key, Value>* c = n -> getRight(); //the taller of the children
			//zig-zag case
			if(c -> getBalance() == 1){
				rotateLeft(n);
				n -> setBalance(0);
				c -> setBalance(0);
				removeFix(p, nDiff);
			}
			//zig-zag case
			else if (c -> getBalance() == 0){
				rotateLeft(n);
				n -> setBalance(1);
				c -> setBalance(-1);
				return;
			}
			//zig-zag case
			else if (c -> getBalance() == -1){
				AVLNode<Key, Value>* g = c -> getLeft();
				rotateRight(c);
				rotateLeft(n);
				if(g -> getBalance() == -1){
					n -> setBalance(0);
					c -> setBalance(1);
					g -> setBalance(0);
				}
				else if (g -> getBalance() == 0){
					n -> setBalance(0);
					c -> setBalance(0);
					g -> setBalance(0);
				}
				else if(g -> getBalance() == 1){
					n -> setBalance(-1);
					c -> setBalance(0);
					g -> setBalance(0);
				}

				removeFix(p, nDiff);
			}
		}
		else if (n -> getBalance() + diff == 1){
			n -> setBalance(1);
			return;
		}
		else if (n -> getBalance() + diff == 0){
			n -> setBalance(0);
			removeFix(p, nDiff);
		}
	}
}

template<class Key, class Value>
AVLNode<Key, Value>*
AVLTree<Key, Value>::predecessor(AVLNode<Key, Value>* node)
{
    // TODO
    if(node -> getLeft() != NULL){
        node = node ->getLeft();
        //return right most node of left subtree
        while(node -> getRight() != NULL){
            node = node -> getRight();
        }
        return node;
    }
    else{
        //walk up ancestor chain until you traverse the first right child pointer
        Node<Key, Value>* parent = node -> getParent();
		//if current node is left child of parent, then it is not the predecessor, keep going
		while(parent != NULL && parent -> getLeft() == node){
            node = node -> getParent();
            parent = parent -> getParent();
        }
		return node;
    }
}


template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
