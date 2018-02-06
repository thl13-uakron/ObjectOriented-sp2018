/*
Date: 2 February 2018
Author: Thomas Li (thl13)
Purpose: Re-write a copy of the standard string class from scratch
*/

#include <iostream>
#include <cstring>
#include <istream>
#include <ostream>

class String
{
private:
    char* c; // c-string that contains the actual contents of the string
public:
    // constructors
    String() : c(new char) {} // default
    String(const char* c2) : c(new char(*c2)) {} // c-string
    String(const String& s) : c(new char(*s.c)) {} // copy
    ~String() {delete c;} // destructor

    // operator overloads (assignment and modification)
    String operator+(const String& s) {return strcat(c, s.c);}
    void operator=(const String& s) {*c = *s.c;}
    void operator=(const char* c2) {*c = *c2;}

    // operator overloads (comparison)
    bool operator==(const String& s) {return *c == *s.c;}
    bool operator!=(const String& s) {return !(*c == *s.c);}
    bool operator>(const String& s) {return *c > *s.c;}
    bool operator>=(const String& s) {return *c >= *s.c;}
    bool operator<(const String& s) {return *c < *s.c;}
    bool operator<=(const String& s) {return *c <= *s.c;}

    // operator overloads (input/output)
    //std::istream operator>>(std::istream& i) {i >> *c; return i;}
    //std::ostream operator<<(std::ostream& o) {o << *c; return o;}

    // basic member functions
    int length() {return strlen(c);}
    char at(int location) {return c[location];}
};

int main()
{
    /// copy-pasted driver program

    // Default construct
    String s0;

    // Construct and assign from a literal value
    String s1 = "hello";
    s1 = "foo";

    // Copy construct and assign
    String s2 = s1;
    s2 = s1; // s2 == s1

    // Move construction and assignment (optional)
    String s3 = std::move(s2);
    s3 = std::move(s1);

    // Compare
    std::cout << (s1 == s1);

    // Order
    std::cout << (s1 < s1); // lexicographical comparison

    // Object independence
    s2 = "bar"; // Does not change the value of s1
    std::cout << (s1 == s2); // check

    return 0;
}
