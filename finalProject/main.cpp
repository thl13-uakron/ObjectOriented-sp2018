/*
Date: 1 March 2018
Author: Thomas Li
Purpose: Write a text-based version of Santa Paravia using C++ and object-oriented programming methods
*/

/*
Game Flow details (Copied from README.md file)
    - Main menu with four options displayed before game starts:
        - Play the game
            - Selecting this option leads to game setup
        - Display instructions
        - View credits
        - Quit the program
    - Game setup:
        - Start by asking the amount of players (min 1, max 6)
        - Prompt each player for starting information:
            - Player name
            - Town name
            - Gender
            - Difficulty
        - Initialize vector of player classes using input as constructors
        - Repeat process for second vector of AI players
    - Repeating turn structure:
        - Done for each player, starting with the first one in the vector
        - Start with main game menu that lists a series of actions that the player can do in their turn
            - All relevant player stats displayed above options
            - Opt 1: Trade goods
            - Opt 2: Buy soldiers
            - Opt 3: Buy assets
            - Opt 4: Adjust tax rates
            - Opt 5: Invade neighbors
            - Opt 6: Help
            - Opt 7: View standings
            - Opt 8: End turn (move on to grain release)
        - Sub-menus that get called from game menu options
            - Make basic goods transactions
                - Opts 1 and 3: Buy or sell grain
                - Opts 2 and 4: Buy or sell land
                - Opt 5:  Go back
                - Demand, current reserve, and price for grain and land are displayed along with playerÕs gold
                - Following decision, player gets another input prompt for quantity
                - Menu repeats after each action until player chooses to continue
            - Buy assets (buildings)
                - Opt 1: Buy Marketplace (1000g)
                - Opt 2: Buy Wool Mill (2000g)
                - Opt 3: Buy Palace (3000g)
                - Opt 4: Buy Cathedral (5000g)
                - Opt 5: Display help
                - Opt 6: Go back
                - Gold displayed, price and owned quantity of each building displayed alongside option to buy
            - Adjust tax rates
                - Opt 1: Sales
                - Opt 2: Income
                - Opt 3: Customs
                - Opt 4: Help
                - Opt 5: Back
                - Current tax rates and wealth displayed
            - Invade neighbors
                - Player's gold and army size displayed at top of menu
                - Each other player or bot in the game is displayed as an option
                - Size of military displayed alongside option to invade
        - Release grain
            - Takes place after all other turn actions completed, required
            - Grain reserves and serf populations displayed
            - Opt 1: Release minimum amount (20% of reserves)
            - Opt 2: Release maximum amount (80% of reserves)
            - Opt 3: Release custom amount (Get input prompt for exact quantity)
            - Opt 4: Buy more grain
            - Opt 5: Display help
        - Display effects of guaranteed events that take place after turn ends (covered in turnResults member function, player-end)
            - Portion of grain reserves get lost
            - Receive grain harvest from serfs
            - Receive tax revenue
            - Serfs get born, die, move in, or leave
            - Economy grows or shrinks
        - Repeat process for bots except no menus are displayed and all actions are randomly chosen
    - Ending conditions:
        - Game ends if one player wins or all players (bots not counted) lose
            - Lose the game by dying
            - Win the game by reaching max rank
    - Return to main menu after game ends, program can be exited by selecting the "quit" option
*/

/*
Changes from 2000 C version:
    - Front-end:
        - More in-game instructions
            - Presented for each in-game mechanic as a menu option
        - Turn menus presented in branching structure instead of being displayed in order automatically
            - Browse action menus through a root menu
        - Invasion mechanic expanded
            - Human players can now invade their neighbors
            - Grain can be seized as well as land
        - Justice mechanic removed
            - Was too confusing to implement
        - Players can now choose the name of their town
        - More fleshed-out AI
            - Actually plays instead of just invading
            - Up to 6 bots per game
        - Turn structure simplified
            - all actions -> grain release -> all results -> done
        - Purchase volumes restricted and bankruptcy system enforced to prevent massive game-breaking purchases
            - C version can be won in one turn by buying several million acres of land at a time
    - Back-end:
        - Better documentation for easier reading
            - Code actually has comments and spacing now
        - Split into multiple files - easier to nagivate program
            - main.cpp (game flow)
            - player.hpp and .cpp (player class)
            - helperFunctions.hpp and .cpp (input and rng)
            - parameters.hpp (constant parameters)
        - Player struct represented as a class with member functions
        - Seperation between private and public member data
            - Access and modification of internal stats restricted to provided methods
        - Some game formulas simplified for easier implementation
            - Tax revenue
            - Harvest
            - Population change
            - Combat
        - Constant parameters stored in seperate file
            - Starting stats
            - Formula coefficients
            - Value limits
            - Ranks and bot mechanics
        - Game elements comprising multiple data values or exhibiting similar behavior patterns represented as structs
            - Taxes
            - Resources
            - Assets
*/

#include <iostream>
#include <vector>
#include "player.hpp" // player class
#include "helperFunctions.hpp" // input, rng, and chance functions
#include "parameters.hpp" // constant game parameters

using playerVector = std::vector<Player*>; // typedef to represent full group of players

/// function for game setup and object initialization
playerVector playerSetup();
// pre: N/A
// post: get user input for amount of player objects to create, take constructor input for each player object, return as a vector of pointers
playerVector botSetup();
// pre: N/A
// post: similar above function, ntended for initialization of AI-controlled players with changes to program output, input process, and parameters accordingly

/// main game function and ending conditions
void playGame(playerVector players, playerVector bots);
// pre: two properly initialized vectors of player object pointers, one to represent human players and one to represent automated bots
// post: execute loop involving player turns, bot turns, and in-game events, terminate upon reaching certain end conditions
bool gameOver(playerVector players);
// pre: properly intialized vector of player object pointers
// post: individually check each player to see if the game should end, which occurs if either one has won or all have lost (returning true)

/// main in-game menu comprising all other doable actions
void gameMenu(Player* currentPlayer, playerVector players, playerVector bots);
// pre: properly constructed pointer to a player object with two other initialized vectors of player pointers (to be passed into the invasion function)
// post: display in-game menu containing all in-game actions, take user choice and call appropriate associated functions to display sub-menus (below)

/// individual action menus
void goodsMenu(Player* player);
// pre: properly constructed pointer to a player object
// post: display in-game menu for buying/selling resources, take user choice and call appropriate associated object member functions with input parameters
void assetMenu(Player* player);
// pre: properly constructed pointer to a player object
// post: display in-game menu for buying assets, take user choice and call appropriate associated  object member functions with input parameters
void taxMenu(Player* player);
// pre: properly constructed pointer to a player object
// post: display in-game menu for adjusting taxes, take user choice and call appropriate associated object member functions with input parameters
void invasionMenu(Player* currentPlayer, playerVector players, playerVector bots);
// pre: properly constructed pointer to a player object with two other initialized vectors of player pointers (for purposes of getting invaded)
// post: display in-game menu containing all other players and bots in the game for the player to choose to invade, take user choice and call invasion member function with choice as parameter
void grainRelease(Player* player);
// pre: properly constructed pointer to a player object
// post: display in-game menu for releasing grain, take user choice and call grain release member function as necessary

/// simulate bot turns
void botActions(Player* bot, playerVector players, playerVector bots);
// pre: properly constructed pointer to a player object with two other initialized vectors of player pointers (for purposes of getting invaded)
// post: simulate AI control of player object by calling public member functions with random in-range parameters

int main()
{
    /// main menu
    do
    {
        std::cout << "\nWelcome to Santa Paravia and Fiumancco\n\n" // display title
                  << "[1] Play Game\n" // and menu options
                  << "[2] View Instructions\n"
                  << "[3] View Credits\n"
                  << "[4] Quit\n";

        switch(intInput("\nPlease select an option: ", 1, 4)) // take and validate user choice
        { // and call operations accordingly
        case 1:
            // call functions set up game objects to start gameplay
            playGame(playerSetup(), botSetup()); // playGame function comprises of all in-game activity, game ends when function call ends
            std::cout << "Game ended. Thanks for playing!\n";
            break;
        case 2:
            // display instructions
            std::cout << "\n\nINSTRUCTIONS\n"
                      << "Overview: \n"
                      << "- In Santa Paravia and Fiumancco, you play as the ruler of an Italian city-state circa 1400.\n"
                      << "- Trade goods and invest in assets to accumulate wealth while keeping your people fed and defending from invasions.\n"
                      << "- Good management of wealth and resources will allow your town to grow and prosper.\n"
                      << "Controls: \n"
                      << "- As a text-based game, most of the controls in Santa Paravia are fairly straightfoward.\n"
                      << "- You will be given a series of text menus and input prompts to make decisions from.\n"
                      << "- Explanations for the implications of individual decisions will be available as they are shown to you.\n"
                      << "How to win: \n"
                      << "- Your score is calculated as an approximation of the total wealth in your town relative to when you started.\n"
                      << "- This is measured in terms of population and resources and assets owned in addition to the gold in your treasury.\n"
                      << "- On higher difficulties, poor financial decisions will result in you losing score, so act carefully to maximize returns.\n"
                      << "- By reaching a sufficiently high score, you can earn progressively more prestigious titles.\n"
                      << "- Reaching the rank of king or queen will allow you to win the game, but you get limited time to do this before your character dies.\n"
                      << "- AI-controlled bots will also be competing against you for the throne, so watch out.\n";

            std::cin.ignore(INT_MAX, '\n');
            pressEnterToContinue("\n(Press ENTER to return to menu)"); // pause program output before displaying menu again
            break;
        case 3:
            // display credits
            std::cout << "\n\nCREDITS"
                      << "\nProgramming: \n"
                      << "Original game created by George Blank (c)1978\n"
                      << "C port written by Thomas Knox (c)2000\n"
                      << "Current C++ port written by Thomas Li (c)2018\n"
                      << "Graphics: \n"
                      << "N/A\n"
                      << "Sound: \n"
                      << "N/A\n";

            std::cin.ignore(INT_MAX, '\n');
            pressEnterToContinue("\n(Press ENTER to return to menu)"); // pause program output before displaying menu again
            break;
        case 4:
            // exit program if user chose to quit
            return 0;
        default:
            // throw exception if input not recognized as one of the above (should already be validated)
            throw std::logic_error("Error: Invalid menu input received.");
        }
    } while (true); // return to menu if player didn't choose to quit
}

playerVector playerSetup()
{
    playerVector players; // start with empty vector
    int8 numPlayers = intInput("\nHow many players would you like? (1-" + std::to_string(MAX_PLAYERS) + "): ", 1, MAX_PLAYERS); // take input for amount of players to create

    for (int i = 0; i < numPlayers; ++i) // initialize individual player objects
    {
        std::cout << "\nPlayer " << i + 1 << ": \n"; // take input for member values in constructor: 
        std::cin.ignore(INT_MAX, '\n');
        players.push_back(new Player(strInput("Enter your name (1-50 letters): ", 1, 50), // name
                                     strInput("Enter the name of your town (1-50 letters): ", 1, 50), // town name
                                     intInput("Enter the difficulty level to play on (1-" + std::to_string(MAX_DIFFICULTY) + "): ", MIN_DIFFICULTY, MAX_DIFFICULTY), // difficulty
                                     static_cast<Gender>(ynInput("Enter your gender (m/f): ", 'f', 'm')))); // gender
    }

    // display results of initialization in program output
    std::cout << "\nWelcome Players:\n";
    for (Player* p: players) std::cout << p->getTitle() << " " << p->getName() << " of " << p->getTownName() << '\n';

    return players; // return filled vector following initialization
}

playerVector botSetup()
{
    playerVector bots; // empty vector to hold objects
    int8 numBots = intInput("\nHow many bots would you like? (1-" + std::to_string(MAX_BOTS) + "): ", 1, MAX_BOTS); // take input for amount to create

    std::cout << "\nWelcome Bots:\n";
    for (int i = 0; i < numBots; ++i) // initialize objects
    {
        // initialization process
        bots.push_back(new Player(BOTNAMES[random(NUM_BOTNAMES - 1)], // name and townName assigned randomly from a list
                                  BOTNAMES[random(NUM_BOTNAMES - 1)])); // other members randomly assigned within allowed range

        std::cout << bots[i]->getTitle() << " " << bots[i]->getName() << " of " << bots[i]->getTownName() << '\n'; // display results of initialization in program output
    }

    return bots; // return after initialization
}

void playGame(playerVector players, playerVector bots)
{
    std::cout << "\nStarting Game...\n"; // header text

    do // start game loop
    {
        // player turns
        for (Player* p : players)
        {
            if (!p->gameEnded()) // player only gets to play their turn if they haven't died yet
            {
                gameMenu(p, players, bots); // main action menu
                grainRelease(p); // post-turn actions
                p->turnResults(); // display results of turn

                if (gameOver(players)) break; // check ending conditions afterwards

                // have the user press a key to continue to the next turn to avoid to much output being displayed at once
                std::cin.ignore(INT_MAX, '\n');
                pressEnterToContinue("Turn completed. (Press ENTER to continue)");
            }
        }
        if (gameOver(players)) break;



        // bot turns
        for (Player* b : bots)
        {
            if (!b->gameEnded())
            {
                botActions(b, players, bots); // all bot activity done wtihin function
                if (b->won()) break; // bots can win the game

                // have the user press a key to continue to the next turn to avoid to much output being displayed at once
                pressEnterToContinue("Turn completed. (Press ENTER to continue)");
            }
        }

    } while (!gameOver(players)); // loop ends if end conditions reached (should already have been checked)

    // view final player standings before exiting
    std::cout << "FINAL STANDINGS\n"
              << "\nPlayers: \n";
    for (Player* p : players) p->printStats();
    std::cout << "\nBots: \n";
    for (Player* b : bots) b->printStats();
    pressEnterToContinue("\n(Press ENTER to return to menu)");
    std::cout << '\n';
}

bool gameOver(playerVector players)
{
    // end conditions: one player has won or every player has died
    for (Player* p : players)
    {
        if (p->won()) return true;
        if (!(p->dead())) return false;
    }
    return true;
}

void gameMenu(Player* currentPlayer, playerVector players, playerVector bots)
{
    do
    {
        // display header text
        std::cout << "\nYear " << currentPlayer->getYear() << " (Turn " << currentPlayer->getYear() - STARTING_YEAR + 1 << ")\n";
        currentPlayer->printStats();

        // and menu choices
        std::cout << "Administrative Decisions: \n"
                  << "[1] Buy Goods\n"
                  << "[2] Buy Soldiers\n"
                  << "[3] Buy Assets\n"
                  << "[4] Adjust Taxes\n"
                  << "[5] Invade Rivals\n"
                  << "[6] View Standings\n"
                  << "[7] Help\n"
                  << "[8] End Turn\n";

        // get choice, call functions accordingly
        switch(intInput("Select an option: ", 1, 8))
        {
        case 1:
            // display options for buying/selling land an grain
            goodsMenu(currentPlayer);
            break;
        case 2:
            // display relevant info (gold, soldiers owned, soldier price, soldier pay)
            std::cout << "\nCurrent Gold: " << currentPlayer->getGold();
            std::cout << "\nSoldiers Owned: " << currentPlayer->getSoldiers();
            std::cout << ", Cost per Soldier: " << currentPlayer->getSoldierPrice() << "g (Yearly Upkeep: " << currentPlayer->getSoldierPay() << "g)\n";

            // directly prompt player to make purchase (not enough available options to warrant a dedicated menu)
            currentPlayer->buySoldiers(intInput("How many soldiers would you like to buy? (max "
                                                + std::to_string(SOLDIER_PURCHASE_LIMIT) + " per purchase, buy 0 to cancel) ",
                                                 0, SOLDIER_PURCHASE_LIMIT)); // purchase volume restricted to prevent mass debt-purchases
            break;
        case 3:
            // display options for buying assets
            assetMenu(currentPlayer);
            break;
        case 4:
            // display options for adjusting tax rates
            taxMenu(currentPlayer);
            break;
        case 5:
            // display options for invading other players
            invasionMenu(currentPlayer, players, bots);
            break;
        case 6:
            // display stats for all players in game
            std::cout << "\nPlayers: \n";
            for (Player* p : players) p->printStats();
            // and for all bots
            std::cout << "\nBots: \n";
            for (Player* b : bots) b->printStats();
            break;
        case 7:
            // display helper instructions
            std::cout << "\nThis the main game menu from which you can make most of your decisions.\n"
                      << "If this is your first time playing, take the time to look"
                      << "though each of your options to see what's available for you to do.\n"
                      << "Once you're done, select the End Turn option to proceed to the next step.\n";

            // pause output before returning to menu so player can see instruction text
            std::cin.ignore(INT_MAX, '\n');
            pressEnterToContinue("(Press ENTER to continue)");
            break;
        case 8:
            // exit function, proceed with game
            return;
        default:
            throw std::logic_error("Invalid menu input received."); // throw exception if input not accounted for
        }
    } while (true); // loop doesn't terminate until user chooses "end turn" option
}

void goodsMenu(Player* player)
{
    do
    {
        // display relevant header info (gold, grain prices, grain owned, land prices, land owned)
        std::cout << "\nCurrent Gold: " << player->getGold() // display relevant info (gold, soldiers owned, soldier price, soldier pay)
                  << "\nGrain Owned: " << player->getGrain()
                  << ", Grain Price: " << player->getGrainPrice() << "g"
                  << "\nLand Owned: " << player->getLand()
                  << ", Land Price: " << player->getLandPrice() << "g\n";

        // display choices
        std::cout << "\nOptions: \n"
                  << "[1] Buy Grain\n"
                  << "[2] Buy Land\n"
                  << "[3] Sell Grain\n"
                  << "[4] Sell Land\n"
                  << "[5] Help\n"
                  << "[6] Back\n";

        // take user choice, call operations accordingly
        switch(intInput("Select an option: ", 1, 6))
        {
        case 1:
            // prompt player to buy grain, take and validate input
            player->buyGrain(intInput("How much grain would you like to buy? (max "
                                     + std::to_string(GRAIN_PURCHASE_LIMIT) + " per purchase, buy 0 to cancel) ",
                                     0, GRAIN_PURCHASE_LIMIT)); // purchase volume restricted to prevent excessive debt-purchases
            break;
        case 2:
            // prompt player to buy land, take and validate input
            player->buyLand(intInput("How much land would you like to buy? (max "
                                     + std::to_string(LAND_PURCHASE_LIMIT) + " per purchase, buy 0 to cancel) ",
                                     0, LAND_PURCHASE_LIMIT)); // purchase volume restricted to prevent mass debt-purchases
            break;
        case 3:
            // prompt player to sell grain, take and validate input
            player->sellGrain(intInput("How much grain would you like to sell? (must keep "
                                     + std::to_string(MIN_GRAIN) + ", sell 0 to cancel) ", // player must keep a minimum amount of land
                                      0, (player->getGrain() > MIN_GRAIN ? player->getGrain() - MIN_GRAIN : 0))); // conditional operator to allow player to sell 0 without causing errors
            break;
        case 4:
            // prompt player to sell land, take and validate input
            player->sellLand(intInput("How much land would you like to sell? (must keep "
                                     + std::to_string(MIN_LAND) + ", sell 0 to cancel) ", // player must keep a minimum amount of land
                                      0, (player->getLand() > MIN_LAND ? player->getLand() - MIN_LAND : 0))); // conditional operator to allow player to sell 0 without causing errors
            break;
        case 5:
            // display help
            std::cout << "\nHere you can make bulk purchases and sales of goods and "
                      << "resources that serve essential purposes for your town: "
                      << "- Grain is needed to feed your serfs and prevent them from starving. They'll produce some on their "
                      << " own each turn but you might need to buy extra in case of a poor harvest\n"
                      << "- Land mainly serves as a measure of your town's power and counts extra towards your score "
                      << "though it doesn't have any other uses implemented yet.\n"
                      << "On higher difficulties, goods sell for less than what you buy them for, "
                      << "so make each transaction count.\n";

            // pause output before returning to action menu so player can see instruction text
            std::cin.ignore(INT_MAX, '\n');
            pressEnterToContinue("(Press ENTER to continue)");
            break;
        case 6:
            // exit menu, return to game menu
            return;
        default:
            throw std::logic_error("Invalid menu input received."); // throw exception if input not accounted for
        }
    } while (true); // menu loop only terminates if user chooses to go back
}

void assetMenu(Player* player)
{
    do
    {
        // display relevant header info (gold)
        std::cout << "\nCurrent Gold: " << player->getGold() << "\n";

        // display choices
        std::cout << "\nOptions: \n"
                  << "[1] Buy Market (" << player->getMarketPrice() << "g)\n" // prices shown alongside purchase option
                  << "[2] Buy Mill (" << player->getMillPrice() << "g)\n"
                  << "[3] Buy Palace ("<<  player->getPalacePrice() << "g)\n"
                  << "[4] Buy Cathedral (" << player->getCathedralPrice() << "g)\n"
                  << "[5] Help\n"
                  << "[6] Back\n";

        // take user choice, call operations accordingly
        switch(intInput("Select an option: ", 1, 6))
        {
        case 1:
            // call function to buy market
            player->buyMarket();
            break;
        case 2:
            // call function to buy mill
            player->buyMill();
            break;
        case 3:
            // call function to buy palace
            player->buyPalace();
            break;
        case 4:
            // call function to buy cathedral
            player->buyCathedral();
            break;
        case 5:
            // display help
            std::cout << "\nAssets are long-term investments that can generate revenue over time, "
                      << "either directly or by attracting tax-paying citizens to your town each turn.\n"
                      << "- Markets create a small amount of yearly revenue and can bring merchants to the town.\n"
                      << "- Mills create a moderate amount of yearly revenue.\n"
                      << "- Palaces can bring nobles to the town.\n"
                      << "- Cathedrals can bring clergy to the town.\n"
                      << "In case of bankruptcy, your assets will be seized by creditors, so be careful.\n";

            // pause output before returning to action menu so player can see instruction text
            std::cin.ignore(INT_MAX, '\n');
            pressEnterToContinue("(Press ENTER to continue)");
            break;
        case 6:
            // exit menu, return to game menu
            return;
        default:
            throw std::logic_error("Invalid menu input received."); // throw exception if input not accounted for
        }
    } while (true); // menu loop only terminates if user chooses to go back
}

void taxMenu(Player* player)
{
    do
    {
        // display choices
        std::cout << "\nOptions: \n"
                  << "[1] Adjust Sales Tax (Current Rate: " << player->getSales() << "%)\n" // current prices shown alongside adjustment option
                  << "[2] Adjust Income Tax (Current Rate: " << player->getIncome() << "%)\n"
                  << "[3] Adjust Customs Duty (Current Rate: "<<  player->getCustoms() << "%)\n"
                  << "[4] Help\n"
                  << "[5] Back\n";

        // take user choice, call operations accordingly
        switch(intInput("Select an option: ", 1, 5))
        {
        case 1:
            // prompt player to adjust sales tax within range, take and validate input
            player->adjustSales(intInput("Enter the new sales tax rate (min " + std::to_string(MIN_TAX) +
                                        ", max " + std::to_string(MAX_SALES_TAX) + "): ",
                                         MIN_TAX, MAX_SALES_TAX));
            break;
        case 2:
            // prompt player to adjust income tax within range, take and validate input
            player->adjustIncome(intInput("Enter the new income tax rate (min " + std::to_string(MIN_TAX) +
                                        ", max " + std::to_string(MAX_INCOME_TAX) + "): ",
                                         MIN_TAX, MAX_INCOME_TAX));
            break;
        case 3:
            // prompt player to adjust customs duty within range, take and validate input
            player->adjustCustoms(intInput("Enter the new customs duty rate (min " + std::to_string(MIN_TAX) +
                                        ", max " + std::to_string(MAX_CUSTOMS_TAX) + "): ",
                                         MIN_TAX, MAX_CUSTOMS_TAX));
            break;
        case 4:
            // display help
            std::cout << "\nTaxes represent the main source of your yearly income, "
                      << "with each tax getting its revenue from different sources:\n"
                      << "- Sales: merchants, nobles, and assets\n"
                      << "- Income: nobles and assets\n"
                      << "- Customs: merchants, clergy, nobles, and assets\n"
                      << "Higher tax rates allow you to earn more revenue, but "
                      << "will make potential taxpayers less willing to move to your town.\n";

            // pause output before returning to action menu so player can see instruction text
            std::cin.ignore(INT_MAX, '\n');
            pressEnterToContinue("(Press ENTER to continue)");
            break;
        case 5:
            // exit menu, return to game menu
            return;
        default:
            throw std::logic_error("Invalid menu input received."); // throw exception if input not accounted for
        }
    } while (true); // menu loop only terminates if user chooses to go back
}

void invasionMenu(Player* currentPlayer, playerVector players, playerVector bots)
{
    playerVector targets; // menu options can differ based on circumstances
    // essentially consist of all players and bots that aren't dead and aren't the current player
    for (Player* p : players) if (!p->dead() && p != currentPlayer) targets.push_back(p);
    for (Player* b : bots) if (!b->dead()) targets.push_back(b);

    // record indexes for options for other optioms 2
    const int help = targets.size() + 1;
    const int cancel = targets.size() + 2;

    do
    {
        // display relevant header info (gold, soldiers)
        std::cout << "\nCurrent Gold: " << currentPlayer->getGold()
                  << "\nSoldiers Owned: " << currentPlayer->getSoldiers() << "\n";

        // display choices
        std::cout << "\nOptions:\n"; //
        for (int i = 0; i < targets.size(); ++i)
        {
            std::cout << "[" << i + 1 << "] Invade "
                      << targets[i]->getTitle() << " " << targets[i]->getName() << " of " << targets[i]->getTownName()
                      << " (" << targets[i]->getSoldiers() << " soldiers)\n";
        } // options to get help and return to game menu
        std::cout << "[" << help << "] Help\n"
                  << "[" << cancel << "] Back\n";



        // take user choice, call operations accordingly
        int choice = intInput("Select an option: ", 1, targets.size() + 2);
        if (choice == help)
        {
            // display help if player chose to do so
            std::cout << "\nIf you're tired of having to buy and manage your own resources, you "
                      << "can use your soldiers to attempt to seize grain, land, or assets "
                      << "by force from any of the other players in the game.  \n"
                      << "The defenders will fight back, though, so be sure to invest well enough "
                      << "into your army to ensure a successful invasion.\n"
                      << "You can buy soldiers from the turn menu if you need more. \n";

            // pause output before returning to action menu so player can see instruction text
            std::cin.ignore(INT_MAX, '\n');
            pressEnterToContinue("(Press ENTER to continue)");
            break;
        }
        else if (choice == cancel)
        {
            // exit function if player chose to go back
            return;
        }
        else
        {
            // invade specified target otherwise
            currentPlayer->invade(targets[choice - 1]);
            break;
        }
    } while (true); // menu loop only terminates if user chooses to go back
}

void grainRelease(Player* player)
{
    do
    {
        // display relevant header info (grain, grain demand)
        std::cout << "\nDistribute Grain for Consumption\n"
                  << "Current Reserve: " << player->getGrain()
                  << "\n" << player->grainDemand() << " grain will be needed to feed " << player->getTownName() << "'s population this year.\n";

        // display choices
        std::cout << "\nOptions: \n"
                  << "[1] Release Minimum Amount (" << MIN_GRAIN_RELEASE << "% - " << player->minRelease() << ")\n"
                  << "[2] Release Maximum Amount (" << MAX_GRAIN_RELEASE << "% - " << player->maxRelease() << ")\n"
                  << "[3] Release Other Amount\n"
                  << "[4] Buy More Grain\n"
                  << "[5] Help\n";

        // take user choice, call operations accordingly
        switch(intInput("Select an option: ", 1, 5))
        {
        case 1:
            // release minimum amount
            player->releaseGrain(player->minRelease());
            return;
        case 2:
            // release maximum amount
            player->releaseGrain(player->maxRelease());
            return;
        case 3:
            // take and validate input for how much to release
            player->releaseGrain(intInput("Enter the amount to release: ", player->minRelease(), player->maxRelease()));
            return;
        case 4:
            // show prompt for player to buy grain along with relevant info, take and valdiate input
            std::cout << "Current Gold: " << player->getGold() << ", Grain Price: " << player->getGrainPrice() << "g\n";
            player->buyGrain(intInput("How much would you like to buy? (max "
                                     + std::to_string(GRAIN_PURCHASE_LIMIT) + " per purchase, buy 0 to cancel) ",
                                     0, GRAIN_PURCHASE_LIMIT));
            break;
        case 5:
            // display help
            std::cout << "\nWhile tax-paying citizens can eat on their own paycheck, "
                      << "your serfs rely on you to release a portion of your grain reserves "
                      << "each year in order to keep them fed.\n"
                      << "Not meeting the demand will result in serfs starving to death, while exceeding "
                      << "it will increase birth rates and spur migration to your town.\n"
                      << "Don't release too much, though, or you might have trouble replenishing"
                      << "your reserves in later turns.\n"
                      << "If you need more grain, you can buy some here.\n";

            // pause output quickly before returning to action menu so player can see instruction text
            std::cin.ignore(INT_MAX, '\n');
            pressEnterToContinue("(Press ENTER to continue)");
            break;
        default:
            throw std::logic_error("Invalid menu input received."); // throw exception if input not accounted for
        }
    } while (true); // menu loop terminates if player chooses option other than help or buy more grain(will return)
}

void botActions(Player* bot, playerVector players, playerVector bots)
{
    // current AI behavior for each bot

    // display stats header
    std::cout << "Year " << bot->getYear() << " (Turn " << bot->getYear() - STARTING_YEAR + 1 << ")\n";
    bot->printStats();

    // quick break in program output
    pressEnterToContinue("(Press ENTER to continue)");

    // bot randomly buys goods within allowed range if they have more than 0 gold
    if (bot->getGold() > 0)
    {
        bot->buyGrain(random((GRAIN_PURCHASE_LIMIT < bot->getGold() / bot->getGrainPrice() ? GRAIN_PURCHASE_LIMIT : (bot->getGold() / bot->getGrainPrice()) - 1)) * (1 - percent(BOT_FRUGALITY)));
        bot->buyLand(random((LAND_PURCHASE_LIMIT < bot->getGold() / bot->getLandPrice() ? LAND_PURCHASE_LIMIT : (bot->getGold() / bot->getLandPrice()) - 1)) * (1 - percent(BOT_FRUGALITY)));
        bot->buySoldiers(random((SOLDIER_PURCHASE_LIMIT < bot->getGold() / bot->getSoldierPrice() ? SOLDIER_PURCHASE_LIMIT : (bot->getGold() / bot->getSoldierPrice()) - 1)) * (1 - percent(BOT_FRUGALITY)));
        // random value goes up to purchase limit if they can afford it, highest affordable amount otherwise
        std::cout << '\n'; // formatting
    }

    // randomly sells goods within allowed range
    if (bot->getGrain() > MIN_GRAIN) bot->sellGrain(random(bot->getGrain() - MIN_GRAIN - 1) * (1 - percent(BOT_FRUGALITY)));
    if (bot->getLand() > MIN_LAND) bot->sellLand(random(bot->getLand() - MIN_LAND - 1) * (1 - percent(BOT_FRUGALITY)));
    std::cout << '\n'; // formatting

    // buys assets if they have more than 0 gold and pass a chance roll
    for (int i = 0; i < BOT_PURCHASES; ++i) // makes three attempts to buy each asset
    {
        if (bot->getGold() > bot->getMarketPrice() && !rollChance(BOT_FRUGALITY, 100)) bot->buyMarket();
        if (bot->getGold() > bot->getMillPrice() && !rollChance(BOT_FRUGALITY, 100)) bot->buyMill();
        if (bot->getGold() > bot->getCathedralPrice() && !rollChance(BOT_FRUGALITY, 100)) bot->buyCathedral();
        if (bot->getGold() > bot->getPalacePrice() && !rollChance(BOT_FRUGALITY, 100)) bot->buyPalace();
    }

    // formatting
    std::cout << '\n';

    // adjusts tax rates to random amounts within range
    bot->adjustSales(random(MAX_SALES_TAX));
    bot->adjustIncome(random(MAX_INCOME_TAX));
    bot->adjustCustoms(random(MAX_CUSTOMS_TAX));

    // random chance to invade random-chosen other player or bot
    if (rollChance(BOT_AGGRESSION, 100)) // roll
    {
        int16 targetIndex = random(players.size() + bots.size() - 1); // get random person to invade among both the players and the other bots
        if (targetIndex < players.size()) // invade target
        {
            bot->invade(players[targetIndex]);
        }
        else
        {
            bot->invade(bots[targetIndex % players.size()]);
        }
        std::cout << '\n'; // formatting
    }

    // release random amount of grain
    bot->releaseGrain(random(bot->minRelease(), bot->maxRelease()));

    // quick break in program output
    pressEnterToContinue("(Press ENTER to continue)");

    // receive year-end report
    bot->turnResults();

    // more intelligent AI might be implemented later
}
