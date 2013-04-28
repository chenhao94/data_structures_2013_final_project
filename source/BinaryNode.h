#ifndef __BINARYNODE_H__
#define __BINARYNODE_H__

#include "AllocationFailure.h"
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
	BinaryNode(const T& dat, const BinaryNodePointer& le, const BinaryNodePointer& ri, const BinaryNodePointer& fat) {}
	
	/** Destructor */
	virtual ~BinaryNode() {}
	
	/** Return a const reference to the element */
	const T& get() const {}
	
	/**
	 *	Return a pointer of the previous node
	 *	@throw ElementNotExist
	 */
	BinaryNodePointer prev() const {}
	
	/**
	 *	Return a pointer of the next node
	 *	@throw ElementNotExist
	 */
	BinaryNodePointer next() const {}
	
	/** 
	 *	Rotate the left son up
	 *	@throw ElementNotExist
	 */
	void leftRotate() {} 
	
	/** 
	 *	Rotate the right son up
	 *	@throw ElementNotExist
	 */
	void rightRotate() {} 
	
	/**
	 *	Add a node to the subtree
	 * @throw AllocationFailure
	 */
	void add(const T& elem) {}
	
	/** Remove this node from the tree */
	void remove() {}
	
	/**
	 *	Find a node with specified data in the subtree
	 *	If that node not exists, return false
	 */
	BinaryNodePointer find( T elem ) const {}
	
	/** the size of the subtree */
	int size() const { return Size; }
}

#endif
