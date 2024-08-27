#include <iostream>
#include <cstdint>
#include <vector>


int main()
{
    float f = 2.34f;
    // Gives warning for narrowing conversion
    uint8_t b{f};

    // Calling the constructor with {} insted of () does not always call the same constructor
    // It can call the initializer_list constructor
    std::string s1(10,'a'); // aaaaaaaaaa
    std::string s2{10,'a'}; // Line Feed (ASCII 10) and an 'a' character

    std::cout << "string s1(10,'a'): " << s1 << " len: " << s1.length() << '\n';
    std::cout << "string s2{10,'a'}: " << s2 << " len: " << s2.length() << '\n';
 
    std::cout << '\n';

    std::vector<uint16_t> v1(10);
    std::vector<uint16_t> v2{10};

    std::cout << "vector v1(10), len: " << v1.size() << '\n';
    std::cout << "vector v2{10}, len: " << v2.size() << '\n';

    return 0;
}
