package quoridor.test;

import java.util.ArrayList;

import quoridor.*;

/**
 * Tests printing out of the board and displays shortest paths info
 */
public class BoardGraphTest { 

	public static void main(String[] args) {
		Board board = Factory.instance().makeBoard(0);

		board.addWall(new Wall(new Space(3, 8), true));
		board.addWall(new Wall(new Space(4, 7), false));
		board.addWall(new Wall(new Space(6, 7), false));
		board.addWall(new Wall(new Space(7, 7), true));
		board.addWall(new Wall(new Space(6, 8), false));
		board.addWall(new Wall(new Space(1, 3), false));

		BoardPrinter.setStyle(0);
		board.print();
		
		ArrayList<Space> exits = new ArrayList<Space>();
		for (int i = 1; i <=7; i++) exits.add(new Space(i, 1));
		board.graph.fillNodeDistances(exits);
		board.graph.printDistanceFills();
	}
}
