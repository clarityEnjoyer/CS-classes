package kontenery;

import struktura.Krawedz;

// Klasyczna implementacja wektora.
public class VectorKrawedzi {
    private static final int ROZMIAR_POCZĄTKOWY = 2;

    private Krawedz[] elementy;

    private int ile;

    public VectorKrawedzi() {
        this(ROZMIAR_POCZĄTKOWY);
    }

    public VectorKrawedzi(int rozmiarPoczątkowy) {
        elementy = new Krawedz[rozmiarPoczątkowy];
        ile = 0;
    }

    private int więcej(int n) {
        return n / 2 * 3 + 2;
    }

    private void rezerwujMiejsce() {
        if (ile < elementy.length) return;
        Krawedz[] nowa = new Krawedz[więcej(elementy.length)];
        for (int i = 0; i < elementy.length; i++) {
            nowa[i] = elementy[i];
        }
        elementy = nowa;
    }

    public void set(int index, Krawedz value) {
        assert (!empty() && index < ile);
        elementy[index] = value;
    }

    public int size() {
        return ile;
    }

    public void pop_back() {
        assert(!empty());
        ile--;
    }

    public void push_back(Krawedz x) {
        rezerwujMiejsce();
        elementy[ile++] = x;
    }

    public boolean empty() {
        return (ile == 0);
    }

    public Krawedz at(int index) {
        // Nie możemy się pytac o nieistniejący element.
        assert (!empty() && index < ile);
        return elementy[index];
    }

}
