package wejscie;

import java.util.Scanner;

public abstract class Czytnik {
    protected final Scanner scannerWejscia; // protected, by podklasy miały dostęp.

    public Czytnik(Scanner scannerWejscia) {
        this.scannerWejscia = scannerWejscia;
    }

    // Wymuszamy na podklasach implementację logiki wczytywania.
    public abstract void wczytaj();
}