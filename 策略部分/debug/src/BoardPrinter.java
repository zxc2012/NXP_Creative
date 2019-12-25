

/**
 * Handles the printing of the current state of the board to the commandline. 
 */
public class BoardPrinter {

	static final String V_WALL = "|";
	static final String H_WALL = "=";
	static final String V_DIVIDER = ":";
	static final String H_DIVIDER_0 = "...";
	static final String H_DIVIDER_1 = "---";
	static final String H_DIVIDER_2 = " - ";
	static final String CORNER_0 = " ";
	static final String CORNER_1 = "+";
	
	static int style = 0;

	/**
	 * Sets the particular board printer style
	 * @param newStyle as set by an int
	 */
	public static void setStyle(int newStyle) {
		if (newStyle >=0 && newStyle <= 2) style = newStyle;
		else throw new RuntimeException("Invalid style");
	}

	/**
	 * Sets the particular board printer style
	 * @param newStyle as set by a string
	 */
	public static void setStyle(String newStyle) {
		setStyle(Integer.parseInt(newStyle) - 1);
	}
	
	/**
	 * Returns the appropriate Horizontal divider, as set by the style
	 * @return the divider as a string.
	 */
	private static String getHDivider() {
		if (style == 1) return H_DIVIDER_1;
		if (style == 2) return H_DIVIDER_2;
		return H_DIVIDER_0;
	}

	/**
	 * Returns the appropriate corner, as set by the style
	 * @return the corner as a string.
	 */
	private static String getCorner() {
		if (style == 0) return CORNER_0;
		return CORNER_1;
	}

	
	/**
	 * Builds the board layout as one large string
	 * @param board to be printer
	 * @return the string representation of the board
	 */
	public static String buildBoardString(Board board) {
		StringBuilder boardString = new StringBuilder("   ---------------------------\n");
		Space thisSpace;
		int row, col, rowNum = 1;

		String hDivider = getHDivider();
		String corner = getCorner();
		
		// Create array for walls
		int [][] wallArray = new int [7][7];
		for (row = 0; row < 7; row++) {
			for (col = 0; col < 7; col++) {
				wallArray[row][col] = 0;
			}
		}

		// Fill array with wall locations
		for (Wall wall : board.getWallList()) {
			row = wall.getSpace().row() - 1;
			col = wall.getSpace().col() - 1;
			if (wall.isVertical()) wallArray[row][col] = 1;
			else wallArray[row][col] = 2;
		}

		for (row = 0; row < 7; row++) {
			// Print row of spaces
			boardString.append(rowNum+" |");
			rowNum++;
			for (col = 0; col < 7; col++) {
				thisSpace = new Space(col+1, row+1);
				
				if      (board.getSpace(board.getPlayers()._1()).equals(thisSpace)) boardString.append(board.getPlayers()._1().getToken());
				else if (board.getSpace(board.getPlayers()._2()).equals(thisSpace)) boardString.append(board.getPlayers()._2().getToken());
				else boardString.append("   ");
				
				if (col != 6) {
					if (wallArray[row][col] == 1 || row != 0 && wallArray[row-1][col] == 1) boardString.append(V_WALL);
					else boardString.append(V_DIVIDER);
				}
			}
			boardString.append("|\n");
			
			// Print divider between rows
			if (row != 6) {
				boardString.append("  |");
				for (col = 0; col < 7; col++) {
					if (wallArray[row][col] == 2 || col != 0 && wallArray[row][col-1] == 2) boardString.append(H_WALL + H_WALL + H_WALL);
					else boardString.append(hDivider);
					
					if (col != 6) {
						if (wallArray[row][col] == 1) boardString.append(V_WALL);
						else if (wallArray[row][col] == 2) boardString.append(H_WALL);
						else boardString.append(corner);
					}
				}
				boardString.append("|\n");
			}
		}
		/*标识4*6+3--a to g**/
		boardString.append("   ---------------------------\n    a   b   c   d   e   f   g");
		boardString.append(wallInfoToString(board));
		return boardString.toString();
	}
	
	/**
	 * Prints out the players remaining wall information 
	 * @param board the board who information is to be printed.
	 * @return the player wall information 
	 */
	private static String wallInfoToString(Board board) {
			Player player1 = board.players._1();
			Player player2 = board.players._2();
			String wallTally1 = (player1.getName() + " has "+ board.getWallsLeft(player1)+ " walls left" );
			String wallTally2 = (player2.getName() + " has "+ board.getWallsLeft(player2)+ " walls left" );
			return ("\n"+wallTally1+"\n"+wallTally2+"\n");
	}
	
}
