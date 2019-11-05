package quoridor;

import static org.junit.Assert.*;

import org.junit.Test;

import java.util.ArrayList;
import java.util.List;

import static java.util.Arrays.asList;

public class ProvidedTests
{
    
    static final String [] ALL_FENCES = {"a1h", "a1v", "a2h", "a2v", "a3h",
            "a3v", "a4h", "a4v", "a5h", "a5v", "a6h", "a6v", "b1h", "b1v",
            "b2h", "b2v", "b3h", "b3v", "b4h",
            "b4v", "b5h", "b5v", "b6h", "b6v",
            "c1h", "c1v", "c2h", "c2v", "c3h", "c3v", "c4h", "c4v", "c5h",
            "c5v", "c6h", "c6v", "d1h", "d1v",
            "d2h", "d2v", "d3h", "d3v", "d4h", "d4v", "d5h", "d5v", "d6h",
            "d6v", "e1h", "e1v", "e2h", "e2v",
            "e3h", "e3v", "e4h", "e4v", "e5h", "e5v", "e6h", "e6v", "f1h",
            "f1v", "f2h", "f2v", "f3h", "f3v",
            "f4h", "f4v", "f5h", "f5v", "f6h", "f6v", "g1h",
            "g1v", "g2h", "g2v", "g3h", "g3v", "g4h", "g4v",
            "g5h", "g5v", "g6h", "g6v"};
    
    Validator              validator  = new Validator (); // Validator is
                                                          // student supplied
                                                          // class
                                                          
    void checkValid ()
    {
        int n = moves.size ();
        List <String> testMoves = new ArrayList <String> (n + 1);
        testMoves.addAll (moves);
        testMoves.add (goodMove);
        
        validator = new Validator (); // initialise quoridor
        assertTrue ("Valid: " + testMoves, validator.check(testMoves));
   
        testMoves.remove (n);
        testMoves.add (badMove);
        
        validator = new Validator (); // re-initialise quoridor
        assertFalse ("Invalid: " + testMoves, validator.check(testMoves));
    }
    
    List <String> moves;   // assumed valid list of moves from start of game
                            
    String        goodMove; // good next move to be tested for validity
    String        badMove; // bad next move to be tested for validity
                            
    @Test
    public void init ()
    {
        moves = asList ();
        goodMove = "d6";
        badMove = "e7";
        checkValid ();
    }
    
    @Test
    public void one ()
    {
        moves = asList ("e8");
        goodMove = "d2";
        badMove = "e7";
        checkValid ();
    }
    
    @Test
    public void two ()
    {
        moves = asList ("d6", "d2");
        goodMove = "e7";
        badMove = "f7";
        checkValid ();
    }
    
    @Test
    public void end ()
    {
        moves = asList ("e8", "e2", "e7", "e3", "e6", "e4", "e5", "e6", "e4",
                "e7", "e3", "e8", "e2");
        goodMove = "e9";
        badMove = "e1";
        checkValid ();
    }
    
    @Test
    public void side ()
    {
        moves = asList ("e8", "e2", "e7", "e3", "e6", "e4", "e5");
        goodMove = "d4";
        badMove = "a4";
        checkValid ();
    }
    
    @Test
    public void fence ()
    {
        moves = asList ();
        goodMove = "e1h";
        badMove = "a9h";
        checkValid ();
    }
    
    @Test
    public void jump ()
    {
        moves = asList ("e8", "e2", "e7", "e3", "e6", "e4", "e5");
        goodMove = "e6";
        badMove = "e5";
        checkValid ();
    }
    
    @Test
    public void jump2 ()
    {
        moves = asList ("e8", "e2", "e7", "e3", "e6", "e4", "e5", "e6");
        goodMove = "e7";
        badMove = "e3";
        checkValid ();
    }
    
    @Test
    public void block ()
    {
        moves = asList ("e8", "d7h");
        goodMove = "f8";
        badMove = "e7";
        checkValid ();
    }
    
    @Test
    public void blockJump ()
    {
        moves = asList ("e8", "e2", "e7", "e3", "e6", "e4", "e5", "e4h");
        goodMove = "d5";
        badMove = "e3";
        checkValid ();
    }
    
    @Test
    public void noBlockJump ()
    {
        moves = asList ("e8", "e2", "e7", "e3", "e6", "e4", "e5", "e2h");
        goodMove = "e3";
        badMove = "f4";
        checkValid ();
    }
    
    @Test
    public void crossedFence ()
    {
        moves = asList ("e8", "e2", "e7", "e3", "e6", "e4", "e5", "d2h");
        goodMove = "e2v";
        badMove = "d2v";
        checkValid ();
    }
    
    @Test
    public void overlappedFence ()
    {
        moves = asList ("e8", "e2", "e7", "e3", "e6", "e4", "e5", "d2v");
        goodMove = "d4v";
        badMove = "d3v";
        checkValid ();
    }
    
    @Test
    public void angleJump ()
    {
        moves = asList ("e8", "e2", "e7", "e3", "e6", "e4", "e5", "d3h");
        goodMove = "f4";
        badMove = "e3";
        checkValid ();
    }
    
    @Test
    public void tenFences ()
    {
        moves = asList ("e1h", "f1", "g1h", "e8h", "d1v", "g1", "h2v", "h1",
                "h4h", "i1", "g3v", "i2", "f2h", "i3", "e3h", "i4", "d3v",
                "h4", "f4h", "h3");
        goodMove = "d9";
        badMove = "d5h";
        checkValid ();
    }
    
    @Test
    public void trap ()
    {
        moves = asList ("e8", "e2", "e7", "e3", "e6", "e4", "e5", "d3h", "d4h",
                "e4v");
        goodMove = "b4h";
        badMove = "c4v";
        checkValid ();
    }
    
    @Test
    public void trap2 ()
    {
        moves = asList ("e1h", "f1", "g1h", "e8h", "d1v", "g1", "h2v", "h1");
        goodMove = "d9";
        badMove = "h3h";
        checkValid ();
    }
    
    @Test
    public void barrier ()
    {
        moves = asList ("a5h", "e2", "c5h", "e3", "e5h", "e4", "e4v", "h4h");
        goodMove = "f3h";
        badMove = "f4h";
        checkValid ();
    }
}
