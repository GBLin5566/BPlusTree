#include <string>
#include <iostream>
#include <assert.h> 
#include "DataPageManager.h"

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
bool DataPageManager<Key>::enoughFreeSpace() {
    if (this->getNumPages() == 0)
        return false;
    data_page last_page = this->page_table[this->getNumPages()-1];
    slot slot_instance;
    int slot_size = sizeof(slot_instance.offset) + sizeof(slot_instance.reclen);
    // All Page = DATA_PAGE_SIZE
    // free_space_pointer & number_of_slot = sizeof(uint16_t) * 2
    // Remain size (all - used) = minus last_page.free_space_pointer
    // Already used size for slots = last_page.number_of_slot * slot_size
    // and the remain size should be larger than record size + one slot = this->getRecordLen() + slot_size
    if (DATA_PAGE_SIZE - sizeof(uint16_t) * 2 
            - last_page.free_space_pointer - last_page.number_of_slot * slot_size >= this->getRecordLen() + slot_size) {
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
        memcpy(&offset, target_page->data + offset_for_slot - i * slot_size, sizeof(offset));
        if (offset == UINT16_MAX) // Deleted
            continue;
        char* rest;
        memcpy(rest, target_page->data + offset, this->getRecordLen());
        std::cout << "[DPM] Pid " << pid << " Slot " << i << " , rest = " << rest << std::endl;
    }
}

template<class Key>
rid DataPageManager<Key>::insert(Record<Key> r) {
    assert(r.getSize() == this->getRecordLen());
    try {
        // Have enough space
        if (this->enoughFreeSpace()) {
            data_page* last_page = &this->page_table[this->getNumPages()-1];
            // Insert the record
            memcpy(last_page->data + last_page->free_space_pointer, r.getRest(), this->getRecordLen());
            last_page->free_space_pointer += this->getRecordLen();
            // Insert the slot
            slot slot_instance;
            slot_instance.offset = last_page->free_space_pointer - this->getRecordLen();
            slot_instance.reclen = this->getRecordLen();
            int slot_size = sizeof(slot_instance.offset) + sizeof(slot_instance.reclen);
            // (DATA_PAGE_SIZE - 1 - 2*(sizeof(uint16_t))) = all size without free_space_pointer & number_of_slot
            // save offset first and reclen second
            memcpy(last_page->data + (DATA_PAGE_SIZE - 1 - 2*(sizeof(uint16_t))) - last_page->number_of_slot * slot_size - slot_size, &slot_instance.offset, sizeof(slot_instance.offset));
            memcpy(last_page->data + (DATA_PAGE_SIZE - 1 - 2*(sizeof(uint16_t))) - last_page->number_of_slot * slot_size - slot_size + sizeof(slot_instance.offset), &slot_instance.reclen, sizeof(slot_instance.reclen));
            last_page->number_of_slot += 1;
            // New RID
            rid* new_rid = new rid;
            new_rid->slot_number = last_page->number_of_slot - 1;
            new_rid->page_id = this->getNumPages() - 1;
            // Set a hash
            this->map_for_index[r.getKey()] = *new_rid;
            return *new_rid;
        } 
        else {
            // Init. page
            data_page* new_page = new data_page;
            new_page->free_space_pointer = 0;
            new_page->number_of_slot = 0;
            // Insert the record
            memset(new_page->data, '\0', sizeof(new_page->data));
            memcpy(new_page->data, r.getRest(), this->getRecordLen());
            new_page->free_space_pointer += this->getRecordLen();
            // Insert the slot
            slot slot_instance;
            slot_instance.offset = 0;
            slot_instance.reclen = this->getRecordLen();
            int slot_size = sizeof(slot_instance.offset) + sizeof(slot_instance.reclen);
            memcpy(new_page->data + (DATA_PAGE_SIZE - 1 - 2*(sizeof(uint16_t))) - slot_size, &slot_instance.offset, sizeof(slot_instance.offset));
            memcpy(new_page->data + (DATA_PAGE_SIZE - 1 - 2*(sizeof(uint16_t))) - slot_size + sizeof(slot_instance.offset), &slot_instance.reclen, sizeof(slot_instance.reclen));
            new_page->number_of_slot += 1;
            // Push the new_page
            this->page_table.push_back(*new_page);
            // New RID
            rid* new_rid = new rid;
            new_rid->slot_number = 0;
            new_rid->page_id = this->getNumPages() - 1;
            // Set a hash key -> rid
            this->map_for_index[r.getKey()] = *new_rid;
            return *new_rid;
        }
    }
    catch (...) {
        rid error;
        error.page_id = UINT16_MAX;
        error.slot_number = UINT16_MAX;
        return error;
    }
};

// Testing
int main() {
    DataPageManager<int> test_manager;
    uint16_t tmp = 1001;
    char buffer[100];
    memset(buffer, '\0', sizeof(buffer));
    memcpy(buffer, &tmp,sizeof(uint16_t));
    uint16_t tmp2 = 0;
    memcpy(&tmp2, buffer, sizeof(uint16_t));
    std::cout << tmp2 << std::endl;
    return 0 ;
}
