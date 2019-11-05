package quoridor;

import java.util.ArrayList;
import java.util.List;

/**
 * A graph structure representing the spaces on the board.
 * Used for finding distances to the exits and checking if a wall move
 * would completely block a player's path.
 */

public class BoardGraph {
	protected BoardNode [][] node;
	
	/**
	 * Constructs a BoardGraph for a new board with no walls.
	 */
	public BoardGraph() {
		int row, col;
		node = new BoardNode [7][7];
		for (row = 0; row < 7; row++) {
			for (col = 0; col < 7; col++) {
				node[row][col] = new BoardNode(new Space(col + 1, row + 1));
			}
		}
		for (row = 0; row < 7; row++) {
			for (col = 0; col < 7; col++) {
				if (row == 0) node[row][col].up = null;
				else node[row][col].up = node[row-1][col];
				if (row == 6) node[row][col].down = null;
				else node[row][col].down = node[row+1][col];
				if (col == 0) node[row][col].left = null;
				else node[row][col].left = node[row][col-1];
				if (col == 6) node[row][col].right = null;
				else node[row][col].right = node[row][col+1];
			}
		}
	}
	
	/**
	 * Removes edges between nodes that have been separated by a new wall.
	 * @param wall The wall that has been added.
	 */
	public void addWall(Wall wall) {
		int row = wall.getSpace().row() - 1;
		int col = wall.getSpace().col() - 1;
		assert(row>=0 && row<7 && col>=0 && col<7);
		if (wall.isVertical()) {
			node[row][col].right = null;
			node[row+1][col].right = null;
			node[row][col+1].left = null;
			node[row+1][col+1].left = null;
		}
		else {
			node[row][col].down = null;
			node[row][col+1].down = null;
			node[row+1][col].up = null;
			node[row+1][col+1].up = null;
		}
	}
	
	/**
	 * Restores edges between nodes when a wall is removed (e.g. with undo).
	 * @param wall The removed wall.
	 */
	public void removeWall(Wall wall) {
		int row = wall.getSpace().row() - 1;
		int col = wall.getSpace().col() - 1;
		assert(row>=0 && row<7 && col>=0 && col<7);
		if (wall.isVertical()) {
			node[row][col].right = node[row][col+1];
			node[row+1][col].right = node[row+1][col+1];
			node[row][col+1].left = node[row][col];
			node[row+1][col+1].left = node[row+1][col];
		}
		else {
			node[row][col].down = node[row+1][col];
			node[row][col+1].down = node[row+1][col+1];
			node[row+1][col].up = node[row][col];
			node[row+1][col+1].up = node[row][col+1];
		}
	}
	
	/**
	 * Performs a breadth-first search across the graph, filling in each node's
	 * distanceToExit attribute with its shortest distance to one of the exit nodes.
	 * This is a player's least possible distance to get to its exit row, not considering
	 * jump moves or additional wall placements.
	 * This must be done for a single player before examining individual spaces with getDist().
	 * @param exits A list of target spaces which make up the player's exit row
	 */
	@SuppressWarnings("unchecked")
	public void fillNodeDistances(List<Space> exits) {
		ArrayList<BoardNode> currentNodes = new ArrayList<BoardNode>();
		ArrayList<BoardNode> nextNodes = new ArrayList<BoardNode>();
		BoardNode checkNode;
		int row, col;
		for (row = 0; row < 7; row++) {
			for (col = 0; col < 7; col++) {
				node[row][col].distanceToExit = -1;
			}
		}
		for (Space space : exits) {
			node[space.row()-1][space.col()-1].distanceToExit = 0;
			currentNodes.add(node[space.row()-1][space.col()-1]);
		}
		
		while (currentNodes.size() > 0) {
			nextNodes.clear();
			for (BoardNode thisNode : currentNodes) {
				checkNode = tryNode(thisNode.up, thisNode.distanceToExit);
				if (checkNode != null && !nextNodes.contains(checkNode)) nextNodes.add(checkNode);
				checkNode = tryNode(thisNode.right, thisNode.distanceToExit);
				if (checkNode != null && !nextNodes.contains(checkNode)) nextNodes.add(checkNode);
				checkNode = tryNode(thisNode.down, thisNode.distanceToExit);
				if (checkNode != null && !nextNodes.contains(checkNode)) nextNodes.add(checkNode);
				checkNode = tryNode(thisNode.left, thisNode.distanceToExit);
				if (checkNode != null && !nextNodes.contains(checkNode)) nextNodes.add(checkNode);
			}
			currentNodes = (ArrayList<BoardNode>) nextNodes.clone();
		}
	}

	/**
	 * Checks whether the search so far has found a shorter path to a node
	 * than a previously found path.
	 * @param thisNode The node to be checked.
	 * @param distance The distance travelled in the new path.
	 * @return The node if a new path was found, otherwise null
	 */
	private BoardNode tryNode(BoardNode thisNode, int distance) {
		if (thisNode == null) return null;
		if (thisNode.distanceToExit != -1 && distance + 1 >= thisNode.distanceToExit) return null;
		thisNode.distanceToExit = distance + 1;
		return thisNode;
	}
	
	/**
	 * Prints a grid of the distances found from the most recent call
	 * to fillNodeDistances().
	 * Only used for testing.
	 */
	public void printDistanceFills() {
		int row, col;
		for (row = 0; row < 7; row++) {
			System.out.print(" ");
			for (col = 0; col < 7; col++) {
				System.out.printf("%4d", node[row][col].distanceToExit);
			}
			System.out.println();
		}
	}
	
	/**
	 * Gets the distanceToExit value for a particular space, as found from
	 * the most recent call to fillNodeDistances().
	 * @param space
	 * @return
	 */
	public int getDist(Space space) {
		return node[space.row()-1][space.col()-1].distanceToExit;
	}
}
