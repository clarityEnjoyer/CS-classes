package wejscie;

import struktura.Wezel;

import java.util.Scanner;

public class CzytnikWezlow extends Czytnik {

    private Wezel[] wczytaneWezly; // Tu będziemy przechowywać nowo-utworzone węzły.

    public CzytnikWezlow(Scanner scannerWejscia) {
        super(scannerWejscia);
    }

    @Override
    public void wczytaj() {
        String linia = scannerWejscia.nextLine();
        Scanner scannerLinii = new Scanner(linia);

        int liczbaWezlow = scannerLinii.nextInt();
        wczytaneWezly = new Wezel[liczbaWezlow];

        // Wczytywanie węzłów.
        for (int i = 0; i < liczbaWezlow; i++) {
            linia = scannerWejscia.nextLine();
            Scanner skanerDanych = new Scanner(linia);

            int wysokosc = skanerDanych.nextInt();
            int x = skanerDanych.nextInt();
            int y = skanerDanych.nextInt();
            // Skoro dane są poprawne, jeśli coś tu jest, to musi być to flaga 's'.
            boolean skomunikowany = skanerDanych.hasNext();

            // Tworzymy węzeł o zadanej charakterystyce.
            wczytaneWezly[i] = new Wezel(wysokosc, x, y, skomunikowany);
        }
    }

    public Wezel[] getWezly() {
        return wczytaneWezly;
    }
}