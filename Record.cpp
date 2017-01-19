#include "Record.h"

#include <cstdint>
#include <cstring>
#include <cstdlib>

#include "KeyString.h"

template <class Key>
Record<Key>::Record(int _size, Key _key, const char * const _rest) {
    size = _size;
    key = _key;
    rest = NULL;
    if (_rest) {
        rest = (char*) malloc(strlen(_rest)+1);
        strcpy(rest, _rest);
    }
}

template <class Key>
Record<Key>::Record(const Record<Key> &rec) {
    size = rec.size;
    key = rec.key;
    rest = (char*) malloc(strlen(rec.rest)+1);
    strcpy(rest, rec.rest);
}

template <class Key>
Record<Key>::~Record() {if (rest)free(rest);}

template <class Key>
int Record<Key>::getSize(){return size;}

template <class Key>
Key Record<Key>::getKey(){return key;}

template <class Key>
char* Record<Key>::getRest(){return rest;}

template <class Key>
uint32_t Record<Key>::getRid(){return rid;}

template <class Key>
void Record<Key>::setRid(uint32_t _rid) {rid = _rid;}

template class Record<int>;
template class Record<KeyString>;
