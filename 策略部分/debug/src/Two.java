

public class Two <A> extends Pair <A, A> {

	public static <X> Two <X> two (X a1, X a2)
    {
        return new Two <X> (a1, a2);
    }
    
    protected Two (A a1, A a2)
    {
        super (a1, a2);
    }
    
    public A other (A a)
    {
        if (a.equals (_1))
            return _2;
        if (a.equals (_2))
            return _1;
        throw new IllegalArgumentException (a + " is not in pair " + this);
    }
    
    @Override
    public Two <A> swap ()
    {
        return two (_2, _1);
    }
    
}
