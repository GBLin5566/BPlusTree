#include "Table.h"

#include "KeyString.h"
#include <cassert>

template <class Key>
Table<Key>::Table(const unsigned int _record_size, const char * const _table_name) {
    record_size = _record_size;
    table_name = _table_name;
    // TODO: DataPageManager
}

template <class Key>
int Table<Key>::insert_record (const Record<Key> key) {
    assert(1 == 0);
}

template <class Key>
bool Table<Key>::delete_by_key (const Key key) {
    assert(1 == 0);
}

template <class Key>
Record<Key> Table<Key>::read_by_key (const Key key) {
    assert(1 == 0);
}

template <class Key>
std::vector< Record<Key> > Table<Key>::read_by_key (const Key key1, const Key key2) {
    assert(1 == 0);
}

template <class Key>
void Table<Key>::scan_table () {
    assert(1 == 0);
}

template <class Key>
int Table<Key>::numLeafPages () {
    assert(1 == 0);
}

template <class Key>
int Table<Key>::numIndexPages () {
    assert(1 == 0);
}

template class Table<int>;
template class Table<KeyString>;