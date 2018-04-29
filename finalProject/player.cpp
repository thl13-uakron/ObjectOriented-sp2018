#ifndef PLAYER_CPP
#define PLAYER_CPP

#include <iostream>
#include "player.hpp"

int8 Player::numPlayers = 0; // static variable definition

/// function definitions for all non-inline player members
/// (see class def for protoypes and inline defs
/// as well as pre- and post-conditions)

/// buyable goods and assets

void Player::buy(Commodity& product, int quantity)
{
    // enforce general void preconditions
    if (gameEnded())
        throw std::logic_error("Error: Game function buy() being called after endgame conditions already reached.");

    // handling for negative quantities
    if (quantity < 0) sell(product, quantity);

    int totalCost = quantity * getPrice(product); // get cost of purchase

    if (gold > totalCost || quantity == 0 || // do quick check to see if player can afford purchase, ask if they'd like to proceed in case that it puts them in debt
        ynInput("You're buying more than you can afford. Proceed with purchase anyways? (y/n) ", 'y', 'n'))
    {
        // continue with purchase
        product.owned += quantity;
        gold -= totalCost;

        // display results in program output
        std::cout << name << " buys " << quantity << ' ' << product.name
                  << " for " << totalCost << " gold.\n";

        // check if the purchase has resulted in bankruptcy, act accordingly
        if (isBankrupt()) bankruptcy();
    }
}

void Player::sell(Commodity& product, int quantity)
{
    // enforce general void preconditions
    if (gameEnded())
        throw std::logic_error("Error: Game function sell() being called after endgame conditions already reached.");

    // handling for negative and excessive quantities
    if (quantity < 0) buy(product, quantity);
    if (quantity > product.owned)
        throw std::logic_error("Error: Calling game function sell() with out-of-range parameters.");

    int earnings = (getPrice(product) * quantity) / // get amount of gold earned from sale
                   (diffModifier() > 1 ? diffModifier() : 1); // higher difficulties can result in lower resale value

    // proceed with sale
    product.owned -= quantity;
    gold += earnings;

    // display results
    std::cout << name << " sells " << quantity << ' ' << product.name
              << " for " << earnings << " gold.\n";
}

void Player::adjustPrice(Commodity& product)
{
    // enforce general void preconditions
    if (gameEnded())
        throw std::logic_error("Error: Game function adjustPrice() being called after endgame conditions already reached.");

    product.basePrice *= percent(random(MIN_PRICE_CHANGE, MAX_PRICE_CHANGE)); // change the price by a random percentage within the allowed range
    std::cout << "The price of " << product.name <<  " in " << townName << " has changed to " << getPrice(product) <<" gold.\n"; // display results in program output
}

/*void Player::buy(Asset& building) no longer necessary due to addition of inheritance hierarchy
{
    // enforce general void preconditions
    if (gameEnded())
        throw std::logic_error("Error: Game function buy() being called after endgame conditions already reached.");

    int totalCost = getPrice(building); // get cost of purchase

    if (gold > totalCost || // do quick check to see if player can afford purchase, ask if they'd like to proceed in case that it puts them in debt
        ynInput("This costs more than you can afford. Proceed with purchase anyways? (y/n) ", 'y', 'n'))
    {
        // continue with purchase
        ++building.owned;
        gold -= totalCost;

        // display results in program output
        std::cout << name << " buys a "<< building.name
                  << " for " << totalCost << " gold.\n";

        // check if purchase has resulted in bankruptcy, act accordingly
        if (isBankrupt()) bankruptcy();
    }
}*/

void Player::attractCitizens(Asset building)
{
    // get quantities based on formula (high tax rates can decrease migration)
    int newMerchants = (random(building.owned * building.merchantsAttracted) / (taxLevel() > diffModifier() ? taxLevel() : diffModifier()));
    int newClergy = (random(building.owned * building.clergyAttracted) / (getCustoms() / CUSTOMS_TAX > diffModifier() ? getCustoms() / CUSTOMS_TAX : diffModifier()));
    int newNobles = (random(building.owned * building.noblesAttracted) / (taxLevel() > diffModifier() ? taxLevel() : diffModifier()));
    // higher taxes will decrease the amount of people who are willing to move in, lower taxes have the opposite affect but only up to (1 / diffModifier)

    // take effects into account
    merchants += newMerchants;
    clergy += newClergy;
    nobles += newNobles;

    // display results
    if (newMerchants > 0) std::cout << newMerchants << " merchants come to "<< townName << ".\n";
    if (newClergy > 0) std::cout << newClergy << " clergy come to " << townName << ".\n";
    if (newClergy > 0) std::cout << newNobles << " nobles come to " << townName << ".\n";
}


/// taxes - helper functions

void Player::adjustRate(int8& oldRate, int8 newRate, int8 minRate, int8 maxRate)
{
    // enforce general void preconditions
    if (gameEnded())
        throw std::logic_error("Error: Game function adjustRate() being called after endgame conditions already reached.");

    if (maxRate < minRate) throw std::logic_error("Error: Calling ranged function adjustRate() with min parameter greater than max parameter."); // enforce precondition contracts
    if (newRate < minRate || newRate > maxRate) throw std::logic_error("Error: Adjusting tax rates to value outside of allowed range.");

    oldRate = newRate; // if no errors triggered, proceed with assignment
}


/// in-game actions and misc events

void Player::invade(Player* defender)
{
    // enforce general void preconditions
    if (gameEnded() || defender->gameEnded())
        throw std::logic_error("Error: Game function invade() being called after endgame conditions already reached.");

    // display header text
    std::cout << getName() << "'s army has invaded " << defender->getTownName() << "!\n";

    // invasion process:
    // randomly determine casualties for each player depending on strength of other player's army
    // if defending (other) player loses greater than a certain percentage of their army, they "lose", otherwise they "win"
    // no resources lost if defenders win, otherwise an amount of grain and land is lost based on the margin of defeat and seized by attacking (this) player
    // deduct casualties and resource seizures from player stats, display in output

    // get casualties
    // formula: proportion of size of opposing army chosen randomly between two parameters
    int16 invaderCasualties = random(defender->getSoldiers() * percent(MIN_CASUALTY_RATE), defender->getSoldiers() * percent(MAX_CASUALTY_RATE));
    if (invaderCasualties > getSoldiers()) invaderCasualties = getSoldiers(); // casualties capped at 100 percent

    // repeat process for other player
    int16 defenderCasualties = random(getSoldiers() * percent(MIN_CASUALTY_RATE), getSoldiers() * percent(MAX_CASUALTY_RATE));
    if (defenderCasualties > defender->getSoldiers()) defenderCasualties = getSoldiers();

    // determine outcome and take into effect, display accordingly

    // take casualties into effect and display

    // break output to allow user to view results
    pressEnterToContinue("(Press ENTER to continue)");
}

void Player::releaseGrain(int quantity)
{
    // enforce general void preconditions
    if (gameEnded())
        throw std::logic_error("Error: Game function releaseGrain() being called after endgame conditions already reached.");
    // and function preconditions (range)
    if (quantity < minRelease() || quantity > maxRelease())
        throw std::logic_error("Error: Calling game function releaseGrain() with out-of-range parameters.");

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
        throw std::logic_error("Error: Game function bankruptcy() being called after endgame conditions already reached.");

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

int Player::getSerfBirths() const
{
    // formula: base amount of births with one extra birth for every (indiv. grain demand * 2) grain released above the total demand
    int baseBirths = random(getSerfs() * percent(MIN_BIRTH_RATE), getSerfs() * percent(MAX_BIRTH_RATE)); // base amount calculated between random parameters
    int bonusBirths = (releasedGrain - grainDemand()) / (GRAIN_DEMAND * 2);

    if (bonusBirths < 0) bonusBirths = 0; // take care of negative values

    return (baseBirths + bonusBirths) / diffModifier();
}

int Player::getSerfDeaths() const
{
    // formula: base amount of deaths with one extra death for every (indiv. grain demand * 2) grain released below the total demand
    int baseDeaths = random(getSerfs() * percent(MIN_DEATH_RATE), getSerfs() * percent(MAX_DEATH_RATE)); // base amount calculated between random parameters
    int bonusDeaths = (grainDemand() - releasedGrain) / (GRAIN_DEMAND * 2);

    if (bonusDeaths < 0) bonusDeaths = 0; // take care of negative values
    if (bonusDeaths > getSerfs() - baseDeaths) bonusDeaths = getSerfs() - baseDeaths; // take care of excessive values

    return (baseDeaths + bonusDeaths) * diffModifier();
}

int Player::getSerfMigration() const
{
    // formula: 1 migrant for every extra (indiv. grain demand * 3) grain released after exceeding the demand by (migration req), divided by difficulty modifier
    int16 surplus = releasedGrain - grainDemand() - MIGRATION_REQ;
    if (surplus < 0) return 0; // no one moves in if no surplus grain is released

    return (surplus / (GRAIN_DEMAND * 3)) / diffModifier();
}

void Player::populationChange()
{
    // get values for changes in serf population by cause
    int serfBirths = getSerfBirths();
    int serfDeaths = getSerfDeaths(); // variable assignments to keep consistent values in case of randomness
    int serfMigration = getSerfMigration();

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

    releasedGrain = 0; // reset released grain using it to calculate changes for serfs
}


/// revenue and expenses

int Player::getRevenue(Tax tax) const
{
    return (percent(tax.rate) // get the percentage as a decimal
    * ((tax.merchantRevenue * merchants)
    + (tax.clergyRevenue * clergy) // factor in taxable wealth in town
    + (tax.nobleRevenue * nobles)
    + (tax.assetRevenue * totalAssets())
    /*+ (taxJustice)*/) // i'm still trying to figure out how tax justice is supposed to get factored in
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
    int16 pay = armyPay(); // variable assignment to keep consistent values in case of randomness
    gold -= pay;

    // display results
    std::cout << pay << " gold paid to " << townName << "'s standing army.\n";

    // check if payments have caused bankruptcy
    if (isBankrupt()) bankruptcy();
}


/// resource gain and loss

int Player::getHarvest()
{
    // formula: serf population multiplied by random value between two parameters, divided by difficulty modifier
    // might change this to a more sophisticated formula later
    return random(getSerfs() * MIN_HARVEST, getSerfs() * MAX_HARVEST) / diffModifier();
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
    int16 grainLoss = getGrainLoss();

    // take changes into effect, display results;
    grain.owned -= grain.owned * percent(grainLoss); // subtract as percentage
    std::cout << grainLoss << "% of " << townName << "'s existing grain reserves lost to various causes.\n";
}


/// scoring and ranking

std::string Player::getTitle() const
{
    // check player gender before returning appropriate title
    switch (getGender())
    {
    case Male:
        return RANKLIST[rankIndex].mTitle; // male title for male player
    case Female:
        return RANKLIST[rankIndex].fTitle; // female title for female player
    default:
        throw std::logic_error // throw exception not returned by this point
        ("Error: Detected invalid value for player's gender.");
    }
}

int Player::getScore() const
{
    // each stat weighed by their "value" in terms of gold for calculating score with the total being the sum (difficulty not accounted, see parameters file for details)
    return (((getGold()/* - STARTING_GOLD*/) * 1) // gold
            + ((getSerfs()/* - STARTING_SERFS*/) * SERF_VALUE) // populations
            + ((getMerchants() /*- STARTING_MERCHANTS*/) * MERCHANT_VALUE)
            + ((getClergy()/* - STARTING_CLERGY*/) * CLERGY_VALUE)
            + ((getNobles()/* - STARTING_NOBLES*/) * NOBLE_VALUE)
            + ((getSoldiers()/* - STARTING_SOLDIERS*/) * SOLDIER_VALUE)
            + ((getGrain()/* - STARTING_GRAIN*/) * GRAIN_VALUE) // resources
            + ((getLand()/* - STARTING_LAND*/) * LAND_VALUE)
            + ((getMarkets() - 0) * MARKET_VALUE) // assets
            + ((getMills() - 0) * MILL_VALUE)
            + ((getCathedrals()- 0) * CATHEDRAL_VALUE)
            + ((getPalaces() - 0) * PALACE_VALUE)
            + 1000); // starting score
}

bool Player::getPromotion() const
{
    // compare current score with score required to reach next rank
    return getScore() > RANKLIST[rankIndex + 1].scoreReq;
}

void Player::promote()
{
    // enforce general void preconditions
    if (gameEnded())
        throw std::logic_error("Error: Game function promote() being called after endgame conditions already reached.");

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
        throw std::logic_error("Error: Game function turnResults() being called after endgame conditions already reached.");

    // program output header
    std::cout << "\nAnnual report for " << getTitle() << ' ' << name
    << " of " << townName << ", Year " << year << "\n";

    // take all the functions scheduled to get called after a player's turn
    // and call all of them by category

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

    std::cout << "\nCensus (Taxpayers): \n";
    attractCitizens(marketplace);
    attractCitizens(mill);
    attractCitizens(cathedral);
    attractCitizens(palace);

    std::cout << "\nCensus (Serfs): \n";
    populationChange();

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
