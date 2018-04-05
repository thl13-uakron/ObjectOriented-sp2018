/*
Date: 5 April 2018
Author: Thomas Li (thl13)
Purpose: Simulate Javascript Object Notation in C++
*/

/// data types
struct Var // base class (empty)
{
    // Var() = default;
};
struct Null : Var // null type
{
    //
};
struct Num : Var // number type
{
    //
};
struct Bool : Var // boolean type
{
    //
};
struct String : Var // string type
{
    //
};

/// array and object types
struct Array // ordered container
{
    //
};
struct Object // unordered hashtable
{
    //
};

#include <iostream>

int main()
{
    //
    return 0;
}
