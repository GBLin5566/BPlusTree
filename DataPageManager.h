#include <vector>
#include <stdint.h>
#include "Record.h"

#ifndef DATA_PAGE
#define DATA_PAGE
#define DATA_PAGE_SIZE 512
typedef struct {char data[DATA_PAGE_SIZE];} data_page;
#endif

#ifndef DATA_PAGE_MANAGER
#define DATA_PAGE_MANAGER
// TODO
// print(pid)
// insert(Record)
// delete(rid)
// read(pid)
// numPages()
//
// cheat sheet
// class Record {
//      int size;
//      Key key;
//      char *rest;
// };
typedef struct {uint8_t page_id; uint8_t slot_number;} rid;
template <class Key>
class DataPageManager {
    public:
    DataPageManager();
    void print(uint8_t pid);
    bool insert(Record<Key> r);
    void setRecordLen(uint8_t len);
    void getNumPages();
    private:
    // In-mem. version.
    std::vector<data_page> page_table;
    uint8_t record_len;
};
#endif
