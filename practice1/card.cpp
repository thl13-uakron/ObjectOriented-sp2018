#ifndef CARD_CPP
#define CARD_CPP

#include <iostream>
#include "card.hpp"

/// constructors

Card::Card(int suit, int value)
{
    suitID = suit;
    valueID = value;
}

Card::Card(std::string suit, std::string value)
{
    suitID = ::getSuitInternal(suit);
    valueID = ::getValueInternal(value);
}

/// score getters (not implemented yet)

#endif // CARD_CPP
