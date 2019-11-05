/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package quoridor;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

/**
 *
 * @author Team Stump
 */

/**
 * Extends Player to make a class that pertains to human based console played games. 
 *
 */
public class HumanPlayer extends Player {

	/**
	 * This sets up a player by calling setUpUser() to get commandline input
	 * @param referenceNumber which player - one or two as an int
	 */
	public HumanPlayer(int referenceNumber) {
        super(referenceNumber);
        this.setName("ycy");
        setToken("^_^");
    }
	
	/**
	 * This constructor is used when all the players details are already knows. Like when loading a game.
	 * @param referenceNumber player 1 or 2 as an int
	 * @param name the players name as a string
	 * @param token the players token as a string
	 */
	public HumanPlayer(int referenceNumber, String name, String token) {
		super(referenceNumber);
        super.setName(name);
        super.setToken(token);
    }

    
    /**
	 * Read one line from user input
	 * @return the read line as a String
	 */
    protected String getFromUser() {
        try {
            while (true) {
                BufferedReader userReader =
                        new BufferedReader(new InputStreamReader(System.in));
                String fromUser = userReader.readLine();
                if (fromUser.trim().length() > 0) { //never returns an empty string
                    return fromUser;
                } else {
                	System.out.println("Sorry empty strings are not valid input");
					System.out.print("Please try again: ");
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }
    }
}

