package struktura;

import kontenery.BuforCykliczny;
import kontenery.KolejkaSportowcow;
import sportowcy.Sportowiec;

import static narzedzia.Zegar.CZAS_OTWARCIA;

// Realizuje pracę wyciągu.
public class Wyciag extends Krawedz {
    // Kolejka oczekujących na wjazd sportowców.
    private final KolejkaSportowcow oczekujacy = new BuforCykliczny();


    private final int odstepCzasowy;
    private final int maxRozmiarGrupy;
    private final int czasPrzejazdu;

    // Reguluje "uśpienie" wyciągu dla symulacji, dopóki kolejka jest pusta.
    private boolean zaplanowanyOdjazd = false;

    public Wyciag(int id, Wezel zrodlo, Wezel cel, int odstep, int maxRozmiar, int czas) {
        super(id, zrodlo, cel);
        this.odstepCzasowy = odstep;
        this.maxRozmiarGrupy = maxRozmiar;
        this.czasPrzejazdu = czas;
    }

    public boolean czyZaplanowanyOdjazd() {
        return zaplanowanyOdjazd;
    }

    public void setZaplanowanyOdjazd(boolean stan) {
        this.zaplanowanyOdjazd = stan;
    }

    public boolean czyKolejkaPusta() {
        return oczekujacy.pusta();
    }

    public Sportowiec pobierzZKolejki() {
        return oczekujacy.nastepnySportowiec();
    }

    public int getMaxRozmiarGrupy() {
        return maxRozmiarGrupy;
    }

    public int getOdstepCzasowy() {
        return odstepCzasowy;
    }

    public int getCzasPrzejazdu() {
        return czasPrzejazdu;
    }

    public void dodajDoKolejki(Sportowiec sportowiec) {
        oczekujacy.ustawDoKolejki(sportowiec);
    }

    // Wyrównywanie do "rozkladu jazdy"
    public int obliczNastepneKrzeselko(int aktualnyCzas) {
        if (aktualnyCzas <= CZAS_OTWARCIA) return CZAS_OTWARCIA;

        int uplynelo = aktualnyCzas - CZAS_OTWARCIA;
        int reszta = uplynelo % odstepCzasowy;

        if (reszta == 0) {
            return aktualnyCzas; // Krzesełko podjechało idealnie w tej sekundzie
        } else {
            return aktualnyCzas + (odstepCzasowy - reszta); // Czas oczekiwania na następne
        }
    }
}