package quoridor;

import java.util.ArrayList;
import java.util.LinkedList;

/**
 * A board has Two Players and up to 20 placed walls (10 per player) and tracks each of their coordinates.
 * @author Team Stump
 */
public class Board implements BoardInterface{

	public Two<Player> players;
	protected Player currentPlayer;
	protected LinkedList<Wall> wallList = new LinkedList<Wall>();
	protected LinkedList<Move> moveList = new LinkedList<Move>();
	protected int moveListIndexOnLoad = -1;
	protected int moveListIndex = -1;
	static final Space player1Start = new Space("d7");
	static final Space player2Start = new Space("d1");
	protected Space player1Space = new Space("d7");
	protected Space player2Space = new Space("d1");
	protected int player1WallsLeft = 8;
	protected int player2WallsLeft = 8;
	Player winner = null;
	public BoardGraph graph = new BoardGraph();
	/***存疑***/
	private final int MOVEMENT_MOVE = 2;
	private final int WALL_MOVE = 3;

	/**
	 * A board takes Two players as a pair as defined by the Two Class.
	 * @param players The Two players for this board instance.
	 */
	public Board(Two<Player> players) {
		this.players = players;
		currentPlayer = players._1();
	}

	/**
	 * Returns the Two players for this particular board instance.
	 * @return The Two players
	 */
	public Two<Player> getPlayers() {
		return this.players;
	}
	
	/**
	 * Accessor for the Players
	 * @param players The Two<Players>
	 */
	public void setPlayers(Two<Player> players){
		this.players = players;
	}

	/**
	 * Returns a pointer to the wall list.
	 * @return The list of all walls at time of calling
	 */
	public LinkedList<Wall> getWallList() {
		return this.wallList;
	}


	/**
	 *Switch the currentPlayer (for next player's turn) 
	 */
	public void nextPlayer() {
		currentPlayer = players.other(currentPlayer);
	}

	/**
	 * Tests if game has been won.
	 * @return 1 is game is won by player 1, 2 if won by player 2, 0 otherwise
	 */
	public int checkWin() {
		Player player1 = players._1();
		Player player2 = players._2();
		if (getSpace(player1).row() == player2Start.row()) {
			winner = player1;
			return 1;
		} else if (getSpace(player2).row() == player1Start.row()) {
			winner = player2;
			return 2;
		}
		winner = null;
		return 0;
	}

	/**
	 * Returns the winner if there is one, otherwise the variable will still be null. 
	 * @return the winner
	 */
	public Player winner() {
		checkWin();
		return winner;
	}
	
	/**
	 * Accessor for current player
	 * @return currentPlayer
	 */
	public Player currentPlayer() {
		return currentPlayer;
	}

	/**
	 * Checks if the players move is occupied by another player.
	 * @param space the space that the player wishes to move to.
	 * @param player the player that is doing the moving.
	 * @return The result.
	 */
	private boolean isOccupied(Space space) {
		Player otherPlayer = players.other(currentPlayer);
		return (getSpace(otherPlayer).equals(space)
				|| getSpace(currentPlayer).equals(space));
	}

	/**
	 * Checks if a wall exists between two given spaces.
	 * @param space a, b the spaces that represent the junction being queried
	 * @return true if a wall exists between these two walls.
	 */
	public boolean wallIsHere(Space a, Space b) {
		if (!(adjacentSpaces(a, b))) {
			return false;
		}
		// swap them if out of order (a should always be above B /left of B)
		if ((b.col() < a.col()) || (b.row() < a.row())) {
			Space temp = a;
			a = b;
			b = temp;
		}
		LinkedList<Wall> toConsider = new LinkedList<Wall>();
		// obtain the walls that we need to consider in order for
		// a->b to be blocked by a wall.

		// for a,b to in same row
		if (b.equals(a.getRight())) {
			if (a.getDown() != null)
				toConsider.add(new Wall(a, true));
			if (a.getUp() != null)
				toConsider.add(new Wall(a.getUp(), true));
		}

		// for a,b in same column
		if (b.equals(a.getDown())) {
			if (a.getRight() != null)
				toConsider.add(new Wall(a, false));
			if (a.getLeft() != null)
				toConsider.add(new Wall(a.getLeft(), false));
		}
		// see if any of these walls actually exist
		while (toConsider.size() > 0) {
			if (wallList.contains(toConsider.remove())) {
				return true;
			}
		}
		return false;
	}


	
	/**
	 * Tests if space a is adjacent to space b on the board
	 * @param a
	 * @param b
	 * @return true if a and b are adjacent
	 */
	public boolean adjacentSpaces(Space a, Space b) {
		// adjacent spaces will differ by at most 1 coordinate from each other
		if ((Math.abs(a.col() - b.col()) + (Math.abs(a.row() - b.row())) == 1)) {
			return true;
		}
		return false;
	}

	/* (non-Javadoc)
	 * @see quoridor.BoardInterface#wallOverlaps(quoridor.Wall)
	 */
	public boolean wallOverlaps(Wall wall) {
		Space sideA = wall.getSpace();	//the top left space for this wall
		Space sideB = null;//the space on opposite side of wall to sideA
		if(wall.isHorizontal()) {
			sideB = sideA.getDown();
		}
		else {
			sideB = sideA.getRight();
		}
		return wallIsHere(sideA, sideB);
	}
	
	/**
	 * Adds a given wall to the board (assumes valid)
	 * @param wall the wall that is to be added to the list
	 */ 
	public void addWall(Wall wall) {
		wallList.add(wall);
		graph.addWall(wall);
		decrementWallTally(currentPlayer);
	}



	/**
	 * Checks whether a placed wall will cut off the path.
	 * @param wall
	 * @return
	 */
	private boolean cutsOffPath(WallMove move) {
		boolean blocks = false;
		int i;
		addWall(move.wall());
		ArrayList<Space> exits = new ArrayList<Space>();
		
		for (i=1; i<=7; i++) exits.add(new Space (i, player2Start.row()));
		graph.fillNodeDistances(exits);
		if (graph.getDist(getSpace(players._1())) == -1) blocks = true;

		exits.clear();
		for (i=1; i<=7; i++) exits.add(new Space (i, player1Start.row()));
		graph.fillNodeDistances(exits);
		if (graph.getDist(getSpace(players._2())) == -1) blocks = true;
		
		removeWall(move.wall());
		return blocks;
	}	

	/**
	 * Remove a previously played wall from the board
	 * @param wall
	 */
	private void removeWall(Wall wall) {
		wallList.remove(wall);
		graph.removeWall(wall);
		incrementWallTally(currentPlayer);
	}

	/**
	 * Returns a String representation of the board's current state
	 * for printing purposes
	 */
	public String toString() {
		return BoardPrinter.buildBoardString(this);
	}
	
	

	/**
	 * Print the board layout to console
	 */
	public void print() {
		System.out.println(this);
	}

	/**
	 * Moves are passed into this function.
	 * makeMove() will check that the move is valid.
	 * If it is valid, the move is done and added to moveList.
	 * Otherwise it throws an exception.
	 * @param move
	 */
	public void makeMove(Move move) {
		move.owner = currentPlayer;
		if (!moveValid(move)) throw new RuntimeException("Invalid move");
		if (moveList.size() > 0) {
			assert (moveListIndex >= 0);
			while (moveList.size() > moveListIndex + 1)
				moveList.removeLast();
		}
		moveList.add(move);
		moveListIndex++;
		applyMove(move);
	}

	/**
	 * Check that a move is valid with respect to the current board state.
	 * @param move
	 * @return True if the move is valid.
	 */
	public boolean moveValid (Move move) {
		if (move instanceof MovementMove) {
			MovementMove mMove = (MovementMove) move;
			if (!wallIsHere(mMove.from(), mMove.to()) 
					&& mMove.from().equals(getSpace(currentPlayer)) 
					&& !isOccupied(mMove.to()) 
					&& (adjacentSpaces(mMove.from(), mMove.to()) || jumpValid(mMove))) {
				return true;
			}
			return false;
		}	
		if (move instanceof WallMove) {
			if(!hasWallsLeft(currentPlayer)) {
				return false;
			}
			WallMove wMove = (WallMove) move;
			Wall proposedWall = wMove.wall();
			if (wallList.contains(new Wall(wMove.wall().getSpace(), !wMove.wall().isVertical()))) return false;
			if (proposedWall.isHorizontal()
					&&!wallIsHere(proposedWall.getSpace(), proposedWall.getSpace().getDown())
					&&!wallIsHere(proposedWall.getSpace().getRight(), proposedWall.getSpace().getDown().getRight())) {
				if (cutsOffPath((WallMove) move)) return false;
				return true;
			}
			else if (!proposedWall.isHorizontal()
					&&!wallIsHere(proposedWall.getSpace(), proposedWall.getSpace().getRight())
					&&!wallIsHere(proposedWall.getSpace().getDown(), proposedWall.getSpace().getRight().getDown())) {
				if (cutsOffPath((WallMove) move)) return false;
				return true;
			}
		}
		return false;
	}
					
	/**
	 * Checks that a jump move is valid.
	 * If the move is a regular (adjacent) move, returns true.
	 * @param move
	 * @return True if valid jump move
	 */
	private boolean jumpValid(MovementMove move) {
		if (!move.isJump()) return true;
		Space middleSpace;
		Space behindSpace;
		// Horizontal jump move
		if (move.from().row() == move.to().row()) {
			assert (Math.abs(move.from().col() - move.to().col()) == 2);
			middleSpace = new Space(Math.min(move.from().col(), move.to().col()) + 1, move.from().row());
			if (getSpace(players.other(move.owner)).equals(middleSpace)
					&& !wallIsHere(move.from(), middleSpace)
					&& !wallIsHere(move.to(), middleSpace)) {
				return true;
			}
			return false;
		}
		// Vertical jump move
		else if (move.from().col() == move.to().col()) {
			assert (Math.abs(move.from().row() - move.to().row()) == 2);
			middleSpace = new Space(move.from().col(), Math.min(move.from().row(), move.to().row()) + 1);
			if (getSpace(players.other(move.owner)).equals(middleSpace)
					&& !wallIsHere(move.from(), middleSpace)
					&& !wallIsHere(move.to(), middleSpace)) {
				return true;
			}
			return false;
		}
		// Diagonal jump move
		else if (Math.abs(move.from().row()-move.to().row()) + Math.abs(move.from().col()-move.to().col()) == 2) {
			if (getSpace(players.other(move.owner)).row() == move.from().row()
					&& getSpace(players.other(move.owner)).col() == move.to().col()
					|| getSpace(players.other(move.owner)).col() == move.from().col()
					&& getSpace(players.other(move.owner)).row() == move.to().row()) {
				middleSpace = getSpace(players.other(move.owner));
				if (wallIsHere(middleSpace, move.from())) return false;
				if (wallIsHere(middleSpace, move.to())) return false;
				try {
					// Try to construct the space behind the opponent
					behindSpace = new Space(2 * middleSpace.col() - move.from().col(), 2 * middleSpace.row() - move.from().row());
				}
				catch (RuntimeException e) {
					// Behind opponent is the edge of the board - treat as wall
					return true;
				}
				return wallIsHere(middleSpace, behindSpace);
			}
			else return false;
		}
		return false;
	}

	/**
	 * Updates the board state with the move.
	 * Assumes that the move is valid.
	 * Used for new (validated) moves or redo.
	 * @param move to be played
	 */
	private void applyMove(Move move) {
		if (move instanceof MovementMove) {
			setSpace(move.owner, ((MovementMove) move).to());
		}
		if (move instanceof WallMove) {
			this.addWall(((WallMove) move).wall());
		}
		currentPlayer = players.other(currentPlayer);
	}

	/**
	 * Undo the last two moves.	 */
	public void undo() {
		if (moveListIndex < 1) throw new RuntimeException("No moves to undo");
		if (moveListIndex - 2 < moveListIndexOnLoad) throw new RuntimeException("Cannot undo moves loaded from a saved game");
		undoLastMove();
		undoLastMove();
	}
	
	/**
	 * Does the actual moving back to the previous state. 
	 */
	public void undoLastMove() {
		Move move = moveList.get(moveListIndex);

		currentPlayer = players.other(currentPlayer);
		if (move instanceof MovementMove) {
			setSpace(move.owner, ((MovementMove) move).from());
		}
		else if (move instanceof WallMove) {
			this.removeWall(((WallMove) move).wall());
		}
		moveListIndex--;
	}

	/**
	 * Redo the last undone move.
	 */
	public void redo() {
		if (moveList.size() == 0 || moveListIndex > moveList.size()-2) throw new RuntimeException("No moves to redo");

		redoLastMove();
		redoLastMove();
	}
	
	/**
	 * Does the actual moving forward to the previous state. 
	 */
	private void redoLastMove() {
		moveListIndex++;
		applyMove(moveList.get(moveListIndex));
	}
	

	/**
	 * Returnes a string of who's turn it is
	 * @return The players name as a string
	 */
	public String whoseTurn(){
		return this.currentPlayer.toString();
	}
	
	/**
	 * Handles in the input string and makes the appropriate move.
	 * @param moveInput the move input 
	 */
	public void makeMoveFromInput(String moveInput) {
		Move move;
		if (moveInput.equals("undo")) {
			undo();
		} else if(moveInput.equals("redo")) {
			redo();
		} else if (moveInput.length() >= 7 && moveInput.substring(0, 6).equalsIgnoreCase("style ")) {
			BoardPrinter.setStyle(moveInput.substring(6, 7));
		} 
		else {	// Regular move
			if (moveInput.length() == MOVEMENT_MOVE) {
				move = new MovementMove(getSpace(currentPlayer), new Space(moveInput));
			} else if (moveInput.length() == WALL_MOVE) {
				move = new WallMove(new Wall(moveInput));
			} else {
				throw new RuntimeException("Invalid input");
			}
			makeMove(move);
		}
	}

	/* (non-Javadoc)
	 * @see java.lang.Object#clone()
	 */
	@SuppressWarnings("unchecked")
	public Board clone() {
		Board cloneBoard = new Board(this.players);
		cloneBoard.wallList = (LinkedList<Wall>) this.wallList.clone();
		cloneBoard.moveList = (LinkedList<Move>) this.moveList.clone();
		cloneBoard.moveListIndex = this.moveListIndex;
		cloneBoard.currentPlayer = this.currentPlayer;
		cloneBoard.winner = this.winner;
		cloneBoard.player1Space = this.player1Space;
		cloneBoard.player2Space = this.player2Space;
		cloneBoard.player1WallsLeft = this.player1WallsLeft;
		cloneBoard.player2WallsLeft = this.player2WallsLeft;
		cloneBoard.graph = new BoardGraph();
		for (Wall wall : wallList) {
			cloneBoard.graph.addWall(wall);
		}
		return cloneBoard;
	}
	
	/**
	 * Returns the moveList as a string, much like a validator string. For use in saving to a textfile. 
	 * @return the string of sequential moves
	 */
	public String moveListToString(){
		String moveString = new String();
		for(Move m : moveList){
			if(m instanceof MovementMove){
				m = (MovementMove) m;
			} else {
				m = (WallMove) m;
			}	
			moveString = moveString+" "+m.toString();
		}
		return moveString;
	}
	
	/**
	 * Sets the passed players space in the board
	 * @param player the player who's space will be set 
	 * @param space the new location
	 */
	protected void setSpace(Player player, Space space) {
		if (player.equals(players._1())) player1Space = space;
		else if (player.equals(players._2())) player2Space = space;
		else throw new RuntimeException("Invalid player");
	}
	
	
	/**
	 * Gets a players location
	 * @param player the player who in being queried 
	 * @return the Space of the player
	 */
	public Space getSpace(Player player) {
		if (player.equals(players._1())) return player1Space;
		else if (player.equals(players._2())) return player2Space;
		else throw new RuntimeException("Invalid player");
	}

	/**
	 * Checks if this player has reached their quota of walls
	 * @return boolean true is player can still place walls
	 */
	public Boolean hasWallsLeft(Player player) {
		if (player.equals(players._1())) return (player1WallsLeft > 0);
		else if (player.equals(players._2())) return (player2WallsLeft > 0);
		else throw new RuntimeException("Invalid player");
	}
	
	/**
	 * Returns how many walls a player has left
	 * @param player the player who is being queried 
	 * @return the walls left as an int
	 */
	public int getWallsLeft(Player player) {
		if (player.equals(players._1())) return player1WallsLeft;
		else if (player.equals(players._2())) return player2WallsLeft;
		else throw new RuntimeException("Invalid player");
	}

	/**
	 * Decrements the passed players wall tally. 
	 * @param player the player to decrement 
	 */
	public void decrementWallTally(Player player) {
		if (player.equals(players._1())) player1WallsLeft--;
		else if (player.equals(players._2())) player2WallsLeft--;
		else throw new RuntimeException("Invalid player");
	}

	/**
	 * Increments the passed players wall tally. 
	 * @param player the player to increment 
	 */
	public void incrementWallTally(Player player) {
		if (player.equals(players._1())) player1WallsLeft++;
		else if (player.equals(players._2())) player2WallsLeft++;
		else throw new RuntimeException("Invalid player");
	}
	
	/**
	 * Used to take in players information from a text file, as passed in from Game and set the board with these players
	 * @param p1Type Player 1's subtype as a string. 
	 * @param p1Name Player 1's  name as a string.
	 * @param p1Token Player 1's token as a string.
	 * @param p2Type Player 2's subtype as a string. 
	 * @param p2Name Player 2's  name as a string.
	 * @param p2Token Player 2's token as a string.
	 */
	protected void loadPlayers(String p1Type, String p1Name, String p1Token, String p2Type, String p2Name, String p2Token){
		Two<Player> players = new Two<Player>(Factory.instance().makePlayer(1, p1Type, p1Name, p1Token), Factory.instance().makePlayer(2, p2Type, p2Name, p2Token));
		this.setPlayers(players);
	}

	/* (non-Javadoc)
	 * @see java.lang.Object#equals(java.lang.Object)
	 */
	@Override
	public boolean equals(Object obj){
		if(!(obj instanceof Board)){
			return false;
		}
		Board other = (Board) obj;
		
		return (this.currentPlayer.equals(other.currentPlayer) && this.moveList.equals(other.moveList)  && this.wallList.equals(other.wallList)
			&& this.player1Space.equals(other.player1Space) && this.player2Space.equals(other.player2Space) && this.players._1().equals(other.players._1()) 
			&& this.players._2().equals(other.players._2()) && this.player1WallsLeft == other.player1WallsLeft && this.player2WallsLeft == other.player2WallsLeft);
	}

}
