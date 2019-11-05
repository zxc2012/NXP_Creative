package quoridor;

public class DeadPlayer extends Player{

	/**
	 * This is "brain-less" players that are neither human or AI. They are solely for the use of the
	 * Validator. 
	 * 
	 * @param playerNumber - Player 1 or 2?
	 */
	public DeadPlayer(int playerNumber) {
		super(playerNumber);
	}	
}
