#include "KeyString.h"

#include <cstring>

KeyString::KeyString() {
    memset(str, 0, KEY_STRING_LEN);
}
KeyString::KeyString(std::string s) {
    int len = KEY_STRING_LEN;
    if (s.length() < len) {
        len=s.length();
    }
    for (int i=0; i<len; i++) {
        str[i]=s[i];
    }
    for (int i=len; i<KEY_STRING_LEN; i++) {
        str[i]=0;
    }
}
KeyString KeyString::operator=(const KeyString rhs) {
    memcpy( str, rhs.str, KEY_STRING_LEN);
    return *this;
}
bool KeyString::operator>=(const KeyString right) {
    for(int i=0; i<KEY_STRING_LEN; i++) {
        if (this->str[i] < right.str[i]) return false;
    }
    return true;
}
bool KeyString::operator<(const KeyString right) {  return !(*this >= right);}
bool KeyString::operator==(const KeyString right) {
    for (int i=0; i<KEY_STRING_LEN; i++) {
        if (this->str[i] != right.str[i]) return false;
    }
    return true;
}
std::ostream& operator<<(std::ostream& os, const KeyString& ks) {
    os << ks.str ;
    return os;
}
