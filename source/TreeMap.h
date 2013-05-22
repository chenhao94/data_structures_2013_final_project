/** @file */
#ifndef __TREEMAP_H
#define __TREEMAP_H

#include "ElementNotExist.h"
#include "AVLTree.h"
#include "AllocationFailure.h"

/**
 * TreeMap is the balanced-tree implementation of map. The iterators must
 * iterate through the map in the natural order (operator<) of the key.
 */
template<class K, class V>
class TreeMap
{
public:
    class Entry
    {
        K key;
        V value;
    public:
    	Entry() {}
		
		Entry(K k) :key(k) {}
		
        Entry(K k, V v): key(k), value(v) {}

        const K& getKey() const { return key; }

        const V& getValue() const { return value; }
        
        bool operator< (const Entry& x) const { return (key<x.key); }
        
        bool operator== (const Entry& x) const { return (key==x.key); }
        
        friend class TreeMap<K,V>;
    };
private:
AVLTree<Entry>* tree;

public:
    class Iterator
    {
    private:
    
    const AVLTree<Entry>* tree;
    const AVLNode<Entry>* pos; //	when pos==NULL, it means the next element pointed by iterator is the first in the map.
    
    public:
    	/** Construct a new iterator */
    	Iterator(const AVLTree<Entry>* t=NULL): tree(t), pos(NULL) {}
    
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
     * TODO Constructs an empty tree map.
     * @throw AllocationFailure
     */
    TreeMap(): tree(new AVLTree<Entry>) { if (tree==NULL) throw AllocationFailure("The operation 'new' is failed."); }

    /**
     * TODO Destructor
     */
    virtual ~TreeMap() { }

    /**
     * TODO Assignment operator
     * @throw AllocationFailure
     */
    TreeMap &operator=(const TreeMap &x) { tree->clear(); tree->getRoot() = copyTree<Entry>(x.tree->getRoot()); }

    /**
     * TODO Copy-constructor
     * @throw AllocationFailure
     */
    TreeMap(const TreeMap &x): tree(new AVLTree<Entry>) { if (tree==NULL) throw AllocationFailure("The operation 'new' is failed."); 
    													  tree->getRoot() = copyTree<Entry>(x.tree->getRoot()); }

    /**
     * TODO Returns an iterator over the elements in this map.
     */
    Iterator iterator() const { Iterator itr(tree); return itr; }

    /**
     * TODO Removes all of the mappings from this map.
     */
    void clear() { tree->clear(); }

    /**
     * TODO Returns true if this map contains a mapping for the specified key.
     */
    bool containsKey(const K &key) const { Entry ent(key); return (tree->find(ent)!=NULL); }

    /** Search specified value in the subtree, if exists, return true */
    bool findValue(AVLNode<Entry>* root, V v) const;
    
    /**
     * TODO Returns true if this map maps one or more keys to the specified value.
     */
    bool containsValue(const V &value) const { return findValue(tree->getRoot(),value); }

    /**
     * TODO Returns a const reference to the value to which the specified key is mapped.
     * If the key is not present in this map, this function should throw ElementNotExist exception.
     * @throw ElementNotExist
     */
    const V &get(const K &key) const { Entry ent(key); return tree->find(ent)->get().value; }

    /**
     * TODO Returns true if this map contains no key-value mappings.
     */
    bool isEmpty() const { return (tree->getRoot()==NULL); }

    /**
     * TODO Associates the specified value with the specified key in this map.
     * @throw AllocationFailure
     */
    void put(const K &key, const V &value) { try { remove(key); } catch (ElementNotExist error) {}
	 Entry ent(key,value); tree->add(ent); }

    /**
     * TODO Removes the mapping for the specified key from this map if present.
     * If there is no mapping for the specified key, throws ElementNotExist exception.
     * @throw ElementNotExist
     */
    void remove(const K &key) { Entry ent(key); tree->remove(ent); }

    /**
     * TODO Returns the number of key-value mappings in this map.
     */
    int size() const { return ( (tree->getRoot()!=NULL) ? (tree->getRoot()->size()) : 0 ); }
};

/**
 * TODO Returns true if the iteration has more elements.
 */
template<class K, class V>
inline bool TreeMap<K,V>::Iterator::hasNext()
{
   	if (tree->getRoot()==NULL)
   	 return false;
   	if (pos==NULL)
   	 return true;
    try
     {
     	pos->next();
     	return true;
     }
    catch (ElementNotExist error) { return false; }
}

/**
 * TODO Returns the next element in the iteration.
 * @throw ElementNotExist exception when hasNext() == false
 */
template<class K, class V>
inline const typename TreeMap<K,V>::Entry& TreeMap<K,V>::Iterator::next()
{
	if (tree->getRoot()==NULL)
	 throw ElementNotExist("There is no element in the map.");
	if (pos==NULL)
	 {
	 	pos=tree->getRoot()->getFirst();
	 	return pos->get();
	 }
	pos = pos->next();
	return pos->get();
}

/** Search specified value in the subtree, if exists, return true */
template<class K,class V>
bool TreeMap<K,V>::findValue(AVLNode<TreeMap<K,V>::Entry>* root, V v) const
{
	if (root==NULL)
	 return false;
	if (root->get().value==v)
	 return true;
	return (findValue(root->getL(),v) || (findValue(root->getR(),v)));
}

#endif
