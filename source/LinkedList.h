/** @file */
#ifndef __LINKEDLIST_H
#define __LINKEDLIST_H

#include "IndexOutOfBound.h"
#include "ElementNotExist.h"
#include "AllocationFailure.h"


/**
 * A linked list.
 *
 * The iterator iterates in the order of the elements being loaded into this list.
 */
template <class T>
class LinkedList
{
	struct node;
	typedef node* nodePointer;
	 	
private:
	struct node
	 {
	 	T data;
	 	nodePointer prev, next;
	 	
	 	/**
	 	 *	Construct an empty node
	 	 */
	 	node(): prev(NULL), next(NULL){}
	 	
	 	/**
	 	 *	Construct a new node
	 	 */
	 	node(const T& dat , const nodePointer& pre = NULL, const nodePointer& nex = NULL): data(dat), prev(pre), next(nex){}
	 	
	 	/**
		 *	Remove the element pointed by a pointer from the list
		 *	@throw ElementNotExist if this is a head or rear pointer of the list
		 */	
		inline void remove();
	
		/**
		 *	Appends the specified element to the next of this node.
		 *  @throw AllocationFailure
		 *  @throw ElementNotExist if this is a rear pointer of the list
		 */
		inline void add(const T& elem);
	 };
	
	int Size;
	nodePointer head, rear;
	
	
public:
    class Iterator
    {
    typedef LinkedList<T>* LinkedListPointer;
    
    private:
    	LinkedListPointer list;
    	bool del; // del == true iff the last element returned by the iterator has been deleted.
    	nodePointer pos, nextPos;
    	
    public:
    	/**
    	 *	Construct an empty iterator.
    	 */
    	Iterator(): list(NULL), del(false), pos(NULL), nextPos(NULL) {}
    	 	
    	/**
    	 *	Construct a new iterator.
    	 */
    	Iterator(const LinkedListPointer& li): list(li), del(false), pos(li->head), nextPos(li->head->next) {}
    	
        /**
    	 * TODO * operator
    	 */
    	inline T& operator*();
    
    	/**
         * TODO Returns true if the iteration has more elements.
         */
        bool hasNext() { return ( nextPos->next != NULL ); }

        /**
         * TODO Returns the next element in the iteration.
         * @throw ElementNotExist exception when hasNext() == false
         */
        inline const T &next();

        /**
         * TODO Removes from the underlying collection the last element
         * returned by the iterator
         * @throw ElementNotExist
         */
        inline void remove();
    };

    /**
     * TODO Constructs an empty linked list
     * @throw AllocationFailure
     */
    inline LinkedList();

    /**
     * TODO Copy constructor
     * @throw AllocationFailure
     */
    inline LinkedList(const LinkedList<T> &c);

    /**
     * TODO Assignment operator
     * @throw AllocationFailure
     */
    inline LinkedList<T>& operator=(const LinkedList<T> &c);

    /**
     * TODO Desturctor
     */
    inline virtual ~LinkedList();

    /**
     * TODO Appends the specified element to the end of this list.
     * Return false if failed to add it into the list.
     * @throw AllocationFailure
     */
    inline bool add(const T& e);

    /**
     * TODO Inserts the specified element to the beginning of this list.
     * @throw AllocationFailure
     */
    inline void addFirst(const T& elem);

    /**
     * TODO Insert the specified element to the end of this list.
     * Equivalent to add.
     * @throw AllocationFailure
     */
    void addLast(const T &elem) { add(elem); /** @throw AllocationFailure; */ }

    /**
     * TODO Inserts the specified element to the specified position in this list.
     * The range of index parameter is [0, size], where index=0 means inserting to the head,
     * and index=size means appending to the end.
     * @throw IndexOutOfBound
     * @throw AllocationFailure
     */
    inline void add(int index, const T& element);

    /**
     * TODO Removes all of the elements from this list.
     */
    inline void clear();

    /**
     * TODO Returns true if this list contains the specified element.
     */
    inline bool contains(const T& e) const;

    /**
     * TODO Returns a const reference to the element at the specified position in this list.
     * The index is zero-based, with range [0, size).
     * @throw IndexOutOfBound
     */
    inline const T& get(int index) const;

    /**
     * TODO Returns a const reference to the first element.
     * @throw ElementNotExist
     */
    inline const T& getFirst() const;

    /**
     * TODO Returns a const reference to the last element.
     * @throw ElementNotExist
     */
    inline const T& getLast() const;

    /**
     * TODO Returns true if this list contains no elements.
     */
    bool isEmpty() const { return (Size==0); }

    /**
     * TODO Removes the element at the specified position in this list.
     * The index is zero-based, with range [0, size).
     * @throw IndexOutOfBound
     */
    inline void removeIndex(int index);

    /**
     * TODO Removes the first occurrence of the specified element from this list, if it is present.
     * Returns true if it was present in the list, otherwise false.
     */
    inline bool remove(const T &e);

    /**
     * TODO Removes the first element from this list.
     * @throw ElementNotExist
     */
    inline void removeFirst();

    /**
     * TODO Removes the last element from this list.
     * @throw ElementNotExist
     */
    inline void removeLast();

    /**
     * TODO Replaces the element at the specified position in this list with the specified element.
     * The index is zero-based, with range [0, size).
     * @throw IndexOutOfBound
     */
    inline void set(int index, const T &element);

    /**
     * TODO Returns the number of elements in this list.
     */
    int size() const { return Size; }

    /**
     * TODO Returns an iterator over the elements in this list.
     */
    inline Iterator iterator();
};

//-------------The end of the class-----------------------------------

/**
 *	Remove the element pointed by a pointer from the list
 *	@throw ElementNotExist if this is a head or rear pointer of the list
 */	
template <class T>
inline void LinkedList<T>::node::remove()
{
 	if ( prev == NULL || next == NULL )
 	 throw ElementNotExist("The element you want to visit does not exsit.");
 	prev->next = next;
 	next->prev = prev;
 	delete this;
}

/**
 *	Appends the specified element to the next of this node.
 *  @throw AllocationFailure
 *  @throw ElementNotExist if this is a rear pointer of the list
 */
template <class T>
inline void LinkedList<T>::node::add(const T& elem)
{
	if ( next == NULL )
 	 throw ElementNotExist("The element you want to visit does not exsit.");
	nodePointer newNode = new node(elem, this, this->next);
	if (newNode==NULL) throw AllocationFailure("The operation 'new' is failed.");
	newNode->next->prev = this->next = newNode;
}

/**
 *	*operator
 */
template <class T>
inline T& LinkedList<T>::Iterator::operator*()
{
	if (pos==NULL || nextPos==NULL || pos->prev==NULL || del == true)
	 throw ElementNotExist("The element you want to visit does not exsit.");
	return pos->data;
}

/**
  * Returns the next element in the iteration.
  * @throw ElementNotExist exception when hasNext() == false
  */
template <class T>
inline const T& LinkedList<T>::Iterator::next()
{
  	if (!hasNext())
  	 throw ElementNotExist("The element you want to visit does not exsit.");
  	del = false;
  	pos = nextPos;
  	nextPos = pos->next;
  	return pos->data;
}

/**
 * Removes from the underlying collection the last element
 * returned by the iterator
 * @throw ElementNotExist
 */
template <class T>
inline void LinkedList<T>::Iterator::remove()
{
  	if (pos==NULL || del == true)
	 throw ElementNotExist("The element you want to visit does not exsit.");
 	pos->remove();
 	del = true;
 	--(list->Size);
}

/**
 * Constructs an empty linked list
 * @throw AllocationFailure
 */
template <class T>
inline LinkedList<T>::LinkedList()
{
   	head = new node;
   	rear = new node;
   	Size = 0;
   	if (head==NULL || rear==NULL) throw AllocationFailure("The operation 'new' is failed.");
   	head -> next = rear;
   	rear -> prev = head;
}

/**
  * Copy constructor
  * @throw AllocationFailure
  */
template <class T>
inline LinkedList<T>::LinkedList(const LinkedList<T> &c)
{
   	nodePointer cNow = c.head, thisNow = new node, thisLast;
   	if (thisNow==NULL) throw AllocationFailure("The operation 'new' is failed.");
   	head = thisNow;
   	 	
   	do
   	 {
   	 	thisLast = thisNow;
   	 	thisNow = new node;
   	 	if (thisNow==NULL) throw AllocationFailure("The operation 'new' is failed.");
   	 	cNow = cNow->next;
   	 	thisNow->data = cNow->data;
   	 	thisNow->prev = thisLast;
   	 	thisLast->next = thisNow;
   	 }
   	while (cNow->next!=NULL);
   	
   	rear = thisNow;
   	Size = c.Size;
}

/**
  * Assignment operator
  * @throw AllocationFailure
  */
template <class T>
inline LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T> &c)
{
	if (this==&c)
	 return *this;
   	nodePointer cNow, cRear = c.rear, thisNow = head, thisLast;
   	clear();
   	
   	cNow = c.head->next;
   	thisNow = head;
   	
   	do
   	 {
   	 	thisLast = thisNow;
   	 	thisNow = new node;
   	 	if (thisNow==NULL) throw AllocationFailure("The operation 'new' is failed.");
   	 	thisNow->data = cNow->data;
   	 	thisNow->prev = thisLast;
   	 	thisLast->next = thisNow;
   	 	cNow = cNow->next;
   	 }
   	while (cNow!=cRear);
   	
   	thisNow->next = rear;
   	Size = c->size;
}

/**
  * Desturctor
  */
template <class T>
inline LinkedList<T>::~LinkedList()
{
  	nodePointer thisNow = head, thisLast;
   	while (thisNow!=NULL)
   	 {
   	 	thisLast = thisNow;
   	 	thisNow = thisNow->next;
   	 	delete thisLast;
   	 }
}

/**
  * Appends the specified element to the end of this list.
  * Return false if failed to add it into the list.
  * @throw AllocationFailure
  */
template <class T>
inline bool LinkedList<T>::add(const T& e) 
{
   	rear->prev->add(e); /** @throw AllocationFailure; */
   	++Size;
}

/**
  * Inserts the specified element to the beginning of this list.
  * @throw AllocationFailure
  */
template <class T>
inline void LinkedList<T>::addFirst(const T& elem)
{
   	head->add(elem); /** @throw AllocationFailure; */
   	++Size;
}

/**
  * Inserts the specified element to the specified position in this list.
  * The range of index parameter is [0, size], where index=0 means inserting to the head,
  * and index=size means appending to the end.
  * @throw IndexOutOfBound
  * @throw AllocationFailure
  */
template <class T>
inline void LinkedList<T>::add(int index, const T& element)
{
   	if ( index<0 || index > Size )
   	 throw IndexOutOfBound("Index out of bound.");
   	nodePointer thisNow = head;
   	for (int i=1; i<=index; ++i)
   	 thisNow = thisNow->next;
   	thisNow->add(element); /** @throw AllocationFailure; */
   	++Size;
}

/**
  * Removes all of the elements from this list.
  */
template <class T>
inline void LinkedList<T>::clear()
{
   	nodePointer thisNow = head->next, thisLast;
   	while (thisNow!=rear)
   	 {
   	 	thisLast = thisNow;
   	 	thisNow = thisNow->next;
   	 	delete thisLast;
  	 }
   	head->next = rear;
   	rear->prev = head;
   	Size = 0;
}

/**
  * Returns true if this list contains the specified element.
  */
template <class T>
inline bool LinkedList<T>::contains(const T& e) const
{
  	nodePointer thisNow = head->next;
   	while (thisNow!=rear)
   	 {
   	 	if (thisNow->data==e)
   	 	 return true;
   	 	thisNow = thisNow->next;
   	 }
   	return false;
}

/**
  * Returns a const reference to the element at the specified position in this list.
  * The index is zero-based, with range [0, size).
  * @throw IndexOutOfBound
  */
template <class T>
inline const T& LinkedList<T>::get(int index) const
{
   	if ( index<0 || index >= Size )
   	 throw IndexOutOfBound("Index out of bound.");
   	nodePointer thisNow = head->next;
   	for (int i=1; i<=index; ++i)
   	 thisNow = thisNow->next;
   	return thisNow->data;
}

/**
 * Returns a const reference to the first element.
 * @throw ElementNotExist
 */
template <class T>
inline const T& LinkedList<T>::getFirst() const 
{
 	if (Size<1)
 	 throw ElementNotExist("The element you want to visit does not exsit.");
 	return head->next->data;
}

/**
 * Returns a const reference to the last element.
 * @throw ElementNotExist
 */
template <class T>
inline const T& LinkedList<T>::getLast() const
{
 	if (Size<1)
 	 throw ElementNotExist("The element you want to visit does not exsit.");
 	return rear->prev->data;
}

/**
 * Removes the element at the specified position in this list.
 * The index is zero-based, with range [0, size).
 * @throw IndexOutOfBound
 */
template <class T>
inline void LinkedList<T>::removeIndex(int index)
{
   	if ( index<0 || index >= Size )
   	 throw IndexOutOfBound("Index out of bound.");
   	nodePointer thisNow = head->next;
   	for (int i=1; i<=index; ++i)
   	 thisNow = thisNow->next;
   	thisNow->remove();
   	--Size;
}

/**
  * Removes the first occurrence of the specified element from this list, if it is present.
  * Returns true if it was present in the list, otherwise false.
  */
template <class T>
inline bool LinkedList<T>::remove(const T &e)
{
   	nodePointer thisNow = head->next;
   	while (thisNow!=rear)
   	 {
   	 	if (thisNow->data==e)
   	 	 {
   	 	 	thisNow->remove();
   	 	 	--Size;
   	 	 	return true;
   	 	 }
   	 	thisNow = thisNow->next;
   	 }
   	return false;
}

/**
  * Removes the first element from this list.
  * @throw ElementNotExist
  */
template <class T>
inline void LinkedList<T>::removeFirst()
{
   	if (Size<1)
 	 throw ElementNotExist("The element you want to visit does not exsit.");
 	head->next->remove();
 	--Size;
}

/**
  * Removes the last element from this list.
  * @throw ElementNotExist
  */
template <class T>
inline void LinkedList<T>::removeLast()
{
   	if (Size<1)
 	 throw ElementNotExist("The element you want to visit does not exsit.");
 	rear->prev->remove();
 	--Size;
}

/**
  * Replaces the element at the specified position in this list with the specified element.
  * The index is zero-based, with range [0, size).
  * @throw IndexOutOfBound
  */
template <class T>
inline void LinkedList<T>::set(int index, const T &element)
{
   	if ( index<0 || index >= Size )
   	 throw IndexOutOfBound("Index out of bound.");
   	nodePointer thisNow = head->next;
   	for (int i=1; i<=index; ++i)
   	 thisNow = thisNow->next;
   	thisNow->data=element;
}

/**
  * Returns an iterator over the elements in this list.
  */
template <class T>
inline class LinkedList<T>::Iterator LinkedList<T>::iterator()
{
   	Iterator itr(this);
   	return itr;
}

#endif
