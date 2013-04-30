#ifndef __BINARYNODE_H__
#define __BINARYNODE_H__

#include "ElementNotExist.h"

/**
 *	BinaryNode is a node in a binary search tree.
 */

template<class T>	// Type 'T' need an opertor '<'
class BinaryNode
{
	typedef BinaryNode<T>* BinaryNodePointer;
	private:
	T data;
	BinaryNodePointer l,r,f; // left subtree, right subtree and father
	int Size;	// the size of the subtree 
	
	public:
	
	/** Construct a new node. */
	BinaryNode(const T& dat = 0) : data(dat) {}
	
	/** Destructor */
	virtual ~BinaryNode() {}
	
	/** Return a const reference to the element */
	const T& get() const { return data; }
	
	/**
	 *	Return a pointer of the previous node
	 *	@throw ElementNotExist
	 */
	BinaryNodePointer prev();
	
	/**
	 *	Return a pointer of the next node
	 *	@throw ElementNotExist
	 */
	BinaryNodePointer next();
	
	/** 
	 *	Rotate the left son up
	 *	@throw ElementNotExist
	 */
	inline void leftRotate();
	
	/** 
	 *	Rotate the right son up
	 *	@throw ElementNotExist
	 */
	void rightRotate();
	
	/**
	 *	Find a node with specified data in the subtree
	 *	If that node not exists, return NULL
	 */
	BinaryNodePointer find( T elem );
	
	/** the size of the subtree */
	int size() const { return Size; }
};

//-------------The end of the class-----------------------------------

/**
 *	Return a pointer of the previous node
 *	@throw ElementNotExist
 */
template <class T>
BinaryNode<T>* BinaryNode<T>::prev()
{
	BinaryNodePointer thisNow = this, thisFather = this->f;
	if ( thisNow->l != NULL )
	 {
	 	thisNow = thisNow->l;
	 	while (thisNow->r != NULL) thisNow = thisNow->r;
	 	return thisNow;
	 }
	while (thisFather != NULL)
	 {
	 	if (thisNow == thisFather->r)
	 	 return thisFather;
	 	thisNow = thisFather;
	 	thisFather = thisFather->f;
	 }
	throw ElementNotExist("The element is the least in the map.");
}

/**
 *	Return a pointer of the next node
 *	@throw ElementNotExist
 */
template <class T>
BinaryNode<T>* BinaryNode<T>::next()
{
	BinaryNodePointer thisNow = this, thisFather = this->f;
	if ( thisNow->r != NULL )
	 {
	 	thisNow = thisNow->r;
	 	while (thisNow->l != NULL) thisNow = thisNow->l;
	 	return thisNow;
	 }
	while (thisFather != NULL)
	 {
	 	if (thisNow == thisFather->l)
	 	 return thisFather;
	 	thisNow = thisFather;
	 	thisFather = thisFather->f;
	 }
	throw ElementNotExist("The element is the largest in the map.");
}

/** 
 *	Rotate the left son up
 *	@throw ElementNotExist
 */
template <class T>
inline void BinaryNode<T>::leftRotate()
{
	BinaryNodePointer leftSon = l, father = f;
	if (leftSon == NULL)
	 throw ElementNotExist("No left son.");
	l = leftSon->r;
	if (leftSon->r->f != NULL)
	 leftSon->r->f = this;
	leftSon->r = this;
	f = leftSon;
	leftSon->f = father;
	if (father != NULL)
	 {
	 	if (father->l == this)
		 father->l = leftSon;
		else
		 father->r = leftSon;
	 }
	leftSon->Size = Size;
	Size = 1 + ((l==NULL)?0:(l->Size)) + ((r==NULL)?0:(r->Size));
}

/** 
 *	Rotate the right son up
 *	@throw ElementNotExist
 */
template <class T>
inline void BinaryNode<T>::rightRotate()
{
	BinaryNodePointer rightSon = r, father = f;
	if (rightSon == NULL)
	 throw ElementNotExist("No right son.");
	r = rightSon->l;
	if (rightSon->l->f != NULL)
	 rightSon->l->f = this;
	rightSon->l = this;
	f = rightSon;
	rightSon->f = father;
	if (father != NULL)
	 {
	 	if (father->l == this)
		 father->l = rightSon;
		else
		 father->r = rightSon;
	 }
	rightSon->Size = Size;
	Size = 1 + ((l==NULL)?0:(l->Size)) + ((r==NULL)?0:(r->Size));
}

/**
 *	Find a node with specified data in the subtree
 *	If that node not exists, return NULL
 */
template <class T>
BinaryNode<T>* BinaryNode<T>::find( T elem )
{
	BinaryNode<T>* thisNow = this;
	T nowData;
	while (thisNow != NULL)
	 {
	 	nowData = thisNow->data;
	 	if (nowData == elem)
	 	 return thisNow;
	 	if (nowData < elem)
	 	 thisNow = thisNow->r;
	 	else
	 	 thisNow = thisNow->l;
	 }
}

#endif
