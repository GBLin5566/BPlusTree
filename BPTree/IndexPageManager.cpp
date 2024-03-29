#include <vector>
#include <cstdlib>
#include <cstring>
#include <cstdio>

#include "IndexPageManager.h"

IndexPageManager::IndexPageManager() {
    firstAvailable = 0;
}

ipg_pntr IndexPageManager::new_page() {
    if (firstAvailable == table.size()) {
        ptEntry newEntry;
        newEntry.isUsing = true;
        newEntry.pointer = (page*)malloc(INDEX_PAGE_SIZE);
        memset(newEntry.pointer, 0xff, INDEX_PAGE_SIZE);
        table.push_back(newEntry);
        firstAvailable++;
        return table.size()-1;
    } else if (firstAvailable < table.size()) {
        if (table[firstAvailable].isUsing) {
            perror("IndexPageManager: Head points to occupied\n");
            return INDEX_PAGE_INVALID;
        } else {
            ipg_pntr ret = firstAvailable;
            firstAvailable = (ipg_pntr) table[firstAvailable].pointer;
            table[ret].isUsing = true;
            table[ret].pointer = (page*)malloc(INDEX_PAGE_SIZE);
            memset(table[ret].pointer, 0xff, INDEX_PAGE_SIZE);
            return ret;
        }
    } else {
        perror("IndexPageManager: Head too large\n");
        return INDEX_PAGE_INVALID;
    }
}

page* IndexPageManager::get_page(ipg_pntr at) {
    if (table[at].isUsing) {
        return table[at].pointer;
    } else {
        perror("IndexPageManager: Get unused page\n");
        return NULL;
    }
}

bool IndexPageManager::del_page(ipg_pntr at) {
    if (table[at].isUsing) {
        free(table[at].pointer);
        table[at].pointer = (page*) (firstAvailable);
        table[at].isUsing = false;
        firstAvailable = at;
        return true;
    } else {
        perror("IndexPageManager: Delete unused page\n");
        return false;
    }
}
