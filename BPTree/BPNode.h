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

//  NO Boundary check
template <class Key> ipg_pntr node_get_pointer (node_page<Key> *node, int pos) {return node->entries[pos-1].value;}
template <class Key> void node_set_pointer (node_page<Key> *node, int pos, ipg_pntr value) {node->entries[pos-1].value = value;}
template <class Key> void node_set_key (node_page<Key> *node, int pos, Key key) {node->entries[pos].key = key;}
template <class Key> Key node_get_key (node_page<Key> *node, int pos) {return node->entries[pos].key;};

template <class Key>
int node_get_position (node_page<Key> *node, Key key, int numEntries) {
    int i;
    for (i=0; i<numEntries; i++) {
        if (key < node->entries[i].key)  break;
    }
    return i;
}

template <class Key>
ipg_pntr node_get_child (node_page<Key> *node, Key key, int numEntries) {
    int i = node_get_position(node, key, numEntries);
    return node_get_pointer(node, i);
}

template <class Key>
void node_insert_entry (node_page<Key> *node, BPEntry<Key> entry, int numEntries) {
    int i = node_get_position(node, entry.key, numEntries);
    //  Shift entries after i by 1
    memcpy( &node->entries[i+1],
            &node->entries[i],
            (numEntries-i)*sizeof(BPEntry<Key>));
    node->entries[i] = entry;
}

template <class Key>
Key node_split_page (node_page<Key> *left, node_page<Key> *right, BPEntry<Key> entry, int page_capacity) {
    int i = node_get_position(left, entry.key, page_capacity);
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

template <class Key>
Key node_delete_position (node_page<Key> *node, int pos, int numEntries) {
    Key ret = node_get_key(node, pos);
    memcpy( &node->entries[pos],
            &node->entries[pos+1],
            (numEntries-pos-1)*sizeof(BPEntry<Key>*));
    return ret;
}

template <class Key>
Key node_redistribute (node_page<Key> *left, node_page<Key> *right, int l_numEntries, int r_numEntries, Key key) {
    int left_increase = (l_numEntries+r_numEntries)/2-l_numEntries;
    if (left_increase < 0) {            //  Left to right
        memcpy( &right->entries[-left_increase-1].value,
                &right->entries[-1].value,
                r_numEntries*sizeof(BPEntry<Key>)+sizeof(ipg_pntr));
        right->entries[-left_increase-1].key = key;
        memcpy( &right->entries[-1].value,
                &left->entries[l_numEntries+left_increase].value,
                (-left_increase-1)*sizeof(BPEntry<Key>));
        return left->entries[l_numEntries+left_increase].key;
    } else if (left_increase > 0) {     //  Right to left
        left->entries[l_numEntries].key = key;
        memcpy( &left->entries[l_numEntries].value,
                &right->entries[-1].value,
                (left_increase)*sizeof(BPEntry<Key>));
        memcpy( &right->entries[-1].value,
                &right->entries[left_increase-1].value,
                (r_numEntries-left_increase)*sizeof(BPEntry<Key>)+sizeof(ipg_pntr));
        return left->entries[l_numEntries+left_increase].key;
    } else {
        perror("BPNode: Redistribute by 0?\n");
    }
    return right->entries[0].key;
}

template <class Key>
void node_merge_pages (node_page<Key> *left, node_page<Key> *right, int l_numEntries, int r_numEntries, Key key) {
    left->entries[l_numEntries].key = key;
    memcpy( &left->entries[l_numEntries].value,
            &right->entries[-1].value,
            r_numEntries*sizeof(BPEntry<Key>)+sizeof(ipg_pntr));
}

#endif