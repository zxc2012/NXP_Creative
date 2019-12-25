


public class Validator {

	Game sampleGame;
	
	public Validator() {
	}
	
	public boolean check(java.util.List<String> testMoves) {
		
		String movesString = testMoves.get(0)+" ";
		
		for(int i=1; i< testMoves.size(); i++) {
			movesString += testMoves.get(i);
			movesString += " ";
		}
		movesString = movesString.substring(0, movesString.length()-1);
		System.out.println("testing moves: "+movesString);
		return check(movesString);
	}

	/**
	 * Check the validity of a given sequence of moves.
	 * The sequence is valid if and only if each (space separated)
	 * move in the list is valid,
	 * starting from the initial position of the game.
	 * When the game has been won, no further moves are valid.
	 * @param moves a list of successive moves
	 * @return validity of the list of moves
	 */
	public boolean check(String moves){
		Game game = Factory.instance().makeGame(moves);
		return game.playGame();
	}
}