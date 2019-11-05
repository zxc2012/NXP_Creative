package quoridor;

import java.lang.Math;

/**
 * Represents a move from one space to another.
 * @author Team Stump
 */
public class MovementMove extends Move {

	private Space from;
	private Space to;
	private boolean isJump;
	
	/**
	 * Constructor
	 * @param from the Space to be moved from
	 * @param to the Space to be moved to
	 */
	public MovementMove(Space from, Space to) {
		if (!this.validate(from, to)) {
        	throw new RuntimeException("Not a valid quoridor move");
		}
		else {
			this.from = from;
			this.to = to;
		}
	}
	
	/**
	 * Checks that the move is a potentially valid quoridor move.
	 * This does NOT guarantee that the move is valid in the current game state.
	 * Also checks whether the move is a jump move.
	 * @param from The Space moved from.
	 * @param to The Space moved to.
	 * @return True if move is valid.
	 */
	private boolean validate(Space from, Space to) {
		if (from.equals(to)) return false;

		if (Math.abs(from.row()-to.row()) <= 1 && Math.abs(from.col()-to.col()) <= 1) {
			if (from.row()==to.row() || from.col()==to.col()) isJump = false;
			else isJump = true;
			return true;
		}
		if (from.row()==to.row() && Math.abs(from.col()-to.col()) == 2
		 || from.col()==to.col() && Math.abs(from.row()-to.row()) == 2) {
			isJump = true;
			return true;
		}
		return false;
	}
	

	/**
	 * Accessor for the Space that the player is moving from
	 * @return the space
	 */
	public Space from() {
		return from;
	}
	
	/**
	 * Accessor for the Space that the player is moving to
	 * @return the space
	 */
	public Space to() {
		return to;
	}
	
	/**
	 * Accessor for if the move is a jump
	 * @return the space
	 */
	public boolean isJump() {
		return isJump;
	}
	
	/* (non-Javadoc)
	 * @see java.lang.Object#toString()
	 */
	@Override
	public String toString(){
		return this.to.toOriginalString();
	}
	
	/* (non-Javadoc)
	 * @see java.lang.Object#equals(java.lang.Object)
	 */
	@Override
	public boolean equals(Object obj){
		if (!(obj instanceof MovementMove)) {
			return false;
		}
		MovementMove other = (MovementMove) obj;
	
		return (this.from().equals(other.from()) && this.to().equals(other.to()) && this.isJump() == other.isJump());	
	}
}
