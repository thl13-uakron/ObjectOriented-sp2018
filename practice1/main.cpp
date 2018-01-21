#include <iostream>
#include "card.hpp"

/*
Card class specifications:
- Two members, representing the suit of the card and its value
- Both are internally stored as ints (IDs) but are abstracted as string through constant arrays in the unnamed namespace
- Three constructors: a default, one that initializes the members with ints, and one that initializes the members with strings
- Getter functions that return both the abstracted and internal members
- Another getter returns the abstracted card name as a string (ace of spades, queen of hearts, etc)
- For purposes of games, additional getters return abstracted "scores" of cards that vary between games (not implemented yet)
- Setter functions modify internal members based on either internal or abstracted values as parameters
*/

int main()
{
    std::cout << "asdf\n";
}
