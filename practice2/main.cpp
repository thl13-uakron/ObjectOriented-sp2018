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
    String() {} // default
    String(const char* c2) : c(new char(*c2)) {} // c-string
    String(const String& s) : c(new char(*s.c)) {} // copy

    // operator overloads (assignment and modification)
    String operator+(const String& s) {return strcat(c, s.c);}
    void operator=(const String& s) {strcpy(s.c, c);}

    // operator overloads (comparison)
    bool operator==(const String& s);
    bool operator!=(const String& s);
    bool operator>(const String& s);
    bool operator>=(const String& s);
    bool operator<(const String& s);
    bool operator<=(const String& s);

    // operator overloads (input/output)
    std::istream operator>>(std::istream&);
    std::ostream operator<<(std::ostream&);

    // basic member functions
    int len() {return strlen(c);}
};

int main()
{
    return 0;
}
