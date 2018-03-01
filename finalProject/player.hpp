#ifndef PLAYER_HPP
#define PLAYER_HPP

/*
Player class abstraction details (copied from README.md file)

    - The player is the ruler of a city-state in 1400s Spain
        - Town stats included with player stats

    - On a high level, members can be split into four categories:
		- Member vars for personal stats; chosen at start of game, entered into class constructor, and remain unchanged:
			- Their name
			- Their gender
			- Game difficulty
			- Which player they are (p1, p2, etc.)
			- The name of the town they rule (based on the above)
		- Stats representing the townÍs status; set to default values with object construction and change with each turn and player decision:
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

    - Lower-level implementation details:
        -
*/

#endif // PLAYER_HPP
