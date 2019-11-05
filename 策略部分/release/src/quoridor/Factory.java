package quoridor;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

/**
 * Handles setting up different instances of various object types, that are needed during the game. 
 */
public class Factory {
	
	private static Factory theFactory;
	public int lookAhead = 3;
	public int timeOut = 5000;

	public static Factory instance ()
	{
		if (theFactory == null) theFactory = new Factory();
		return theFactory;
	}

	/**
	 * Used to make a validator game
	 * @param presetMoves the preset string of moves
	 * @return the constructed Validator Game;
	 */
	public Game makeGame(String presetMoves) {
		if(presetMoves != null) {
			return new ValidatorGame(presetMoves);
		}
		return null;
	}

	/**
	 * Construct and return a new board with the specified types of players
	 * @param type - integer representation of the type of game: (human/AI player combinations)
	 * 1 - Human vs Human Game
	 * 2 - Play Human vs AI Game
	 * 3 - Play AI vs AI Game
	 * @return the new board
	 */
	public Board makeBoard(int type) {
		Player player1;
		Player player2;
		switch (type){
			case 0: player1 = new DeadPlayer(1);
					player2 = new DeadPlayer(2);
					break;
			case 1: player1 = HardAI(1);
					player2 = new HumanPlayer(2);
					break;
			case 2: player1 = new HumanPlayer(1);
					player2 = HardAI(2);
					break;
			default: return null;
		}
		return new Board(new Two<Player>(player1, player2));
	}

	/**
	 * Makes an AIPlayer - Also prompts the user for the level of which the AI player will be.
	 * @param playerNumber Player 1 or 2
	 * @return the constructed player. 
	 */
	public Player makeAIPlayer(int playerNumber) {
		while(true) {
			System.out.println("Select difficulty level for player " + playerNumber + ":");
			System.out.println("1 - Easy");
			System.out.println("2 - Medium");
			System.out.println("3 - Hard");
			String input = getFromUser();
			int difficulty;
			try {
				difficulty = Integer.parseInt(input.substring(0,1));
			}
			catch( Exception e) {
				difficulty =0;
			}
			if(difficulty>0) {
				switch (difficulty){
					case 1: return new RandomAIPlayer(playerNumber);
					case 2: return new NoLookAIPlayer(playerNumber);
					case 3: return new MultipleLookAIPlayer(playerNumber, lookAhead, timeOut);
					default:
				}
				System.out.println("Invalid input.");
			}
		}
	}
	public Player HardAI(int playerNumber) {
		while(true) {
			System.out.println("HardAI");
			return new MultipleLookAIPlayer(playerNumber, lookAhead, timeOut);

		}
	}
	/**
	 * Used but the load method to reinstanciate the type of player that was originally saved 
	 * @param playerNumber where it is player one or teo
	 * @param playerType the subtype of player
	 * @param playerName the players name
	 * @param playerToken the players token
	 * @return the constructed player
	 */
	public Player makePlayer(int playerNumber, String playerType, String playerName, String playerToken){
		Player newPlayer;
		if(playerType.equalsIgnoreCase("quoridor.HumanPlayer")){
			return new HumanPlayer(playerNumber, playerName, playerToken);
		} else if(playerType.equalsIgnoreCase("quoridor.RandomAIPlayer")){
			newPlayer = new RandomAIPlayer(playerNumber);
		} else if(playerType.equalsIgnoreCase("quoridor.NoLookAIPlayer")) {
			newPlayer = new NoLookAIPlayer(playerNumber);
		} else if(playerType.equalsIgnoreCase("quoridor.MultipleLookAIPlayer")){
			newPlayer = new MultipleLookAIPlayer(playerNumber, lookAhead, timeOut);
		} else {
			newPlayer = new DeadPlayer(playerNumber);
		}
		newPlayer.setName(playerName);
		newPlayer.setToken(playerToken);
		return newPlayer;
	}


	/**
	 * Gets input from user, loops until valid
	 * @return
	 */
	private String getFromUser() {
		try {
			while (true) {
				BufferedReader userReader = new BufferedReader(new InputStreamReader(System.in));
				System.out.print(">");
				String fromUser = userReader.readLine();
				if (fromUser.trim().length() > 0) { // never returns an empty string
					return fromUser;
				} else {
					System.out.println("Sorry empty strings are not valid input");
					System.out.print("Please try again: ");
				}
			}
		} catch (IOException e) {
			e.printStackTrace();
			return null;
		}
	}

}
