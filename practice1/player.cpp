#ifndef PLAYER_CPP
#define PLAYER_CPP

#include "player.hpp"

void Player::reveal()
{
    //
}

void Player::seize(Player* opponent)
{
    //
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
                p1->deck.push_back(new Card(static_cast<Suit>(i), static_cast<Rank>(j)));
                break;
            case 1:
                p2->deck.push_back(new Card(static_cast<Suit>(i), static_cast<Rank>(j)));
                break;
            }
        }
    }
}

void battle(Player* p1, Player* p2)
{
    //
}


#endif // PLAYER_CPP
