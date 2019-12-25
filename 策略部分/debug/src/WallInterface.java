


/**
 * @author Stump
 * A wall can be either vertical or horizontal as it's location is defined by a space. 
 */
public interface WallInterface {	
	
	/**
	 * @return The space from which the wall is located.
	 */
	public Space getSpace();
	
	/**
	 * @return If the wall is vertical or not.
	 */
	public Boolean isVertical();
	
	/**
	 * @return If the wall is horizontal or not.
	 */
	public Boolean isHorizontal();
	
	/* (non-Javadoc)
	 * @see java.lang.Object#equals(java.lang.Object)
	 */
	@Override
	public boolean equals(Object obj);
}
