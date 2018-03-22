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
				- Justice: determines how strictly taxes are enforced on a scale of 1 to 4
				- Revenue formulas for taxes included in class for ease-of-use
			- Player's rank
				- Total of six possible values for ranks, players start at lowest one by default
				- More info in the bottom section

		- Functions encompassing decisions that players make during turns that can affect stats and gameplay:
			- Raise or lower taxes
                - Rates can be adjusted within a certain range that varies by category
                - Higher rates result in more income generated next turn but too much can
			- Release grain
				- Certain percentage of grain reserves has to be distributed each turn for serf consumption
				- More grain released means less death rate and higher population growth
				- Releasing too much means you'll deplete the reserves for future turns though
			- Buy assets
                - People can move into the town each turn for each building owned (details in prev. section)
			- Buy grain or land
            - Sell grain or land
                - Must keep a minimum amount
			- Draft serfs into the army
				- Convert of some of the serf population into soldiers
				- Increases military strength but less economic output / grain production
				- Drafting costs an amount of gold and soldiers must be paid each turn
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
            - 7 possible ranks, with the player starting at the lowest one
            - Each rank is attached to a title that gets displayed with the player's name in the game output
                - Male titles: Sir, Baron, Count, Marquis, Duke, Grand Duke, Prince, King
                - Female titles: Lady, Baroness, Countess, Marquise, Duchess, Grand Duchess, Princess, Queen
            - To rank up, a certain score must be reached
                - Score calculated with formula involving land, population, assets, and gold
            - The player wins the game once they reach the highest rank
            - If a certain amount of years pass before the player wins, they die and lose the game
                - Death year randomly chosen between a minimum and maximum value
                - (somewhat unsophisticated implementation, can get changed later)

        - Constant parameters and bounds for game behavior (defined out-of-class)
            - Default values for in-game stats (section 2)
            - Prices for buyable assets
            - Parameters for game formulas, including:
                - Tax revenue
                - Asset revenue
                - Price flunctuations
                - Grain demand
                - Birth and death rates
            - Limits on tax rates
            - Limits on grain release
            - Minimum amount of land a player has to keep whne selling
            - Minimum amount of gold a player can have before bankruptcy is called
            - Limits on how long the game can last before the player dies
            - Parameters for the scoring and ranking system

    - Summarized version of the above without details/clutter:
        - Identifying/Meta-Game Stats
            - Names
            - Difficulty
            - Gender
            - Player Number
        - Mechanical/In-Game Stats
            - Gold
            - Grain (quantity, price, demand)
            - Land (quantity, price)
            - Assets (categories, quantity, price, effects)
            - Taxes (rates, revenues)
            - Townspeople (categories, populations)
        - Doable actions
            - Adjust taxes
            - Buy/Sell land
            - Buy/Sell grain
            - Buy assets
            - Invade neighbors
            - Release grain (required)
            - Display stats
        - In-game events
            - Resource gain (income, harvest)
            - Resource loss (expenses, rats)
            - Price adjustments (grain, land)
            - Population changes (births, deaths, migration)
            - Bankruptcy
        - Game-ending conditions
            - Rank system
            - Score system
            - Death system

    - Class usage/interface guidelines
        - Proper class construction:
            - Valid string values for both player name and town name at the very least
            - Values for game difficulty and player gender can also be passed into the constructor (in that order)
                - Entered values must be within alloted range set by consts (enforced through assertions)
        - General preconditions should be followed for member functions
            - Game flow should be structured so that:
                - Void gameplay functions don't get called after endgame conditions are reached
                - Purchases can't be made while the player is still bankrupt
                - Population changes don't take effect before grain gets released
                - Assertions will be thrown in the case that these are not followed
            - Functions that call other functions are bound to the same preconditions
        - Program input for function parameters should be controlled to avoid passing out-of-range values
            - Anon namespace contains limits for most game-related values
            - helperFunctions.hpp file contains functions for input with range validation
        - Scoped structs and private member functions not intended to be called outside of the class definition
            - All instances of intended usage covered by public-access functions

*/

#include <string>
#include "helperFunctions.hpp" // rng and input functions
#include "parameters.hpp" // constant parameters

enum Gender {Male, Female}; // player gender represented with enum values to make higher-level usage easier

class Player
{
private:
    static int8 numPlayers; // measures total number of player objects, incremented with constructor, decremented with destructor

    /// first set of members denoted in the abstraction details - personal stats
    /// basic identifying info that's mostly independent from gameplay and game flow (aside of the difficulty member)
    /// member values here are initialized by constructor and don't have any methods for further mutation

    std::string name; // labels by which players are identified in program output
    std::string townName;

    int8 playerNum; // internal player designator (player 1, player 2, etc)
    int8 difficulty; // modifies value of in-game stats to make game easier or harder
    bool gender;
public:
    // member values in this section are the only ones determined by constructor input
    // all others are assigned to default values
    Player() = default; // default constructor, included for sake of good practice
    Player(std::string n, std::string tn, int8 diff, Gender gen) // "main" constructor, takes input for all the above members and assigns accordingly
    : name(n), townName(tn), playerNum(++numPlayers), difficulty(diff), gender(gen) {}
    Player(std::string n, std::string tn, int8 diff) // "abbreviated" constructor only takes input for name, townName, and difficulty, gender assigned randomly
    : name(n), townName(tn), playerNum(++numPlayers), difficulty(diff), gender(random(Male, Female)) {}
    Player(std::string n, std::string tn) // further-shortened version that only needs input for name and town name
    : name(n), townName(tn), playerNum(++numPlayers), difficulty(random(MIN_DIFFICULTY, MAX_DIFFICULTY)), gender(random(Male, Female)) {}
    ~Player() {--numPlayers;}

    // public accessor functions for usage in program output
    std::string getName() {return name;}
    std::string getTownName() {return townName;}

    int8 getPlayerNum() const {return playerNum;}
    // int8 getDifficulty() const {return difficulty;}
    Gender getGender() const {return static_cast<Gender>(gender);}

private:
    /// value by which some stats are modified based on game difficulty (very important to gameplay)
    float diffModifier() const {return DIFF_MODIFIERS[difficulty - 1];}
    // expenses and resource loss get multiplied, revenue and resource gain get divided

    /// second set of members denoted in the abstraction details - in-game stats
    /// all the variables that the gameplay and game mechanics center around (aside of score and endgame stuff)
    /// initialized to default values at construction, values get modified indirectly through events and decisions (functions defined in next section)

    // basic stuff that doesn't really fit into any other category
    int gold = STARTING_GOLD; // amount of gold in the player's town treasury. gold is used to buy assets and pay expenses
    int16 year = STARTING_YEAR; // current in-game year, increases by one at the end of each turn

    /// implementation for townspeople
    // townspeople all represented solely in terms of current population (no other relevant shared attributes to warrant making a struct)
    // implementation of unique serf and soldier behavior can be found in the game functions

    // the "main" people in the town, core to gameplay and game flow
    int16 serfs = STARTING_SERFS; // attracted by surplus grain distribution, form the majority of the population and the backbone of the town, produce yearly grain harvests but no tax revenue
    // affected heavily by births, deaths, and migration between turns unlike other people, good management of grain required to maintain population

    // wealthy taxpayer classes brought in by asset purchases, relatively generic behavior patterns
    int16 merchants = STARTING_MERCHANTS; // attracted by markets, generate moderate amount of taxable customs and sales revenue
    int16 clergy = STARTING_CLERGY; // attracted by cathedrals, generate moderate amount of taxable customs revenue
    int16 nobles = STARTING_NOBLES; // attracted by palaces, generate large amounts of taxable revenue in all categories

    /// implementation for goods
    // essential items that can be bought, sold, or consumed in bulk
    struct Commodity // data structure, consists of the owned quantity, the cost in gold to buy more on normal difficulty, and the displayed name of the good
    {
        int owned;
        int8 basePrice; // base prices can flunctuate, generally don't exceed 100-200
        const std::string name;
        // other in-game behavior varies, mainly covered in the game functions
    };
    // take difficulty into account for the "true" prices
    int16 getPrice(Commodity product) const {return product.basePrice * diffModifier();}

    // helper functions do basic processes of "buying" or selling a quantity of goods in the game
    // intended for indirect usage (called by other member functions in the public access)
    void buy(Commodity& product, int quantity);
    // pre: player object initialized, commodity parameter is member of object, function called by public member of object, valid quantity parameter greater than 0
    // post: increase commodity's owned quantity by quantity parameter, decrease gold by quantity times price, display results in program output to inform user
    // inform player if purchase would put them into debt and check if player has become bankrupt from purchase
    void sell(Commodity& product, int quantity);
    // pre: player object initialized, commodity parameter is member of object, function called by public member of object, valid quantity parameter greater than 0 and less than the owned product quantity
    // post: decrease commodity's owned quantity by quantity parameter, increase gold by quantity times price, display results in program output to inform user
    void adjustPrice(Commodity& product);
    // pre: player object intialized, commodity parameter is member of object
    // post: changes the price member of the commodity to a randomized value

    // grain implementation
    Commodity grain = {STARTING_GRAIN, GRAIN_PRICE, "grain"}; // grain is required to feed the town's population, which can grow or starve depending on the amount it gets access to
    int releasedGrain = 0; // how much of the grain reserves the player distributes to the townspeople, set to half the starting grain for the first turn

    // land implementation
    Commodity land = {STARTING_LAND, LAND_PRICE, "land"}; // land is needed for

    // soldiers are part of the population on an abstract level but are implemented as commodities because they can be bought
    Commodity soldiers = {STARTING_SOLDIERS, SOLDIER_COST, "soldiers"}; // require yearly payments in gold, mainly used as part of the invasion mechanic

    /// implementation for assets
    // set of high-value in-game investments that serve similar, generic purposes of attracting tax-paying citizens and/or generating monthly revenu
    struct Asset // data structure, consists of base prices, owned quantity, displayed name, and behavior parameters
    {
        int16 owned;
        const int16 basePrice; // asset prices tend to be constant
        const std::string name;

        // yearly revenue generation
        const int16 minRevenue;
        const int16 maxRevenue; // randomly decided between two limits

        // population growth per turn per owned building
        const int8 merchantsAttracted; // represent maximum values (can be 0)
        const int8 clergyAttracted;
        const int8 noblesAttracted;
    };
    // take difficulty into account for the "true" prices
    int16 getPrice(Asset building) const {return building.basePrice * diffModifier();}

    // buy function similar to one found above that conducts addtional operations shared among asset objects
    void buy(Asset& building);
    // pre: player object initialized, asset parameter is member of object, function called by public member of object, player isn't dead, game hasn't ended
    // post: increase asset's owned quantity by one and deduct the price from the player's gold, increase player's population members based on asset attributes, display results in program output
    // inform player if purchase would put them into debt and check if player has become bankrupt from purchase

    // function to bring new people into the town each turn from assets
    void attractCitizens(Asset building);
    // pre: player object intialized, asset parameter is member of object, player isn't dead, game hasn't ended
    // post: increase populations of taxpayers in the town based on asset members, display results in program output

    int getRevenue(Asset building) const // helper function for getting yearly revenue generated by building category in the town
    {return building.owned * random(building.minRevenue, building.maxRevenue) / diffModifier();}

    // public works are also sources of tax revenue in addition to any income generated on their own
    int16 totalAssets() const
    {return marketplace.owned + mill.owned + cathedral.owned + palace.owned;} // simple helper function returns total number of town buildings for taxation purposes

    // current implemented types
    Asset marketplace = {0, MARKET_PRICE, "market", MIN_MARKET_REVENUE, MAX_MARKET_REVENUE, MARKET_MERCHANTS, 0, 0}; // markets bring merchants to the town and generate revenue
    Asset mill = {0, MILL_PRICE, "mill", MIN_MILL_REVENUE, MAX_MILL_REVENUE, 0, 0, 0}; // mills don't bring in new people but generate revenue
    Asset cathedral = {0, CATHEDRAL_PRICE, "cathedral", 0, 0, 0, CATHEDRAL_CLERGY, 0}; // cathedrals bring clergy to the town
    Asset palace = {0, PALACE_PRICE, "palace", 0, 0, 0, 0, PALACE_NOBLES}; // palaces bring nobles to the town

    /// implementation for taxes
    // data structure consisting of all the relevant attributes in a tax
    struct Tax
    {
        int8 rate; // percentage of taxable revenue that gets collected, only member intended to be modified after initialization

        const int8 merchantRevenue; // other members serve to help determine the total taxable quantity
        const int8 clergyRevenue; // for purposes of calculating tax income
        const int8 nobleRevenue;
        const int8 assetRevenue;
    };

    // int8 taxJustice = TAX_JUSTICE; // measured on a scale of 1-4, determines strictness of enforcement of taxes, which affects revenue (currently not implemented)

    // helper function with basic formula to determine the income generated by a tax within a year
    int getRevenue(Tax tax) const;
    // pre: player object initialized, tax object is member of player object
    // post: return yearly revenue generated by tax as calculated based on tax rates and targets

    // and to change the tax rate
    void adjustRate(int8& oldRate, int8 newRate, int8 minRate, int8 maxRate);
    // pre: player object initialized, tax object is member of player object, min rate is lower than max rate, old rate and new rate inside of rate range
    // post: change the rate member of the tax to the value of the parameter, throws error if outside of range (in case it escapes input validation) (no program output)
    void adjustRate(int8& oldRate, int8 newRate) {oldRate = newRate;}
    // alt version of the above with no contract enforcement, easier to use the but less safe

    // tax categories implemented in the game
    Tax salesTax = {SALES_TAX, MERCHANT_SALES, CLERGY_SALES, NOBLE_SALES, ASSET_SALES}; // generates revenue from: merchants, nobles, public works
    Tax incomeTax = {INCOME_TAX, MERCHANT_INCOME, CLERGY_INCOME, NOBLE_INCOME, ASSET_INCOME}; // nobles, public works
    Tax customsTax = {CUSTOMS_TAX, MERCHANT_CUSTOMS, CLERGY_CUSTOMS, NOBLE_CUSTOMS, ASSET_CUSTOMS}; // clergy, nobles, merchants, public works

    // relative taxation rates compared to starting values
    float taxLevel() // used to calculate adverse effects on migration
    {return static_cast<float>(salesTax.rate + incomeTax.rate + customsTax.rate) / (SALES_TAX + INCOME_TAX + CUSTOMS_TAX);}

public:
    /// functions for public read-only access to above members

    int getGold() const {return gold;} // gold
    int16 getYear() const {return year;} // current year

    // populations
    int16 getSerfs() const {return serfs;}
    int16 getMerchants() const {return merchants;}
    int16 getClergy() const {return clergy;}
    int16 getNobles() const {return nobles;}
    int16 getSoldiers() const {return soldiers.owned;}

    // soldier prices
    int16 getSoldierPrice() const {return getPrice(soldiers);} // purchase cost
    int16 getSoldierPay() const {return SOLDIER_PAY * diffModifier();} // yearly upkeep (per soldier)

    // commodity quantities
    int getGrain() const {return grain.owned;}
    int getLand() const {return land.owned;}

    // and prices
    int16 getGrainPrice() const {return getPrice(grain);}
    int16 getLandPrice() const {return getPrice(land);}

    // asset quantites
    int16 getMarkets() const {return marketplace.owned;}
    int16 getMills() const {return mill.owned;}
    int16 getCathedrals() const {return cathedral.owned;}
    int16 getPalaces() const {return palace.owned;}

    // and prices
    int16 getMarketPrice() const {return getPrice(marketplace);}
    int16 getMillPrice() const {return getPrice(mill);}
    int16 getCathedralPrice() const {return getPrice(cathedral);}
    int16 getPalacePrice() const {return getPrice(palace);}

    // tax rates
    int16 getSales() const {return salesTax.rate;}
    int16 getIncome() const {return incomeTax.rate;}
    int16 getCustoms() const {return customsTax.rate;}
    // int8 getJustice() const {return taxJustice;}


    /// third set and fourth set of members - gameplay mechanics/decisions
    /// public-access functions representing in-game actions or events that can modify the values in the previous section
    /// most functions here consist of direct, in-line calls of private helper functions, all detailed definitions can be found in the implementation file

    // buying and selling goods
    void buyGrain(int16 quantity) {buy(grain, quantity);}
    void sellGrain(int16 quantity) {sell(grain, quantity);}
    void buyLand(int16 quantity) {buy(land, quantity);}
    void sellLand(int16 quantity) {sell(grain, quantity);}
    // and buying soldiers
    void buySoldiers(int16 quantity) {buy(soldiers, quantity);}
    // and buying buildings
    void buyMarket() {buy(marketplace);}
    void buyMill() {buy(mill);}
    void buyPalace() {buy(palace);}
    void buyCathedral() {buy(cathedral);}

    // adjusting taxes
    void adjustSales(int8 newRate) {adjustRate(salesTax.rate, newRate, MIN_TAX, MAX_SALES_TAX);}
    void adjustIncome(int8 newRate) {adjustRate(salesTax.rate, newRate, MIN_TAX, MAX_INCOME_TAX);}
    void adjustCustoms(int8 newRate) {adjustRate(salesTax.rate, newRate, MIN_TAX, MAX_CUSTOMS_TAX);}
    // void adjustJustice(int8 newVal) {adjustRate(taxJustice, newVal, MIN_TAX_JUSTICE, MAX_TAX_JUSTICE);}

    // invasion
    void invade(Player* opponent);
    // pre: two player objects initialized, neither player dead, game hasn't ended
    // post: both players lose a random number of soldiers, invading player has chance to take land from opponent, results displayed in program output

    // releasing grain
    int grainDemand() const {return serfs * GRAIN_DEMAND * diffModifier();} // how much grain is needed to be released to feed the population
    int minRelease() {return (grain.owned * percent(MIN_GRAIN_RELEASE));}
    int maxRelease() {return (grain.owned * percent(MAX_GRAIN_RELEASE));} // limits on how much grain the player can release (put in public access for usage in program output)
    void releaseGrain(int quantity);
    // pre: player object initialized, valied quantity parameter between min and max percentage of releasable grain, player isn't dead, game hasn't ended
    // post: deducts the parameter member from the player's grain stash and adds it to the stockpile of released grain

    void printStats();
    // pre: player object initialized
    // post: display all data values relevant to player's in-game performance (grain, gold, land, populations, assets) in program output

    // go bankrupt
    bool isBankrupt() {return gold < BANKRUPTCY_LIMIT;} // check if the player is bankrupt
    void bankruptcy();
    // pre: player object initialized, player gold lower than bankruptcy limit, player isn't dead, game hasn't ended
    // post: if player is bankrupt, player loses all assets in the town and gold gets reset to certain value

private:
    /// set of functions (w/ helper formulas) that always get called after every turn as part of the game flow
    /// results can depend on player actions during the turn

    // population changes
    int getSerfBirths() const;
    // pre: player object initialized, more than 0 grain released (preferably)
    // post: calculate number of serfs projected to be born in player's town following their turn based on randomized formula involving population and grain release
    int getSerfDeaths() const;
    // pre: player object initialized, more than 0 grain released (preferably)
    // post: calculate number of serfs projected to die in player's town following their turn based on randomized formula involving population and grain release
    int getSerfMigration() const;
    // pre: player object initialized, more than 0 grain released (preferably)
    // post: calculate number of serfs projected to move to player's town following their turn based on randomized formula involving grain release
    void populationChange();
    // pre: player object initialized, game hasn't ended for player yet, more than 0 grain released
    // post: take all changes in serf and other populations into effect, display the results and reset released grain to 0

    // receive revenues from taxes
    int getSalesRevenue() const {return getRevenue(salesTax);} // calculate individual revenues
    int getIncomeRevenue() const {return getRevenue(customsTax);}
    int getCustomsRevenue() const {return getRevenue(customsTax);}
    void receiveTaxRevenue();
    // pre: player object initalized, game hasn't ended yet for player
    // post: call functions to calculate revenue generated from taxation, add to treasury and display results in program output

    // and from assets
    int getMarketRevenue() const {return getRevenue(marketplace);} // calculate individual revenues
    int getMillRevenue() const {return getRevenue(mill);}
    void receiveAssetRevenue();
    // pre: player object initialized, game hasn't ended yet for player
    // post: call functions to calculate revenue generated from town buildings, add to treasury and display results in program output

    // receive harvest
    int getHarvest();
    // pre: player object initialized,
    // post: calculate amount of grain that the player receives between turns based on formula involving serf populations and randomized variables
    void receiveHarvest();
    // pre: player object initialized, game hasn't ended yet for player
    // post: call the getHarvest function to determine an amount of grain to add to the player's reserves, display results in program output

    // pay expenses
    int armyPay() {return getSoldiers() * getSoldierPay();} // calculate amount of gold needed to pay army each turn
    void paySoldiers();
    // pre: player object initialized, game hasn't ended yet for player
    // post: deduct army upkeep from treasury and display results in program output, check if player has become bankrupt from expenses

    // lose resources
    int8 getGrainLoss() {return random(MIN_GRAIN_LOSS, MAX_GRAIN_LOSS) * diffModifier();}
    void loseGrain();
    // pre: player object initialized, game hasn't ended yet for player
    // post: calculate random percentage of player's grain reserves to get lost between turns, deduct, and display results in program output

    // commodity prices change
    void adjustGrainPrice() {adjustPrice(grain);}
    void adjustLandPrice() {adjustPrice(land);}


    /// last section - rank, scoring, and endgame implementation
    /// two ways for the game to end for a player: reaching the highest rank (relies on a scoring system to measure player performance) or dying (more-or-less random)
    /// all void member functions bound to the precondition that the player hasn't reached either ending condition yet

    // internal status measurements
    int8 rankIndex = 0; // player rank stored internally as a number corresponding to an index in the const vector of rank structs (see namespace)
    int16 deathYear = STARTING_YEAR + random(MIN_LIFESPAN, MAX_LIFESPAN); // game ends for the player in a random in-game year between two parameter limits if they haven't won yet

public:
    // all public access to rank data
    std::string getTitle() const;
    // pre: player object initialized, valid value for player gender
    // post: return the title of the player's gender attached to the their in-game rank for program output
    int getScore() const; // score used to determine increases in rank
    // pre: player object initialized
    // post: return the player's game score as determined by a formula involving all of their other stats
    bool getPromotion() const;
    // pre: player object initialized
    // post: checks if player's score is higher than the threshold to reach the next rank, return the results
    void promote();
    // pre: player object initialized, game hasn't ended for player yet
    // post: increases the player's rank index if they meet the score requirements, display results in program output

    // endgame conditions (see above)
    bool won() const {return rankIndex >= MAX_RANK;}
    bool dead() const {return year >= deathYear;}
    bool gameEnded() const {return won() || dead();}

    /// combined super-function that comprises of all functions scheduled to get called between game turns
    /// as well as advancing to the next in-game year and checking if the game has ended (critical to game flow)
    void turnResults();
    // pre: player object initialized, game started, player isn't dead and game hasn't ended (yet), at least one turn finished
    // post: calls all functions for events that take place after a player's turn, increments the year, and checks for endgame conditions, all results displayed in program output
};

#endif // PLAYER_HPP
