package quoridor;

import java.util.Collections;
import java.util.LinkedList;
import java.util.List;

/**
 * AI player capable of looking multiple moves ahead using the
 * negamax algorithm with alpha-beta pruning.
 * @author Team Stump
 */
public class MultipleLookAIPlayer extends AIPlayer {

	protected int maxDepth = 3;
	public static int i = 0;
	long startTime;
	protected long timeOut = 5000;
	
	/**
	 * Constructor sets the specifications 
	 * 
	 * @param playerNumber the players number
	 * @param lookAhead how far the AI player will look ahead
	 * @param timeOut the time out
	 */
	public MultipleLookAIPlayer(int playerNumber, int lookAhead, int timeOut) {
		super(playerNumber);
		this.maxDepth = lookAhead;
		this.timeOut = timeOut;
	}

	/**
	 * Looks at the board and figures out what move to make.
	 * Calls negamax() for each possible move and picks the best one.
	 * If there are multiple equally best, it picks one of them at random.
	 * @see quoridor.Player#getMove(quoridor.Board)
	 */
	public Move getMove(Board board) {
		assert(this.equals(board.currentPlayer()));
		Board newBoard;
		List<Move> potentialMoves = validMoves(board);
		List<Move> bestMoves = new LinkedList<Move>();
		Collections.sort(potentialMoves, new MinimaxComparator(board.getSpace(board.players.other(this))));
		int bestCase = Integer.MIN_VALUE;
		int evaluation;
		int depth;
		startTime = System.currentTimeMillis();
		for (depth = 1; depth <= maxDepth && System.currentTimeMillis() - startTime < timeOut; depth++) {
			for (Move move : potentialMoves) {
				try {
					newBoard = board.clone();
					newBoard.makeMove(move);
					evaluation = -negamax(newBoard, Integer.MIN_VALUE + 1, Integer.MAX_VALUE, depth - 1);
					move.awesomeness = evaluation;
					if (evaluation > bestCase) {
						bestMoves.clear();
						bestMoves.add(move);
						bestCase = evaluation;
					}
					else if (evaluation == bestCase) {
						bestMoves.add(move);
					}
				}
				catch (Exception e) {}
			}
			Collections.sort(potentialMoves, new MoveComparator());
		}
		
		// If there are several equal best moves, pick one at random
		int pick = (int) Math.floor(bestMoves.size() * Math.random());
		System.out.println(i);
		i = 0;
		return bestMoves.get(pick);
	}

	/**
	 * Performs a negamax search of the game tree to evaluate a game state.
	 * Uses alpha-beta pruning to reduce search time.
	 * @param board The board state under consideration.
	 * @param alpha Alpha parameter of alpha-beta.
	 * @param beta Beta parameter of alpha-beta.
	 * @param depth Depth to search the game tree.
	 * @return Negamax value.
	 */
	private int negamax(Board board, int alpha, int beta, int depth) {
		i++;
		if (board.currentPlayer().equals(board.winner())) return Integer.MAX_VALUE;
		else if (board.players.other(board.currentPlayer()).equals(board.winner())) return Integer.MIN_VALUE + 1;
		else if (depth <= 0 || System.currentTimeMillis() - startTime > timeOut) return board.currentPlayer().minMax() * evaluate(board);
		
		Board newBoard;
		List<Move> potentialMoves = board.currentPlayer().allMoves(board);
		Collections.sort(potentialMoves, new MinimaxComparator(board.getSpace(board.players.other(board.currentPlayer()))));
		int evaluation;
		for (Move move : potentialMoves) {
			try {
				newBoard = board.clone();
				newBoard.makeMove(move);
				evaluation = -negamax(newBoard, -beta, -alpha, depth - 1);
				if (evaluation > alpha) alpha = evaluation;
				if (alpha >= beta) return alpha;
			}
			catch (Exception e) {}
		}
		
		return alpha;
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
		return 2;
	}

}
