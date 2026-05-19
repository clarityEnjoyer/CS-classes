package kontenery;

import sportowcy.Sportowiec;

// Implementuje jednostronną kolejkę FIFO za pomocą bufora cyklicznego
public class BuforCykliczny implements KolejkaSportowcow {

    private static final int ROZMIAR_POCZĄTKOWY = 2;

    private Sportowiec[] elementy;

    private int zajęty;

    private int wolny;

    private int ile;

    public BuforCykliczny() {
        this(ROZMIAR_POCZĄTKOWY);
    }

    public BuforCykliczny(int rozmiarPoczątkowy) {
        elementy = new Sportowiec[rozmiarPoczątkowy];
        zajęty = 0;
        wolny = 0;
        ile = 0;
    }

    private int indeks(int i) {
        return i % elementy.length;
    }

    private int więcej(int n) {
        return n / 2 * 3 + 2;
    }

    private void rezerwujMiejsce() {
        // Już się nie mieścimy. Zwiększymy rozmiar.
        if (ile == elementy.length) {
            Sportowiec[] nowa = new Sportowiec[więcej(elementy.length)];

            // Bezpieczne przepisywanie dokładnie 'ile' elementów
            for (int j = 0; j < ile; j++) {
                nowa[j] = elementy[zajęty];
                zajęty = indeks(zajęty + 1);
            }

            // Resetujemy wskaźniki dla nowej, ciągłej tablicy
            zajęty = 0;
            wolny = ile;
            elementy = nowa;
        }
    }

    // Wstawiamy na początek kolejki.
    @Override
    public void ustawDoKolejki(Sportowiec sportowiec) {
        rezerwujMiejsce();
        elementy[wolny] = sportowiec;
        wolny = indeks(wolny + 1);
        ++ile;
    }

    @Override
    public boolean pusta() {
        return (ile == 0);
    }

    @Override
    public Sportowiec nastepnySportowiec() {
        assert (!pusta());

        // 1. Pobranie obiektu
        Sportowiec wynik = elementy[zajęty];

        // 2. OCHRONA PAMIĘCI - usuwamy referencję z tablicy
        elementy[zajęty] = null;

        // 3. Przesunięcie wskaźników
        zajęty = indeks(zajęty + 1);
        --ile;

        return wynik;
    }
}