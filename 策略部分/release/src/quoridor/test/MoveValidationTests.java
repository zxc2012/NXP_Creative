package quoridor.test;

import static org.junit.Assert.assertTrue;
import static org.junit.Assert.assertFalse;

import org.junit.Before;
import org.junit.Test;

import quoridor.Factory;
import quoridor.Game;

/**
 * Tests moves as explained by the method names
 */
public class MoveValidationTests {
	
	@Before
	public void setUp() throws Exception {
	}
	
	@Test
	public void testCannotMoveDownTwoPlaces() {
		String testString = "e3";
		Game game = Factory.instance().makeGame(testString);
		assertFalse(game.playGame());
	}
	
	@Test
	public void testCannotMoveLeftTwoPlaces() {
		String testString = "c1";
		Game game = Factory.instance().makeGame(testString);
		assertFalse(game.playGame());
	}
	
	@Test
	public void testCannotMoveRightTwoPlaces() {
		String testString = "g1";
		Game game = Factory.instance().makeGame(testString);
		assertFalse(game.playGame());
	}
	
	@Test
	public void testCannotMoveUpTwoPlaces() {
		String testString = "e2 e7";
		Game game = Factory.instance().makeGame(testString);
		assertFalse(game.playGame());
	}
	
	@Test
	public void NoWinner() {
		String testString = "e8 e2 e7 e3 d7 e4";
		Game game = Factory.instance().makeGame(testString);
		assertTrue(game.playGame() && game.checkWin() == 0);
	}
	
	//
	@Test
	public void testP1Winner() {
		String testString = "e8 d1 e7 d2 e6 d3 e5 d4 e4 d5 e3 d6 e2 d7 e1";
		Game game = Factory.instance().makeGame(testString);
		assertTrue(game.playGame() && game.checkWin() == 1);
	}
	
	
	//
	@Test
	public void testP1WinnerThusP2MoveInvalid() {
		String testString = "e8 d1 e7 d2 e6 d3 e5 d4 e4 d5 e3 d6 e2 d7 e1 d8";
		Game game = Factory.instance().makeGame(testString);
		assertFalse(game.playGame());
		assertTrue(game.checkWin() == 1);
	}
	
	//
	@Test
	public void testP2Winner() {

		String testString = "e8 e2 e7 e3 d7 e4 d6 e5 d5 e6 d4 e7 d3 e8 d2 e9";
		Game game = Factory.instance().makeGame(testString);
		assertTrue(game.playGame() && game.checkWin() == 2);
	}
	
	//
	@Test
	public void testP2WinnerThusP1MoveInvalid() {
		String testString = "e8 e2 e7 e3 d7 e4 d6 e5 d5 e6 d4 e7 d3 e8 d2 e9 d3";	
		Game game = Factory.instance().makeGame(testString);
		assertFalse(game.playGame());
		assertTrue(game.checkWin() == 2);
	}
	
	@Test
	public void testCannotBlockOffPath(){
		String testString = "a5h c5h e5h g5h h5v h6h";
		Game game = Factory.instance().makeGame(testString);
		assertFalse(game.playGame());
		assertTrue(game.checkWin() == 0);
	}
	
	//
	@Test
	public void testPlaceTwentyWalls(){
		String testString = "a2h c2h e2h g2h a3h c3h e3h g3h a4h c4h e4h g4h a5h c5h e5h g5h a6h c6h e6h g6h";
		Game game = Factory.instance().makeGame(testString);
		assertTrue(game.playGame());
		assertTrue(game.checkWin() == 0);
	}
	
	//
	@Test
	public void testInvalidToPlaceTwentyPlusWalls(){
		String testString = "a2h c2h e2h g2h a3h c3h e3h g3h a4h c4h e4h g4h a5h c5h e5h g5h a6h c6h e6h g6h a7h";
		Game game = Factory.instance().makeGame(testString);
		assertFalse(game.playGame());
		assertTrue(game.checkWin() == 0);
	}
	
	
	@Test
	public void testInvalidForPlayerToPlaceMoreThenTenWallsPerPlayer(){
		String testString = "d9 f1 a1h g1 a2h h1 a3h i1 a4h i2 a5h i3 a6h i4 a7h i5 a8h i6 c1h i7 c2h i8 c5h";
		Game game = Factory.instance().makeGame(testString);
		assertFalse(game.playGame());
		assertTrue(game.checkWin() == 0);
	}
	
	@Test
	public void testStayingInOriginalSpotDoesNotCountAsAWin(){
		String testString = "d9 f1 a1h g1 a2h h1 a3h i1 a4h i2 a5h i3 a6h i4 a7h i5 a8h i6 c1h i7 c2h i8";
		Game game = Factory.instance().makeGame(testString);
		assertTrue(game.playGame());
		assertTrue(game.checkWin() == 0);
	}
}
