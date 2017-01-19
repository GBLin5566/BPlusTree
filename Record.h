#include <cstdint>
#include <cstring>
#ifndef RECORD_H
#define RECORD_H

template <class Key>
class Record {
    public:
    Record(int _size, Key _key, const char * const _rest);
    Record(const Record<Key> &rec);
    ~Record();
    int getSize();
    Key getKey();
    char* getRest();
    uint32_t getRid();
    void setRid(uint32_t _rid);
    private:
    int size;
    Key key;
    char *rest;
    uint32_t rid;
};

#endif
