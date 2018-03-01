/*
Date: 1 March 2018
Author: Thomas Li
Purpose: Write a text-based version of Santa Paravia using C++ and object-oriented programming methods
*/

/*
Game Flow details (Copied from README.md file)
    - Before game starts:
        - User receives Y/N prompt for instructions
        - Display instructions depending on response and move on
    - Game setup:
        - Start by asking the amount of players (min 1, max 6)
        - Prompt each player for starting information:
            - Player name
            - Town name
            - Gender
            - Difficulty
        - Initialize vector of player classes using input as constructors
    - Repeating turn structure:
        - Done for each player, starting with the first one in the vector
        - Start by displaying effects of guaranteed events that take place before turn starts
            - Rats eat portion of grain reserves
            - Receive grain harvest from serfs
            - Receive tax revenue
            - Serfs get born, die, move in, or leave
            - Economy grows or shrinks
        - Display series of action menus that allow player to make purchases and decision with relevant stats being showed
            - Each decision calls another member function that either displays a sub-menu or modifies player stats, or both
        - First allows player to make basic goods transactions
            - Opts 1 and 3: Buy or sell grain
            - Opts 2 and 4: Buy or sell land
            - Opt 5: Continue to next menu
            - Demand, current reserve, and price for grain and land are displayed along with player’s gold
            - Following decision, player gets another input prompt for quantity
            - Menu repeats after each action until player chooses to continue
        - Buy assets (buildings)
            - Opt 1: Buy Marketplace (1000g)
            - Opt 2: Buy Wool Mill (2000g)
            - Opt 3: Buy Palace (3000g)
            - Opt 4: Buy Cathedral (5000g)
            - Gold displayed, price and owned quantity of each building displayed alongside option to buy
        - Release grain
            - Grain reserves and serf populations displayed
            - Opt 1: Release minimum amount (20% of reserves)
            - Opt 2: Release maximum amount (80% of reserves)
            - Opt 3: Release custom amount (Get input prompt for exact quantity)
            - Automatically continues to next menu after decision is made
        - Other administrative decisions
            - Opt 1: Adjust taxes
            - Opt 2: Draft serfs into the army
            - Opt 3: Invade neighbors
        - Game process for handling debt:
            -
    - Ending conditions:
        - Game ends if one player wins or all players lose
            - Lose the game by dying
            - Win the game by reaching max rank
        - (See player specifications for details)
*/

#include <iostream>
#include "player.hpp"

int main()
{
    //
}
