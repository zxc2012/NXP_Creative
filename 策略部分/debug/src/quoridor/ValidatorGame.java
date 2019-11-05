package quoridor;

import java.util.LinkedList;
import java.util.Queue;
import java.util.regex.Pattern;

/**
 * Designed for validator games
 *
 */
public class ValidatorGame extends Game{
	protected Queue<String> q = null;
	
	public ValidatorGame(String string) {
		if(string == null){
			throw new RuntimeException("String cannot be null");
		}
		this.breakUpStringIntoQueue(string);
		this.consoleGame = false;
		board = Factory.instance().makeBoard(gameType);
	}
	
	/**
	 * Places the validator string into a queue
	 * @param string the original validator string
	 */
	private void breakUpStringIntoQueue(String string) {
		q = new LinkedList<String>();
		Pattern p = Pattern.compile("[\\s]");
		String[] result = p.split(string);
		for(int i = 0; i < result.length; i++) {
			q.add(result[i]);
		}
	}

	/**
	 * This function controls the flow of game play.
	 */
	public boolean playGame() {
//		boolean movePlayed;
		while (!gameOver) {
//			movePlayed = false;
			try {
				playNextTurn();
			}
			catch (RuntimeException e) {
				return false;
			}
		}
		this.checkWin = board.checkWin();
		return true;
	}

	/* (non-Javadoc)
	 * @see quoridor.Game#playNextTurn()
	 */
	protected void playNextTurn() {
		if(q.size() > 0){
			if (board.checkWin() != 0) {
				throw new RuntimeException("Can't play move after win.");
			}
			board.makeMoveFromInput(q.remove());
		}
		else gameOver = true;
	}
}