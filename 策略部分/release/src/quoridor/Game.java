package quoridor;

import java.io.*;
import java.util.*;


public abstract class Game implements GameInterface {
	protected int checkWin = 0;
	protected Player winner = null;
	protected Board board = null;
	protected Boolean gameOver = false;
	protected int gameType;
	protected boolean consoleGame;	//i changed validatorGame to consoleGame only to show we can use the validatorGame class to run all sorts of test

	/**
	 * This function controls the flow of game play.
	 */
	public abstract boolean playGame();

	/**
	 * Play the next turn in the game.
	 * Method: prompt user for a move
	 * attempt to play the move on the board
	 * prompt user again if the move is invalid
	 */

	protected abstract void playNextTurn();

	
	/**
	 * Checks for a winner
	 * @return returns the result as an int
	 */
	public int checkWin() {
		return board.checkWin();
	}
	
	
	/**
	 * Saves the game to a text file. One row per each of the individual player's details and one for the string of move in a validator style
	 * @param fileName the file same from which to save to - appends the name with .qdr file type
	 * @return returns the result of this action
	 */
	public boolean save(String fileName){
		String player1Details = this.board.getPlayers()._1().getClass().getName()+"\n"+this.board.getPlayers()._1().getName()+"\n"+this.board.getPlayers()._1().getToken()+"\n";
		String player2Details = this.board.getPlayers()._2().getClass().getName()+"\n"+this.board.getPlayers()._2().getName()+"\n"+this.board.getPlayers()._2().getToken()+"\n";
		String moveString = this.getCurrentListOfMovesAsString();

		System.out.println("Saving game....");
		try {
			BufferedWriter out = new BufferedWriter(new FileWriter(fileName+".qdr"));
			out.write(player1Details+player2Details+moveString);
			out.close();
		} catch (IOException e) { 
			System.out.println("Exception: Unable to save file");
			return false;
		}
		return true;
	}

	/**
	 * Reads in the individual lines from the user stated text file and places the input into variables from which is passed into the board 
	 * @param fileName the file name from which to load from.
	 */
	public void load(String fileName){
		boolean p1CurrentPlayer;
		Scanner inputStream = null;
		String player1Type = null, player1Name = null, player1Token = null, player2Type = null, player2Name = null, player2Token = null, moveString = null;

		System.out.println("Loading game....");	

		try{
			inputStream = new Scanner(new File(fileName+".qdr"));
		} catch(FileNotFoundException e){
			System.out.println("Exception: Unable to load file, file not found.");
		}

		try {
			player1Type = inputStream.nextLine().trim();
			player1Name = inputStream.nextLine().trim();
			player1Token = inputStream.nextLine().trim();
			player2Type = inputStream.nextLine().trim();
			player2Name = inputStream.nextLine().trim();
			player2Token = inputStream.nextLine().trim();
			moveString = inputStream.nextLine().trim();
		} catch (NoSuchElementException e) {
			System.out.println("Exception: Unable to read save file.");
		}

		Game game = Factory.instance().makeGame(moveString);
		game.playGame();

		if(game.board.players._1().equals(game.board.currentPlayer)){
			p1CurrentPlayer = true;
		} else {
			p1CurrentPlayer = false;
		}

		game.board.loadPlayers(player1Type, player1Name, player1Token, player2Type, player2Name, player2Token);

		if(p1CurrentPlayer){
			game.board.currentPlayer = game.board.players._1();
		} else {
			game.board.currentPlayer = game.board.players._2();
		}

		this.board = game.board;
	}

	/**
	 * Displays a text message before exiting the program
	 */
	protected void quit() {
		System.out.println("Whatever man. Bye...");
		System.exit(0);
	}
	
	/**
	 * Returns the moveList from board in a validator format
	 * @return
	 */
	protected String getCurrentListOfMovesAsString(){
		return this.board.moveListToString();
	}
	
	/* (non-Javadoc)
	 * @see java.lang.Object#equals(java.lang.Object)
	 */
	@Override
	public boolean equals(Object obj){
		if(!(obj instanceof Game)){
			return false;
		}
		Game other = (Game) obj;
		
		return (this.board.equals(other.board) && this.winner == other.winner && this.gameType == other.gameType 
				&& this.gameOver == other.gameOver && this.consoleGame == other.consoleGame && this.checkWin == other.checkWin);
	}
	
}
