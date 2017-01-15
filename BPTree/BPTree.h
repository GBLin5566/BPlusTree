#include <unordered_map>

#include "../TableIndex.h"
#include "BPLeaf.h"
#include "BPEntry.h"

#ifndef BP_TREE_H
#define BP_TREE_H

template <class Key>
struct SplitInfo {
    Key divider;
    ipg_pntr right_pg;
};

template<class Key>
class BPTree {
    public:
    BPTree();
    void insert(BPEntry<Key> entry);
    unsigned int leafPageNum;
    unsigned int totalPageNum;
    unsigned int depth;
    // BPNode<Key> *root;
    // BPLeaf<Key> *first;
    // BPLeaf<Key> *last;
    private:
    std::unordered_map<ipg_pntr, int> numEntries;
    IndexPageManager ipm;
    ipg_pntr rootPage;
    SplitInfo<Key> * insert_node(ipg_pntr node, BPEntry<Key> entry, unsigned int nowDepth);
    SplitInfo<Key> * insert_leaf(ipg_pntr leaf, BPEntry<Key> entry);

    public:
    //  63<int>, 36<KeyString>
    const int page_capacity = 4;
    // const int page_capacity = (INDEX_PAGE_SIZE - 2*INDEX_PAGE_POINTER_SIZE)/sizeof(BPEntry<Key>);
};

#endif