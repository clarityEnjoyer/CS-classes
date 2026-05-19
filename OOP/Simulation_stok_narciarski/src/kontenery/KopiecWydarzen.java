package kontenery;

import wydarzenia.Wydarzenie;

// Realizuje kolejkę priorytetową za pom. kopca.
public class KopiecWydarzen implements KolejkaWydarzen {
    private final VectorZdarzen elementy;
    private int licznikWstawien = 0; // Lokalny licznik kopca

    public KopiecWydarzen() {
        this.elementy = new VectorZdarzen();
    }

    public KopiecWydarzen(int rozmiarPoczątkowy) {
        this.elementy = new VectorZdarzen(rozmiarPoczątkowy);
    }

    @Override
    public boolean pusta() {
        return elementy.empty();
    }

    @Override
    public void wstawWydarzenie(Wydarzenie noweWydarzenie) {
        noweWydarzenie.setIdentyfikator(licznikWstawien++);
        // 1. Dodajemy na sam koniec
        elementy.push_back(noweWydarzenie);

        // 2. Przywracamy warunek kopca (przesiewanie w górę)
        int aktualnyIndeks = elementy.size() - 1;
        przesiejWGore(aktualnyIndeks);
    }

    @Override
    public Wydarzenie nastepneWydarzenie() {
        assert(!pusta()); // ew. na throws podmienic

        // 1. Najmniejszy element zawsze jest na szczycie (indeks 0)
        Wydarzenie minimum = elementy.at(0);

        // 2. Bierzemy ostatni element i wrzucamy go na szczyt
        Wydarzenie ostatni = elementy.at(elementy.size() - 1);
        elementy.set(0, ostatni);
        elementy.pop_back(); // Usuwamy fizycznie ostatni

        // 3. Przywracamy warunek kopca (przesiewanie w dół)
        if (!pusta()) {
            przesiejWDol(0);
        }

        return minimum;
    }

    // Funkcją porównująca dwa wydarzenia.
    // Zachowuje własność FIFO dla równych czasów.
    private boolean czyPierwszePilniejsze(Wydarzenie w1, Wydarzenie w2) {
        if (w1.getCzas() < w2.getCzas()) {
            return true;
        } else if (w1.getCzas() == w2.getCzas()) {
            // Remis czasu! Wygrywa to, które wstawiono wcześniej (mniejszy numer)
            return w1.getIdentyfikator() < w2.getIdentyfikator();
        }
        return false; // z1 jest późniejsze
    }

    private void przesiejWGore(int indeks) {
        while (indeks > 0) {
            int rodzic = (indeks - 1) / 2;

            // Jeśli aktualny element jest mniejszy od swojego rodzica (wymóg Min-Heap)
            if (czyPierwszePilniejsze(elementy.at(indeks), elementy.at(rodzic))) {
                // Zamieniamy je miejscami w wektorze
                Wydarzenie temp = elementy.at(indeks);
                elementy.set(indeks, elementy.at(rodzic));
                elementy.set(rodzic, temp);

                // Przesuwamy wskaźnik do góry
                indeks = rodzic;
            } else {
                // Warunek kopca jest spełniony, przerywamy
                break;
            }
        }
    }

    private void przesiejWDol(int indeks) {
        int rozmiar = elementy.size();

        while (true) {
            int leweDziecko = 2 * indeks + 1;
            int praweDziecko = 2 * indeks + 2;
            int najmniejszy = indeks;

            // Czy lewe dziecko istnieje i czy jest mniejsze od obecnego najmniejszego?
            if (leweDziecko < rozmiar && czyPierwszePilniejsze(elementy.at(leweDziecko), elementy.at(najmniejszy))) {
                najmniejszy = leweDziecko;
            }

            // Czy prawe dziecko istnieje i czy jest jeszcze mniejsze?
            if (praweDziecko < rozmiar && czyPierwszePilniejsze(elementy.at(praweDziecko), elementy.at(najmniejszy))) {
                najmniejszy = praweDziecko;
            }

            // Jeśli któryś z potomków był mniejszy, robimy zamianę
            if (najmniejszy != indeks) {
                Wydarzenie temp = elementy.at(indeks);
                elementy.set(indeks, elementy.at(najmniejszy));
                elementy.set(najmniejszy, temp);

                // Przesuwamy wskaźnik w dół i powtarzamy pętlę
                indeks = najmniejszy;
            } else {
                // Dzieci są większe (lub ich nie ma) - warunek kopca spełniony
                break;
            }
        }
    }
}