#include "Record.h"
#include "KeyString.h"
#include <string>
int main() {
    int record_size = 123;
    std::string str = "123";
    std::string rest = "456";
    Record<int> r(123, std::stoi("456"), str.c_str());
    Record<int> f(123, std::stoi("456"), "345");
    printf("%u, %u, %s\n", r.getSize(), r.getKey(), r.getRest());
    printf("%u, %u, %s\n", f.getSize(), f.getKey(), f.getRest());
    printf("%llu\n", sizeof(KeyString));
}