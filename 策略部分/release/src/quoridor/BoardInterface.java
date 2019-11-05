package quoridor;


import java.util.LinkedList;


/**
 * A board has Two Players and up to 20 placed walls (10 per player) and tracks each of their coordinates.
 * @author Team Stump
 */
public interface BoardInterface {

	/**
	 * Returns the Two players for this particular board instance.
	 * @return The Two players
	 */
	public Two<Player> getPlayers();
	
	/**
	 * Returns a pointer to the wall list.
	 * @return The list of all walls at time of calling
	 */
	public LinkedList<Wall> getWallList();


	/**
	 *Switch the currentPlayer (for next player's turn) 
	 */
	public void nextPlayer();

	/**
	 * Tests if game has been won.
	 * @return true is game is over, false otherwise
	 */
	public int checkWin();
	
	/**
	 * accessor for current player
	 * @return currentPlayer
	 */
	public PlayerInterface currentPlayer();

	/**
	 * Checks if a wall exists between two given spaces.
	 * @param space a, b the spaces that represent the junction being queried
	 * @return true if a wall exists between these two walls.
	 */
	public boolean wallIsHere(Space a, Space b);
	
	/**
	 * Tests if space a is adjacent to space b on the board
	 * @param a
	 * @param b
	 * @return true if a and b are adjacent
	 */
	public boolean adjacentSpaces(Space a, Space b);

	/**
	 * Tests if a given wall will intersect an existing wall on the board
	 * @param wall the wall the be tested for intersections
	 * @return true if the wall intersects with an existing wall on board.
	 */
	public boolean wallOverlaps(Wall wall);
	
	/**
	 * Returns a String representation of the board's current state
	 * for printing purposes
	 */
	public String toString();

	/**
	 * Print the board layout to console
	 */
	public void print();

	/**
	 * Moves are passed into the board here.
	 * makeMove() will check that the move is valid,
	 * then add it to moveList.
	 * @param move
	 */
	public void makeMove(Move move);
	
	/**
	 * Check that a move is valid with respect to the current board state.
	 * @param move
	 * @return True if the move is valid.
	 * TODO: Make it throw exceptions for invalid moves.
	 */
	public boolean moveValid(Move move);
					

	/**
	 * Undo the last move.	 */
	public void undo();

	/**
	 * Redo the last undone move.
	 */
	public void redo();
}
