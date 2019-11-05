package quoridor.test;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import org.junit.Test;
import quoridor.Factory;
import quoridor.Game;

public class MiscTests {

	/**
	 * Tests that two different instantiations of the same game as counted as equal 
	 */
	@Test
	public void gamesAreEqual(){
		Game game1 = Factory.instance().makeGame("d9 f1 a1h");
		Game game2 = Factory.instance().makeGame("d9 f1 a1h");
		game1.playGame();
		game2.playGame();
		assertTrue(game1.equals(game2));
	}
	
	/**
	 *  * Tests that two different instantiations of the different games are NOT counted as equal 
	 */
	@Test
	public void gamesAreNotEqual(){
		Game game1 = Factory.instance().makeGame("d9 f1 a1h");
		Game game2 = Factory.instance().makeGame("d9");
		game1.playGame();
		game2.playGame();
		assertFalse(game1.equals(game2));
	}
	
	/**
	 * Tests loading overwrites an old game with the correct information
	 */
	@Test
	public void savedAndLoadedGameAreEqual(){
		String testString1 = "d9 f1 a1h g1 a2h h1 a3h i1 a4h i2 a5h i3";
		String testString2 = "d9 f1 a1h g1 a2h h1";
		Game game = Factory.instance().makeGame(testString1);
		assertTrue(game.playGame());
		game.save("test");
		Game newGame = Factory.instance().makeGame(testString2);
		newGame.load("test");
		newGame.playGame();
		game.equals(newGame);
	}
}
