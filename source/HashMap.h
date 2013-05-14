/** @file */

#ifndef __HASHMAP_H
#define __HASHMAP_H

#include "ElementNotExist.h"
#include "AllocationFailure.h"

/**
 * HashMap is a map implemented by hashing. Also, the 'capacity' here means the
 * number of buckets in your internal implemention, not the current number of the
 * elements.
 *
 * Template argument H are used to specify the hash function.
 * H should be a class with a static function named ``hashCode'',
 * which takes a parameter of type K and returns a value of type int.
 * For example, the following class
 * @code
 *      class Hashint {
 *      public:
 *          static int hashCode(int obj) {
 *              return obj;
 *          }
 *      };
 * @endcode
 * specifies an hash function for integers. Then we can define:
 * @code
 *      HashMap<int, int, Hashint> hash;
 * @endcode
 *
 * Hash function passed to this class should observe the following rule: if two keys
 * are equal (which means key1 == key2), then the hash code of them should be the
 * same. However, it is not generally required that the hash function should work in
 * the other direction: if the hash code of two keys are equal, the two keys could be
 * different.
 *
 * Note that the correctness of HashMap should not rely on the choice of hash function.
 * This is to say that, even the given hash function always returns the same hash code
 * for all keys (thus causing a serious collision), methods of HashMap should still
 * function correctly, though the performance will be poor in this case.
 *
 * The order of iteration could be arbitary in HashMap. But it should be guaranteed
 * that each (key, value) pair be iterated exactly once.
 */
template <class K, class V, class H>
class HashMap
{
public:
    class Entry
    {
        K key;
        V value;
        bool flag; // flag = true means that this is a valid element, otherwise means empty.
    public:
        Entry() :flag(false){}
    
        Entry(K k, V v)
        {
            key = k;
            value = v;
            flag=true;
        }

        K getKey() const { return key; }

        V getValue() const { return value; }
        
        bool getFlag() const { return flag; }
        
        friend class HashMap<K,V,H>;
    };

private:
	static const int initialSize = 100;
	long capacity, Size;
	Entry* storage;
	static H hash;
	
	/**
     *	TODO Double the capacity of the container.
     *  Returns false if fail to enlarge the size.
     */
    inline bool doubleSize();

public:
    class Iterator
    {
    
    typedef HashMap<K,V,H>* HashMapPointer;
    
    private:
    	HashMapPointer list;
    	long pos;
    
    public:
    	/**
    	 * TODO Constructs a new iterator.
    	 */
    	Iterator(const HashMapPointer& li = NULL): list(li), pos(-1) {}

        /**
         * TODO Returns true if the iteration has more elements.
         */
        inline bool hasNext();

        /**
         * TODO Returns the next element in the iteration.
         * @throw ElementNotExist exception when hasNext() == false
         */
        inline const Entry &next();
    };

    /**
     * TODO Constructs an empty hash map.
     * @throw AllocationFailure
     */
    inline HashMap();

    /**
     * TODO Destructor
     */
    ~HashMap() { delete[] storage; }

    /**
     * TODO Assignment operator
     * @throw AllocationFailure
     */
    inline HashMap &operator=(const HashMap &x);

    /**
     * TODO Copy-constructor
     * @throw AllocationFailure
     */
    inline HashMap(const HashMap &x);

    /**
     * TODO Returns an iterator over the elements in this map.
     */
    inline Iterator iterator();

    /**
     * TODO Removes all of the mappings from this map.
     */
    inline void clear();

    /**
     * TODO Returns true if this map contains a mapping for the specified key.
     */
    inline bool containsKey(const K &key) const;

    /**
     * TODO Returns true if this map maps one or more keys to the specified value.
     */
    inline bool containsValue(const V &value) const;

    /**
     * TODO Returns a const reference to the value to which the specified key is mapped.
     * If the key is not present in this map, this function should throw ElementNotExist exception.
     * @throw ElementNotExist
     */
    inline const V &get(const K &key) const;

    /**
     * TODO Returns true if this map contains no key-value mappings.
     */
    bool isEmpty() const { return (Size==0); }

    /**
     * TODO Associates the specified value with the specified key in this map.
     */
    inline void put(const K &key, const V &value);

    /**
     * TODO Removes the mapping for the specified key from this map if present.
     * If there is no mapping for the specified key, throws ElementNotExist exception.
     * @throw ElementNotExist
     */
    inline void remove(const K &key);

    /**
     * TODO Returns the number of key-value mappings in this map.
     */
    int size() const { return Size; }
};

//-------------The end of the class-----------------------------------

/**
  * Double the capacity of the container.
  *	Returns false if fail to enlarge the size.
  */
template <class K, class V, class H>
inline bool HashMap<K,V,H>::doubleSize()
{
   	HashMap<K,V,H>::Entry *tmp = storage;
   	long oldCap = capacity, pos;
   	capacity *= 2;
   	
   	try
   	 {
   		storage = new HashMap<K,V,H>::Entry[capacity];
   		if (storage == NULL)
   		 {
   		 	storage = tmp;
   		 	throw AllocationFailure("The operation 'new' is failed.");
   		 }
   	 }
   	catch (AllocationFailure error) { return false; }
	
   	for (int i=0; i<oldCap ;++i)
   	 if (tmp[i].getFlag()==true)
   	  {
   	  	pos = hash.hashCode(tmp[i].key)%capacity;
   	  	for (; storage[pos].flag==true; pos=(pos+1)%capacity);
   	  	storage[pos]=tmp[i];
   	  }
   	
   	delete []tmp;
   	return true;
}

/**
 * TODO Returns true if the iteration has more elements.
 */
template <class K, class V, class H>
inline bool HashMap<K,V,H>::Iterator::hasNext()
{
	long cap = list->capacity;
	HashMap<K,V,H>::Entry *tmp = this->storage;
	for (long i=list->pos+1; i<cap; ++i)
	 if (tmp[i].getFlag()==true)
	  return true;
	return false;
}

/**
 * TODO Returns the next element in the iteration.
 * @throw ElementNotExist exception when hasNext() == false
 */
template <class K, class V, class H>
inline const typename HashMap<K,V,H>::Entry& HashMap<K,V,H>::Iterator::next()
{
	long cap = list->capacity;
	HashMap<K,V,H>::Entry *tmp = list->storage;
	for (long i=pos+1; i<cap; ++i)
	 if (tmp[i].getFlag()==true)
	  {
	  	pos=i;
	  	return tmp[i];
	  }
	throw ElementNotExist("The element you want to visit does not exsit.");
}

/**
 * TODO Constructs an empty hash map.
 * @throw AllocationFailure
 */
template <class K, class V, class H>
inline HashMap<K,V,H>::HashMap(): Size(0), capacity(initialSize) 
{
	storage = new HashMap<K,V,H>::Entry[initialSize];
   	if (storage == NULL) throw AllocationFailure("The operation 'new' is failed.");
}

/**
 * TODO Assignment operator
 * @throw AllocationFailure
 */
template <class K, class V, class H>
inline HashMap<K,V,H>& HashMap<K,V,H>::operator=(const HashMap &x)
{
	delete[] storage;
	capacity=x.capacity;
   	Size=x.Size;
   	storage = new HashMap<K,V,H>::Entry[capacity];
   	if (storage == NULL) throw AllocationFailure("The operation 'new' is failed.");
   	for (int i=0 ; i<capacity ; ++i)
  	 storage[i]=x.storage[i];
}

/**
 * TODO Copy-constructor
 * @throw AllocationFailure
 */
template <class K, class V, class H>
inline HashMap<K,V,H>::HashMap(const HashMap &x): capacity(x.capacity), Size(x.Size)
{
   	storage = new HashMap<K,V,H>::Entry[capacity];
   	if (storage == NULL) throw AllocationFailure("The operation 'new' is failed.");
   	for (int i=0 ; i<capacity ; ++i)
   	 storage[i]=x.storage[i];
}

/**
 * TODO Returns an iterator over the elements in this map.
 */
template <class K, class V, class H>
inline class HashMap<K,V,H>::Iterator HashMap<K,V,H>::iterator()
{
	Iterator itr(this);
  	return itr;
}

/**
 * TODO Removes all of the mappings from this map.
 */
template <class K, class V, class H>
inline void HashMap<K,V,H>::clear()
{
	Size=0;
	for (int i=0; i<capacity; ++i)
	 storage[i].flag=false;
}

/**
 * TODO Returns true if this map contains a mapping for the specified key.
 */
template <class K, class V, class H>
inline bool HashMap<K,V,H>::containsKey(const K &key) const
{
	long pos = hash.hashCode(key)%capacity;
   	for (; storage[pos].flag==true ; pos=(pos+1)%capacity)
   	 if (storage[pos].key==key)
   	  return true;
   	return false;
}

/**
 * TODO Returns true if this map maps one or more keys to the specified value.
 */
template <class K, class V, class H>
inline bool HashMap<K,V,H>::containsValue(const V &value) const
{
	for (long i=0; i<capacity; ++i)
   	 if (storage[i].flag==true && storage[i].value==value)
   	  return true;
   	return false;
}

/**
 * TODO Returns a const reference to the value to which the specified key is mapped.
 * If the key is not present in this map, this function should throw ElementNotExist exception.
 * @throw ElementNotExist
 */
template <class K, class V, class H>
inline const V& HashMap<K,V,H>::get(const K &key) const
{
	long pos = hash.hashCode(key)%capacity;
   	for (; storage[pos].flag==true ; pos=(pos+1)%capacity)
   	 if (storage[pos].key==key)
   	  return storage[pos].value;
   	throw ElementNotExist("The element you want to visit does not exsit.");
}
    
/**
 * TODO Associates the specified value with the specified key in this map.
 * @throw AllocationFailure
 */
template <class K, class V, class H>
inline void HashMap<K,V,H>::put(const K &key, const V &value)
{
	HashMap<K,V,H>::Entry element(key,value);
	if (Size >= capacity/2 && doubleSize()==false) throw AllocationFailure("The operation 'new' is failed.");
	long pos = hash.hashCode(key)%capacity;
   	++Size;
	for (; storage[pos].flag==true; pos=(pos+1)%capacity);
   	storage[pos]=element;
}

/**
 * TODO Removes the mapping for the specified key from this map if present.
 * If there is no mapping for the specified key, throws ElementNotExist exception.
 * @throw ElementNotExist
 */
template <class K, class V, class H>
inline void HashMap<K,V,H>::remove(const K &key)
{
	long pos = hash.hashCode(key)%capacity, nowPos;
	int hashInt;
   	for (nowPos=pos; storage[nowPos].flag==true; nowPos=(nowPos+1)%capacity)
   	 if (storage[nowPos].key==key)
   	  break;
   	if (storage[nowPos].key!=key) throw ElementNotExist("The element you want to visit does not exsit.");
   	--Size;
   	storage[pos].flag=false;
   	for (nowPos=pos+1; storage[nowPos].flag==true; nowPos=(nowPos+1)%capacity)
   	 {
   	 	hashInt = hash.hashCode(storage[nowPos].key)%capacity;
   	 	if (hashInt<=pos)
   	 	 {
   	 	 	storage[pos] = storage[nowPos];
   	 	 	storage[nowPos].flag = false;
   	 	 	pos = nowPos;
   	 	 }
   	 }
}

#endif
