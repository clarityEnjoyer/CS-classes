package kontenery;

import wydarzenia.Wydarzenie;

// Klasyczna implementacja wektora.
public class VectorZdarzen {
    private static final int ROZMIAR_POCZĄTKOWY = 2;

    private Wydarzenie[] elementy;

    private int ile;

    public VectorZdarzen() {
        this(ROZMIAR_POCZĄTKOWY);
    }

    public VectorZdarzen(int rozmiarPoczątkowy) {
        elementy = new Wydarzenie[rozmiarPoczątkowy];
        ile = 0;
    }

    private int więcej(int n) {
        return n / 2 * 3 + 2;
    }

    private void rezerwujMiejsce() {
        if (ile < elementy.length) return;
        Wydarzenie[] nowa = new Wydarzenie[więcej(elementy.length)];
        for (int i = 0; i < elementy.length; i++) {
            nowa[i] = elementy[i];
        }
        elementy = nowa;
    }

    public void set(int index, Wydarzenie value) {
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

    public void push_back(Wydarzenie x) {
        rezerwujMiejsce();
        elementy[ile++] = x;
    }

    public boolean empty() {
        return (ile == 0);
    }

    public Wydarzenie at(int index) {
        // Nie możemy się pytac o nieistniejący element.
        assert (!empty() && index < ile);
        return elementy[index];
    }

}
