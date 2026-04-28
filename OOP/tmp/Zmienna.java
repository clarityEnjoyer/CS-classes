package wyrazenia;

public class Zmienna extends Wyrażenie {

    private static Zmienna instancja = null;

    private Zmienna() {
        // puste
    }

    public static Zmienna instancja() {
        if (instancja == null) {
            instancja = new Zmienna();
        }
        return instancja;
    }

    @Override
    public String toString() {
        return "x";
    }

    @Override
    public String toInfixString() {
        return "x";
    }

    @Override
    public double wartość(double wartośćZmiennej) {
        return wartośćZmiennej;
    }

    @Override
    public Wyrażenie pochodna() {
        return Stała.oWartości(1);
    }

}
