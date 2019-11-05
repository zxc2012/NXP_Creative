
package quoridor;

/**
 * An abstract class for quoridor moves.
 * @author Team Stump
 */
public abstract class Move {

	/**
	 * How good the move is. (To be used fleetingly by the AI algorithms.)
	 */
	public double awesomeness;
	
	public Player owner;

	/**
	 * Sets the owner for this move object
	 * @param player
	 */
	public void setOwner(Player player) {
		this.owner = player;
	}
	
	/**
	 * Accessor for the owner of this move object
	 * @return player owner
	 */
	public Player getOwner() {
		return owner;
	}
}
