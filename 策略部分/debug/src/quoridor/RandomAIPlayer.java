
package quoridor;

/**
 * An AI player that picks a random move.
 * @author Team Stump
 */
public class RandomAIPlayer extends AIPlayer {

	public RandomAIPlayer(int playerNumber) {
		super(playerNumber);
	}

	/* (non-Javadoc)
	 * @see quoridor.Player#getMove(quoridor.Board)
	 */
	public Move getMove(Board board) {
		Move move = null;
		boolean valid = false;
		while (!valid) {
			try {
				if (Math.random() < 0.7) {
					// Do a movement move
					double dir = Math.random();
					if      (dir < 0.25) move = new MovementMove(board.getSpace(this), board.getSpace(this).getUp());
					else if (dir < 0.5)  move = new MovementMove(board.getSpace(this), board.getSpace(this).getRight());
					else if (dir < 0.75) move = new MovementMove(board.getSpace(this), board.getSpace(this).getDown());
					else                 move = new MovementMove(board.getSpace(this), board.getSpace(this).getLeft());
				}
				else {
					// Do a wall move
					int row = (int) Math.floor(Math.random() * 7);
					int col = (int) Math.floor(Math.random() * 7);
					boolean vertical = (Math.random() < 0.5);
					move = new WallMove(new Wall(new Space(col, row), vertical));
				}
				valid = board.moveValid(move);
			}
			catch (RuntimeException e) {
				valid = false;
			}
		}
		return move;
	}

	/* (non-Javadoc)
	 * @see quoridor.AIPlayer#distanceWeight()
	 */
	@Override
	protected int distanceWeight() {
		return 0;
	}

	/* (non-Javadoc)
	 * @see quoridor.AIPlayer#wallsLeftWeight()
	 */
	@Override
	protected int wallsLeftWeight() {
		return 0;
	}

}
