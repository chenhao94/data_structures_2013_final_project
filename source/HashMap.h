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
        bool flag, del; // flag = true means that this is a valid element, otherwise means empty. del = true means that the element has been deleted.
    public:
        Entry() :flag(false), del(false){}
    
        Entry(K k, V v)
        {
            key = k;
            value = v;
            flag = true;
			del = false;
        }

        const K& getKey() const { return key; }

        const V& getValue() const { return value; }
        
        bool getFlag() const { return flag; }
       
		bool getDel() const { return del; }

        friend class HashMap<K,V,H>;
    };

private:
	const static int prime[11];
	static const int initialSize = 103;
	long _p;
	long capacity, Size, totDel;
	Entry* storage;
	static H hash;
	
	/**
     *	TODO Double the capacity of the container.
     *  Returns false if fail to enlarge the size.
     */
    inline bool reSize();
	
	/**
	 *	TODO Execute when there are too much "del" marks.
	 *  @throw AllocationFailure
	 */
	inline void reHash();

public:
    class Iterator
    {
    typedef const HashMap<K,V,H>* const_HashMapPointer;
    
    private:
    	const_HashMapPointer list;
    	long pos;
    
    public:
    	/**
    	 * TODO Constructs a new iterator.
    	 */
    	Iterator(const const_HashMapPointer& li = NULL): list(li), pos(-1) {}

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
    virtual ~HashMap() { delete[] storage; }

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
    inline Iterator iterator() const;

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
template<class K, class V, class H>
const int HashMap<K,V,H>::prime[11]={103, 457, 1619, 6701, 25931, 110623, 575777, 2099941, 16769023, 39916801, 100000007};

//-------------The end of the class-----------------------------------

/**
  * Double the capacity of the container.
  *	Returns false if fail to enlarge the size.
  */
template <class K, class V, class H>
inline bool HashMap<K,V,H>::reSize()
{
   	HashMap<K,V,H>::Entry *tmp = storage;
   	long oldCap = capacity, pos;
	++_p;
   	capacity = prime[_p];
   	
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
   	 if (tmp[i].getFlag()==true && tmp[i].getDel()==false)
   	  {
   	  	pos = hash.hashCode(tmp[i].key)%capacity;
		if (pos<0)
		 pos+=capacity;
   	  	for (; storage[pos].flag==true; pos=(pos+1)%capacity);
   	  	storage[pos]=tmp[i];
   	  }
	totDel = 0;
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
	HashMap<K,V,H>::Entry *tmp = list->storage;
	for (long i=pos+1; i<cap; ++i)
	 if (tmp[i].getFlag()==true && tmp[i].getDel()==false)
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
	 if (tmp[i].getFlag()==true && tmp[i].getDel()==false)
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
inline HashMap<K,V,H>::HashMap(): _p(0), Size(0), capacity(initialSize), totDel(0) 
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
	if (this==&x)
	 return *this;
	delete[] storage;
	capacity=x.capacity;
   	Size=x.Size;
	totDel = x.totDel;
   	storage = new HashMap<K,V,H>::Entry[capacity];
   	if (storage == NULL) throw AllocationFailure("The operation 'new' is failed.");
	for (int i=0; i<capacity; ++i)
	 storage[i]=x.storage[i];
	return *this;
}

/**
 * TODO Copy-constructor
 * @throw AllocationFailure
 */
template <class K, class V, class H>
inline HashMap<K,V,H>::HashMap(const HashMap &x): _p(x._p), capacity(x.capacity), Size(x.Size), totDel(x.totDel)
{
   	storage = new HashMap<K,V,H>::Entry[capacity];
   	if (storage == NULL) throw AllocationFailure("The operation 'new' is failed.");
	for (int i=0; i<capacity; ++i)
	 storage[i]=x.storage[i];
}

/**
 * TODO Returns an iterator over the elements in this map.
 */
template <class K, class V, class H>
inline class HashMap<K,V,H>::Iterator HashMap<K,V,H>::iterator() const
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
	Size = totDel = 0;
	for (int i=0; i<capacity; ++i)
	 storage[i].flag = storage[i].del = false;
}

/**
 * TODO Returns true if this map contains a mapping for the specified key.
 */
template <class K, class V, class H>
inline bool HashMap<K,V,H>::containsKey(const K &key) const
{
	long pos = hash.hashCode(key)%capacity;
	if (pos<0)
	 pos+=capacity;
   	for (; storage[pos].flag==true ; pos=(pos+1)%capacity)
   	 if (storage[pos].del==false && storage[pos].key==key)
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
   	 if (storage[i].flag==true && storage[i].del==false && storage[i].value==value)
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
	if (pos<0)
	 pos+=capacity;
   	for (; storage[pos].flag==true ; pos=(pos+1)%capacity)
   	 if (storage[pos].del==false && storage[pos].key==key)
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
	if (Size + totDel >= capacity/2 && reSize()==false) throw AllocationFailure("The operation 'new' is failed.");
	try { remove(key); } catch (ElementNotExist error) {}
	long pos = hash.hashCode(key)%capacity;
	if (pos<0)
	 pos+=capacity;
   	++Size;
	for (; storage[pos].flag==true || storage[pos].del==true; pos=(pos+1)%capacity);
	if (storage[pos].del==true)
	 --totDel;
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
	if (pos<0)
	 pos+=capacity;
	int hashInt;
   	for (nowPos=pos; storage[nowPos].flag==true; nowPos=(nowPos+1)%capacity)
   	 if (storage[nowPos].del==false && storage[nowPos].key==key)
   	  break;
   	if (storage[nowPos].flag==false || storage[nowPos].key!=key) throw ElementNotExist("The element you want to visit does not exsit.");
   	--Size;
	storage[nowPos].del = true;
	++totDel;
	if (totDel>=Size)
	 reHash();
}

/**
 *	TODO Execute when there are too much "del" marks.
 *	@throw AllocationFailure
 */
template <class K, class V, class H>
inline void HashMap<K,V,H>::reHash()
{
   	HashMap<K,V,H>::Entry *tmp = storage;
   	long pos;
   	
   	try
   	 {
   		storage = new HashMap<K,V,H>::Entry[capacity];
   		if (storage == NULL)
   		 {
   		 	storage = tmp;
   		 	throw AllocationFailure("The operation 'new' is failed.");
   		 }
   	 }
   	catch (AllocationFailure error) { return; }
	
   	for (int i=0; i<capacity ;++i)
   	 if (tmp[i].getFlag()==true && tmp[i].getDel()==false)
   	  {
   	  	pos = hash.hashCode(tmp[i].key)%capacity;
		if (pos<0)
		 pos+=capacity;
   	  	for (; storage[pos].flag==true; pos=(pos+1)%capacity);
   	  	storage[pos]=tmp[i];
   	  }
	totDel = 0;
   	delete []tmp;
}

#endif
