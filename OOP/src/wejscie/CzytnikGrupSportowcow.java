package wejscie;

import kontenery.KolejkaWydarzen;
import kontenery.KopiecWydarzen;
import losowanie.Generator;
import narzedzia.Zegar;
import sportowcy.Sportowiec;
import struktura.Wezel;
import wydarzenia.Przybycie;

import java.util.Locale;
import java.util.Scanner;

public class CzytnikGrupSportowcow extends Czytnik {
    private final Wezel[] wezly; // Posłuży wyznaczenia statystyk.
    private final KolejkaWydarzen kolejkaWydarzen; // Posłuży do przeprowadzenia symulacji.
    private final Generator generator; // Generuje liczby pseudolosowe.

    public CzytnikGrupSportowcow(Scanner scanner, Wezel[] wezly, KolejkaWydarzen kolejkaWydarzen) {
        super(scanner);
        this.wezly = wezly;
        this.kolejkaWydarzen = kolejkaWydarzen;
        this.generator = new Generator();
    }

    @Override
    public void wczytaj() {
        int globalnyIdSportowca = 0;

        // Wczytujemy liczbę grup.
        String linia = scannerWejscia.nextLine();
        Scanner scannerLinii = new Scanner(linia);
        int liczbaGrup = scannerLinii.nextInt();

        // PĘTLA PO GRUPACH
        for (int k = 0; k < liczbaGrup; k++) {
            // --- LINIA 1 ---
            Scanner skaner1 = new Scanner(scannerWejscia.nextLine());
            //Wymagane aby poprawnie wczytać liczby zmiennoprzecinkowe o części ułamkowej oddzielanej kropką.
            skaner1.useLocale(Locale.ENGLISH);

            int liczebnosc        = skaner1.nextInt();
            int zaawansowanie     = skaner1.nextInt();
            double spontanicznosc = skaner1.nextDouble();
            boolean monitorowany  = skaner1.hasNext(); // Czy ten sportowiec jest monitorowany? (oznaczane przez 's')

            // --- LINIA 2 ---
            Scanner skaner2 = new Scanner(scannerWejscia.nextLine());
            skaner2.useLocale(Locale.ENGLISH);

            double wagaDopasowania = skaner2.nextDouble();
            double wagaWyrownania  = skaner2.nextDouble();

            // --- LINIA 3 ---
            Scanner skaner3 = new Scanner(scannerWejscia.nextLine());

            int idStartowe = skaner3.nextInt();
            int czasStartu = Zegar.parsujCzas(skaner3.next());
            int odstep = 0;

            // Jeżeli grupa ma kilku sportowców to wczytujemy odstęp między ich startami.
            if (liczebnosc > 1 && skaner3.hasNextInt()) {
                odstep = skaner3.nextInt();
            }

            Wezel wezelStartowy = wezly[idStartowe];

            // ROZPAKOWANIE GRUPY NA POJEDYNCZYCH SPORTOWCÓW
            for (int i = 0; i < liczebnosc; i++) {
                int czasPrzybyciaTegoSportowca = czasStartu + (i * odstep);

                // Tworzymy nowych sportowców o zadanej charakterystyce.
                Sportowiec nowySportowiec = new Sportowiec(
                        globalnyIdSportowca++, monitorowany, wezelStartowy,
                        generator, zaawansowanie, wagaDopasowania, wagaWyrownania, spontanicznosc
                );

                // Dodajemy sportowców do symulacji (pierwsze wydarzenie = przybycie).
                kolejkaWydarzen.wstawWydarzenie(new Przybycie(czasPrzybyciaTegoSportowca, nowySportowiec));
            }
        }
    }
}