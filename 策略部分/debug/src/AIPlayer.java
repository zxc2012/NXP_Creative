

import java.util.ArrayList;

public abstract class AIPlayer extends Player {

	/**
	 * Constructor
	 * @param playerNumber delegates player1 or player2 default construction
	 */
	public AIPlayer(int playerNumber) {
		super(playerNumber);
	}
	
	/**
	 * Evaluates how much better the board is for player 1 than for player 2.
	 * Used by the AI to decide on moves.
	 * @param board
	 * @return
	 */
	protected int evaluate(Board board) {
		return awesomeness(board, board.players._1()) - awesomeness(board, board.players._2());
	}
	
	/**
	 * Checks how favourable the board situation is for one of the players.
	 * This is found from the player's distance to their exit, and how many
	 * walls they have left.
	 * @param board The board under consideration.
	 * @param player The player whose situation is being assessed.
	 * @return An awesomeness value in arbitrary units (higher is better)
	 */
	@SuppressWarnings("static-access")
	protected int awesomeness(Board board, Player player) {
		int awesomeness = 0;
		
		awesomeness += board.getWallsLeft(player) * wallsLeftWeight();
		
		Space otherSide;
		if (player.equals(board.players._1())) otherSide = board.player2Start;
		else otherSide = board.player1Start;
		ArrayList<Space> exits = new ArrayList<Space>();
		for (int i = 1; i <=7; i++) exits.add(new Space(i, otherSide.row()));
		board.graph.fillNodeDistances(exits);
		int distance = board.graph.node[board.getSpace(player).row()-1][board.getSpace(player).col()-1].distanceToExit;
		awesomeness -= distance * distanceWeight();
		
		if (player.equals(board.winner())) awesomeness = Integer.MAX_VALUE/2;
		
		return awesomeness;
	}

	/**
	 * Weighting function for board evaluation.
	 * @return Weight for a player's distance from the exit.
	 */
	protected abstract int distanceWeight();

	/**
	 * Weighting function for board evaluation.
	 * @return Weight for a player's number of walls left.
	 */
	protected abstract int wallsLeftWeight();
}
