#include "IndexPageManager.h"
#include "../KeyString.h"

#ifndef BP_ENTRY_H
#define BP_ENTRY_H

#pragma pack(push)
#pragma pack(1)
template <class Key>
struct BPEntry {
    BPEntry();
    Key key;
    ipg_pntr value;
};
#pragma pack(pop)

#endif