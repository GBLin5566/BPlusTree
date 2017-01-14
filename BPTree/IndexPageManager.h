#include <vector>

#define INDEX_PAGE_POINTER_SIZE 4
typedef unsigned int ipg_pntr;

#ifndef INDEX_PAGE_T
#define INDEX_PAGE_T
#define INDEX_PAGE_SIZE 512
typedef struct {char data[INDEX_PAGE_SIZE];} page;
#endif

#ifndef INDEX_PAGE_MANAGER_H
#define INDEX_PAGE_MANAGER_H
class IndexPageManager {
    public:
    ipg_pntr new_page();
    page* get_page(ipg_pntr at);
    bool del_page(ipg_pntr at);
    private:
    std::vector<bool> isUsing;
    std::vector<page*> holder;
    ipg_pntr firstAvailable;
};
#endif