package quoridor;

public class Pair <A, B>
{
    public static <X, Y> Pair <X, Y> pair (X a, Y b)
    {
        return new Pair <X, Y> (a, b);
    }
    
    public final A _1;
    public final B _2;
    
    protected Pair (A a, B b)
    {
        this._1 = a;
        this._2 = b;
    }
    
    public A _1 ()
    {
        return _1;
    }
    
    public B _2 ()
    {
        return _2;
    }
    
    public boolean has (Object o)
    {
        return _1.equals (o) || _2.equals (o);
    }
    
    public Pair <A, B> copy ()
    {
        return this; // no need to copy immutable object
    }
    
    public Pair <B, A> swap ()
    {
        return pair (_2, _1);
    }
    
    @Override
    public String toString ()
    {
        return "(" + _1.toString () + ", " + _2.toString () + ")";
    }
}
