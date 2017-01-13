#include <string>
#define KEY_STRING_LEN 10
class KeyString {
    public:
    char str[KEY_STRING_LEN];
    KeyString(std::string s);
    bool operator<=(const KeyString right);
    bool operator>(const KeyString right);
    bool operator==(const KeyString right);
};
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
inline bool KeyString::operator<=(const KeyString right) {
    for(int i=0; i<KEY_STRING_LEN; i++) {
        if (this->str[i] > right.str[i]) return false;
    }
    return true;
}
inline bool KeyString::operator>(const KeyString right) {  return !(*this <= right);}
inline bool KeyString::operator==(const KeyString right) {
    for (int i=0; i<KEY_STRING_LEN; i++) {
        if (this->str[i] != right.str[i]) return false;
    }
    return true;
}