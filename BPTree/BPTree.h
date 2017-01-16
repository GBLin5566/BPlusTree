#include <unordered_map>

#include "../TableIndex.h"
#include "BPEntry.h"

#ifndef BP_TREE_H
#define BP_TREE_H

template <class Key>
struct SplitInfo {
    Key divider;
    ipg_pntr right_pg;
};

struct RemoveInfo {
    bool needRedis;
    unsigned int rid;
    // rid rid;
};

template<class Key>
class BPTree {
    public:
    BPTree();
    void insert(BPEntry<Key> entry);
    BPEntry<Key> read_match(Key key);
    std::vector< BPEntry<Key> > read_range(Key key1, Key key2);
    unsigned int remove_by_key(Key key);

    unsigned int leafPageNum;
    unsigned int totalPageNum;
    unsigned int depth;

    private:
    std::unordered_map<ipg_pntr, int> numEntries;
    IndexPageManager ipm;
    ipg_pntr rootPage;
    SplitInfo<Key> * insert_node(ipg_pntr node, BPEntry<Key> entry, unsigned int nowDepth);
    SplitInfo<Key> * insert_leaf(ipg_pntr leaf, BPEntry<Key> entry);
    RemoveInfo remove_node(ipg_pntr node, Key key, unsigned int nowDepth);
    RemoveInfo remove_leaf(ipg_pntr leaf, Key key);

    public:
    //  63<int>, 36<KeyString>
    // const int page_capacity = 4;
    const int page_capacity = (INDEX_PAGE_SIZE - 2*INDEX_PAGE_POINTER_SIZE)/sizeof(BPEntry<Key>);
};

#endif