
#ifndef RECORD_H
#define RECORD_H

template <class Key>
class Record {
    public:
    int getSize(){return size;}
    Key getKey(){return key;}
    char* getRest(){return rest;}
    private:
    int size;
    Key key;
    char *rest;
};

#endif
