#include <vector>
#include <string>
#include "KeyString.h"
#include "Record.h"

template <class Key>
class TableIndex {
    public:
    TableIndex(unsigned int record_size, char *table_name);
    int insert_record(Record<Key>);
    int delete_by_key(Key key);
    Record<Key> read_by_key(Key key);
    std::vector< Record<Key> > read_by_key(Key key1, Key key2);
    void scan_table();
    int numLeafPages();
    int numIndexPages();

    private:
};
