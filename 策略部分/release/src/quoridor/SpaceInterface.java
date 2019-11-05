package quoridor;


public interface SpaceInterface {
	
	
    /**
     * @return the column associated with this space
     */
    public int col();
    
    /**
     * @return the row associated with this space
     */
    public int row();

    /**
     * obtain the space above
     * @return space above this space
     */
    public SpaceInterface getUp();

    /**
     * obtain the space below
     * @return space below this space
     */
    public SpaceInterface getDown();

    /**
     * obtain the space to the left
     * @return space to left of this space
     */
    public SpaceInterface getLeft();

    /**
     * obtain the space to the right
     * @return space to right of this space
     */
    public SpaceInterface getRight();

    /* (non-Javadoc)
     * @see java.lang.Object#equals(java.lang.Object)
     */
    @Override
    public boolean equals(Object obj);
}
