#include <vector>
#include <map>
#include <stdint.h>
#include "Record.h"

#ifndef DATA_PAGE
#define DATA_PAGE
#define DATA_PAGE_SIZE 512
typedef struct {char data[DATA_PAGE_SIZE-sizeof(uint16_t)*2];uint16_t free_space_pointer;uint16_t number_of_slot;} data_page;
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
typedef struct {uint16_t page_id; uint16_t slot_number;} rid;
template <class Key>
class DataPageManager {
    public:
    DataPageManager();
    DataPageManager(int);
    // Public API
    void print(const int& pid);
    rid insert(Record<Key> r);
    bool del(rid id);
    int getNumPages();
    //
    void setRecordLen(int len);
    int getRecordLen();
    private:
    // In-mem. version.
    std::vector<data_page> page_table;
    std::map<Key, rid> map_for_index;
    bool enoughFreeSpace();
    int record_len;
};
#endif
