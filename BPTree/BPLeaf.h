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
ipg_pntr leaf_get_prev (leaf_page<Key> *leaf) {
    return leaf->prev;
}

template <class Key>
ipg_pntr leaf_get_next (leaf_page<Key> *leaf) {
    return leaf->next;
}

template <class Key>    //  NO boundary check
BPEntry<Key> leaf_get_entry (leaf_page<Key> *leaf, unsigned int i) {
    return leaf->entries[i];
}

template <class Key>
void leaf_insert_entry (leaf_page<Key> *leaf, BPEntry<Key> entry, int numEntries) {
    int i;
    for (i=0; i<numEntries; i++) {          //  Find position
        if (entry.key < leaf->entries[i].key)   break;
    }
    //  Shift entries after i by 1
    memcpy( &leaf->entries[i+1],
            &leaf->entries[i],
            (numEntries-i)*sizeof(BPEntry<Key>));
    leaf->entries[i] = entry;
}

//  Right is always new !
template <class Key>
Key leaf_split_page (leaf_page<Key> *left, leaf_page<Key> *right, ipg_pntr left_this, ipg_pntr right_this, int page_capacity) {
    memcpy( &right->entries[0],
            &left->entries[page_capacity/2],
            (page_capacity - page_capacity/2)*sizeof(BPEntry<Key>));
    right->next = left->next;
    right->prev = left_this;
    left->next = right_this;
    return right->entries[0].key;
}

#endif