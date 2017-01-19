#include <cstdint>
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
    void setRid(uint32_t _rid) {rid = _rid;}
    private:
    int size;
    Key key;
    char *rest;
    uint32_t rid;
};

#endif
