#include "IndexPageManager.h"
#include "BPEntry.h"

#ifndef BP_NODE_H
#define BP_NODE_H

#pragma pack(push)
#pragma pack(1)
template <class Key>
struct node_page {
    ipg_pntr smallest;
    BPEntry<Key> entries[(INDEX_PAGE_SIZE - 2*INDEX_PAGE_POINTER_SIZE)/sizeof(BPEntry<Key>)];
    ipg_pntr stuffing;
};
#pragma pack(pop)

template <class Key>
void node_set_pointer (node_page<Key> *node, int pos, ipg_pntr value) {
    if (pos == 0) {
        node->smallest = value;
    } else {
        node->entries[pos-1].value = value;
    }
}

template <class Key>
ipg_pntr node_get_child (node_page<Key> *node, Key key, int numEntries) {
    int i;
    for (i=0; i<numEntries; i++) {
        if (key < node->entries[i].key)  break;
    }
    if (i == 0) {
        return node->smallest;
    } else {
        return node->entries[i-1].value;
    }
}

template <class Key>
void node_insert_entry (node_page<Key> *node, BPEntry<Key> entry, int numEntries) {
    int i;
    for (i=0; i<numEntries; i++) {
        if (entry.key < node->entries[i].key)   break;
    }
    //  Shift entries after i by 1
    memcpy( &node->entries[i+1],
            &node->entries[i],
            (numEntries-i)*sizeof(BPEntry<Key>));
    node->entries[i] = entry;
}

template <class Key>
Key node_split_page (node_page<Key> *left, node_page<Key> *right, BPEntry<Key> entry, int page_capacity) {
    int i;
    for (i=0; i<page_capacity; i++) {
        if (entry.key < left->entries[i].key)   break;
    }
    if (i == page_capacity/2) {
        right->smallest = entry.value;
        memcpy( &right->entries[0],
                &left->entries[i],
                (page_capacity - page_capacity/2)*sizeof(BPEntry<Key>));
        return entry.key;
    } else if (i < page_capacity/2) {
        memcpy( &right->entries[0],
                &left->entries[page_capacity/2],
                (page_capacity - page_capacity/2)*sizeof(BPEntry<Key>));
        node_insert_entry(left, entry, page_capacity/2);
        return left->entries[page_capacity/2].key;
    } else {
        memcpy( &right->entries[0],
                &left->entries[page_capacity/2+1],
                (i-(page_capacity/2+1))*sizeof(BPEntry<Key>));
        right->entries[i-(page_capacity/2+1)] = entry;
        memcpy( &right->entries[i-page_capacity/2],
                &left->entries[i],
                (page_capacity-i)*sizeof(BPEntry<Key>));
        return left->entries[page_capacity/2].key;
    }
}

#endif