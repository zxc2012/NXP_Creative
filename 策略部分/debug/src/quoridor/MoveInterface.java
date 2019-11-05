package quoridor;




/**
 * An abstract class for quoridor moves.
 * @author Team Stump
 */
public abstract class MoveInterface {
	public PlayerInterface owner;

	/**
	 * Sets the owner for this move object
	 * @param player
	 */
	public void setOwner(PlayerInterface player) {
		this.owner = player;
	}
	
	/**
	 * Accessor for the owner of this move object
	 * @return player owner
	 */
	public PlayerInterface getOwner() {
		return owner;
	}
}
