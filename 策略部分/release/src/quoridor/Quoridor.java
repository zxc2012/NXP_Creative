package quoridor;



/**
 * The Quoridor class that get the game going
 */
public class Quoridor {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		Game game = new ConsoleGame();
		//communication a=new communication();
		game.playGame();
	}
}
