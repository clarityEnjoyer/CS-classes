package wyrazenia;

public abstract class Wyrażenie {

    public static final int N_PODRZEDZIAŁÓW = 1000;

    public abstract String toString();

    public abstract String toInfixString();

    public abstract double wartość(double wartośćZmiennej);

    public abstract Wyrażenie pochodna();

    public double całka(double początek, double koniec, int nPodrzedziałów) {
        double długość = (koniec - początek) / nPodrzedziałów;
        double suma = 0;
        for (double p = początek + długość; p + długość <= koniec; p += długość) {
            suma += wartość(p);
        }
        return (2 * suma + wartość(początek) + wartość(koniec)) / 2 * długość;
    }

    public double całka(double początek, double koniec) {
        return całka(początek, koniec, N_PODRZEDZIAŁÓW);
    }

    public Wyrażenie plus(Wyrażenie wyrażenie) {
        return wyrażenie.odwróconyPlus(this);
    }

    protected Wyrażenie odwróconyPlus(Wyrażenie wyrażenie) {
        return new Suma(wyrażenie, this);
    }

    /* TO NIE DZIAŁA
    public Wyrażenie plus(Zero wyrażenie) {
        return this;
    }
    */

    protected Wyrażenie odwróconeRazy(Wyrażenie wyrażenie) {
        return new Iloczyn(wyrażenie, this);
    }

    public Wyrażenie razy(Wyrażenie wyrażenie) {
        return wyrażenie.odwróconeRazy(this);
    }

}
