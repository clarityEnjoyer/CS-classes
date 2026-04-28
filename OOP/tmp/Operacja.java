package wyrazenia;

public abstract class Operacja extends Wyrażenie {

    private final Wyrażenie lewy;

    private final Wyrażenie prawy;

    public Operacja(Wyrażenie lewy, Wyrażenie prawy) {
        this.lewy = lewy;
        this.prawy = prawy;
    }

    protected Wyrażenie lewy() {
        return lewy;
    }

    protected Wyrażenie prawy() {
        return prawy;
    }

    protected abstract double oblicz(double wartośćLewego, double wartośćPrawego);

    @Override
    public double wartość(double wartośćZmiennej) {
        return oblicz(
                lewy.wartość(wartośćZmiennej),
                prawy.wartość(wartośćZmiennej));
    }

    protected abstract String operator();

    @Override
    public String toString() {
        return "(" + operator() + " " + lewy + " " + prawy + ")";
    }

    @Override
    public String toInfixString() {
        return "(" + lewy.toInfixString() + " " + operator() + " " +  prawy.toInfixString() + ")";
    }

}
