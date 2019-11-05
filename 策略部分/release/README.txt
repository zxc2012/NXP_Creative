QUORIDOR   README  SUBMISSION			30/05/2011

GROUP: STUMP
--Edward McLean   3188294
--Nathan Sturgess 3335686
--Elyse Wise      3308940

TUTOR: Tim Cerexhe
LAB: Mon 18:00-20:00 Oboe




------------------------TO RUN GAME-------------------------

To play game from console: run the file "Quoridor.class"


-----------------------TO PLAY GAME-------------------------


--GAME SETUP--


	Once running, you will be prompted with this menu:
		
		Welcome to Quoridor!
	
		Please select your choice: 
		1 - Play Human vs Human Game
		2 - Play Human vs AI Game
		3 - Play AI vs AI Game
		4 - Load previously saved game
		5 - Advanced options
		6 - Quit
		Enter selection:

	Enter the number which corresponds to your choice.

	
	- 1 2 3 - MAKING A NEW GAME:
 
	If you enter options 1, 2 or 3 a new quoridor board will be 
	created for you to play on with your specified combination
	of human/AI players.
	
	Human Player Setup:
	If a new human player needs to be made, you will be prompted 
	for the players name. Enter any name that you wish. 
	You will also be prompted to enter a token for the player. 
	Only the first 3 characters that you enter will be used to 
	set the token.
	
	AI Player Setup:
	If a new  AI player needs to be made, you will be asked to 
	choose the player's level of difficulty as such:

		Select difficulty level for player 2:
		1 - Easy
		2 - Medium
		3 - Hard
		
	Enter the number which corresponds to your choice. An AI 
	Player will then be constructed with a set default token 
	(X or O).


	- 4 - LOAD A PREVIOUSLY SAVED GAME:

	If you enter option 4, you will be prompted to enter the name
	of the game file which you wish to load. Enter the name 
	(excluding the .qdr file extension).
	If you enter an invalid file name, you will be prompted again 
	for a valid name.
	If you enter a valid file name, the game will be loaded and 
	game play will resume.


	- 5 - ADVANCED OPTIONS:

	This option opens a sub-menu which lets you set options
	relating to the hard (multiple look-ahead) AI. e.g.
	
		Advanced options
		1 - Set AI look-ahead depth
		2 - Set AI timeout
		3 - Return to main menu


	- 6 - QUIT

	If selected, you will receive a message indicating that the 
	game is shutting down and the game will exit. 
	


--GAME PLAY--

	When it is a human player's turn, the player will be prompted 
	for input as such:
	
		harry's Turn
		Enter Move: 
	
	If an invalid move is entered, the move will not be executed 
	and the player will be prompted again.
	
	To move to a new space: enter the coordinates of the space you 
	wish to move to (e.g. e4). The input is not case sensitive.
	
	To place a wall: 
	Enter the coordinates of the wall (eg. e5h ) where coordinates 
	correspond to the top-left space which will be affected by 
	this wall, and v and h stand for vertical and horizontal 
	orientation, respectively.
	
	To view possible moves:
	Type "moves". You will be shown a list of all possible moves
	you can make.
	
	To save the current game: 
	Type "save". You will be prompted to enter a filename for 
	your game.
	
	To quit the current game:
	Type "quit". Any unsaved changes 
	will be lost.
	
	To undo a move:
	Type "undo". The last two moves that were 
	played will be undone.
	
	To redo a move:
	Type "redo". The last two moves that where 
	undone will be played again.
	undo/redo can be applied multiple times - provided there are 
	moves to undo/redo.

	To view all valid moves:
	Type "moves". All valid moves for the current player will 
	be printed to the console.

	To change the look of the board:
	Type "Style 1" or "Style 2" to toggle the look of the board.

-----------------CHANGES TO REVISED DESIGN------------------

--GAME CLASSES--

	We made the Game class abstract, and made two new game 
	implementation classes which extend Game: 

	- Validator Game (for playing a game with preset moves).

	- Console Game (for playing a game which requires moves to 
	  be entered from console for human players.

--AI PLAYER CLASSES--

	We implemented the following player classes (to represent 
	different levels of difficulty in AI play). All of these 
	classes extend the abstract Player class.

	- RandomAIPlayer - which plays random (valid) moves - 
	  used for implementation of easy AI player.

	- NoLookAIPlayer - which evaluates the optimal move to play 
	  without looking ahead - used for implementation of medium 
	  AI Player.

	- MultipleLookAIPlayer - which evaluates the optimal move to 
          play with multiple looks ahead - used for implementation 
          of hard AI Player.

--DEAD PLAYER CLASSES--

	We implemented a DeadPlayer class which asks for no setup 
	routine from the console - this is used to allow the class 
	ValidatorGame to play a game without being held up by 
	prompts from the console.
	A Dead player is created with name and token to be "X" or "O"
	depending on whether it is player 1 or 2.

--FACTORY METHOD--

	We implemented a factory class which implements the following methods:
	
	- makeGame(String presetMoves) which constructs a ValidatorGame 
	  if preset moves are not null, and a console game if preset moves are
 	  null. This method ensures that the game implementation used will suit
          the requirements of retrieving and playing moves (i.e. either from 
          preset strings or from console). It also supports dynamic binding - 
          allowing any class to call makeGame() and retrieve the game object 
          that it requires.  
	
	- makeBoard(int type) which constructs a board and creates the 
	  appropriate types of players (human / AI) for the given game type. 
	  The type number corresponds to the number entered from the console 
          when creating a new game - and type 0 is used for validator games.
	
	- makeAIPlayer(int playerNumber) constructs an AI Player to suit the 
	  level of difficulty entered from the console.


-----------------PROJECT REQUIREMENTS------------------

The following sections briefly details our efforts to satisfy the given
project requirements. 
	
	
---GENERATING VALID MOVES---
	
---Allow a game to be initialised in a position defined by a sequence
	of moves

	This is achieved using the ValidatorGame class. We also implement 
	the same strategy to load a saved game. A ValidatorGame class is 
	constructed with a string of moves, and these moves are 'played'
	upon construction in order to achieve the initial state. 


---Allow for inputting a sequence of moves as a space separated sequence 
	of (unnumbered) individual moves

	This is also achieved by constructing a ValidatorGame which 
	separates the Glendenning's moves into a queue.


---Check validity of moves

	There are two levels of validation of moves in our project:

	Human error checking - is performed in the ConsoleGame class on all 
	moves entered from the console. This basically ensures that the 
	entered move can be interpreted as a possible move in Quoridor.
	Validating move for board - is done within the board class, to ensure 
	the move object can be applied to the board in the current state of 
	game play. The following methods are used for validation:
	- MoveValid()
	- JumpValid()
	- WallIsHere(), adjacentSpaces(), isWithinBounds(), cutsOffPath(), 
		isOccupied()
	
---Generate a list of valid moves from a given position

	Type "moves" to view the list. The list is generated by the Player 
	class in the validMoves() method.


---USER INTERFACE---

---define a set of commands for console game play
---allow moves for both players to be entered via the command line
---display the game on an ascii terminal in a human-readable format
---other commmands you should implement include:
---save current game state to file
---load game from file
---start a new game
---undo / redo individual moves to any level

	All requirements have been satisfied. Refer to the game instructions 
	for further details.


---HUMAN VS. COMPUTER AI PLAY---

---Support for random (valid) move selection
 
	This is used for the 'easy' level of difficulty for AI Players and 
    is implemented by the RandomAIPlayer class.

---Support for a sensible move selection, but with no look ahead

	This is used for the 'medium' level of difficulty for AI Players 
	and is implemented by the NoLookAIPlayer class. It evaluates the 
	best possible move and will pick this move around 90% of the time 
	(to add unpredictability)

---A minimax algorithm 
---Iterated search to different levels
---Alpha-beta pruning algorithm 

	All of these algorithm techniques have been used to model the 
	MultipleLookAheadAIPlayer. See the section on game scoring 
	functionality for further information.

	The behaviour of the 'Hard' Player (MultipleLookAIPlayer) can be 
	altered from the console menu by selecting option 5 on the main menu. 
	Here you can set the depth of look ahead and the timeout value for 
	the AI's algorithm.


----------------GAME SCORING FUNCTION------------------

	The game scoring function relies on two features:
	- A player's distance from their target row
	- The number of walls they have left
	
	An evaluation for one player is given by:
		eval = weight1 * wallsLeft - weight2 * distanceToExit
	where each type of AI player can define its own weights to determine 
	how it will evaluate the game state. The distance to the exit is 
	determined using a breadth-first search of the board.
	
	To find the total game score at a given game state, the evaluation 
	for player 2 is subtracted from the evaluation for player 1.
