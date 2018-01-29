#ifndef PLAYER_CPP
#define PLAYER_CPP

#include "player.hpp"

void Player::reveal()
{
    if (!deck.empty())
    {
        hand.push_back(deck.front()); // moves card object from deck to hand if deck isn't empty
        deck.pop_front();
        std::cout << name << " draws card: " << hand.back()->getRank() << " of " << hand.back()->getSuit() << "\n"; // display the abstracted name of the drawn card (overloaded operators used)
    }
    else // otherwise, hand remains unchanged and gets re-used (game would've already ended if hand was also empty)
    {
        std::cout << name << " has no more face-down cards. Re-using previous card: " << hand.back()->getRank() << " of " << hand.back()->getSuit() << "\n";
    }
}

void Player::seize(Player* opponent)
{
    // member is called by "winner" of play, parameter is that of "loser"
    std::cout << name << "'s card has a higher value.\n";
    std::cout << opponent->getName() << "'s face-up cards now belong to " << name << ".\n\n";

    // moves full contents of both players' hands to the "winner"'s face-down deck
    while (!hand.empty())
    {
        deck.push_back(hand.front());
        hand.pop_front();
    }

    while (!opponent->hand.empty())
    {
        deck.push_back(opponent->hand.front());
        opponent->hand.pop_front();
    }
}

void assignCards(Player* p1, Player* p2)
{
    srand(time(0)); // seed RNG
    int random;

    for (int i = 0; i < SUITS; ++i)
    {
        for (int j = 0; j < RANKS; ++j) // generate cards in order and randomly assign them
        {
            random = rand() % 2;
            switch (random)
            {
            case 0:
                if (p1->totalCards() < CARDS / 2) // if one player already has half of the cards, the cards are given to the other player.
                {
                    p1->deck.push_back(new Card(static_cast<Suit>(i), static_cast<Rank>(j)));
                }
                else
                {
                    p2->deck.push_back(new Card(static_cast<Suit>(i), static_cast<Rank>(j)));
                }
                break;
            case 1:
                if (p2->totalCards() < CARDS / 2) // if one player already has half of the cards, the cards are given to the other player.
                {
                    p2->deck.push_back(new Card(static_cast<Suit>(i), static_cast<Rank>(j)));
                }
                else
                {
                    p1->deck.push_back(new Card(static_cast<Suit>(i), static_cast<Rank>(j)));
                }
                break;
            }
        }
    }
}

void battle(Player* p1, Player* p2)
{
    // start turn by displaying current status of game (# of cards possessed by the players)
    std::cout << p1->getName() << ": " << p1->cardsInDeck() << " face-down cards, " << p1->cardsOnHand() << " face-up cards.\n";
    std::cout << p2->getName() << ": " << p2->cardsInDeck() << " face-down cards, " << p2->cardsOnHand() << " face-up cards.\n\n";

    // call functions for both players to "reveal" a card in their deck
    p1->reveal();
    p2->reveal();
    std::cout <<'\n';

    // compare the cards and call the seize function for the player with the "winning" card
    if (p1->getHandValue() > p2->getHandValue())
    {
        p1->seize(p2);
    }
    else if (p1->getHandValue() < p2->getHandValue())
    {
        p2->seize(p1);
    }
    else
    {
        std::cout << "Cards are equal. Playing new cards...\n\n"; // if cards are equal, display message and re-play turn
        battle(p1, p2); // recursively repeats function
    }
}


#endif // PLAYER_CPP
