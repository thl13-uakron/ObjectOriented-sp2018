#Program Overview
	This is a port of the 1979 video game *Santa Paranoia and Fiumancco* written in C++ with modern “object-oriented” programming techniques. *Santa Paravia* is essentially a primitive, text-based version of the popular *Civilization* series in which the player controls a single town in 1400s Italy and is tasked with managing wealth and resources to allow growth.
	In making this project, I attempted to neatly split it into two parts: the objects and member functions that make up the game elements and mechanics, and the main program loop that would initialize objects and call functions as necessary to simulate gameplay and game flow, as will all be detailed below.

#Framework
##Helper Functions
	In addition to high-level classes representing game elements, the program’s framework also consists of a set of functions I wrote to carry out repetitive, low-level mechanical tasks in the program. This included functions for input validation, random number generation, random chance, and pausing the output.
##Classes and Members
	The player is the main element in the game. As an object, it consists of the player’s stats, as well as the stats for the player’s town, which can be bundled in since each player is assigned to a single town in the game. All of the data is stored in the class’s private access. The member functions in the class’s public access include functions representing in-game actions and events that can affect a player’s stats, as well as getter functions for read-only access of private data that is otherwise not intended for direct mutation.
###Back-end
	asdf
###Front-end
	asdf

#Program Flow
	asdf