/*
Date: 11 February 2018
Project: Practice 3
Author: Thomas Li (thl13)
Purpose: Create a value-semantic version of the card class from practice 1 using bitfields
*/

#include <iostream>

/// namespace contents copy-pasted from practice 1
namespace
{
    /// cards consist of a suit and a rank as members.
    /// Both are represented through enumerated labels.

    enum Suit
    {
        Spades,
        Clubs,
        Hearts,
        Diamonds,
    };


    enum Rank
    {
        Ace,
        Two,
        Three,
        Four,
        Five,
        Six,
        Seven,
        Eight,
        Nine,
        Ten,
        Jack,
        Queen,
        King,
    };

    // overloaded ostream operators - enum labels get displayed instead of internal values
    // abstracted info is more useful to outside users
    // (code copied and modified from cppreference)
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

    const int CARDS = 52;
    const int RANKS = 13;
    const int SUITS = 4;

    const int RBITS = 4; // bits in the bitfield reserved for the rank member
}


class Card
{
private:
    /// member specifications:
    // bitfield holds all member data, consisting of the card's rank and suit
    // 13 possible ranks, represented by first 4 bits (const int RBITS)
    // 4 possible suits, represented by next 2 bits
    // data stored in form ssrrrr for s as one suit bit and r as one rank bit
    unsigned char bitfield;

    // functions to isolate portions of the bitfield represeting data
    unsigned char getRankBits() {return bitfield - (getSuitBits() << RBITS);} // get the rank bits by isolating the first four bits as a single value (remove the suit bits)
    unsigned char getSuitBits() {return bitfield >> 4;} // get the suit bits by isolating the last two bits as a single value (rightshift out the rank bits)
public:
    // constructors
    Card() = default; // assign indeterminate value
    Card(Suit s, Rank r) : bitfield((unsigned)s << RBITS | (unsigned)r) {} // take enum values for rank and suit and put it into the bitfield
    // c-style casts - make the data unsigned for easier bit manipulation
    // first four bits in the bitfield are reserved for rank so suit bits are leftshifted by 4 to make room
    // leftshifted suit bits combined with rank bits to form bitfield

    // accessors - convert bitfield back into high-level data for program access
    Rank getRank() {return (Rank)(getRankBits());} // convert rank bits back into rank type for outside access
    Suit getSuit() {return (Suit)(getSuitBits());} // convert suit bits back into suit type for outside access

    // overloaded operators for value semantics
    bool operator>(Card c2) {return getRankBits() > c2.getRankBits();}
    bool operator<(Card c2) {return getRankBits() < c2.getRankBits();}
    bool operator>=(Card c2) {return getRankBits() >= c2.getRankBits();}
    bool operator<=(Card c2) {return getRankBits() <= c2.getRankBits();}
    bool operator==(Card c2) {return getRankBits() == c2.getRankBits();}
    bool operator!=(Card c2) {return getRankBits() != c2.getRankBits();}
};

int main()
{
    //
}
