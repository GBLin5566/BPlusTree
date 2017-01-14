
#include "TableIndex.h"

template <class Key>
TableIndex<Key>::TableIndex(const unsigned int _record_size, const char * const _table_name) {
    record_size = _record_size;
    table_name = _table_name;
}

/*
template <class Key>
TableIndex<Key>::TableIndex(const unsigned int _record_size, const char * const _table_name, DataPagesHandler * const _dph) {
    record_size = _record_size;
    table_name = _table_name;
    setDataPagesHandler(_dph);
}
*/
