/** @file */
#ifndef __ARRAYLIST_H
#define __ARRAYLIST_H

#include "IndexOutOfBound.h"
#include "ElementNotExist.h"
#include "AllocationFailure.h"

/**
 * The ArrayList is just like vector in C++.
 * You should know that "capacity" here doesn't mean how many elements are now in this list, where it means
 * the length of the array of your internal implemention
 *
 * The iterator iterates in the order of the elements being loaded into this list
 */
template <class T>
class ArrayList
{
private:
    
    T *data;
    static const int initialSize = 100;
    long capacity, length;
    
    /**
     *	TODO Returns false if fail to enlarge the size.
     */
    bool doubleSize()
     {
     	T *tmp = data;
     	capacity *= 2;
     	
     	try
     	 {
     		data = new T[capacity];
     		if (data == NULL) throw AllocationFailure("The operation 'new' is failed.");
     	 }
     	catch (AllocationFailure error)
     	 {
     	 	return false;
     	 }
     	
     	for (int i=0 ; i<length ;++i)
     	 data[i] = tmp[i];
     	delete []tmp;
     	
     	return true;
     }
    
    
public:
    class Iterator
    {
    
    typedef ArrayList<T>* ArrayListPointer;
    
    private:
    	ArrayListPointer list;
    	T pos;
    	bool flag; // iff next() has been called, flag = true
    
    public:
    	/**
    	 * TODO Constructs a new iterator.
    	 */
    	Iterator(const ArrayListPointer& li = NULL): list(li), pos(-1), flag(false){}
    	
    	/**
    	 * TODO * operator
    	 */
    	T& operator*()
    	 {
    	 	if (pos < 0 || pos >= list->length || flag == false)
    	 	 throw ElementNotExist("The element you want to visit does not exsit.");
    	 	return list->data[pos];
    	 }
    
        /**

         * TODO Returns true if the iteration has more elements.
         */
        bool hasNext() { return (pos+1<list->length && pos>=-1); }

        /**
         * TODO Returns the next element in the iteration.

         * @throw ElementNotExist exception when hasNext() == false
         */
        const T &next()
         {
         	if (!hasNext())
         	 throw ElementNotExist("The element you want to visit does not exsit.");
         	flag = true;
         	return (list->data[++pos]);
         }

        /**
         * TODO Removes from the underlying collection the last element
         * returned by the iterator

         * @throw ElementNotExist
         */
        void remove()
         {
         	if ( pos<0 || pos >= list->length || flag == false)
         	 throw ElementNotExist("The element you want to visit does not exsit.");
         	list->removeIndex(pos);
         	--pos;
         	flag = false;
         }
	};

    /**
     * TODO Constructs an empty array list.
     * @throw AllocationFailure
     */
    ArrayList(): length(0), capacity(initialSize) 
     {
     	data = new T[initialSize];
     	if (data == NULL) throw AllocationFailure("The operation 'new' is failed.");
     }

    /**
     * TODO Destructor
     */
    virtual ~ArrayList() { delete []data; }

    /**
     * TODO Assignment operator
     * @throw AllocationFailure
     */
    ArrayList& operator=(const ArrayList& x)
     {
     	delete []data;
     	capacity=x.capacity;
     	length=x.length;
     	data = new T[capacity];
     	if (data == NULL) throw AllocationFailure("The operation 'new' is failed.");
     	for (int i=0 ; i<length ; ++i)
     	 data[i]=x.data[i];
     }
    
    /**
     * TODO Copy-constructor
     * @throw AllocationFailure
     */
    ArrayList(const ArrayList& x) : capacity(x.capacity), length(x.length)
     {
     	data = new T[capacity];
     	if (data == NULL) throw AllocationFailure("The operation 'new' is failed.");
     	for (int i=0 ; i<length ; ++i)
     	 data[i]=x.data[i];
     }

    /**
     * TODO Appends the specified element to the end of this list.
     * return false if failed to add it into the list.
     */
    bool add(const T& e) 
     {
     	if (length == capacity && !doubleSize())
     	 return false;
     	data[length++] = e;
     	return true;
     }

    /**
     * TODO Inserts the specified element to the specified position in this list.
     * The range of index parameter is [0, size], where index=0 means inserting to the head,
     * and index=size means appending to the end.
     * @throw IndexOutOfBound
     */
    void add(int index, const T& element)
     {
     	if ( index<0 || index > length )
     	 throw IndexOutOfBound("Index out of bound.");
     	if (length == capacity && !doubleSize())
     	 throw IndexOutOfBound("Failed to add the element into the list, because we cannot enlarge the capacity of the list.");
     	++length;
     	for (int i = length-1; i>index; --i)
     	 data[i] = data[i-1];
     	data[index] = element;
     }

    /**
     * TODO Removes all of the elements from this list.
     */
    void clear() { length=0; }

    /**
     * TODO [] operator

     */
    T& operator[](int index)
     {
     	if (index < 0 || index >= length)
    	 throw IndexOutOfBound("The position of the Iterator is out of bound.");
    	return data[index];
     }

    /**
     * TODO Returns a const reference to the element at the specified position in this list.
     * The index is zero-based, with range [0, size).
     * @throw IndexOutOfBound
     */
    const T& get(int index) const
     {
     	if ( index < 0 || index >= length )
     	 throw IndexOutOfBound("Index out of bound.");
     	return data[index];
     }

    /**
     * TODO Returns true if this list contains the specified element.
     */
    bool contains(const T& e) const
     {
     	for (int i=0; i<length; ++i)
     	 if ( e == data[i] )
     	  return true;
     	return false;
     }

    /**
     * TODO Returns true if this list contains no elements.
     */
    bool isEmpty() const { return (length==0); }

    /**
     * TODO Removes the element at the specified position in this list.
     * The index is zero-based, with range [0, size).
     * @throw IndexOutOfBound
     */
    void removeIndex(int index)
     {
     	if ( index < 0 || index >= length )
     	 throw IndexOutOfBound("Index out of bound.");
     	--length;
     	for (int i=index; i<length; ++i)
     	 data[i] = data[i+1];
     }

    /**
     * TODO Removes the first occurrence of the specified element from this list, if it is present.
     * Returns true if it is present in the list, otherwise false.
     */
    bool remove(const T &e)
     {
     	for (int i=0; i<length; ++i)
     	 if (data[i] == e)
     	  {
     	  	removeIndex(i);
     	  	return true;
     	  }
     	return false;
     }

    /**
     * TODO Replaces the element at the specified position in this list with the specified element.
     * The index is zero-based, with range [0, size).
     * @throw IndexOutOfBound
     */
    void set(int index, const T &element)
     {
     	if ( index < 0 || index >= length )
     	 throw IndexOutOfBound("Index out of bound.");
     	data[index] = element;
     }

    /**
     * TODO Returns the number of elements in this list.
     */
    int size() const { return length; }

    /**
     * TODO Returns an iterator over the elements in this list.
     */
    Iterator iterator()
     {
     	Iterator itr(this);
     	return itr;
     }
};

#endif
