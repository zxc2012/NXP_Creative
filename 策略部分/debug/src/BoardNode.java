

import java.util.ArrayList;

/**
 *  A Node is an Space that has been extended to factor in connectivity for shortest path algorithm.
 *
 */
public class BoardNode {

	public Space space;
	public BoardNode up;
	public BoardNode right;
	public BoardNode down;
	public BoardNode left;
	public ArrayList<BoardNode> path;
	public int distanceToExit;
	
	/**
	 * Node constructor. 
	 * @param space The passed space from the board. 
	 */
	public BoardNode(Space space) {
		this.space = space;
		up = null;
		right = null;
		down = null;
		left = null;
		path = new ArrayList<BoardNode>();
		distanceToExit = -1;
	}
}
