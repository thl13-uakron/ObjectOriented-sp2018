/*
Date: 23 March 2018
Author: Thomas Li (thl13)
*/

#ifndef CARD_CPP
#define CARD_CPP

#include <iostream>
#include <assert.h>

/// enums for card data
enum Rank{Ace, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King};
enum Suit{Spades, Clubs, Hearts, Diamonds};
enum Color{Black, Red};
enum CardType{Standard, Joker};

/// card sub-types
class StandardCard
{
private:
    // members
    Rank rank;
    Suit suit;

public:
    // constructors
    StandardCard() = default;
    StandardCard(Rank r, Suit s) : rank(r), suit(s) {}
    StandardCard(const StandardCard& sc) : rank(sc.rank), suit(sc.suit) {}

    // accessors
    Rank getRank() {return rank;}
    Suit getSuit() {return suit;}
};

class JokerCard
{
private:
    // members
    Color color;

public:
    // constructors
    JokerCard() = default;
    JokerCard(Color c) : color(c) {}
    JokerCard(const JokerCard& jc) : color(jc.color) {}

    // accessors
    Color getColor() {return color;}
};

/// card implementations
// with discriminated unions
class UnionCard
{
private:
    // card data members
    StandardCard sc;
    JokerCard jc;

    CardType type; // determine which member is active

public:
    // constructors
    UnionCard() = default; // default
    UnionCard(UnionCard uc) : sc(uc.sc), jc(uc.jc), type(uc.type){} // copy
    UnionCard(StandardCard s) : sc(s), type(Standard) {} // standard
    UnionCard(Rank r, Suit s) : sc(r, s), type(Standard) {}
    UnionCard(JokerCard j) : jc(j), type(Joker) {} // joker
    UnionCard(Color c) : jc(c), type(Joker) {}

    // accessors:
    // card type
    CardType getType() {return type;}
    // standard card members
    StandardCard getStandardCard()
    {
        assert(getType() == Standard); // check if accessing correct (active) member
        return sc; // return
    }
    Rank getRank() {return getStandardCard().getRank();}
    Suit getSuit() {return getStandardCard().getSuit();}
    // joker card members
    JokerCard getJokerCard()
    {
        assert(getType() == Joker); // check if accessing correct (active) member
        return jc; // return
    }
    Color getColor() {return getJokerCard().getColor();}
};

// with bitfields
class BitCard
{
private:
    // members
    unsigned char cardData; // bitfield

    // bit accessors for indiv. members
    unsigned char rankBits() {return cardData & 0b00001111;} // bits 1-4 hold rank for standard card
    unsigned char suitBits() {return cardData & 0b00110000;} // bits 5-6 hold suit for standard card
    unsigned char colorBits() {return cardData & 0b01000000;} // bit 7 holds color for jokers
    unsigned char typeBits() {return cardData & 0b10000000;} // leading bit holds card type to determine which bits represent active members
public:
    // constructors
    BitCard() = default; // default
    BitCard(BitCard bc) : cardData(bc.cardData) {} // copy
    BitCard(StandardCard sc) : cardData(Standard << 7 | sc.suit << 4 | sc.rank) {} // standard card
    BitCard(Rank r, Suit s) : cardData(Standard << 7 | s << 4 | r) {} // standard card members
    BitCard(JokerCard jc) : cardData(Joker << 7 | jc.color << 6) {} // joker
    BitCard(Color c) : cardData(Joker << 7 | c << 6) {} // joker members

    // public accessors
};

// with inheritance hierarchies
class inheritanceCard
{
private:
    //
public:
    //
};

int main()
{
    // driver program
    return 0;
}

#endif // CARD_CPP
