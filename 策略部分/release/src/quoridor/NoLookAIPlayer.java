package quoridor;

import java.util.LinkedList;
import java.util.List;

/**
 * An AI player that picks a good move without looking ahead.
 * @author Team Stump
 */
public class NoLookAIPlayer extends AIPlayer {
	
	public NoLookAIPlayer(int playerNumber) {
		super(playerNumber);
	}

	/**
	 * Looks at the board and figures out what move to make.
	 * Strongly favours advantageous moves, but with some randomness
	 * to make it less predictable.
	 * @see quoridor.Player#getMove(quoridor.Board)
	 */
	public Move getMove(Board board) {
		Board newBoard;
		List<Move> potentialMoves = allMoves(board);
		List<Move> validMoves = new LinkedList<Move>();
		double awesomeTotal = 0;
		for (Move move : potentialMoves) {
			try {
				newBoard = board.clone();
				newBoard.makeMove(move);
				move.awesomeness = Math.pow(4, this.minMax() * evaluate(newBoard));
				awesomeTotal += move.awesomeness;
				validMoves.add(move);
			}
			catch (Exception e) {}
		}
		double choice = awesomeTotal * Math.random();
		for (Move move : validMoves) {
			if (choice < move.awesomeness) return move;
			else choice -= move.awesomeness;
		}
		
		// It should never get here, but just in case...
		return validMoves.get(0);
	}
	
	/* (non-Javadoc)
	 * @see quoridor.AIPlayer#distanceWeight()
	 */
	@Override
	protected int distanceWeight() {
		return 5;
	}

	/* (non-Javadoc)
	 * @see quoridor.AIPlayer#wallsLeftWeight()
	 */
	@Override
	protected int wallsLeftWeight() {
		return 1;
	}
}