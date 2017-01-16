#include <cstring>

#include "IndexPageManager.h"
#include "BPEntry.h"

#ifndef BP_LEAF_H
#define BP_LEAF_H

#pragma pack(push)
#pragma pack(1)
template <class Key>
struct leaf_page {
    ipg_pntr prev;
    BPEntry<Key> entries[(INDEX_PAGE_SIZE - 2*INDEX_PAGE_POINTER_SIZE)/sizeof(BPEntry<Key>)];
    ipg_pntr next;
};
#pragma pack(pop)

template <class Key>
int _leaf_find_position (leaf_page<Key> *leaf, Key key, int numEntries) {
    int i;
    for (i=0; i<numEntries; i++) {
        if (key < leaf->entries[i].key) break;
    }
    return i;
}

template <class Key> ipg_pntr leaf_get_prev (leaf_page<Key> *leaf) {return leaf->prev;}
template <class Key> ipg_pntr leaf_get_next (leaf_page<Key> *leaf) {return leaf->next;}
template <class Key> void leaf_set_prev (leaf_page<Key> *leaf, ipg_pntr _prev) {leaf->prev = _prev;}
template <class Key> void leaf_set_next (leaf_page<Key> *leaf, ipg_pntr _next) {leaf->next = _next;}

template <class Key>
BPEntry<Key> leaf_get_entry_by_key (leaf_page<Key> *leaf, Key key, int numEntries) {
    BPEntry<Key> ret;
    ret.key = key;
    int i = _leaf_find_position(leaf, key, numEntries);
    if (i == 0) {                                   //  Less than all: No match
        ret.value = INDEX_PAGE_INVALID;
        return ret;
    } else if (leaf->entries[i-1].key < key) {      //  Less than i & larger than i-1: No match
        ret.value = INDEX_PAGE_INVALID;
        return ret;
    } else {
        return leaf->entries[i-1];
    }
}

template <class Key>
ipg_pntr leaf_scan_entries (leaf_page<Key> *leaf, Key key1, Key key2, int numEntries, std::vector< BPEntry<Key> > *vector) {
    int i = _leaf_find_position(leaf, key1, numEntries);
    if (i != 0) {
        if (leaf->entries[i-1].key < key1);
        else {
            i--;
        }
    }
    for (; i<numEntries; i++) {
        if (key2 < leaf->entries[i].key) {
            return INDEX_PAGE_INVALID;
        }
        vector->push_back(leaf->entries[i]);
    }
    return leaf->next;
}

template <class Key>
void leaf_insert_entry (leaf_page<Key> *leaf, BPEntry<Key> entry, int numEntries) {
    int i = _leaf_find_position(leaf, entry.key, numEntries);
    //  Shift entries after i by 1
    memcpy( &leaf->entries[i+1],
            &leaf->entries[i],
            (numEntries-i)*sizeof(BPEntry<Key>));
    leaf->entries[i] = entry;
}

//  Right is always new !
template <class Key>
Key leaf_split_page (leaf_page<Key> *left, leaf_page<Key> *right, int page_capacity) {
    memcpy( &right->entries[0],
            &left->entries[page_capacity/2],
            (page_capacity - page_capacity/2)*sizeof(BPEntry<Key>));
    return right->entries[0].key;
}

template <class Key>
unsigned int leaf_delete_by_key (leaf_page<Key> *leaf, Key key, int numEntries) {
    int i = _leaf_find_position(leaf, key, numEntries);
    if (i == 0)     return INDEX_PAGE_INVALID;                       //  Less than all: No match
    if (leaf->entries[i-1].key < key)   return INDEX_PAGE_INVALID;   //  Less than i & larger than i-1: No match
    unsigned int ret = leaf->entries[i-1].value;
    memcpy( &leaf->entries[i-1],
            &leaf->entries[i],
            (numEntries-i)*sizeof(BPEntry<Key>));
    return ret;
}

template <class Key>
Key leaf_redistribute (leaf_page<Key> *left, leaf_page<Key> *right, int l_numEntries, int r_numEntries) {
    int left_increase = (l_numEntries+r_numEntries)/2-l_numEntries;
    if (left_increase < 0) {            //  Left to right
        memcpy( &right->entries[-left_increase],
                &right->entries[0],
                r_numEntries*sizeof(BPEntry<Key>));
        memcpy( &right->entries[0],
                &left->entries[l_numEntries+left_increase],
                -left_increase*sizeof(BPEntry<Key>));
    } else if (left_increase > 0) {     //  Right to left
        memcpy( &left->entries[l_numEntries],
                &right->entries[0],
                left_increase*sizeof(BPEntry<Key>));
        memcpy( &right->entries[0],
                &right->entries[left_increase],
                (r_numEntries-left_increase)*sizeof(BPEntry<Key>));
    } else {
        perror("BPLeaf: Redistribute by 0?\n");
    }
    return right->entries[0].key;
}

#endif