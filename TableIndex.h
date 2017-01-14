#include <vector>
#include <string>
#include "KeyString.h"
#include "Record.h"

template <class Key>
class TableIndex {
    public:
    /* Constructers */
    TableIndex(const unsigned int _record_size, const char * const _table_name);
    //TableIndex(const unsigned int _record_size, const char * const _table_name, DataPagesHandler * const _dph);

    /* Setter */
    //void setDataPagesHandler(DataPagesHandler * const _dph) {}

    int insert_record(const Record<Key>);
    bool delete_by_key(const Key key);
    Record<Key> read_by_key(const Key key);
    std::vector< Record<Key> > read_by_key(const Key key1, const Key key2);
    void scan_table();
    int numLeafPages();
    int numIndexPages();

    private:
    unsigned int record_size;
    char *table_name;
    //DataPagesHandler *dph;

};
