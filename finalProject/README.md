Game Elements and Abstractions:

Player Specifications:
	- On an abstract level, the player is the ruler of medieval town in Europe.
	- Code-wise, the Player class consists their game stats as members, dividable into the following categories :
		- Personal stats, chosen at the start, remain unchanged:
			- Their name
			- Their gender
			- Which player they are (p1, p2, etc.)
			- The name of the town they rule (based on the above)
		- Stats representing the town’s status, set to default values at the start and change with each turn and player decision:
			- Grain reserves, price, and demand
			- Land owned, land prices
			- Gold in treasury
			- Town’s population, in the following categories:
				- Serfs
				- Clergy
				- Noblemen
				- Soldiers
			- Town buildings and assets, in the following categories:
				- Marketplaces
				- Mills
				- Palaces
				- Cathedrals
			- Tax rates, in the following categories:
				- Customs
				- Sales
				- Income
				- Justice
			- Player’s rank
		- Functions that get called between turns to modify some of the above values based on others:
			- Receive tax revenue
			- Economy grows or shrinks
			- People die, get born, immigrate, or leave
			- Grain gets eaten by rats (random event)
			- Invaded by neighboring nation (random event)
		- Functions encompassing decisions that players make during turns that can affect the above members and functions:
			- Raise or lower taxes
			- Release grain
			- Buy assets
			- Draft serfs into the army
			- View status and standings (no effect on game itself)
- 

Game Flow:
-