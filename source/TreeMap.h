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
        Entry(K k, V v): key(k), value(v) {}

        K getKey() { return key; }

        V getValue() { return value; }
        
        bool operator< (const Entry& x) const { return (key<x.key); }
        
        bool operator== (const Entry& x) const { return (key==x.key); }
    };
private:
AVLTree<Entry>* tree;

public:
    class Iterator
    {
    private:
    
    AVLTree<Entry>* tree;
    AVLNode<Entry>* pos; //	when pos==NULL, it means the next element pointed by iterator is the first in the map.
    
    public:
    	/** Construct a new iterator */
    	Iterator(AVLTree<Entry>* t): tree(t), pos(NULL) {}
    
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
     */
    TreeMap(): tree() { }

    /**
     * TODO Destructor
     */
    virtual ~TreeMap() { }

    /**
     * TODO Assignment operator
     */
    TreeMap &operator=(const TreeMap &x) { tree->root = copyTree(x->tree->root); }

    /**
     * TODO Copy-constructor
     */
    TreeMap(const TreeMap &x) { tree->clear(); tree->root = copyTree(x->tree->root); }

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
    bool containsKey(const K &key) const { Entry ent(key,0); return (tree->find(ent)!=NULL); }

    /** Search specified value in the subtree, if exists, return true */
    bool findValue(AVLNode<Entry>* root, V v);
    
    /**
     * TODO Returns true if this map maps one or more keys to the specified value.
     */
    bool containsValue(const V &value) const { return findValue(tree->root,value); }

    /**
     * TODO Returns a const reference to the value to which the specified key is mapped.
     * If the key is not present in this map, this function should throw ElementNotExist exception.
     * @throw ElementNotExist
     */
    const V &get(const K &key) const { Entry ent(key,0); return tree->find(ent)->data.value; }

    /**
     * TODO Returns true if this map contains no key-value mappings.
     */
    bool isEmpty() const { return (tree->root==NULL); }

    /**
     * TODO Associates the specified value with the specified key in this map.
     * @throw AllocationFailure
     */
    void put(const K &key, const V &value) { Entry ent(key,value); tree->add(ent); }

    /**
     * TODO Removes the mapping for the specified key from this map if present.
     * If there is no mapping for the specified key, throws ElementNotExist exception.
     * @throw ElementNotExist
     */
    void remove(const K &key) { Entry ent(key,0); tree->remove(ent); }

    /**
     * TODO Returns the number of key-value mappings in this map.
     */
    int size() const { return tree->root->size(); }
};

/**
 * TODO Returns true if the iteration has more elements.
 */
template<class K, class V>
inline bool TreeMap<K,V>::Iterator::hasNext()
{
   	if (tree->root==NULL)
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
	if (tree->root==NULL)
	 throw ElementNotExist("There is no element in the map.");
	if (pos==NULL)
	 {
	 	pos=tree->root;
	 	while (pos->l!=NULL)
	 	 pos = pos->l;
	 	return pos->get();
	 }
	pos = pos->next();
	return pos->get();
}

/** Search specified value in the subtree, if exists, return true */
template<class K,class V>
bool TreeMap<K,V>::findValue(AVLNode<TreeMap<K,V>::Entry>* root, V v)
{
	if (root==NULL)
	 return false;
	if (root->data.value==v)
	 return true;
	return (findValue(root->l,v) || (findValue(root->r,v)));
}

#endif
