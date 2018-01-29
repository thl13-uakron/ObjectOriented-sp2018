#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include <cstdlib>
#include <deque>
#include "card.hpp"

class Player
{
private:
    std::deque<Card*> deck; // "inactive" pile of face-down cards in the player's possession
    /// "front" of queue represents "top" of deck, new cards stored in "back" (bottom)

    std::string name; // name displayed for the player in-game
public:
    // constructors
    Player() : name("") {}
    Player(std::string s) : name(s) {}

    std::deque<Card*> hand; // player's current "active" hand
    /// "back" of queue represents most recently played card

    /// getter functions
    int cardsOnHand() {return hand.size();} // returns the total amount of cards the player has in their hand
    int cardsInDeck() {return deck.size();} // returns the total amount of cards in the decks
    int totalCards() {return hand.size() + deck.size();} // returns overall total number of cards
    std::string getName() {return name;} // returns player's display name
    int getHandValue() {return hand.back()->getRank();} // returns value of most-recently played card

    /// game functions

    void reveal(); // moves a card from the player's deck to their hand
    void seize(Player*); // takes all the cards in the other player's hand as well as this player's hand and moves it to this player's deck when they win the "war"
    friend void assignCards(Player*, Player*);
    friend void battle(Player*, Player*);
};

void assignCards(Player* p1, Player* p2);
// pre: two player objects intialized
// post: creates a virtual "deck" of card objects and distributes them to the deck members of the two player objects to start the game

void battle(Player* p1, Player* p2);
// pre: two player objects, preferably with cards assigned
// post: simulates an individual play within the larger game in which the two players take out cards and compare them, with the winner taking all of the cards

#endif // PLAYER_HPP
