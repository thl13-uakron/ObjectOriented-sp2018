/*
Date: 23 March 2018
Author: Thomas Li (thl13)
*/

#ifndef CARD_CPP
#define CARD_CPP

#include <iostream>
#include <assert.h>
#include <vector>

/// enums for card data
enum Rank{Ace, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King};
enum Suit{Spades, Clubs, Hearts, Diamonds};
enum Color{Black, Red};
enum CardType{Standard, Joker};

/// ostream operator overloads for enum output (copy-pasted from previous program)
std::ostream& operator<<(std::ostream& os, Suit s)
{
    switch(s)
    {
        case Spades : os << "Spades";    break;
        case Clubs : os << "Clubs"; break;
        case Hearts : os << "Hearts";  break;
        case Diamonds : os << "Diamonds";   break;
        default    : os.setstate(std::ios_base::failbit);
    }
    return os;
}
std::ostream& operator<<(std::ostream& os, Rank r)
{
    switch(r)
    {
        case Ace   : os << "Ace";    break;
        case Two : os << "Two"; break;
        case Three : os << "Three";  break;
        case Four : os << "Four";   break;
        case Five  : os << "Five";    break;
        case Six : os << "Six"; break;
        case Seven : os << "Seven";  break;
        case Eight : os << "Eight";   break;
        case Nine   : os << "Nine";    break;
        case Ten : os << "Ten"; break;
        case Jack : os << "Jack";  break;
        case Queen : os << "Queen";   break;
        case King  : os << "King";    break;
        default    : os.setstate(std::ios_base::failbit);
    }
    return os;
}
std::ostream& operator<<(std::ostream& os, Color c)
{
    switch(c)
    {
        case Red: os << "Red"; break;
        case Black: os << "Black"; break;
    }
    return os;
}

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
    UnionCard(const UnionCard& uc) : sc(uc.sc), jc(uc.jc), type(uc.type){} // copy
    UnionCard(StandardCard& s) : sc(s), type(Standard) {} // standard
    UnionCard(Rank r, Suit s) : sc(r, s), type(Standard) {}
    UnionCard(JokerCard& j) : jc(j), type(Joker) {} // joker
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
    BitCard(const BitCard& bc) : cardData(bc.cardData) {} // copy
    BitCard(StandardCard& sc) : cardData(Standard << 7 | sc.getSuit() << 4 | sc.getRank()) {} // standard card
    BitCard(Rank r, Suit s) : cardData(Standard << 7 | s << 4 | r) {} // standard card members
    BitCard(JokerCard& jc) : cardData(Joker << 7 | jc.getColor() << 6) {} // joker
    BitCard(Color c) : cardData(Joker << 7 | c << 6) {} // joker members

    // public accessors
    CardType getType()
    {return static_cast<CardType>(typeBits());}

    Color getColor() {assert(getType() == Joker);
    return static_cast<Color>(colorBits());}

    Suit getSuit() {assert(getType() == Standard);
    return static_cast<Suit>(suitBits());}

    Rank getRank() {assert(getType() == Standard);
    return static_cast<Rank>(rankBits());}
};

// with inheritance hierarchies
class InheritanceCard // parent class (static type)
{
private:
    // member data
    // CardType type;
public:
    virtual InheritanceCard* clone() = 0; // virtual function returns copy of card

    // constructors
    InheritanceCard() = default; // default
    // InheritanceCard(const InheritanceCard& ic) = ic.clone(); // copy
    // InheritanceCard(InheritanceSC isc) // standard
    // InheritanceCard(Rank r, Suit s) = 0;
    // InhertianceCard(InheritanceJC ijc) // joker
    // InheritanceCard(Color c)

    // CardType getType() {return type;} // public accessors
};
class InheritanceSC : InheritanceCard // sub-classes (dynamic types)
{
private:
    // mmeber data
    Rank rank;
    Suit suit;
public:
    // constructors
    InheritanceSC() = default;
    InheritanceSC(const InheritanceSC& isc) : rank(isc.rank), suit(isc.suit) {}
    InheritanceSC(Rank r, Suit s) : rank(r), suit(s) {}

    virtual InheritanceCard* clone() {return new InheritanceSC(*this);} // virtual function override

    // public accessors
    Rank getRank() {return rank;}
    Suit getSuit() {return suit;}

    // output operator overload
    std::ostream operator<<(std::ostream& os)
    {
        os << rank << " of " << suit;
        // return os;
    }
};
class InheritanceJC : InheritanceCard
{
private:
    // member data
    Color color;
public:
    // constructors
    InheritanceJC() = default;
    InheritanceJC(const InheritanceJC& ijc) : color(ijc.color) {}
    InheritanceJC(Color c) : color(c) {}

    virtual InheritanceCard* clone() {return new InheritanceJC(*this);} // virtual function override

    // public accessors
    Color getColor() {return color;}

    // output operator overload
    std::ostream operator<<(std::ostream& os)
    {
        os << color << " Joker";
        // return os;
    }
};

/// deck class (inheritance)
class Deck
{
private:
    // representation for cards in deck
    std::vector<InheritanceCard*> deck;
public:
    // constructors
    Deck() {} // default
    Deck(const Deck& d) : deck(d.deck) {} // copy

    // add cards to deck
    void addCard(InheritanceCard* newCard) // inheritance card as static type for parameter, dynamic type can vary
    {deck.push_back(newCard->clone());} // overrided virtual function

    // shuffle deck
    void shuffle()
    {std::random_shuffle(deck.begin(), deck.end());}

    //
};

int main()
{
    // driver program
    return 0;
}

#endif // CARD_CPP
