package kontenery;

import wydarzenia.Wydarzenie;

// Realizuje kolejkę priorytetową dla symulacji wydarzeń w czasie.
public interface KolejkaWydarzen {
    // Wstawia nowe wydarzenie do kolejki.
    void wstawWydarzenie(Wydarzenie noweWydarzenie);

    // Zwraca najbliższe wydarzenie.
    Wydarzenie nastepneWydarzenie();

    // Sprawdza, czy kolejka jest pusta.
    boolean pusta();
}
