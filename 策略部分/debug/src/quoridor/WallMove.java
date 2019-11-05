package quoridor;

/**
 * Represents a move where a wall is placed on the board.
 * @author Team Stump
 */
public class WallMove extends Move {

	private Wall wall;
	
	/**
	 * Constructor 
	 * @param wall the wall coordinates 
	 */
	public WallMove(Wall wall) {
		this.wall = wall;
	}
	
	/**
	 * Accessor 
	 * @return returns the wall
	 */
	public Wall wall() {
		return wall;
	}
	
	/* (non-Javadoc)
	 * @see java.lang.Object#toString()
	 */
	@Override
	public String toString(){
		return this.wall.toOriginalString();
	}
	
	/* (non-Javadoc)
	 * @see java.lang.Object#equals(java.lang.Object)
	 */
	@Override
	public boolean equals(Object obj){
		if (!(obj instanceof WallMove)) {
			return false;
		}
		WallMove other = (WallMove) obj;
		return (this.wall().equals(other.wall()));	
	}
}