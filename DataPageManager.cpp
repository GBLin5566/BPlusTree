#include <string>
#include <iostream>
#include <assert.h> 
#include "DataPageManager.h"
#include "KeyString.h"

#ifndef DPM_DETAIL
//#define DPM_DETAIL
#endif

#ifndef DPM_TEST
#define DPM_TEST
#endif

typedef struct {uint16_t offset; uint16_t reclen;} slot;

template<class Key>
DataPageManager<Key>::DataPageManager() {
    record_len = 0;
};

template<class Key>
DataPageManager<Key>::DataPageManager(int len) {
    record_len = len;
};

template<class Key>
int DataPageManager<Key>::getNumPages() {
    return this->page_table.size();
};

template<class Key>
int DataPageManager<Key>::getRecordLen() {
    return this->record_len;
};

template<class Key>
bool DataPageManager<Key>::del(const rid& id) {
    try {
        const uint16_t page_id = id.page_id;
        const uint16_t slot_number = id.slot_number;
        // Deleting slot not exists
        if (this->getNumPages() <= page_id)
            return false;
        if (this->page_table[page_id].number_of_slot <= slot_number)
            return false;
        uint16_t deleted_msg = UINT16_MAX;
        data_page* target_page = &this->page_table[page_id];
        uint16_t offset_for_slot = DATA_PAGE_SIZE - 1 - 2 * sizeof(uint16_t);
        std::cout << "[DBM] Deleting page id " << page_id << ", slot_number " << slot_number << std::endl;
        memcpy(target_page->data + (DATA_PAGE_SIZE - 1 - 2*(sizeof(uint16_t))) - (slot_number + 1) * (sizeof(id.page_id) + sizeof(id.slot_number)), 
                &deleted_msg, sizeof(deleted_msg));
        return true;
    }
    catch(...) {
        return false;
    }
};

template<class Key>
char* DataPageManager<Key>::query(const rid& id) {
    const uint16_t page_id = id.page_id;
    const uint16_t slot_number = id.slot_number;

    data_page* target_page = &this->page_table[page_id];
    uint16_t offset_for_slot = DATA_PAGE_SIZE - 1 - 2 * sizeof(uint16_t);
    std::cout << "[DBM] Querying page id " << page_id << ", slot_number " << slot_number << std::endl;
    slot target_slot;
    memcpy(&target_slot.offset, target_page->data + (DATA_PAGE_SIZE - 1 - 2*(sizeof(uint16_t))) - (slot_number + 1) * (sizeof(id.page_id) + sizeof(id.slot_number)), sizeof(target_slot.offset));
    memcpy(&target_slot.reclen, target_page->data + (DATA_PAGE_SIZE - 1 - 2*(sizeof(uint16_t))) - (slot_number + 1) * (sizeof(id.page_id) + sizeof(id.slot_number)) + sizeof(target_slot.offset), sizeof(target_slot.reclen));
    if (target_slot.offset == UINT16_MAX) {
        std::cout << "[DBM] *ERROR* Querying a deleted rid.\n";
        return NULL;
    }
    char* return_rest = new char [target_slot.reclen];
    memcpy(return_rest, target_page->data + target_slot.offset, target_slot.reclen);
    std::cout << "[DBM] Query result, rest = " << return_rest << std::endl;
    return return_rest;
}

template<class Key>
bool DataPageManager<Key>::enoughFreeSpace() {
    if (this->getNumPages() == 0)
        return false;
    data_page* last_page = &this->page_table[this->getNumPages()-1];

    slot slot_instance;
    int slot_size = sizeof(slot_instance.offset) + sizeof(slot_instance.reclen);
    // All Page = DATA_PAGE_SIZE
    // free_space_pointer & number_of_slot = sizeof(uint16_t) * 2
    // Remain size (all - used) = minus last_page.free_space_pointer
    // Already used size for slots = last_page.number_of_slot * slot_size
    // and the remain size should be larger than record size + one slot = this->getRecordLen() + slot_size
    if (DATA_PAGE_SIZE - sizeof(uint16_t) * 2 
            - last_page->free_space_pointer - last_page->number_of_slot * slot_size >= this->getRecordLen() + slot_size) {
        return true;
    }
    return false;
}

template<class Key>
void DataPageManager<Key>::print(const int & pid) {
    assert(pid < this->getNumPages());
    std::cout << "[DPM] Printing Page Id " << pid << std::endl;
    data_page *target_page = &this->page_table[pid];
    std::cout << "[DPM] Page " << pid << " has " << target_page->number_of_slot << " slots." << std::endl;
    if (target_page->number_of_slot == 0)
        return ;
    uint16_t offset_for_slot = DATA_PAGE_SIZE - 1 - 2 * sizeof(uint16_t);
    slot slot_instance;
    int slot_size = sizeof(slot_instance.offset) + sizeof(slot_instance.reclen);
    for (uint16_t i = 0 ; i < target_page->number_of_slot ; i ++) {
        uint16_t offset;
        memcpy(&offset, target_page->data + offset_for_slot - (i + 1) * slot_size, sizeof(offset));
#ifdef DPM_DETAIL
        std::cout << "    [DPM_DETAIL] Read offset at " << offset_for_slot - (i + 1) * slot_size << std::endl;
        std::cout << "    [DPM_DETAIL] Read out offset " << offset << std::endl;
#endif
        if (offset == UINT16_MAX) // Deleted
            continue;
        char* rest = new char [this->getRecordLen()];
        memcpy(rest, target_page->data + offset, this->getRecordLen());
        std::cout << "[DPM] Pid " << pid << " Slot " << i << " , rest = " << rest << std::endl;
        delete rest;
    }
}

template<class Key>
void DataPageManager<Key>::printAllPage() {
    for (int i = 0 ; i < this->getNumPages() ; i ++)
        this->print(i);
}

template<class Key>
rid DataPageManager<Key>::insert(Record<Key> r) {
//    assert(r.getSize() == this->getRecordLen());
    std::string t(r.getRest());
    assert(t.length() < this->getRecordLen());
    try {
        std::cout << "[DPM] Insert a record, size(" << r.getSize() << "), key(" << r.getKey() << "), rest(" << r.getRest() << ").\n" ; 
        // Have enough space
        if (this->enoughFreeSpace()) {
#ifdef DPM_DETAIL
            std::cout << "[DPM_DETAIL] Enough space in page " << this->getNumPages() - 1 << std::endl;
#endif
            data_page* last_page = &this->page_table[this->getNumPages()-1];
            // Insert the record
#ifdef DPM_DETAIL
            std::cout << "    [DPM_DETAIL] Insert " << r.getRest() << " at " << last_page->free_space_pointer << std::endl;
#endif
            memcpy(last_page->data + last_page->free_space_pointer, r.getRest(), this->getRecordLen());
            last_page->free_space_pointer += this->getRecordLen();
            // Insert the slot
            slot slot_instance;
            slot_instance.offset = last_page->free_space_pointer - this->getRecordLen();
            slot_instance.reclen = this->getRecordLen();
#ifdef DPM_DETAIL
            std::cout << "    [DPM_DETAIL] Slot info offset(" << slot_instance.offset << "), reclen(" << slot_instance.reclen << ").\n";
#endif
            int slot_size = sizeof(slot_instance.offset) + sizeof(slot_instance.reclen);
            // (DATA_PAGE_SIZE - 1 - 2*(sizeof(uint16_t))) = all size without free_space_pointer & number_of_slot
            // save offset first and reclen second
            memcpy(last_page->data + (DATA_PAGE_SIZE - 1 - 2*(sizeof(uint16_t))) - last_page->number_of_slot * slot_size - slot_size, &slot_instance.offset, sizeof(slot_instance.offset));
            memcpy(last_page->data + (DATA_PAGE_SIZE - 1 - 2*(sizeof(uint16_t))) - last_page->number_of_slot * slot_size - slot_size + sizeof(slot_instance.offset), &slot_instance.reclen, sizeof(slot_instance.reclen));
#ifdef DPM_DETAIL
            std::cout << "    [DPM_DETAIL] Save slot.offset at " << (DATA_PAGE_SIZE - 1 - 2*(sizeof(uint16_t))) - last_page->number_of_slot * slot_size - slot_size << std::endl;
            std::cout << "    [DPM_DETAIL] Save slot.reclen at " << (DATA_PAGE_SIZE - 1 - 2*(sizeof(uint16_t))) - last_page->number_of_slot * slot_size - slot_size + sizeof(slot_instance.offset) << std::endl;
#endif
            last_page->number_of_slot += 1;
            // New RID
            rid new_rid = {.page_id = this->getNumPages() - 1, .slot_number = last_page->number_of_slot - 1};
            // Set a hash
//            this->map_for_index[r.getKey()] = *new_rid;
            return new_rid;
        } 
        else {
#ifdef DPM_DETAIL
            std::cout << "    [DPM_DETAIL] Create a new page.\n";
#endif
            // Init. page
            data_page* new_page = new data_page;
            new_page->free_space_pointer = 0;
            new_page->number_of_slot = 0;
#ifdef DPM_DETAIL
            std::cout << "    [DPM_DETAIL] Insert " << r.getRest() << " at " << new_page->free_space_pointer << std::endl;
#endif
            // Insert the record
            memset(new_page->data, '\0', sizeof(new_page->data));
            memcpy(new_page->data, r.getRest(), this->getRecordLen());
            new_page->free_space_pointer += this->getRecordLen();
            // Insert the slot
            slot slot_instance;
            slot_instance.offset = 0;
            slot_instance.reclen = this->getRecordLen();
#ifdef DPM_DETAIL
            std::cout << "    [DPM_DETAIL] Slot info offset(" << slot_instance.offset << "), reclen(" << slot_instance.reclen << ").\n";
#endif
            int slot_size = sizeof(slot_instance.offset) + sizeof(slot_instance.reclen);
            memcpy(new_page->data + (DATA_PAGE_SIZE - 1 - 2*(sizeof(uint16_t))) - slot_size, &slot_instance.offset, sizeof(slot_instance.offset));
            memcpy(new_page->data + (DATA_PAGE_SIZE - 1 - 2*(sizeof(uint16_t))) - slot_size + sizeof(slot_instance.offset), &slot_instance.reclen, sizeof(slot_instance.reclen));
#ifdef DPM_DETAIL
            std::cout << "    [DPM_DETAIL] Save slot.offset at " << (DATA_PAGE_SIZE - 1 - 2*(sizeof(uint16_t))) - slot_size << std::endl;
            std::cout << "    [DPM_DETAIL] Save slot.reclen at " << (DATA_PAGE_SIZE - 1 - 2*(sizeof(uint16_t))) - slot_size + sizeof(slot_instance.offset) << std::endl;
#endif
            new_page->number_of_slot += 1;
            // Push the new_page
            this->page_table.push_back(*new_page);
            // New RID
            rid new_rid = {.page_id = this->getNumPages() - 1, .slot_number = 0};
            // Set a hash key -> rid
//            this->map_for_index[r.getKey()] = *new_rid;
            return new_rid;
        }
    }
    catch (...) {
        rid error;
        error.page_id = UINT16_MAX;
        error.slot_number = UINT16_MAX;
        return error;
    }
};

template class DataPageManager<int>;
template class DataPageManager<KeyString>;

// #ifdef DPM_TEST
// // Testing
// int main() {
//     Record<int> r1(90, 10, "Kappa10");
//     Record<int> r2(90, 20, "Kappa20");
//     Record<int> r3(90, 30, "Kappa30");
//     Record<int> r4(90, 120, "Kappa120");
//     Record<int> r5(90, 1, "Kappa1");
//     Record<int> r6(90, 2, "Kappa2");
//     Record<int> r7(90, 3, "Kappa3");
//     Record<int> r8(90, 4, "Kappa4");
//     DataPageManager<int> test_manager(90);
//     rid insert_r1 = test_manager.insert(r1);
//     rid insert_r2 = test_manager.insert(r2);
//     rid insert_r3 = test_manager.insert(r3);
//     rid insert_r4 = test_manager.insert(r4);
//     rid insert_r5 = test_manager.insert(r5);
//     rid insert_r6 = test_manager.insert(r6);
//     rid insert_r7 = test_manager.insert(r7);
//     rid insert_r8 = test_manager.insert(r8);
//     char* rest1 = test_manager.query(insert_r6);
//     char* rest2 = test_manager.query(insert_r5);
//     test_manager.del(insert_r5);
//     test_manager.printAllPage();
//     char* rest3 = test_manager.query(insert_r5);
//     /*
//     char* tmp = "...";
//     char buffer[1000];
//     //memset(buffer, '\0', sizeof(buffer));
//     memcpy(buffer, tmp, 4);
//     char tmp2[4] = {0};
//     memcpy(tmp2, buffer, 4);
//     std::cout << tmp2 << std::endl;
//     */
//     return 0 ;
// }
// #endif
