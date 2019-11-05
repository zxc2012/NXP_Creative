package quoridor;

/**
 * @author Stump
 * A wall can be either vertical or horizontal as it's location is defined by a space. 
 */
public class Wall implements WallInterface{
	private Space space;
	private Boolean vertical;
	/***参数匹配，这就改了个9***/
	private final int FIRST_CHAR = 0;
	private final int THIRD_CHAR = 2;
	private final int LAST_ROW_OR_COL = 7;
	private final int WALL_TYPE = 2;
	private final int VALID_INPUT = 3;
	
	/**
	 * Sets the type and coordinates of the wall. 
	 * @param space  
	 * @param vertical If the wall is vertical or horizontal as defined by a boolean.
	 * @exception 
	 */
	public Wall(Space space, Boolean vertical) {
		if(!validWallSpace(space)){
			throw new RuntimeException("Not a valid Wall");
		} else {
			this.space = space;
			this.vertical = vertical;
		}
	}
	/**
	 * Sets the type and coordinates of the wall. 
	 * @param command  
	 * @exception 
	 */
	public Wall(String command) {
		assert(command.length() == VALID_INPUT);
		String wallType = command.toLowerCase().substring(WALL_TYPE);
		if(wallType.equalsIgnoreCase("h")) {
			this.vertical = false;
		} else if (wallType.equalsIgnoreCase("v")) {
			this.vertical = true;
		} else {
			throw new RuntimeException("Invalid wall");
		}
		
		Space space = new Space(command.substring(FIRST_CHAR,THIRD_CHAR));
		if(!validWallSpace(space)){
			throw new RuntimeException("Invalid wall");
		} else {
			this.space = space;
		}		
	}

	/**
	 * @return The space from which the wall is located.
	 */
	public Space getSpace() {
		return space;
	}
	
	/**
	 * Checks if it is a valid wall space and returns the result as boolean
	 * @param space the space that is being checked. 
	 * @return the result 
	 */
	private boolean validWallSpace(Space space) {
		return !(space.col() == LAST_ROW_OR_COL || space.row() == LAST_ROW_OR_COL);
	}
	
	/**
	 * @return If the wall is vertical or not.
	 */
	public Boolean isVertical() {
		return vertical;
	}
	
	/**
	 * @return If the wall is horizontal or not.
	 */
	public Boolean isHorizontal() {
		return !vertical;
	}
	
	/* (non-Javadoc)
	 * @see java.lang.Object#equals(java.lang.Object)
	 */
	@Override
	public boolean equals(Object obj) {
		if (!(obj instanceof Wall)) return false;
		Wall otherWall = (Wall) obj;
		return (this.getSpace().equals(otherWall.getSpace()) && this.isVertical() == otherWall.isVertical());
	}
	
	/**
	 * Places the wall information back into its original form. 
	 * @return
	 */
	public String toOriginalString(){
		String wallType;
		if(this.vertical) {
			wallType = "v";
		} else {
			wallType ="h";
		}
		return this.getSpace().toOriginalString().concat(wallType);
	}
}
