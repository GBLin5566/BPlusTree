#include <vector>
#include <cstdint>
#include <stdint.h>
typedef uint32_t ipg_pntr;
#define INDEX_PAGE_INVALID UINT32_MAX
#define INDEX_PAGE_POINTER_SIZE sizeof(ipg_pntr)

#ifndef INDEX_PAGE_T
#define INDEX_PAGE_T
#define INDEX_PAGE_SIZE 512
typedef struct {char data[INDEX_PAGE_SIZE];} page;
#endif

#ifndef INDEX_PAGE_MANAGER_H
#define INDEX_PAGE_MANAGER_H
typedef struct {bool isUsing; page* pointer;} ptEntry;
class IndexPageManager {
    public:
    IndexPageManager();
    ipg_pntr new_page();
    page* get_page(ipg_pntr at);
    bool del_page(ipg_pntr at);
    private:
    std::vector<ptEntry> table;
    ipg_pntr firstAvailable;
};
#endif
