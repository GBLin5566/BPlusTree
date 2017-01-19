#include <string>
#include <iostream>

#ifndef KEY_STRING_H
#define KEY_STRING_H
#define KEY_STRING_LEN 10
class KeyString {
    public:
    unsigned char str[KEY_STRING_LEN];
    KeyString();
    KeyString(std::string s);
    KeyString operator=(const KeyString rhs);
    bool operator<(const KeyString right);
    bool operator>=(const KeyString right);
    bool operator==(const KeyString right);
    friend std::ostream& operator<<(std::ostream&, const KeyString&);
};
#endif
