#ifndef CARD_HPP
#define CARD_HPP

#include <iostream>

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

const int CARDS = 52;
const int RANKS = 13;
const int SUITS = 4;

class Card
{
private:
    // card members
    Suit suit;
    Rank face;
public:
    // constructors, initializes both members which can then no longer be modified
    Card(Suit s, Rank r) : suit(s), face(r) {}
    // Card(int s, int r) : suit(static_cast<Suit>(s)), face(static_cast<Rank>(r)) {}

    // getter functions present but no setter functions
    // members are view-only after initialization, as is the case for real cards
    Suit getSuit() {return suit;}
    Rank getRank() {return face;}
};

/// old code (ignore)
/*namespace
{
    extern const int SUITS;
    extern const int SUITS = 4; // total number of card suits

    /// array contains abstracted suit names as strings indexed by corresponding internal int values in the card class
    extern const std::string SUITNAMES[SUITS];
    extern const std::string SUITNAMES[SUITS] =
    {"spades", // internal code: 1
     "clubs", // 2
     "hearts", // 3
     "diamonds"}; // 4

     extern const int VALUES;
     extern const int VALUES = 13; // total number of possible card values

     /// abstracted value names indexed by corresponding internal int values in the card class
     extern const std::string VALUENAMES[VALUES];
     extern const std::string VALUENAMES[VALUES] =
     {"ace", // internal code: 1
      "two", // 2
      "three", // 3
      "four", // 4
      "five", // 5
      "six", // 6
      "seven", // 7
      "eight", // 8
      "nine", // 9
      "ten", // 10
      "jack", // 11
      "queen", // 12
      "king"}; // 13
}

/// helper functions to convert abstracted versions of card members back into ints

int getSuitInternal(std::string suitName)
{
    for (int i = 0; i < SUITS; ++i)
        if (suitName == SUITNAMES[i]) return i + 1;

    throw std::runtime_error("Error: Invalid value processed.");
}

int getValueInternal(std::string valueName)
{
    for (int i = 0; i < VALUES; ++i)
        if (valueName == VALUENAMES[i]) return i + 1;

    throw std::runtime_error("Error: Invalid value processed.");
}

class Card
{
private:
    /// internal member values

    int suitID;
    // 1 = spades
    // 2 = clubs
    // 3 = hearts
    // 4 = diamonds

    int valueID;
    // 1 = ace
    // 2-10 = [same]
    // 11 = jack
    // 12 = queen
    // 13 = king

public:
    /// constructors
    Card() {}; // default
    Card(int, int); // using internal values
    Card(std::string, std::string); // using abstracted values

    /// getter functions
    std::string getSuit() {return SUITNAMES[suitID - 1];} // abstracted string version
    int getSuitInternal() {return suitID;} // internal int version

    std::string getValue() {return VALUENAMES[valueID - 1];} // abstracted string version
    int getValueInternal() {return valueID;} // internal int version

    std::string getName() {return this->getValue() + " of " + this->getSuit();} // abstraced card name (ace of spades, two of hearts, etc)

    // return abstracted score values tied to cards based on suit and face content
    // different functions for different games, none implemented yet
    int getScore_Poker();
    int getScore_War();
    int getScore_GoFish();

    /// setter functions
    void setSuit(int newSuit) {suitID = newSuit;} // takes internal parameter and directly assigns´
    void setSuit(std::string newSuit) {suitID = ::getSuitInternal(newSuit);} // takes abstracted parameter and converts to internal version

    void setValue(int newValue) {valueID = newValue;} // see previous
    void setValue(std::string newValue) {valueID = ::getValueInternal(newValue);}
};*/


#endif // CARD_HPP
