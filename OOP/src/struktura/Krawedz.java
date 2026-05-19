package struktura;

public abstract class Krawedz {
    // Identyfikator nadawany na wejściu zgodnie z kolejnością pojawienia.
    protected final int id;

    // Krawędzie są skierowane.
    protected final Wezel zrodlo; // Początek krawędzi.
    protected final Wezel cel; // Koniec krawędzi.

    // Ile razy ta krawędź została użyta przez sportowców.
    protected int   liczbaPrzejazdow = 0;

    public Krawedz(int id, Wezel zrodlo, Wezel cel) {
        this.id = id;
        this.zrodlo = zrodlo;
        this.cel = cel;
    }

    // Zwiększa liczbę użyć krawędzi.
    public void zarejestrujPrzejazd() { liczbaPrzejazdow++; }
    // Daje liczbę użyć.
    public int   getLiczbaPrzejazdow(){ return liczbaPrzejazdow; }

    public Wezel getCel() {             return cel; }
    public int getId() { return id; }
}