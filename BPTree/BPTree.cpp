#include "BPTree.h"

#include <cstring>
#include <vector>

#include "BPEntry.h"
#include "BPNode.h"
#include "BPLeaf.h"
#include "../KeyString.h"

/******************  PRIVATE ZONE START
 *
 *
 *
 *
***************************************/
template <class Key>
SplitInfo<Key> * BPTree<Key>::insert_node(ipg_pntr node, BPEntry<Key> entry, unsigned int nowDepth) {
    node_page<Key> *n_page = (node_page<Key>*)ipm.get_page(node);
    if (nowDepth == depth) {                        //  Is leaf
        return insert_leaf(node, entry);
    }
    ipg_pntr child = node_get_child(n_page, entry.key, numEntries[node]);
    SplitInfo<Key> *result = insert_node(child, entry, nowDepth+1);
    if (result) {                                   //  There was a split at child
        SplitInfo<Key> *ret = NULL;
        if (numEntries[node] == page_capacity) {    //  Need split
            ret = new SplitInfo<Key>;
            ret->right_pg = ipm.new_page();
            totalPageNum++;
            ret->divider = node_split_page(n_page, (node_page<Key>*)ipm.get_page(ret->right_pg), *((BPEntry<Key>*)result), page_capacity);
            numEntries[node] = page_capacity/2;
            numEntries[ret->right_pg] = page_capacity - page_capacity/2;
        } else {
            node_insert_entry(n_page, *((BPEntry<Key>*)result), numEntries[node]);
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
        leafPageNum++;
        totalPageNum++;
        
        leaf_page<Key> *l_page = (leaf_page<Key>*)ipm.get_page(leaf);
        leaf_page<Key> *r_page = (leaf_page<Key>*)ipm.get_page(ret->right_pg);
        ret->divider = leaf_split_page(l_page, r_page, page_capacity);
        leaf_set_prev(r_page, leaf);
        leaf_set_next(r_page, leaf_get_next(l_page));
        leaf_set_next(l_page, ret->right_pg);
        numEntries[leaf] = page_capacity/2;
        numEntries[ret->right_pg] = page_capacity - page_capacity/2;
        if (rightMost == leaf)  rightMost = ret->right_pg;

        if (entry.key < ret->divider) {
            leaf_insert_entry(l_page, entry, numEntries[leaf]);
            numEntries[leaf]++;
        } else {
            leaf_insert_entry(r_page, entry, numEntries[ret->right_pg]);
            numEntries[ret->right_pg]++;
        }
        return ret;
    } else {
        leaf_insert_entry((leaf_page<Key>*)ipm.get_page(leaf), entry, numEntries[leaf]);
        numEntries[leaf]++;
        return NULL;
    }
}

template <class Key>
RemoveInfo BPTree<Key>::remove_node(ipg_pntr node, Key key, unsigned int nowDepth) {
    if (nowDepth == depth) {
        return remove_leaf(node, key);
    }
    node_page<Key> *n_page = (node_page<Key>*)ipm.get_page(node);
    int child_pos = node_get_position(n_page, key, numEntries[node]);
    ipg_pntr child = node_get_pointer(n_page, child_pos);
    RemoveInfo result = remove_node(child, key, nowDepth+1);
    if (result.needRedis) {                       //  The child needs a redistribution
        bool need_merge = true;
        int is_with_left = 1;
        ipg_pntr left, right;
        if (nowDepth == depth-1) {      //  And the child is a leaf
            if (child_pos != 0) {       //  Child has left sibling
                left = node_get_pointer(n_page, child_pos-1);
                right = child;
                if ((numEntries[child]+numEntries[node_get_pointer(n_page, child_pos-1)])/2 >= page_capacity/2) {
                    need_merge = false;
                }
            }
            //  Haven't find redistribution peer
            if (need_merge && child_pos != numEntries[node]) {   //  Child has right sibling
                left = child;
                right = node_get_pointer(n_page, child_pos+1);
                is_with_left = 0;
                if ((numEntries[child]+numEntries[node_get_pointer(n_page, child_pos+1)])/2 >= page_capacity/2) {
                    need_merge = false;
                }
            }
            if (need_merge) {                   //  Do merge
                leaf_merge_pages((leaf_page<Key>*)ipm.get_page(left), (leaf_page<Key>*)ipm.get_page(right), numEntries[left], numEntries[right]);
                if (rightMost == right) rightMost = left;
                numEntries[left] += numEntries[right];
                ipm.del_page(right);
                leafPageNum--;
                totalPageNum--;
                node_delete_position(n_page, child_pos-is_with_left, numEntries[node]);
                numEntries[node]--;
                result.needRedis = numEntries[node] < page_capacity/2;
            } else {                            //  Do redistribution
                node_set_key(n_page, child_pos-is_with_left, leaf_redistribute( (leaf_page<Key>*)ipm.get_page(left),
                                                                                (leaf_page<Key>*)ipm.get_page(right),
                                                                                numEntries[left], numEntries[right]));
                int tmp = numEntries[left];
                numEntries[left] = (numEntries[left]+numEntries[right])/2;
                numEntries[right] += tmp - numEntries[left];
                result.needRedis = false;
            }
        } else {                        //  The child is not leaf
            if (child_pos != 0) {       //  Child has left sibling
                left = node_get_pointer(n_page, child_pos-1);
                right = child;
                if ((numEntries[child]+numEntries[node_get_pointer(n_page, child_pos-1)])/2 >= page_capacity/2) {
                    need_merge = false;
                }
            }
            //  Haven't find redistribution peer
            if (need_merge && child_pos != numEntries[node]) {   //  Child has right sibling
                left = child;
                right = node_get_pointer(n_page, child_pos+1);
                is_with_left = 0;
                if ((numEntries[child]+numEntries[node_get_pointer(n_page, child_pos+1)])/2 >= page_capacity/2) {
                    need_merge = false;
                }
            }
            if (need_merge) {                   //  Do merge
                node_merge_pages(   (node_page<Key>*)ipm.get_page(left), (node_page<Key>*)ipm.get_page(right),
                                    numEntries[left], numEntries[right],
                                    node_delete_position(n_page, child_pos-is_with_left, numEntries[node]));
                numEntries[left] += numEntries[right] + 1;
                ipm.del_page(right);
                totalPageNum--;
                numEntries[node]--;
                result.needRedis = numEntries[node] < page_capacity/2;
            } else {                            //  Do redistribution
                node_set_key(n_page, child_pos-is_with_left, node_redistribute( (node_page<Key>*)ipm.get_page(left),
                                                                                (node_page<Key>*)ipm.get_page(right),
                                                                                numEntries[left], numEntries[right],
                                                                                node_get_key(n_page, child_pos-is_with_left)));
                int tmp = numEntries[left];
                numEntries[left] = (numEntries[left]+numEntries[right])/2;
                numEntries[right] += tmp - numEntries[left];
                result.needRedis = false;
            }
        }
    }
    return result;
}

template <class Key>
RemoveInfo BPTree<Key>::remove_leaf(ipg_pntr leaf, Key key) {
    RemoveInfo ret;
    ret.needRedis = false;
    ret.rid = leaf_delete_by_key((leaf_page<Key>*)ipm.get_page(leaf), key, numEntries[leaf]);
    if (ret.rid != INDEX_PAGE_INVALID) {    //  Key exists
        numEntries[leaf]--;
        ret.needRedis = numEntries[leaf] < page_capacity/2;      //  Request a redis if no enough members
    }
    return ret;
}
/********************  PRIVATE ZONE END
 *
 *
 *
 *
***************************************/

template <class Key>
BPTree<Key>::BPTree() {
    rightMost = leftMost = rootPage = ipm.new_page();
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
        node_page<Key> *n_page = (node_page<Key>*)ipm.get_page(tmp);
        numEntries[tmp] = 0;
        totalPageNum++;
        node_set_pointer(n_page, 0, rootPage);
        node_insert_entry(n_page, *((BPEntry<Key>*)result), numEntries[tmp]);
        numEntries[tmp]++;
        rootPage = tmp;
        depth++;
        delete result;
    }
}

template <class Key>
BPEntry<Key> BPTree<Key>::read_match(Key key) {
    int nowDepth = 0;
    ipg_pntr nowPage = rootPage;
    while (nowDepth < depth) {                  //  Non-leaf
        nowPage = node_get_child((node_page<Key>*)ipm.get_page(nowPage), key, numEntries[nowPage]);
        nowDepth++;
    }
    //  Leaf
    return leaf_get_entry_by_key((leaf_page<Key>*)ipm.get_page(nowPage), key, numEntries[nowPage]);
}

template <class Key>
std::vector< BPEntry<Key> > *BPTree<Key>::read_range(Key key1, Key key2) {
    if (key2 < key1) {
        return new std::vector< BPEntry<Key> >();
    }
    int nowDepth = 0;
    ipg_pntr nowPage = rootPage;
    while (nowDepth < depth) {                  //  Non-leaf
        nowPage = node_get_child((node_page<Key>*)ipm.get_page(nowPage), key1, numEntries[nowPage]);
        nowDepth++;
    }
    //  Leaf
    std::vector< BPEntry<Key> > *ret;
    do {
        nowPage = leaf_scan_entries((leaf_page<Key>*)ipm.get_page(nowPage), key1, key2, numEntries[nowPage], ret);
    } while (nowPage != INDEX_PAGE_INVALID);
    return ret;
}

template <class Key>
unsigned int BPTree<Key>::remove_by_key(Key key) {
    RemoveInfo result = remove_node(rootPage, key, 0);
    if(result.needRedis) {
        if (depth != 0) {
            if(numEntries[rootPage] == 0) {
                ipg_pntr tmp = node_get_pointer((node_page<Key>*)ipm.get_page(rootPage), 0);
                ipm.del_page(rootPage);
                totalPageNum--;
                depth--;
                rootPage = tmp;
            }
        }
    }
    return result.rid;
}

template class BPTree<int>;
template class BPTree<KeyString>;

// int main(int argc, char **argv) {
//     BPTree<int> tree;
//     BPEntry<int> entry;
//     printf("%d, %d, %d\n", tree.depth, tree.leafPageNum, tree.totalPageNum);
//     int num = 0;
//     int key1 = 0;
//     int key2 = 0;
//     int del1 = 0;
//     int del2 = 0;
//     if (argc > 1) {
//         if (argc > 2) {
//             if (argc > 3) {
//                 if (argc > 4) {
//                     if (argc > 5) {
//                         del2 = strtol(argv[5], NULL, 10);
//                     }
//                     del1 = strtol(argv[4], NULL, 10);
//                 }
//                 key2 = strtol(argv[3], NULL, 10);
//             }
//             key1 = strtol(argv[2], NULL, 10);
//         }
//         num = strtol(argv[1], NULL, 10);
//     }
//     for (int i=0; i<num; i++) {
//         entry.key = i;
//         entry.value = i+5;
//         tree.insert(entry);
//     }
//     printf("%d, %d, %d\n", tree.depth, tree.leafPageNum, tree.totalPageNum);
//     std::vector< BPEntry<int> > vector = tree.read_range(key1, key2);
//     for (int i=0; i<vector.size(); i++) {
//         printf("(%d, %u)\n", vector[i].key, vector[i].value);
//     }
//     tree.remove_by_key(del1);
//     tree.remove_by_key(del2);
//     printf("%d, %d, %d\n", tree.depth, tree.leafPageNum, tree.totalPageNum);
//     vector = tree.read_range(key1, key2);
//     for (int i=0; i<vector.size(); i++) {
//         printf("(%d, %u)\n", vector[i].key, vector[i].value);
//     }
//     return 0;
// }