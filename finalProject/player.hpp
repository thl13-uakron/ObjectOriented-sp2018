 #ifndef PLAYER_HPP
#define PLAYER_HPP

/*
Player class abstraction details (copied from README.md file)

    - The player is the ruler of a city-state in 1400s Spain
        - Town stats included with player stats


    - On a high level, members can be split into four categories:

		- Member vars for personal stats; chosen at start of game, entered into class constructor, and remain unchanged:
			- Their name
			- The name of their town
			- Their gender
                - Male or Female
			- Game difficulty
                - 4 possible difficulty levels
			- Which player they are (p1, p2, etc.)
                - Maximum of 6 players allowed in a single game

		- Stats representing the town's status; set to default values with object construction and change with each turn and player decision:
			- Grain reserves, price, and demand
			- Land owned, land prices
			- Gold in treasury
			- TownÍs population, in the following categories:
				- Serfs
				- Clergy
				- Noblemen
				- Soldiers
			- Town buildings and assets owned, in the following categories:
				- Marketplaces
				- Mills
				- Palaces
				- Cathedrals
			- Tax rates, in the following categories:
				- Customs
				- Sales
				- Income
				- Justice
			- Player's rank
				- Total of six possible values for ranks, players start at lowest one by default

		- Functions that get called between turns to modify some of the above values based on others:
			- Receive tax revenue
				- Depends on tax rates
				- Personal wealth increases
			- Receive grain harvest
				- Depends on serf population
				- Increases size of grain reserves
			- Economy grows or shrinks
				- Higher population can lead to higher economic growth
				- Results in more wealth for the townspeople and more tax revenue in the future
				- High taxes can restrict current economic growth though
			- People die, get born, immigrate, or leave
				- Depends on grain released
				- Serfs will move in if more grain released than minimum
				- Can increase or decrease population size
			- Grain eaten by rats
				- Grain reserves decreased certain percentage each turn.
			- Pay expenses
				- Upkeep for army and assets
			- (Higher difficulty increases death rates, prices, and resource loss while decreasing revenue and growth rates)

		- Same as previous, but not guaranteed to happen each turn:
			- Invaded by neighboring nation
				- Triggered by other players, doesnÍt happen in single-player
				- Can lose land or soldiers depending on amount of soldiers in your army and the opponentÍs
			- Assets seized to pay debts
				- Can happen if wealth less than 0
			- Promoted
				- Rank increases by one if certain conditions are met
				- Each rank promotion contains a requirement for
				- Win game if max rank reached
			- Die of certain causes
				- Natural causes random but chance increases in later years
				-
				- Results in game over

		- Functions encompassing decisions that players make during turns that can affect the above members and functions:
			- (Mostly self-explanatory in above context)
			- Raise or lower taxes
			- Release grain
				- Certain percentage of grain reserves has to be distributed each turn serf consumption
				- More grain released means less death rate and higher population growth
				- New serfs will also move in if you have enough grain
				- Releasing too much means youÍll deplete the reserves for future turns though
			- Buy assets
			- Buy grain or land
			- Draft serfs into the army
				- Convert of some of the serf population into soldiers
				- Increases military strength but less economic output
			- View status and standings (no effect on game itself)
				- View ranks, wealth, population sizes, and land owned for each player
			- Invade other playerÍs city-state
				-
				- Only possible in multiplayer when other players are available to invade

*/

namespace
{
    /// set of default values to initialize player in-game stats to
    // values not accounted for here are assumed to start at 0

    // general stats
    const int STARTING_YEAR = 1400;
    const int STARTING_GOLD = 1000;
    const int STARTING_RANK = 1;

    // prices and starting supply of buyable assets
    const int STARTING_GRAIN = 10000;
    const int GRAIN_PRICE = 10;
    const int STARTING_LAND = 10000;
    const int LAND_PRICE = 10;
    const int MARKET_PRICE = 1000;
    const int MILL_PRICE = 2000;
    const int PALACE_PRICE = 3000;
    const int CATHEDRAL_PRICE = 5000;

    // default populations
    const int STARTING_SERFS = 2000;
    const int STARTING_SOLDIERS = 25;
    const int STARTING_MERCHANTS = 25;
    const int STARTING_CLERGY = 5;

    // default tax rates
}

enum Gender {Male, Female}; // player gender represented with enum values to make higher-level usage easier

class Player
{
private:
    /// first set of members denoted in the abstraction details
    // "personal" stats: basic identifying info that's mostly independent from gameplay and game flow (aside of the difficulty member)
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
    int getPlayerNum() {return static_cast<int>getPlayerNum();}
    int getDifficulty() {return static_cast<int>difficultyBits();}
    Gender getGender() {return static_cast<Gender>genderBits();}

private:
    /// second set of members denoted in the abstraction details
    // "in-game" stats: all the variables that the gameplay and game mechanics center around
    // initialized to default values at construction, values get modified indirectly through events and decisions (functions defined in later sections)

    int gold = STARTING_GOLD; // amount of gold in the player's town treasury. gold is used to buy assets and pay expenses
    int year = STARTING_YEAR; // current in-game year, increases by one at the end of each turn

    // data structure to represent buyable assets in the player's town, consists of the owned quantity and the cost in gold to buy more
    struct Asset
    {
        int owned;
        int price;
    };

    // helper function does basic process of "buying" an asset in the game
    void buy(Asset product, int quantity)
    {
        product.owned += quantity;
        gold -= quantity * product.price;
    } // intended for indirect usage (called by other member functions in the public access)

    // essential commodities that can be bought, sold or consumed in bulk
    Asset grain = {STARTING_GRAIN, GRAIN_PRICE}; // grain is required to feed the town's population, which can grow or starve depending on the amount it gets access to
    Asset land = {STARTING_LAND, LAND_PRICE}; // land is needed

    // public works, high-value non-disposable assets that serve specialized purposes
    Asset marketplace = {0, MARKET_PRICE};
    Asset mill = {0, MILL_PRICE};
    Asset cathedral = {0, CATHEDRAL_PRICE};
    Asset palace = {0, PALACE_PRICE};

    // tax rates


public:
    //


};

#endif // PLAYER_HPP
