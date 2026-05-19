package struktura;

import kontenery.VectorKrawedzi;

// Realizuje wierzchołek w naszym grafie.
public class Wezel {
    private final int wysokosc; // nad poziomem morza (w metrach)

    // Położenie:
    private final int wspolrzednaX;
    private final int wspolrzednaY;

    private final boolean czySkomunikowany;

    // Wierzchołek trzyma listę incydentnych z nim krawędzi.
    private final VectorKrawedzi krawedzie = new VectorKrawedzi();

    public Wezel(int wysokosc, int wspolrzednaX, int wspolrzednaY, boolean czySkomunikowany) {
        this.wysokosc = wysokosc;
        this.wspolrzednaX = wspolrzednaX;
        this.wspolrzednaY = wspolrzednaY;
        this.czySkomunikowany = czySkomunikowany;
    }

    // Dodaje krawędź do listy krawędzi.
    public void dodajKrawedz(Krawedz k) {
        krawedzie.push_back(k);
    }

    // Zwraca listę incydentnych krawędzi.
    public VectorKrawedzi getKrawedzie() {
        return krawedzie;
    }
}
