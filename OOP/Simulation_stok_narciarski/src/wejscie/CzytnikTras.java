package wejscie;

import struktura.Trasa;
import struktura.Wezel;

import java.util.Locale;
import java.util.Scanner;

public class CzytnikTras extends Czytnik {
    private final Wezel[] wszystkieWezly;

    public CzytnikTras(Scanner scanner, Wezel[] wezly) {
        super(scanner);
        this.wszystkieWezly = wezly;
    }

    @Override
    public void wczytaj() {
        Scanner scannerLinii = new Scanner(scannerWejscia.nextLine());
        //Wymagane aby poprawnie wczytać liczby zmiennoprzecinkowe o części ułamkowej oddzielanej kropką.
        scannerLinii.useLocale(Locale.ENGLISH);
        
        int liczbaTras = scannerLinii.nextInt();

        for (int i = 0; i < liczbaTras; i++) {
            Scanner skanerDanych = new Scanner(scannerWejscia.nextLine());

            int idZrodla = skanerDanych.nextInt();
            int idCelu = skanerDanych.nextInt();
            int trudnosc = skanerDanych.nextInt();
            int czas = skanerDanych.nextInt();
            double bazowaAtrakcyjnosc = skanerDanych.nextDouble();
            double odpornosc = skanerDanych.nextDouble();

            // Są to odpowiednio początek i koniec krawędzi.
            Wezel zrodlo = wszystkieWezly[idZrodla];
            Wezel cel = wszystkieWezly[idCelu];

            Trasa nowaTrasa = new Trasa(i, zrodlo, cel, trudnosc, czas, bazowaAtrakcyjnosc, odpornosc);
            zrodlo.dodajKrawedz(nowaTrasa);
        }
        scannerLinii.close();
    }
}