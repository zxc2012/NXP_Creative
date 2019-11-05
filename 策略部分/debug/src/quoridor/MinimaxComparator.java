package quoridor;

import java.util.Comparator;

/**
 * A comparator for sorting moves before checking them with minimax.
 * The moves that are probably better (i.e. movement moves, and wall moves
 * that are close to the opponent) are placed first to optimise the
 * alpha-beta pruning algorithm.
 * @author Team Stump
 */
public class MinimaxComparator implements Comparator<Move> {

	/**
	 * The opponent's current space.
	 * (Wall moves close to this space are checked first as they are more
	 * likely to be useful moves.)
	 */
	//距离比较，7/2+1
	Space enemySpace = new Space(4, 4);

	public MinimaxComparator() {
	}

	/**
	 * Constructs the comparator setting the opponent's current space.
	 * @param enemySpace The opponent's current location.
	 */
	public MinimaxComparator(Space enemySpace) {
		setEnemySpace(enemySpace);
	}

	@Override
	public int compare(Move move1, Move move2) {
		if (move1 instanceof MovementMove && move2 instanceof MovementMove) return 0;
		else if (move1 instanceof MovementMove && move2 instanceof WallMove) return -1;
		else if (move1 instanceof WallMove && move2 instanceof MovementMove) return 1;
		else if (move1 instanceof WallMove && move2 instanceof WallMove) {
			Wall wall1 = ((WallMove)move1).wall();
			Wall wall2 = ((WallMove)move2).wall();
			double distDiff = distance(enemySpace, wall1) - distance(enemySpace, wall2);
			if (distDiff < 0) return -1;
			if (distDiff > 0) return 1;
			return 0;
		}
		return 0;
	}

	/**
	 * Used to set the opponent's current space.
	 * @param enemySpace The opponent's current location.
	 */
	public void setEnemySpace(Space enemySpace) {
		this.enemySpace = enemySpace;
	}
	
	/**
	 * The distance between the centre of a wall and a space.
	 * @param space The space.
	 * @param wall The wall.
	 * @return The distance as a double.
	 */
	private double distance(Space space, Wall wall) {
		double colDiff = (double) space.col() - (double) wall.getSpace().col() + 0.5;
		double rowDiff = (double) space.row() - (double) wall.getSpace().row() + 0.5;
		return Math.sqrt(colDiff * colDiff + rowDiff * rowDiff);
	}
}
