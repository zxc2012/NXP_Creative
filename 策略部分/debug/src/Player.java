

import java.util.LinkedList;
import java.util.List;

/**
 * @author Stump
 * Contains all the information that is relevant to a player. 
 * Thus being - Their name, their token as to be displayed on the board and their current space. 
 */
public abstract class Player implements PlayerInterface  {

	private String name;
	private String token;
	private int minMax;

	/**
	 * Constructor for a default player
	 * @param playerNumber can be of value 1 or 2 - for default filling of fields: name, token. 
	 */
	public Player(int playerNumber) {
		if (playerNumber == 1) {
			name = "Player X";
			token = " X ";
			minMax = 1; // Maximising player
		}
		else if (playerNumber == 2) {
			name = "Player O";
			token = " O ";
			minMax = -1; // Minimising player
		}
		else throw new RuntimeException("Invalid player number");
	}

	public void setToken(String t) {
		if (t.length() == 0) return;
		else if (t.length() == 1) token = " " + t + " ";
		else if (t.length() == 2) token = t + " ";
		else if (t.length() == 3) token = t;
		else token = t.substring(0, 3);
	}

	/**
	 * @return The player's token as a String
	 */
	public String getToken() {
		return token;
	}

	/**
	 * @return The player's name as a String 
	 */
	public String getName() {
		return name;
	}
	
	/**
	 * Accessor to set the Players name
	 * @param the players name to be
	 */
	public void setName(String name){
		this.name = name;
	}

	/**
	 * Returns a list of all potentially valid moves.
	 * i.e. guaranteed to be in-range, but not necessarily valid given the board state.
	 * @param board
	 * @return List of moves
	 */
	public List<Move> allMoves(Board board) {
		LinkedList<Move> potentialMoves = new LinkedList<Move>();
		if (board.getSpace(this).getUp() != null) {
			potentialMoves.add(new MovementMove(board.getSpace(this), board.getSpace(this).getUp()));
			if (board.getSpace(this).getUp().getRight() != null) potentialMoves.add(new MovementMove(board.getSpace(this), board.getSpace(this).getUp().getRight()));
			if (board.getSpace(this).getUp().getUp()    != null) potentialMoves.add(new MovementMove(board.getSpace(this), board.getSpace(this).getUp().getUp()));
		}
		if (board.getSpace(this).getRight() != null) {
			potentialMoves.add(new MovementMove(board.getSpace(this), board.getSpace(this).getRight()));
			if (board.getSpace(this).getRight().getDown()  != null) potentialMoves.add(new MovementMove(board.getSpace(this), board.getSpace(this).getRight().getDown()));
			if (board.getSpace(this).getRight().getRight() != null) potentialMoves.add(new MovementMove(board.getSpace(this), board.getSpace(this).getRight().getRight()));
		}
		if (board.getSpace(this).getDown() != null) {
			if (board.getSpace(this).getDown().getLeft() != null) potentialMoves.add(new MovementMove(board.getSpace(this), board.getSpace(this).getDown().getLeft()));
			if (board.getSpace(this).getDown().getDown() != null) potentialMoves.add(new MovementMove(board.getSpace(this), board.getSpace(this).getDown().getDown()));
			potentialMoves.add(new MovementMove(board.getSpace(this), board.getSpace(this).getDown()));
		}
		if (board.getSpace(this).getLeft() != null) {
			if (board.getSpace(this).getLeft().getUp()   != null) potentialMoves.add(new MovementMove(board.getSpace(this), board.getSpace(this).getLeft().getUp()));
			if (board.getSpace(this).getLeft().getLeft() != null) potentialMoves.add(new MovementMove(board.getSpace(this), board.getSpace(this).getLeft().getLeft()));
			potentialMoves.add(new MovementMove(board.getSpace(this), board.getSpace(this).getLeft()));
		}
		if (board.hasWallsLeft(this)) {
			for (int col = 1; col <= 6; col++) {
				for (int row = 1; row <= 6; row++) {
					potentialMoves.add(new WallMove(new Wall(new Space(col, row), true)));
					potentialMoves.add(new WallMove(new Wall(new Space(col, row), false)));
				}
			}
		}
		return potentialMoves;
	}

	/**
	 * Returns a list of moves that are valid within the current game state.
	 * @param board
	 * @return
	 */
	public List<Move> validMoves(Board board) {
		Board newBoard;
		List<Move> potentialMoves = allMoves(board);
		List<Move> validMoves = new LinkedList<Move>();
		for (Move move : potentialMoves) {
			try {
				newBoard = board.clone();
				newBoard.makeMove(move);
				validMoves.add(move);
			}
			catch (Exception e) {}
		}
		return validMoves;
	}
	
	/**
	 * Obtain the next moved to be played by this player.
	 * Implementation will differ depending on instance of player object
	 * @return move to be played
	 */
	public Move getMove(Board board) {
		return null;
	}

	/* (non-Javadoc)
	 * @see java.lang.Object#toString()
	 */
	public String toString() {
		return name;
	}

	/**
	 * Returns whether the players is a minimising player or a maximising player.
	 * @return the value as an int
	 */
	public int minMax() {
		return minMax;
	}
	
	/* (non-Javadoc)
	 * @see java.lang.Object#equals(java.lang.Object)
	 */
	@Override
	public boolean equals(Object obj){
		if(!(obj instanceof Player)){
			return false;
		}
		Player other = (Player) obj;
		return (this.getName().equals(other.getName()) && this.getToken().equals(other.getToken()) && this.minMax == other.minMax);
	}
}
