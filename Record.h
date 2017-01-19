#include <cstdint>
#include "DataPageManager.h"
#ifndef RECORD_H
#define RECORD_H

template <class Key>
class Record {
    public:
    Record(int _size, Key _key, char *_rest) {
        size = _size;
        key =_key;
        rest = _rest;
    }
    int getSize(){return size;}
    Key getKey(){return key;}
    char* getRest(){return rest;}
    uint32_t getRid();
    void setRid(rid _rid) {rid = *((uint32_t*)&rid);}
    private:
    int size;
    Key key;
    char *rest;
    uint32_t rid;
};

#endif
