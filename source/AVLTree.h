#ifndef __AVLTREE_H__
#define __AVLTREE_H__

#include "AllocationFailure.h"
#include "ElementNotExist.h"

/**
 *	An AVL tree.
 *	Type 'T' need an opertor '<'
 */

template<class T>
class AVLTree;

template<class T>
class AVLNode;

/**
 *	Copy a AVL tree, return the pointer of the root
 *	@throw AllocationFailure
 */
template <class T>
AVLNode<T>* copyTree(AVLNode<T>* root);

/** Return height of the subtree. */
template <class T>
inline int getHeight(AVLNode<T>* root);

/** Maintain height of the subtree. */
template <class T>
inline void maintainHeight(AVLNode<T>* root);

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
	AVLNode(const T& dat = 0) : data(dat), height(1), Size(1), l(NULL), r(NULL), f(NULL) {}
	
	/** Destructor */
	virtual ~AVLNode() {}
	
	/** Return a const reference to the element */
	const T& get() const { return data; }
	
	/** Return a reference to the left son */
	AVLNodePointer& getL() { return l; }
	
	/** Return a reference to the right son */
	AVLNodePointer& getR() { return r; }
	
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
	 *	Return the 1st pointer of the subtree
	 */
	AVLNodePointer getFirst();
	
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
	
	friend class AVLTree<T>;
	friend AVLNode<T>* copyTree<T>(AVLNode<T>*);
	friend int getHeight<T>(AVLNode<T>* root);
	friend void maintainHeight<T>(AVLNode<T>* root);
};

//-------------The end of AVLNode-----------------------------------

template<class T>
class AVLTree
{
	private:
	AVLNode<T>* root;
	
	public:
	
	/** Construct a new tree. */
	AVLTree(): root(NULL) {}
	
	/** Remove all the nodes in the subtree. */
	void removeAll(AVLNode<T>* node);
	
	/** Destructor */
	virtual ~AVLTree() { if (root!=NULL) removeAll(root); }
	
	/** Removes all of the elements from this tree. */
	void clear() { if (root!=NULL) removeAll(root); root = NULL; }
	
	/**
	 *	Add a element into the AVL tree
	 *	@throw AllocationFailure
	 */
	void add( const T& elem );
	
	/** 
	 *	Remove a specified element from the AVL tree
	 *	@throw ElementNotExist
	 */
	void remove( const T& elem );
	
	/** 
	 *	Find a node with specified data from the AVL tree
	 *	If not exists, return NULL
	 */
	AVLNode<T>* find( const T& elem ) { return root->find(elem); }
	
	/** Return the reference of root of the tree */
	AVLNode<T>* &getRoot() { return root; }
};

//-------------The end of AVLTree-----------------------------------

/** Return height of the subtree. */
template <class T>
inline int getHeight(AVLNode<T>* root)
{
	if (root==NULL)
	 return 0;
	return root->height;
}

/** Maintain height of the subtree. */
template <class T>
inline void maintainHeight(AVLNode<T>* root)
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

/**	Return the 1st pointer of the subtree */
template <class T>
AVLNode<T>* AVLNode<T>::getFirst()
{
	AVLNodePointer thisNow = this;
	while (thisNow->l != NULL) thisNow = thisNow->l;
	 return thisNow;
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
	return false;
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
	 	sonDiff = getHeight(r->l) - getHeight(r->r);
	 	
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
	 	sonDiff = getHeight(l->l) - getHeight(l->r);
	 	
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
	return false;
}

/** Remove all the nodes in the subtree. */
template <class T>
void AVLTree<T>::removeAll(AVLNode<T>* node)
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
	AVLNode<T>* newNode = new AVLNode<T>(elem);
	AVLNode<T>* pos = root;
	int diff, heightTmp;
	if (newNode==NULL) throw AllocationFailure("The operation 'new' is failed.");
	if (root==NULL)
	 {
	 	root = newNode;
	 	return;
	 }
	while (1)
	 {
	 	++pos->Size;
	 	if (elem<pos->data)
	 	 {
	 	 	if (pos->l==NULL)
	 	 	 {
	 	 	 	pos->l=newNode;
	 	 	 	newNode->f=pos;
	 	 	 	break;
	 	 	 }
	 	 	pos=pos->l;
	 	 }
	 	else
	 	 {
	 	 	if (pos->r==NULL)
	 	 	 {
	 	 	 	pos->r=newNode;
	 	 	 	newNode->f=pos;
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
	 	 	if (pos->f->f==NULL)
	 	 	 root = pos->f;
	 	 	break;
	 	 }
	 	pos = pos->f;
	 }
}

/** 
 *	Remove a specified element from the AVL tree
 *	@throw ElementNotExist
 */
template <class T>
void AVLTree<T>::remove( const T& elem )
{
	AVLNode<T>* node = find(elem);
	AVLNode<T>* pos;
	bool stop = false;
	int diff, fDiff, heightTmp;
	
	if (node==NULL)
	 throw ElementNotExist("The element you want to remove does not exsit.");
	
	if (node->l!=NULL && node->r!=NULL)
	 {
	 	pos = node->l;
	 	while (pos->r!=NULL) pos=pos->r;
	 	node->data = pos->data;
	 	node = pos;
	 }
	if (node->l!=NULL)
	 {
	 	node->leftRotate();
	 	if (node==root)
	 	 root=node->f;
	 }
	if (node->r!=NULL)
	 {
	 	node->rightRotate();
	 	if (node==root)
	 	 root=node->f;
	 }
	pos = node->f;
	if (pos==NULL)
	 {
	 	root = NULL;
	 	delete node;
	 	return;
	 }
	if (pos->l==node)
	 pos->l=NULL;
	else
	 pos->r=NULL;
	maintainHeight(pos);
	delete node;
	
	while (pos!=NULL)
	 {
	 	--pos->Size;
	 	if (stop==false)
	 	 {
	 	 	diff = getHeight(pos->l) - getHeight(pos->r);
	 	 	if (pos->f==NULL)
	 	 	 {
	 	 	 	root = pos;
	 	 	 	heightTmp = pos->height;
	 	 		maintainHeight(pos);
	 	 	 	if (diff>1 || diff<-1)
	 	 	 	 pos->removeMaintain();
	 	 	 	if (pos->f!=NULL) root = pos->f;
	 	 	 	break;
	 	 	 }
	 	 	
	 	 	heightTmp = pos->f->height;
	 	 	maintainHeight(pos->f);
	 	 	fDiff = getHeight(pos->f->l) - getHeight(pos->f->r);
	 	 	
	 	 	/** (heightTmp == pos->f->height && fDiff<2 && fDiff>-2) means no influence on the father node. */
	 	 	if ((heightTmp == pos->f->height && fDiff<2 && fDiff>-2) && diff<2 && diff>-2)
	 	 	 stop = true;
	 	 	else if (heightTmp == pos->f->height && fDiff<2 && fDiff>-2)
	 	 	 {
	 	 	 	stop = pos->removeMaintain();
	 	 	 	if (pos->f->f==NULL)
	 	 		 {
	 	 		 	root = pos->f;
	 	 		 	break;
	 	 		 }
	 	 		pos = pos->f;
	 	 	 }
	 	 }
	 	pos = pos->f;
	 }
}

/**
 *	Copy a AVL tree, return the pointer of the root
 *	@throw AllocationFailure
 */
template <class T>
AVLNode<T>* copyTree(AVLNode<T>* root)
{
	if (root==NULL)
	 return NULL;
	AVLNode<T>* newNode = new AVLNode<T>(root->data);
	if (newNode==NULL) throw AllocationFailure("The operation 'new' is failed.");
	AVLNode<T>* l=copyTree(root->l), *r=copyTree(root->r);
	newNode->l = l;
	newNode->r = r;
	if (l!=NULL)
	 newNode->l->f = newNode;
	if (r!=NULL)
	 newNode->r->f = newNode;
	return newNode;
}

#endif
