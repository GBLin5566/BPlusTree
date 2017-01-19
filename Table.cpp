#include "Table.h"

#include "KeyString.h"
#include "DataPageManager.h"
#include "BPTree/BPTree.h"

#include <cassert>

template <class Key>
Table<Key>::Table(const unsigned int _record_size, const char * const _table_name) {
    record_size = _record_size;
    table_name = _table_name;
    dpm = new DataPageManager<Key>(record_size);
    index = new BPTree<Key>();
}

template <class Key>
Table<Key>::~Table() {
    delete index;
    delete dpm;
}

template <class Key>
int Table<Key>::insert_record (Record<Key> record) {
    BPEntry<Key> entry;
    entry.key = record.getKey();
    rid rid = dpm->insert(record);
    entry.value = *((ipg_pntr*)&rid);
    index->insert(entry);
    return (int) entry.value;
}

template <class Key>
bool Table<Key>::delete_by_key (const Key key) {
    ipg_pntr rid = index->remove_by_key(key);
    if (rid == INDEX_PAGE_INVALID) {
        return false;
    } else {
        if (dpm->del(*((rid_ptr)&rid)))   return true;
        else {
            perror("Key exists in index but missing in data pages\n");
            return false;
        }
    }
}

template <class Key>
Record<Key> Table<Key>::read_by_key (const Key key) {
    BPEntry<Key> entry = index->read_match(key);
    rid rid = *((rid_ptr)&entry.value));
    Record<Key> record(record_size, key, dpm->query(rid));
    record.setRid(rid);
    return 
}

template <class Key>
std::vector< Record<Key> > *Table<Key>::read_by_key (const Key key1, const Key key2) {
    std::vector< Record<Key> > *ret = new std::vector< Record<Key> >();
    std::vector< BPEntry<Key> > *entries = index->read_range(key1, key2);
    rid rid;
    for (int i=0; i<entries->size(); i++) {
        rid = *((rid_ptr)&entries->at(i).value))
        Record<Key> record(record_size, entries->at(i).key, dpm->query(rid);
        record.setRid(rid);
        ret->push_back();
    }
    delete entries;
    assert(1 == 0);
}

template <class Key>
void Table<Key>::scan_table () {
    printf("%u, %u\n", index->getLeafPageNum(), index->getTotalPageNum());
    // assert(1 == 0);
}

template <class Key>
void Table<Key>::statistics () {
    printf("%u, %d\n", index->getTotalPageNum(), dpm->getNumPages());
    // assert(1 == 0);
}

template <class Key>
void Table<Key>::printDataPages () {
    dpm->printAllPage();
    // assert(1 == 0);
}

template <class Key>
void Table<Key>::printDataPage (int pid) {
    dpm->print(pid);
    // assert(1 == 0);
}

template class Table<int>;
template class Table<KeyString>;
