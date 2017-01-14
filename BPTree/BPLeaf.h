#include "IndexPageManager.h"
#include "BPEntry.h"

#ifndef BP_LEAF_H
#define BP_LEAF_H

ipg_pntr leaf_get_prev(page *leaf) {
    return *((ipg_pntr*)&leaf->data[0]);
}

ipg_pntr leaf_get_next(page *leaf) {
    return *((ipg_pntr*)&leaf->data[INDEX_PAGE_SIZE-INDEX_PAGE_POINTER_SIZE]);
}

template<class Key>
//  NO boundary check
BPEntry<Key> leaf_get_entry(page *leaf, unsigned int i) {
    return *((BPEntry<Key>)&leaf->data[i*sizeof(BPEntry<Key>)+INDEX_PAGE_POINTER_SIZE])
}

#endif