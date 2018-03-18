#ifndef PLAYER_CPP
#define PLAYER_CPP

#include <iostream>
#include "player.hpp"

int8 Player::numPlayers = 0; // static variable definition

/// function definitions for all non-inline player members
/// (see class def for protoypes and inline defs
/// as well as pre- and post-conditions)

/// buyable goods and assets

void Player::buy(Commodity product, int quantity)
{
    // enforce general void preconditions
    if (gameEnded())
        throw std::logic_error("Error: Game functions being called after endgame conditions already reached.");

    // handling for negative quantities
    if (quantity < 0) sell(product, quantity);

    int totalCost = quantity * getPrice(product); // get cost of purchase

    if (gold > totalCost || // do quick check to see if player can afford purchase, ask if they'd like to proceed in case that it puts them in debt
        ynInput("You're buying more than you can afford. Proceed with purchase anyways? (y\n) ", 'y', 'n'))
    {
        // continue with purchase
        product.owned += quantity;
        gold -= totalCost;

        // display results in program output
        std::cout << name << " buys " << quantity << ' ' << product.name
                  << " for " << gold << " gold.\n";

        // check if the purchase has resulted in bankruptcy, act accordingly
        if (isBankrupt()) bankruptcy();
    }
}

void Player::sell(Commodity product, int quantity)
{
    // enforce general void preconditions
    if (gameEnded())
        throw std::logic_error("Error: Game functions being called after endgame conditions already reached.");

    // handling for negative and excessive quantities
    if (quantity < 0) buy(product, quantity);
    if (quantity > product.owned)
        throw std::logic_error("Error: Calling game functions with out-of-range parameters.");

    int earnings = getPrice(product) * quantity; // get amount of gold earned from sale

    // proceed with sale
    product.owned -= quantity;
    gold += earnings;

    // display results
    std::cout << name << " sells " << quantity << ' ' << product.name
              << " for " << earnings << " gold.\n";
}

void Player::adjustPrice(Commodity product)
{
    // enforce general void preconditions
    if (gameEnded())
        throw std::logic_error("Error: Game functions being called after endgame conditions already reached.");

    //
}

void Player::buy(Asset building)
{
    // enforce general void preconditions
    if (gameEnded())
        throw std::logic_error("Error: Game functions being called after endgame conditions already reached.");

    int totalCost = getPrice(building); // get cost of purchase

    if (gold > totalCost || // do quick check to see if player can afford purchase, ask if they'd like to proceed in case that it puts them in debt
        ynInput("You're buying more than you can afford. Proceed with purchase anyways? (y\n) ", 'y', 'n'))
    {
        // continue with purchase
        ++building.owned;
        gold -= totalCost;

        // display results in program output
        std::cout << name << " buys a "<< building.name
                  << " for " << gold << " gold.\n";

        // get population changes caused by purchase by category
        int8 newMerchants = random(building.merchantsAttracted);
        int8 newNobles= random(building.noblesAttracted);
        int8 newClergy = random(building.clergyAttracted);

        // take changes into effect
        merchants += newMerchants;
        clergy += newClergy;
        nobles += newNobles;

        // display results in program output
        if (building.merchantsAttracted > 0) std::cout << newMerchants << " merchants move to " << townName << ".\n";
        if (building.clergyAttracted > 0) std::cout << newClergy << " clergy move to " << townName << ".\n";
        if (building.merchantsAttracted > 0) std::cout << newNobles << " nobles move to " << townName << ".\n";

        // check if purchase has resulted in bankruptcy, act accordingly
        if (isBankrupt()) bankruptcy();
    }
}


/// taxes - helper functions

void Player::adjustRate(int8& oldRate, int8 newRate, int8 minRate, int8 maxRate)
{
    // enforce general void preconditions
    if (gameEnded())
        throw std::logic_error("Error: Game functions being called after endgame conditions already reached.");

    if (maxRate < minRate) throw std::logic_error("Error: Calling ranged function with min parameter greater than max parameter."); // enforce precondition contracts
    if (newRate < minRate || newRate > maxRate) throw std::logic_error("Error: Adjusting tax rates to value outside of allowed range.");

    oldRate = newRate; // if no errors triggered, proceed with assignment
}


/// in-game actions and misc events

void Player::invade(Player* opponent)
{
    // enforce general void preconditions
    if (gameEnded() || opponent->gameEnded())
        throw std::logic_error("Error: Game functions being called after endgame conditions already reached.");

    //
}

void Player::releaseGrain(int quantity)
{
    // enforce general void preconditions
    if (gameEnded())
        throw std::logic_error("Error: Game functions being called after endgame conditions already reached.");
    // and function preconditions (range)
    if (quantity < (grain.owned * MIN_GRAIN_RELEASE / 100.0) || quantity > (grain.owned * MAX_GRAIN_RELEASE / 100.0))
        throw std::logic_error("Error: Calling game function with out-of-range parameters.");

    // proceed to "move" grain from reserves to released pile
    grain.owned -= quantity;
    releasedGrain += quantity;

    // display results
    std::cout << name << " distibutes " << quantity
              << " grain to the citizens of " << townName << " for consumption.\n";
}

void Player::printStats()
{
    // function can be called after game ends, no preconditions need to be enforced

    // display full player title
    std::cout << getTitle() << ' ' << name << " of " << townName;
    if (won()) {std::cout << " (WINNER)";}
    else if (dead()) {std::cout << " (deceased)";} // small indicator for if player is out of the game
    std::cout << ": \n"; // formatting

    // display player stats by category
    std::cout << "Core stats: " << gold << " Gold, " << getGrain() << " Grain, " << getLand() << " Land.\n"
              << "Population: " << getSerfs() << " Serfs, " << getSoldiers() << " Soldiers, " << getMerchants() << " Merchants, " << getClergy() << " Clergy, " << getNobles() << " Nobles.\n"
              << "Assets: " << getMarkets() << " Markets, " << getMills() << " Mills, " << getCathedrals() << " Cathedrals, " << getPalaces() << " Palaces.\n"
              << "Total Score: " << getScore() << "\n\n";
}

void Player::bankruptcy()
{
    // enforce general void preconditions
    if (gameEnded())
        throw std::logic_error("Error: Game functions being called after endgame conditions already reached.");

    // check if player is considered bankrupt, proceed with bankruptcy process if yes
    if (isBankrupt())
    {
        // calculate randomized portion of player's assets to get "seized by creditors" (removed from game)
        int16 marketsSeized = random(marketplace.owned);
        int16 millsSeized = random(mill.owned); // simple formula involving a ranged random number
        int16 cathedralsSeized = random(cathedral.owned); // variable assignment to keep consistent values
        int16 palacesSeized = random(palace.owned);
        // and deduct
        marketplace.owned -= marketsSeized;
        mill.owned -= millsSeized;
        cathedral.owned -= cathedralsSeized;
        palace.owned -= palacesSeized;

        // in exchange for restoring their gold to a positive amount
        gold = BANKRUPTCY_BENEFITS;

        // display results in program output to inform user of event
        std::cout << name << " has gone bankrupt from excessive debt.\n"
                  << "Creditors in " << townName << " seize "
                  << marketsSeized << " markets, " << millsSeized << " mills, "
                  << cathedralsSeized << " cathedrals, and " << palacesSeized << " palaces to bail them out.\n";
    }
}


/// population changes

int16 Player::getSerfBirths() const
{
    // formula:
}

int16 Player::getSerfDeaths() const
{
    // formula:
}

int16 Player::getSerfMigration() const
{
    // formula:
}

void Player::populationChange()
{
    // get values for changes in serf population by cause
    int16 serfBirths = getSerfBirths();
    int16 serfDeaths = getSerfDeaths(); // variable assignments to keep consistent values in case of randomness
    int16 serfMigration = getSerfMigration();

    // take changes into effect and display results in program output
    // for births
    serfs += serfBirths;
    std::cout << serfBirths << " serfs are born in " << townName << ".\n";
    // for deaths
    serfs -= serfDeaths;
    std::cout << serfDeaths << " serfs in " << townName << " die.\n";
    // and for migration
    serfs += serfMigration;
    std::cout << serfMigration << " serfs move to " << townName << ".\n";

    releasedGrain = 0; // reset released grain using it to calculate changes
}


/// revenue and expenses

int16 Player::getRevenue(Tax tax) const
{
    return (static_cast<float>(tax.rate / 100.0) // get the percentage as a decimal
    * ((tax.merchantRevenue * merchants)
    + (tax.clergyRevenue * clergy) // factor in taxable wealth in town
    + (tax.nobleRevenue * nobles)
    + (tax.assetRevenue * totalAssets())
    + (taxJustice)) // i'm still trying to figure out how tax justice is supposed to get factored in
    / diffModifier()); // take difficulty into account
}

void Player::receiveTaxRevenue()
{
    // get values for tax revenue by category
    int16 salesRevenue = getSalesRevenue();
    int16 incomeRevenue = getIncomeRevenue(); // variable assignments to keep consistent values in case of randomness
    int16 customsRevenue = getCustomsRevenue();

    // take changes into effect and display results in program output
    // for sales
    gold += salesRevenue;
    std::cout << salesRevenue << " gold received from sales taxes.\n";
    // for income
    gold += incomeRevenue;
    std::cout << incomeRevenue << " gold received from income taxes.\n";
    // and for customs
    gold += customsRevenue;
    std::cout << customsRevenue << " gold received from customs duties.\n";
}

// int16 getRevenue(Asset building) const {return building.owned * random(building.minRevenue, building.maxRevenue) / diffModifier();}
void Player::receiveAssetRevenue()
{
    // get values for asset revenue by category
    int16 marketRevenue = getMarketRevenue();
    int16 millRevenue = getMillRevenue(); // variable assignments to keep consistent values in case of randomness

    // take changes into effect and display results in program output
    // for sales
    gold += marketRevenue;
    std::cout << marketRevenue << " gold earned by " << townName << "'s markets.\n";
    // for income
    gold += millRevenue;
    std::cout << millRevenue << " gold earned by " << townName << "'s mills.\n";
}

void Player::paySoldiers()
{
    // calculate and deduct expenses
    int16 pay = getSoldierPay(); // variable assignment to keep consistent values in case of randomness
    gold -= pay;

    // display results
    std::cout << pay << " gold paid to" << townName << "'s standing army.\n";

    // check if payments have caused bankruptcy
    if (isBankrupt()) bankruptcy();
}


/// resource gain and loss

int16 Player::getHarvest()
{
    // formula:
}

void Player::receiveHarvest()
{
    // get amount of grain to receive
    int harvest = getHarvest();

    // take changes into effect, display results;
    grain.owned += harvest;
    std::cout << harvest << " grain harvested by " << townName << "'s serfs.\n";
}

// int8 getGrainLoss() {return random(20, 40) * diffModifier();}
void Player::loseGrain()
{
    // get percentage of current grain to deduct
    int8 grainLoss = getGrainLoss();

    // take changes into effect, display results;
    grain.owned -= grain.owned * (grainLoss / 100.0); // subtract as percentage
    std::cout << grainLoss << " percent of " << townName << "'s existing grain reserves lost to various causes.\n";
}


/// scoring and ranking

std::string Player::getTitle() const
{
    // check player gender before returning appropriate title
    switch (getGender())
    {
    case Male:
        return RANKS[rankIndex].mTitle; // male title for male player
    case Female:
        return RANKS[rankIndex].fTitle; // female title for female player
    default:
        throw std::logic_error // throw exception not returned by this point
        ("Error: Detected invalid value for player's gender.");
    }
}

int Player::getScore() const
{
    //
}

bool Player::getPromotion() const
{
    // compare current score with score required to reach next rank
    return getScore() > RANKS[rankIndex + 1].scoreReq;
}

void Player::promote()
{
    // enforce general void preconditions
    if (gameEnded())
        throw std::logic_error("Error: Game functions being called after endgame conditions already reached.");

    if (getPromotion()) // check if player meets score conditions
    {
        ++rankIndex; // do the actual promotion

        // display results of promotion in program output
        std::cout <<  "Thanks to their diligent leadership of "
        << townName << ", " << name
        << " has attained the rank of "
        << getTitle() << "!\n";
    }
}

/// the big post-turn function
void Player::turnResults()
{
    // enforce general void preconditions
    if (gameEnded())
        throw std::logic_error("Error: Game functions being called after endgame conditions already reached.");

    // program output header
    std::cout << "Annual report for " << getTitle() << ' ' << name
    << " of " << townName << ": Year " << year << "\n\n";

    // take all the functions scheduled to get called after a player's turn
    // and call all of them by category

    std::cout << "Census: \n";
    populationChange();

    std::cout << "\nFinances: \n";
    receiveTaxRevenue();
    receiveAssetRevenue();
    paySoldiers();

    std::cout << "\nResources: \n";
    receiveHarvest();
    loseGrain();

    std::cout << "\nEconomy: \n";
    adjustGrainPrice();
    adjustLandPrice();
    std::cout << "\n"; // formatting

    // check if player should get promoted following stat changes¦°hgž
    if(getPromotion()) promote();

    // increment the in-game year in anticipation of the next turn
    ++year;

    // check if ending conditions have been reached following promotion and/or year change, display
    // actual handling to be done in game flow functions
    if (won())
    { // if you win and die in the same turn, victory takes precedence
        std::cout << "\n" << getTitle() << ' ' << name << " of " << townName
        << " has won the game by achieving the highest rank!\n"
        << "Congratulations, " << name << "!\n";
    }
    else if (dead())
    {
        std::cout << "\nAfter ruling " << townName
        << " for " << year - STARTING_YEAR << " years, "
        << getTitle() << name << " has died.\n"
        << "Thanks, for playing, " << name << "!\n";
    }

    std::cout << "\n"; // formatting
}


#endif // PLAYER_CPP
