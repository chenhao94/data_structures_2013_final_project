#ifndef __AVLTREE_H__
#define __AVLTREE_H__

#include "AVLNode.h"
#include "AllocationFailure.h"
#include "ElementNotExist.h"

/**
 *	An AVL tree.
 *	Type 'T' need an opertor '<'
 */

template<class T>	// Type 'T' need an opertor '<'
class AVLNode
{
	typedef AVLNode<T>* AVLNodePointer;
	private:
	T data;
	AVLNodePointer l,r,f; // left subtree, right subtree and father
	int Size, height;	// size and height of the subtree 
	
	public:
	
	/** Construct a new node. */
	AVLNode(const T& dat = 0) : data(dat), height(1), Size(1) {}
	
	/** Destructor */
	virtual ~AVLNode() {}
	
	/** Return a const reference to the element */
	const T& get() const { return data; }
	
	/**
	 *	Return a pointer of the previous node
	 *	@throw ElementNotExist
	 */
	AVLNodePointer prev();
	
	/**
	 *	Return a pointer of the next node
	 *	@throw ElementNotExist
	 */
	AVLNodePointer next();
	
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
	AVLNodePointer find( const T& elem );
	
	/** 
	 *	Maintain the node out of balence after added a new node.
	 *	Return false when further maintainance needed.
	 */
	bool addMaintain();
	
	/** 
	 *	Maintain the node out of balence after removed a node.
	 *	Return false when further maintainance needed.
	 */
	bool removeMaintain();
	
	/** the size of the subtree */
	int size() const { return Size; }
};

//-------------The end of AVLNode-----------------------------------

template<class T>
class AVLTree
{
	private:
	AVLNode<T>* root;
	
	public:
	
	/** Construct a new tree. */
	AVLNode(): root(NULL);
	
	/** Remove all the nodes in the subtree. */
	void removeAll(AVLNode<T>* node);
	
	/** Destructor */
	virtual ~AVLTree() { removeAll(root); }
	
	/**
	 *	Add a element into the AVL tree
	 *	@throw AllocationFailure
	 */
	void add( const T& elem );
	
	/** Remove a specified element from the AVL tree */
	void remove( const AVLNodePointer& node );
	
	/** 
	 *	Find a node with specified data from the AVL tree
	 *	If not exists, return NULL
	 */
	AVLNode<T>* find( const T& elem ) { return root->find(elem); }
}

//-------------The end of AVLTree-----------------------------------

/** Return height of the subtree. */
template <class T>
inline int getHeight(AVLNode<T>* root) const
{
	if (root==NULL)
	 return 0;
	return root->height;
}

/** Maintain height of the subtree. */
template <class T>
inline void maintainHeight(AVLNode<T>* root) const
{
	if (root==NULL)
	 return;
	int heightTmp1 = getHeight(root->l), heightTmp2 = getHeight(root->r);
	
	root->height = 1 + (heightTmp1 > heightTmp2 ? heightTmp1 : heightTmp2);
}

/**
 *	Return a pointer of the previous node
 *	@throw ElementNotExist
 */
template <class T>
AVLNode<T>* AVLNode<T>::prev()
{
	AVLNodePointer thisNow = this, thisFather = this->f;
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
AVLNode<T>* AVLNode<T>::next()
{
	AVLNodePointer thisNow = this, thisFather = this->f;
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
inline void AVLNode<T>::leftRotate()
{
	AVLNodePointer leftSon = l, father = f;
	if (leftSon == NULL)
	 throw ElementNotExist("No left son.");
	l = leftSon->r;
	if (leftSon->r != NULL)
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
	
	maintainHeight(this);
	maintainHeight(leftSon);
	maintainHeight(father);
	
	leftSon->Size = Size;
	Size = 1 + ((l==NULL)?0:(l->Size)) + ((r==NULL)?0:(r->Size));
}

/** 
 *	Rotate the right son up
 *	@throw ElementNotExist
 */
template <class T>
inline void AVLNode<T>::rightRotate()
{
	AVLNodePointer rightSon = r, father = f;
	if (rightSon == NULL)
	 throw ElementNotExist("No right son.");
	r = rightSon->l;
	if (rightSon->l != NULL)
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
	 
	maintainHeight(this);
	maintainHeight(rightSon);
	maintainHeight(father);
	
	rightSon->Size = Size;
	Size = 1 + ((l==NULL)?0:(l->Size)) + ((r==NULL)?0:(r->Size));
}

/**
 *	Find a node with specified data in the subtree
 *	If that node not exists, return NULL
 */
template <class T>
AVLNode<T>* AVLNode<T>::find( const T& elem )
{
	AVLNode<T>* thisNow = this;
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
	return NULL;
}

/** 
 *	Maintain the node out of balence after added a new node.
 *	Return false when further maintainance needed.
 */
template <class T>
bool AVLNode<T>::addMaintain()
{
	int diff = getHeight(l) - getHeight(r), sonDiff;
	if (diff>-2 && diff<2)
	 return true;
	if (diff==2)
	 {
	 	sonDiff = getHeight(l->l) - getHeight(l->r);
	 	
	 	//---------LL----------
	 	if (sonDiff==1)
	 	 {
	 	 	leftRotate();
	 	 	return true;
	 	 }
	 	
	 	//---------LR----------
	 	else if (sonDiff==-1)
	 	 {
	 	 	l->rightRotate();
	 	 	leftRotate();
	 	 	return true;
	 	 }
	 }
	else if (diff==-2)
	 {
	 	sonDiff = getHeight(r->l) - getHeight(r->r);
	 	
	 	//---------RL----------
	 	if (sonDiff==1)
	 	 {
	 	 	r->leftRotate();
	 	 	rightRotate();
	 	 	return true;
	 	 }
	 	
	 	//---------RR----------
	 	else if (sonDiff==-1)
	 	 {
	 	 	rightRotate();
	 	 	return true;
	 	 }
	 }
}

/** 
 *	Maintain the node out of balence after removed a node.
 *	Return false when further maintainance needed.
 */
template <class T>
bool AVLNode<T>::removeMaintain()
{
	int diff = getHeight(l) - getHeight(r), sonDiff;
	if (diff>-2 && diff<2)
	 return true;
	if (diff==-2)
	 {
	 	sonDiff = getHeight(l->l) - getHeight(l->r);
	 	
	 	if (sonDiff==0)				//	Q==0
	 	 {
	 	 	rightRotate();
	 	 	return true;
	 	 }
	 	else if (sonDiff*diff>0)	//	Q same as P
	 	 {
	 	 	rightRotate();
	 	 	return false;
	 	 }
	 	else						//	Q differ from P (RL)
	 	 {
	 	 	r->leftRotate();
	 	 	rightRotate();
	 	 	return false;
	 	 }
	 }
	else if (diff==2)
	 {
	 	sonDiff = getHeight(r->l) - getHeight(r->r);
	 	
	 	if (sonDiff==0)				//	Q==0
	 	 {
	 	 	leftRotate();
	 	 	return true;
	 	 }
	 	else if (sonDiff*diff>0)	//	Q same as P
	 	 {
	 	 	leftRotate();
	 	 	return false;
	 	 }
	 	else						//	Q differ from P	(LR)
	 	 {
	 	 	l->rightRotate();
	 	 	leftRotate();
	 	 	return false;
	 	 }
	 }
}

/** Remove all the nodes in the subtree. */
template <class T>
void removeAll(AVLNode<T>* node)
{
	if (node==NULL)
	 return;
	removeAll(node->l);
	removeAll(node->r);
	delete node;
}

/**
 *	Add a element into the AVL tree
 *	@throw AllocationFailure
 */
template <class T>
void AVLTree<T>::add( const T& elem )
{
	AVLNode<T>* newNode = new AVLNode<T>(elem), pos = root;
	int diff, heightTmp;
	if (newNode==NULL) throw AllocationFailure("The operation 'new' is failed.");
	if (root==NULL)
	 {
	 	root = newNode;
	 	return;
	 }
	while (1)
	 {
	 	++pos.Size;
	 	if (elem<pos->data)
	 	 {
	 	 	if (pos->l==NULL)
	 	 	 {
	 	 	 	pos->l=newNode;
	 	 	 	break;
	 	 	 }
	 	 	pos=pos->l;
	 	 }
	 	else
	 	 {
	 	 	if (pos->r==NULL)
	 	 	 {
	 	 	 	pos->r=newNode;
	 	 	 	break;
	 	 	 }
	 	 	pos=pos->r;
	 	 }
	 }
	while (pos!=NULL)
	 {
	 	heightTmp = pos->height;
	 	maintainHeight(pos);
	 	if (heightTmp == pos->height)
	 	 break;
	 	diff = getHeight(pos->l) - getHeight(pos->r);
	 	if (diff>1 || diff<-1)
	 	 {
	 	 	pos->addMaintain();
	 	 	break;
	 	 }
	 	pos = pos->f;
	 }
}

/** Remove a specified element from the AVL tree */
template <class T>
void AVLTree<T>::remove( const AVLNodePointer& node )
{
	
}

#endif
