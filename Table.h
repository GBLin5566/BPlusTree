#include <vector>
#include "Record.h"
#include "DataPageManager.h"

#ifndef TABLE_H
#define TABLE_H

template <class Key>
class Table {
    public:
    /* Constructers */
    Table(const unsigned int _record_size, const char * const _table_name);
    //TableIndex(const unsigned int _record_size, const char * const _table_name, DataPageManager * const _dpm);

    bool isIntType() {return sizeof(Key)==sizeof(int);}

    int insert_record(const Record<Key>);
    bool delete_by_key(const Key key);
    Record<Key> read_by_key(const Key key);
    std::vector< Record<Key> > read_by_key(const Key key1, const Key key2);
    void scan_table();
    int numLeafPages();
    int numIndexPages();

    private:
    unsigned int record_size;
    std::string table_name;
    DataPageManager<Key> *dpm;

};

#endif
