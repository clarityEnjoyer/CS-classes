package wejscie;
import narzedzia.Zegar;
import struktura.Wezel;
import struktura.Wyciag;
import java.util.Scanner;

public class CzytnikWyciagow extends Czytnik {
    private final Wezel[] wszystkieWezly;

    public CzytnikWyciagow(Scanner scanner, Wezel[] wezly) {
        super(scanner);
        this.wszystkieWezly = wezly;
    }

    @Override
    public void wczytaj() {
        Scanner scannerLinii = new Scanner(scannerWejscia.nextLine());
        
        int liczbaWyciagow = scannerLinii.nextInt();

        for (int i = 0; i < liczbaWyciagow; i++) {
            Scanner skanerDanych = new Scanner(scannerWejscia.nextLine());

            int idZrodla = skanerDanych.nextInt();
            int idCelu = skanerDanych.nextInt();
            int odstep = skanerDanych.nextInt();
            int maxGrupa = skanerDanych.nextInt();
            int czas = skanerDanych.nextInt();

            Wezel zrodlo = wszystkieWezly[idZrodla];
            Wezel cel = wszystkieWezly[idCelu];

            Wyciag nowyWyciag = new Wyciag(i,zrodlo, cel, odstep, maxGrupa, czas);
            zrodlo.dodajKrawedz(nowyWyciag);
        }
    }
}