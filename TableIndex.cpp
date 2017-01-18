#include "TableIndex.h"

#include "KeyString.h"
#include <cassert>

template <class Key>
TableIndex<Key>::TableIndex(const unsigned int _record_size, const char * const _table_name) {
    record_size = _record_size;
    table_name = _table_name;
    // TODO: DataPageManager
}

template <class Key>
int TableIndex<Key>::insert_record (const Record<Key> key) {
    assert(1 == 0);
}

template <class Key>
bool TableIndex<Key>::delete_by_key (const Key key) {
    assert(1 == 0);
}

template <class Key>
Record<Key> TableIndex<Key>::read_by_key (const Key key) {
    assert(1 == 0);
}

template <class Key>
std::vector< Record<Key> > TableIndex<Key>::read_by_key (const Key key1, const Key key2) {
    assert(1 == 0);
}

template <class Key>
void TableIndex<Key>::scan_table () {
    assert(1 == 0);
}

template <class Key>
int TableIndex<Key>::numLeafPages () {
    assert(1 == 0);
}

template <class Key>
int TableIndex<Key>::numIndexPages () {
    assert(1 == 0);
}

template class TableIndex<int>;
template class TableIndex<KeyString>;