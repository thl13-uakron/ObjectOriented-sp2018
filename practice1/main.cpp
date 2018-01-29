/*
Date: 28 January 2018
Author: Thomas Li
Purpose: Simulate the card game of war
*/

#include <iostream>
#include "player.hpp"

/*
Card class specifications:
- A card essentially consists of a combination of a suit and a rank
- Both are represented as private members using enumerated labels for abstraction
- Overloaded ostream operators allow the enums to be displayed by their abstractedlabels
- Card members are initialized through a constructor and cannot be modified further
- Public getter functions are present for both members but there are no setters
*/

/*
Player class specifications:
- The player's member data mainly consists of their face-down deck (private) and their face-up hand (public), both represented by deques of card objects
- In addition, there are also members for the player's displayed name and the value of their most-recently played card
- The "back" of each queue represents the most recently added card
- Getter functions are present to display the amount of cards in a player's possession as well as their other private member data
- Additional member functions allow players to "reveal" cards (move from deck to hand) and take all of the cards in the other player's hand if they "win" the play
- Friend functions implemented to create and randomly split the original deck of cards at the beginning of the game and simulate plays within the game in which the players reveal and compare cards
*/

/*
Game process;
- Two player objects are initialized and the assignCards function is called to "give" them their cards
- Each turn of the game is represented by a repeating call of the battle function
- Within the call, the reveal member function is called in which the players "flip over" the "top" card on their deck, moving it to their hand member
- If the cards are equal, the battle function is recursively called again and the process repeats
- Once one player's card "beats" the other player's, the seize member is called and all of the "loser"'s "face-up" cards are added to the "winner"'s deck
- The battle function is iteratively recalled and the overall process repeats once again until a winning condition is reached of one player having all of the cards, in which the endgame function is called
*/

int main()
{
    std::cout << "Game: WAR\n"; // display title text
    std::cout << "Suits ignored, aces low.\n\n";

    // create objects for the players and assign cards to their decks
    Player* p1 = new Player("Player 1");
    Player* p2 = new Player("Player 2");
    assignCards(p1, p2);
    std::cout <<"Splitting deck...\n";

    // call the necessary functions to start and play the game
    while (p1->totalCards() != 0 && p2->totalCards() != 0)
    {
        battle(p1, p2); //function plays on turn of gameplay
    }  // loops as long as neither player has all/none of the cards (winning condition)

    // declare one player as the winner after game finishes
    if (p1->totalCards() > p2->totalCards())
    {
        std::cout<< p1->getName();
    }
    else
    {
        std::cout<< p2->getName();
    }
    std::cout << " has won the game.\n\n";
}

