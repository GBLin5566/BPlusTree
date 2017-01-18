#include "TableIndex.h"

#include "KeyString.h"
#include <cassert>

template <class Key>
TableIndex<Key>::TableIndex(const unsigned int _record_size, const char * const _table_name) {
    record_size = _record_size;
    table_name = _table_name;
    // TODO: DataPageManager
}
template TableIndex<int>::TableIndex (const unsigned int, const char * const);
template TableIndex<KeyString>::TableIndex (const unsigned int, const char * const);

template <class Key>
int TableIndex<Key>::insert_record (const Record<Key> key) {
    assert(1 == 0);
}
template int TableIndex<int>::insert_record (const Record<int>);
template int TableIndex<KeyString>::insert_record (const Record<KeyString>);

template <class Key>
bool TableIndex<Key>::delete_by_key (const Key key) {
    assert(1 == 0);
}
template bool TableIndex<int>::delete_by_key (const int);
template bool TableIndex<KeyString>::delete_by_key (const KeyString);

template <class Key>
Record<Key> TableIndex<Key>::read_by_key (const Key key) {
    assert(1 == 0);
}
template Record<int> TableIndex<int>::read_by_key (const int);
template Record<KeyString> TableIndex<KeyString>::read_by_key (const KeyString);

template <class Key>
std::vector< Record<Key> > TableIndex<Key>::read_by_key (const Key key1, const Key key2) {
    assert(1 == 0);
}
template std::vector< Record<int> > TableIndex<int>::read_by_key (const int, const int);
template std::vector< Record<KeyString> > TableIndex<KeyString>::read_by_key (const KeyString, const KeyString);

template <class Key>
void TableIndex<Key>::scan_table () {
    assert(1 == 0);
}
template void TableIndex<int>::scan_table ();
template void TableIndex<KeyString>::scan_table ();

template <class Key>
int TableIndex<Key>::numLeafPages () {
    assert(1 == 0);
}
template int TableIndex<int>::numLeafPages ();
template int TableIndex<KeyString>::numLeafPages ();

template <class Key>
int TableIndex<Key>::numIndexPages () {
    assert(1 == 0);
}
template int TableIndex<int>::numLeafPages ();
template int TableIndex<KeyString>::numLeafPages ();