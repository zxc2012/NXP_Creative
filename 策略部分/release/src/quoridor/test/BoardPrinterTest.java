package quoridor.test;

import quoridor.*;

/**
 * Prints out the board
 */
public class BoardPrinterTest {

	public static void main(String[] args) {
		Board board = Factory.instance().makeBoard(0);
		board.addWall(new Wall (new Space (2,2), false));
		board.addWall(new Wall (new Space (4,1), false));
		BoardPrinter.setStyle(0);
		board.print();
		BoardPrinter.setStyle(1);
		board.print();
		BoardPrinter.setStyle(2);
		board.print();
	}
}
