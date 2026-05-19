package kontenery;

import sportowcy.Sportowiec;

public interface KolejkaSportowcow {
    // Wstawia sportowca na koniec kolejki.
    void ustawDoKolejki(Sportowiec sportowiec);
    // Sprawdza, czy kolejka jest pusta.
    boolean pusta();
    // Zwraca sportowca z początku kolejki.
    Sportowiec nastepnySportowiec();
}
