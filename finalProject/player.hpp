#ifndef PLAYER_HPP
#define PLAYER_HPP

/*
Player class abstraction details (copied from README.md file)

    - The player is the ruler of a city-state in 1400s Spain
        - Town stats included with player stats


    - On a high level, members can be split into the following categories:

		- Member vars for personal stats; chosen at start of game, entered into class constructor, and remain unchanged:
			- Player name and town name
                - Used to identify players within the external game output
			- Their gender
                - Male or Female
                - Mainly used to determine titles used (see later)
			- Game difficulty
                - 4 possible difficulty levels
                - Higher difficulties can amplify the affects of in-game events detrimental to the player
			- Which player they are (p1, p2, etc.)
                - Maximum of 6 players allowed in a single game
                - Player number used to identify players within the code

		- Stats representing the town's status; set to default values with object construction and change with each turn and player decision:
			- Grain reserves, price, and demand
                - Grain is needed to feed the serf population
                - Demand determines the amount required to prevent starvation
                - Prices and demand change randomly between turns
			- Land owned, land prices
                - Land is needed in order to
                - Price gets adjusted between turns as well
			- Gold in treasury
                - Can be exchanged for goods and assets
			- Town's population, in the following categories:
				- Serfs: main backbone of town population, only people that can die or flee
                - Merchants: source of tax revenue
				- Clergy: source of tax revenue
				- Noblemen: major source of tax revenue
				- Soldiers: used for invading other players or defending from invasions
			- Town buildings and assets owned, in the following categories:
				- Marketplaces: attracts merchants, produces income
				- Mills: produces income
				- Palaces: attracts nobles
				- Cathedrals: attracts clergy
			- Tax rates, in the following categories:
				- Customs: gets revenue from merchants, nobles, clergy and town buildings
				- Sales: gets revenue from merchants, nobles, and town buildings
				- Income: gets revenue from nobles, and town buildings
				- Justice: determines how strictly taxes are enforced
			- Player's rank
				- Total of six possible values for ranks, players start at lowest one by default
				- More info in the bottom section

		- Functions encompassing decisions that players make during turns that can affect gameplay:
			- Raise or lower taxes
                - Rates can be adjusted within a certain range that varies by category
                - Higher rates result in more income generated next turn but too much can
			- Release grain
				- Certain percentage of grain reserves has to be distributed each turn for serf consumption
				- More grain released means less death rate and higher population growth
				- Releasing too much means you'll deplete the reserves for future turns though
			- Buy assets
			- Buy grain or land
			- Draft serfs into the army
				- Convert of some of the serf population into soldiers
				- Increases military strength but less economic output / grain production
			- View status and standings (no effect on game itself)
				- Game displays ranks, wealth, population sizes, and land owned for each player
			- Invade other player's city-state
				- Attempt to seize a portion of someone else's land using your soldiers
				- Only possible in multiplayer when other players are available to invade

		- Functions representing in-game events that can affect a player's stats and in-game status:
			- Receive income
                - Add gold back into the treasury, allowing for more spending on essential goods
				- Mostly comes in the form of taxes
				- Each tax category has its own formula for revenue based on rates and populations
				- Mills and markets and also create income
			- Receive grain harvest
                - Grain reserves get replenished between turns by a certain amount
				- Depends on serf population and land owned, results can vary randomly
            - Grain eaten by rats
				- Grain reserves also decrease by random percentage each turn
				- Can counteract or be counteracted by effects of harvest
			- Economy grows or shrinks
				-
			- People die, get born, immigrate, or leave
				- Depends on grain released
				- Serfs move in if more grain released than demanded
				- Not enough grain can result in serf starvation
				- Other people move in with the construction of respective public buildings
            - Pay expenses
				- Upkeep for army and assets deducted from treasury
            - Price adjustments for grain and land
            - Bankruptcy
                - Assets can get seized to pay off debts if wealth falls below 0

        - Rank/scoring system and game-over conditions
            -

*/

#include <string>

namespace
{
    /// set of default values to initialize player in-game stats to
    // values not accounted for here are assumed to start at 0

    // general stats
    const short int STARTING_YEAR = 1400;
    const short int STARTING_GOLD = 1000;
    const unsigned char STARTING_RANK = 1;

    // prices and starting supply of buyable assets
    const short int STARTING_GRAIN = 10000;
    const short int GRAIN_PRICE = 10;
    const short int STARTING_LAND = 10000;
    const short int LAND_PRICE = 10;
    const short int MARKET_PRICE = 1000;
    const short int MILL_PRICE = 2000;
    const short int PALACE_PRICE = 3000;
    const short int CATHEDRAL_PRICE = 5000;

    // default populations
    const short int STARTING_SERFS = 2000;
    const short int STARTING_SOLDIERS = 25;
    const short int STARTING_MERCHANTS = 25;
    const short int STARTING_NOBLES = 0;
    const short int STARTING_CLERGY = 5;

    // default tax rates
    const unsigned char SALES_TAX = 10;
    const unsigned char INCOME_TAX = 5;
    const unsigned char CUSTOMS_TAX = 25;
    const unsigned char TAX_JUSTICE = 2;


    /// value limits and other constant parameters for gameplay

    // taxation limits
    const unsigned char MIN_TAX = 0;
    const unsigned char MAX_SALES_TAX = 25;
    const unsigned char MAX_INCOME_TAX = 50;
    const unsigned char MAX_CUSTOMS_TAX = 100;
    const unsigned char MAX_TAX_JUSTICE = 4;
}

enum Gender {Male, Female}; // player gender represented with enum values to make higher-level usage easier

class Player
{
private:
    /// first set of members denoted in the abstraction details - personal stats
    // basic identifying info that's mostly independent from gameplay and game flow (aside of the difficulty member)
    // member values here are initialized by constructor and don't have any methods for further mutation

    std::string name;
    std::string townName;

    unsigned char playerDetails;
    // to save space, non-string members in this section are stored in an 8-bit bitfield
    // first 3 bits hold the player number (p1, p2, etc.) (6 possible values, 8 alloted)
    // next 2 bits hold the difficulty level (4 possible values, 4 alloted)
    // one more bit holds the gender (2 possible values, 2 alloted)
    unsigned char playerNumBits() {return (playerDetails & 0b000111);}
    unsigned char difficultyBits() {return (playerDetails & 0b011000) >> 3;} // private accessors isolate the bits representing each member
    unsigned char genderBits() {return (playerDetails & 0b100000) >> 5;}

public:
    // member values in this section are the only ones determined by constructor input
    // all others are assigned to default values
    Player() = default; // default constructor, included for sake of good practice
    Player(std::string n, std::string tn, int pNum, int diff, Gender gender) // "main" constructor, takes input for all the above members and assigns accordingly
    : name(n), townName(tn), playerDetails(pNum | diff << 3 | gender << 5) {}

    std::string getName() {return name;}
    std::string getTownName() {return townName;}

    // public accessors convert member values from bits back to higher-level types for public access
    short int getPlayerNum() {return static_cast<short int>(getPlayerNum());}
    short int getDifficulty() {return static_cast<short int>(difficultyBits());}
    Gender getGender() {return static_cast<Gender>(genderBits());}

private:
    /// second set of members denoted in the abstraction details - in-game stats
    // all the variables that the gameplay and game mechanics center around (aside of score and endgame stuff)
    // initialized to default values at construction, values get modified indirectly through events and decisions (functions defined in next section)

    int gold = STARTING_GOLD; // amount of gold in the player's town treasury. gold is used to buy assets and pay expenses
    short int year = STARTING_YEAR; // current in-game year, increases by one at the end of each turn

    // data structure to represent buyable assets in the player's town, consists of the owned quantity and the cost in gold to buy more
    struct Asset
    {
        int owned;
        short int price;
    };

    // helper function does basic process of "buying" an asset in the game
    void buy(Asset product, int quantity)
    {
        product.owned += quantity;
        gold -= quantity * product.price;
    } // intended for indirect usage (called by other member functions in the public access)

    // essential commodities that can be bought, sold or consumed in bulk
    Asset grain = {STARTING_GRAIN, GRAIN_PRICE}; // grain is required to feed the town's population, which can grow or starve depending on the amount it gets access to
    Asset land = {STARTING_LAND, LAND_PRICE}; // land is needed for

    // public works: high-value non-disposable assets that serve specialized purposes
    Asset marketplace = {0, MARKET_PRICE}; // markets bring merchants to the town and generate revenue
    Asset mill = {0, MILL_PRICE}; // mills don't bring in new people but generate revenue
    Asset cathedral = {0, CATHEDRAL_PRICE}; // cathedrals bring clergy to the town
    Asset palace = {0, PALACE_PRICE}; // palaces bring nobles to the town
    // public works are also sources of tax revenue in addition to any income generated on their own
    short int totalBuildings() {return marketplace.owned + mill.owned + cathedral.owned + palace.owned;} // simple helper function returns total number of town buildings for taxation purposes

    // tax rates: determine revenue for the player's yearly income
    unsigned char salesTax = SALES_TAX; // generates revenue from: merchants, nobles, public works
    unsigned char incomeTax = INCOME_TAX; // nobles, public works
    unsigned char customsTax = CUSTOMS_TAX; // clergy, nobles, merchants, public works
    unsigned char taxJustice = TAX_JUSTICE; // measured on a scale of 1-4, determines strictness of enforcement of taxes, which affects revenue
    // no more than 100 values needed to represent each; can be stored in 8 bits to save memory

    // town populations
    short int serfs = STARTING_SERFS; // attracted by surplus grain distribution, form the majority of the population and the backbone of the town, produce yearly grain harvests but no tax revenue
    // affected heavily by births, deaths, and migration between turns unlike other people, good management of grain required to maintain population
    short int soldiers = STARTING_SOLDIERS; // can be drafted from the serf population, require yearly payments in gold, mainly used as part of the invasion mechanic
    short int merchants = STARTING_MERCHANTS; // attracted by markets, generate moderate amount of taxable customs and sales revenue
    short int clergy = STARTING_CLERGY; // attracted by cathedrals, generate moderate amount of taxable customs revenue
    short int nobles = STARTING_NOBLES; // attracted by palaces, generate large amounts of taxable revenue in all categories

    // functions and formulas to determine the amount of personal income created by taxes and assets each turn/year


public:
    // direct accessor functions for above members

    /// third set and fourth set of members - gameplay mechanics/decisions
    // functions representing in-game actions or events that can modify the values in the previous section
    // prototypes w/ descriptions of preconditions and/or effects for all functions below (detailed definitions written in the implementation file)


    /// last section - rank, scoring, and endgame conditions
    // two ways for the game to end: reaching the highest rank, with promotions determined based on a scoring system involving all the player's other stats
    // or dying, which happens in a more-or-less random year if the player hasn't won after a certain amount of years

    // rank/title/promotion system implementation:

    // death implementation:

};

#endif // PLAYER_HPP
