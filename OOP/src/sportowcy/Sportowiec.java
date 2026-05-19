package sportowcy;

import kontenery.VectorKrawedzi;
import losowanie.Generator;
import struktura.Krawedz;
import struktura.Trasa;
import struktura.Wezel;
import struktura.Wyciag;

import static java.lang.Math.max;
import static narzedzia.Zegar.KONIEC_CZASU;

// Implementuje sportowca: jego podejmowanie decyzji oraz niezbędne atrybuty.
public class Sportowiec {
    // Stan ogólny
    private final int id;
    private final boolean monitorowany;
    private Wezel aktualnePolozenie;

    // Generator liczb pseudolosowych.
    private final Generator generator;

    // Preferencje i kryteria sportowca.
    private final int poziomZaawansowania;
    private final double wagaDopasowania;
    private final double wagaWyrownania;
    private final double wspolczynnikSpontanicznosci; // epsilon

    public Sportowiec(int id, boolean monitorowany, Wezel polozenieStartowe, Generator generator,
                      int poziomZaawansowania, double wagaDopasowania, double wagaWyrownania, double spontanicznosc) {
        this.id = id;
        this.monitorowany = monitorowany;
        this.aktualnePolozenie = polozenieStartowe;
        this.generator = generator;
        this.poziomZaawansowania = poziomZaawansowania;
        this.wagaDopasowania = wagaDopasowania;
        this.wagaWyrownania = wagaWyrownania;
        this.wspolczynnikSpontanicznosci = spontanicznosc;
    }

    public boolean jestMonitorowany() {
        return monitorowany;
    }

    public int getId() {
        return id;
    }

    // Ustawia nowe położenie (sportowiec się przemieszcza).
    public void setAktualnePolozenie(Wezel aktualnePolozenie) {
        this.aktualnePolozenie = aktualnePolozenie;
    }

    // Sportowiec ocenia trasę wg kombinacji liniowych współczynników i dopasowania
    // wyliczonej na podstawie własych kryteriów.
    private double ocenTrase(Trasa trasa) {
        // 1. Dopasowanie poziomu
        double dopasowanie;
        double pt = trasa.getPoziomTrudnosci();
        double pn = poziomZaawansowania;
        if (pt >= pn + 5) dopasowanie = 0;
        else if (pt >= pn) dopasowanie = 1 - (pt - pn) / 5;
        else dopasowanie = max(0.2, 1 - (pn - pt) / 7);

        // 2. Wyrównanie trasy
        double wyrownanie;
        double ot = trasa.getOdpornosc();
        double bt = trasa.getBazowaAtrakcyjnosc();
        double kt = trasa.getLiczbaPrzejazdow();
        wyrownanie = bt + (1 - bt) * Math.pow(ot, kt);

        return this.wagaDopasowania * dopasowanie + this.wagaWyrownania * wyrownanie;
    }

    // Sportowiec rozważa wszystkie trasy dostępne z aktualnego węzła
    // oraz za pośrednictwem jednego wjechania wyciągiem.
    // Wybiera tę o najwyższej (subiektywnej) ocenie.
    public Krawedz wybierzKolejnyRuch(int aktualnyCzas) {
        // Dalej już nie symulujemy.
        if (aktualnyCzas >= KONIEC_CZASU) return null;

        VectorKrawedzi dostepneKrawedzie = aktualnePolozenie.getKrawedzie();
        int liczbaKrawedzi = dostepneKrawedzie.size();

        // Losujemy, czy zostanie podjęta spontaniczna decyzja.
        if (generator.losujZPrzedzialu01() < wspolczynnikSpontanicznosci) {
            // Dokonujemy spontanicznej decyzji. (Ignorujemy typ krawędzi)
            int losowyIndeks = generator.generujLiczbeZPrzedzialu(0, liczbaKrawedzi);
            return dostepneKrawedzie.at(losowyIndeks);
        }

        // ŚCIEŻKA RACJONALNA (Odróżniamy Trasy od Wyciągów)
        double najlepszaOcena = -Double.MAX_VALUE;
        int liczbaTras = 0;
        Krawedz najlepszaKrawedz = null;
        final double EPSILON = 0.0000001;

        // Przeglądamy krawędzie w poszukiwaniu tras i ich ocen.
        for (int i = 0; i < liczbaKrawedzi; i++) {
            Krawedz k = dostepneKrawedzie.at(i);

            // Liczymy ocenę trasy.
            if (k instanceof Trasa t) {
                double ocena = ocenTrase(t);

                if (ocena > najlepszaOcena + EPSILON) {
                    najlepszaOcena = ocena;
                    // Zapamiętujemy najlepszą trasę.
                    najlepszaKrawedz = t;
                }
            }
            // Trafiliśmy na wyciąg, więc rozważymy
            // wszystkie (i wyłącznie) trasy wychodzące z jego drugiego końca.
            else{
                Wezel wezelPoDrugiejStronie = k.getCel();
                VectorKrawedzi krawedziePoDrugiejStronie = wezelPoDrugiejStronie.getKrawedzie();
                int liczbaKrawedziPoDrugiejStronie = krawedziePoDrugiejStronie.size();
                for (int j = 0; j < liczbaKrawedziPoDrugiejStronie; j++) {
                    Krawedz krawedz = krawedziePoDrugiejStronie.at(j);

                    // Liczymy ocenę trasy.
                    if (krawedz instanceof Trasa t) {
                        double ocena = ocenTrase(t);

                        if (ocena > najlepszaOcena + EPSILON) {
                            najlepszaOcena = ocena;
                            // Zapamiętujemy WYCIĄG, który prowadzi do najlepszej trasy.
                            najlepszaKrawedz = k;
                        }
                    }
                }
            }
        }

        // Brak jakichkolwiek tras w zasięgu jednego ruchu: Sportowiec jest zmuszony wybrać jakiś wyciąg.
        if (najlepszaKrawedz == null) {
            // Weźmiemy po prostu pierwszy wyciąg.
            return dostepneKrawedzie.at(0); // (zostawiłem 0, bo w tej sytuacji jest to czytelne)
        }

        // Nawet jeśli był remis, to mogę go rozstrzygnąć dowolnie.
        return najlepszaKrawedz;
    }
}