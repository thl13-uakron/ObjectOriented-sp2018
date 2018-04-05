/*
Date: 5 April 2018
Author: Thomas Li (thl13)
Purpose: Simulate Javascript Object Notation in C++
*/

#include <iostream>
#include <string>
#include <vector>
#include <map>

/// data types
struct Var // base class (empty)
{
    // virtual function for data output
    virtual void print() = 0;
};
struct Null : Var // null type
{
    // constructors
    Null() = default;

    // virtual overload - display value
    void print() {std::cout << "NULL";}
};
struct Num : Var // number type
{
    int val; // data

    // virtual overload - display value
    void print() {std::cout << val;}
};
struct Bool : Var // boolean type
{
    bool val; // data

    // virtual overload - display value
    void print() {std::cout << val;}
};
struct String : Var // string type
{
    std::string val; // data

    // virtual overload - display value
    void print() {std::cout << val;}
};

/// array and object types
// counts as static type var for recursion
struct Array : Var // ordered container
{
    std::vector<Var*> arr; // data
    // vector with static type Var, possibly including other Arrays and Objects

    // virtual overload - display contents
    void print()
    {
        std::cout << "["; // formatting

        // print each element
        for (Var* i : arr)
        {
            i->print();
            std::cout << ", ";
        }

        std::cout << "]"; // formatting
    }
};
struct Object : Var // unordered hashtable
{
    std::map<Var*, Var*> obj; // data
    // map with static type Var, possibly including other Arrays and Objects

    // virtual overload - display contents
    void print()
    {
        std::cout << "{"; // formatting

        // print each key and element
        for (std::pair<Var*, Var*> i : obj)
        {
            i.first->print();
            std::cout << " : ";
            i.second->print();

            std::cout << ", ";
        }
        std::cout << "}";
    }
};

#include <iostream>

int main()
{
    //
    return 0;
}
