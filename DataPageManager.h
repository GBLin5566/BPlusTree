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
// read(pid)
//
// cheat sheet
// class Record {
//      int size;
//      Key key;
//      char *rest;
// };
typedef struct {uint16_t page_id; uint16_t slot_number;} rid, *rid_ptr;
template <class Key>
class DataPageManager {
    public:
    DataPageManager();
    DataPageManager(int record_len);
    // Public API
    void print(const int& pid);
    void printAllPage();
    rid insert(Record<Key> record);
    bool del(const rid& record_id);
    char* query(const rid& record_id);
    int getNumPages();
    //
    void setRecordLen(int record_len);
    int getRecordLen();
    private:
    // In-mem. version.
    std::vector<data_page> page_table;
    bool enoughFreeSpace();
    int record_len;
};
#endif
