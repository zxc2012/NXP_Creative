package quoridor.test;


import org.junit.Before;
import org.junit.Test;

import quoridor.Move;
import quoridor.MovementMove;
import quoridor.Space;

public class SpaceTest {
	
	Space space;

	@Before
	public void setUp() throws Exception {
		space = new Space("a",5);
	}
	
	@Test
	public void test1(){
		Move m1 = new MovementMove(new Space("a",5), new Space("a",6));
		m1 = (MovementMove) m1;
		m1.toString();
		System.out.println(space.toOriginalString());
		System.out.println(m1.toString());
	}

}
