package symulator;

import kontenery.KolejkaWydarzen;
import kontenery.KopiecWydarzen;
import kontenery.VectorKrawedzi;
import struktura.Krawedz;
import struktura.Trasa;
import struktura.Wezel;
import wejscie.CzytnikGrupSportowcow;
import wejscie.CzytnikTras;
import wejscie.CzytnikWezlow;
import wejscie.CzytnikWyciagow;
import wydarzenia.Wydarzenie;

import java.util.Scanner;

// Autor: Aliaksei Papkouski, 484 417 :)
// GŁÓWNA KLASA
public class Symulator {
    public static void main(String[] args) {
        Scanner scannerWejscia = new Scanner(System.in);

        // 1. WCZYTYWANIE WĘZŁÓW
        CzytnikWezlow czytnikWezlow = new CzytnikWezlow(scannerWejscia);
        czytnikWezlow.wczytaj();
        Wezel[] wezly = czytnikWezlow.getWezly();

        // ZJEDZENIE PUSTEJ LINII
        scannerWejscia.nextLine();

        // 2. WCZYTYWANIE WYCIĄGÓW
        CzytnikWyciagow czytnikWyciagow = new CzytnikWyciagow(scannerWejscia, wezly);
        czytnikWyciagow.wczytaj();

        // ZJEDZENIE PUSTEJ LINII
        scannerWejscia.nextLine();

        // 3. WCZYTYWANIE TRAS
        CzytnikTras czytnikTras = new CzytnikTras(scannerWejscia, wezly);
        czytnikTras.wczytaj();

        // ZJEDZENIE PUSTEJ LINII
        scannerWejscia.nextLine();

        // INICJALIZACJA SILNIKA SYMULACJI
        KolejkaWydarzen kolejka = new KopiecWydarzen();

        // Wczytanie sportowców (Czytnik sam generuje obiekty Przybycie i wrzuca je na kolejkę)
        CzytnikGrupSportowcow czytnikGrup = new CzytnikGrupSportowcow(scannerWejscia, wezly, kolejka);
        czytnikGrup.wczytaj();

        // 3. GŁÓWNA PĘTLA SYMULACJI
        // Trwa tak długo, aż nastąpi KONIEC_CZASU,
        // przy czym pozwala dokończyć się rozpoczętym wjazdom/zjazdom.
        while (!kolejka.pusta()) {
            Wydarzenie obecne = kolejka.nastepneWydarzenie();
            // Czasem wydarzenie może nie pociągać za sobą innego wydarzenia, dlatego
            // przekazujemy kolejkę wydarzeniu, aby mogło ono wykonać swoją akcję bezpiecznie.
            obecne.wykonaj(kolejka);
        }

        // 4. GENEROWANIE STATYSTYK
        // Zgodnie z treścią, na koniec symulacji dla każdej trasy i wyciągu należy wypisać
        // na standardowe wyjście łączną liczbę przejazdów tą trasą czy wyciągiem.
        // Nie ma tu żadnej mowy o kolejności, więc najwygodniej nam będzie
        // krawędzie wyłuskać z wierzchołków, jako że one je przechowują.
        System.out.println("=== STATYSTYKI PRZEJAZDÓW ===");

        // Dla każdego wierzchołka, przejrzyj jego (skierowane) krawędzie:
        for (int i = 0; i < wezly.length; i++) {
            Wezel w = wezly[i];
            VectorKrawedzi krawedzie = w.getKrawedzie();

            for (int j = 0; j < krawedzie.size(); j++) {
                Krawedz k = krawedzie.at(j);

                String typ = (k instanceof Trasa) ? "Trasa" : "Wyciag";
                System.out.println(typ + "[" + k.getId() + "]\t" +
                        k.getLiczbaPrzejazdow() + " przejazdów.");
            }
        }
    }
}