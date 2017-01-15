#include "BPTree.h"

#include <cstring>

#include "BPEntry.h"
#include "BPNode.h"
#include "BPLeaf.h"

template <class Key>
BPTree<Key>::BPTree() {
    rootPage = ipm.new_page();
    numEntries[rootPage] = 0;
    leafPageNum = 1;
    totalPageNum = 1;
    depth = 0;
}

template <class Key>
void BPTree<Key>::insert(BPEntry<Key> entry) {
    SplitInfo<Key> *result = insert_node(rootPage, entry, 0);
    if (result) {                                   //  Split on root
        ipg_pntr tmp = ipm.new_page();
        numEntries[tmp] = 0;
        totalPageNum++;
        node_set_pointer((node_page<Key>*)ipm.get_page(tmp), 0, rootPage);
        node_insert_entry((node_page<Key>*)ipm.get_page(tmp), *((BPEntry<Key>*)result), numEntries[tmp]);
        numEntries[tmp]++;
        rootPage = tmp;
        depth++;
        delete result;
    }
}

template <class Key>
SplitInfo<Key> * BPTree<Key>::insert_node(ipg_pntr node, BPEntry<Key> entry, unsigned int nowDepth) {
    if (nowDepth == depth) {                        //  Is leaf
        return insert_leaf(node, entry);
    }
    ipg_pntr child = node_get_child((node_page<Key>*)ipm.get_page(node), entry.key, numEntries[node]);
    SplitInfo<Key> *result = insert_node(child, entry, nowDepth+1);
    if (result) {                                   //  There was a split at child
        SplitInfo<Key> *ret = NULL;
        if (numEntries[node] == page_capacity) {    //  Need split
            ret = new SplitInfo<Key>;
            ret->right_pg = ipm.new_page();
            ret->divider = node_split_page((node_page<Key>*)ipm.get_page(node), (node_page<Key>*)ipm.get_page(ret->right_pg), *((BPEntry<Key>*)result), page_capacity);
            totalPageNum++;
            numEntries[node] = page_capacity/2;
            numEntries[ret->right_pg] = page_capacity - page_capacity/2;
        } else {
            node_insert_entry((node_page<Key>*)ipm.get_page(node), *((BPEntry<Key>*)result), numEntries[node]);
            numEntries[node]++;
        }
        delete result;
        return ret;
    } else {
        return NULL;
    }
}

template <class Key>
SplitInfo<Key> * BPTree<Key>::insert_leaf(ipg_pntr leaf, BPEntry<Key> entry) {
    if (numEntries[leaf] == page_capacity) {        //  Need split
        SplitInfo<Key> *ret = new SplitInfo<Key>;
        ret->right_pg = ipm.new_page();
        ret->divider = leaf_split_page((leaf_page<Key>*)ipm.get_page(leaf), (leaf_page<Key>*)ipm.get_page(ret->right_pg), leaf, ret->right_pg, page_capacity);
        leafPageNum++;
        totalPageNum++;
        numEntries[leaf] = page_capacity/2;
        numEntries[ret->right_pg] = page_capacity - page_capacity/2;
        if (entry.key < ret->divider) {
            leaf_insert_entry((leaf_page<Key>*)ipm.get_page(leaf), entry, numEntries[leaf]);
            numEntries[leaf]++;
        } else {
            leaf_insert_entry((leaf_page<Key>*)ipm.get_page(ret->right_pg), entry, numEntries[ret->right_pg]);
            numEntries[ret->right_pg]++;
        }
        return ret;
    } else {
        leaf_insert_entry((leaf_page<Key>*)ipm.get_page(leaf), entry, numEntries[leaf]);
        numEntries[leaf]++;
        return NULL;
    }
}

int main(int argc, char **argv) {
    BPTree<int> tree;
    BPEntry<int> entry;
    printf("%d, %d, %d\n", tree.depth, tree.leafPageNum, tree.totalPageNum);
    for (int i=0; i<strtol(argv[1], NULL, 10); i++) {
        entry.key = 3;
        entry.value = 5;
        tree.insert(entry);
    }
    printf("%d, %d, %d\n", tree.depth, tree.leafPageNum, tree.totalPageNum);
    return 0;
}