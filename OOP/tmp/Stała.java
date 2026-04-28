package wyrazenia;

public abstract class Stała extends Wyrażenie {

    private static final double EPSILON = 0.0000001;

    public static Stała oWartości(double wartość) {
        if (Math.abs(wartość) < EPSILON) {
            return new Zero();
        } else if (Math.abs(wartość - 1) < EPSILON) {
            return new Jeden();
        } else {
            return new Zwykła(wartość);
        }
    }

    protected abstract double wartość();

    @Override
    public String toString() {
        return String.valueOf(wartość());
    }

    @Override
    public String toInfixString() {
        return String.valueOf(wartość());
    }

    @Override
    public double wartość(double wartośćZmiennej) {
        return wartość();
    }

    @Override
    public Wyrażenie pochodna() {
        return Stała.oWartości(0);
    }

    @Override
    public double całka(double początek, double koniec, int nPodrzedziałów) {
        return (koniec - początek) * wartość();
    }

}
