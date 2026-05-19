package wydarzenia;

import kontenery.KolejkaWydarzen;
import sportowcy.Sportowiec;
import struktura.Wyciag;

import static narzedzia.Zegar.KONIEC_CZASU;

// Realizuje odjazd wyciagu.
// Zbiera grupkę pasażerów z początku kolejki i wysyła ich w podróż.
public class OdjazdWyciagu extends Wydarzenie {
    private final Wyciag wyciag;

    public OdjazdWyciagu(int czas, Wyciag wyciag) {
        super(czas, null); // Zdarzenie infrastruktury, brak konkretnego sportowca
        this.wyciag = wyciag;
    }

    @Override
    public void wykonaj(KolejkaWydarzen kolejka) {
        // Po tym czasie już nie symulujemy kolejnych wjazdów.
        if (this.czas >= KONIEC_CZASU) return;

        // AUTO-USYPIANIE: Skoro nikogo nie ma, wyciąg idzie "spać" by oszczędzić CPU.
        // Fizycznie kręci się dalej, ale w programie nie istnieje, dopóki ktoś nie przyjdzie.
        if (wyciag.czyKolejkaPusta()) {
            wyciag.setZaplanowanyOdjazd(false);
            return;
        }

        int zabraniPaszazerowie = 0;
        int maxGrupa = wyciag.getMaxRozmiarGrupy();
        int czasDojazduNaGore = this.czas + wyciag.getCzasPrzejazdu();

        // Pakujemy narciarzy na krzesełko
        while (zabraniPaszazerowie < maxGrupa && !wyciag.czyKolejkaPusta()) {
            Sportowiec s = wyciag.pobierzZKolejki();

            // Planujemy odpowiednio rozpoczęcie i koniec wjazdów.
            kolejka.wstawWydarzenie(new RozpoczecieWjazdu(this.czas, s, wyciag));
            kolejka.wstawWydarzenie(new ZakonczenieWjazdu(czasDojazduNaGore, s, wyciag));

            zabraniPaszazerowie++;
        }

        // Kontynuujemy cykl: planujemy przyjazd kolejnego krzesełka
        kolejka.wstawWydarzenie(new OdjazdWyciagu(this.czas + wyciag.getOdstepCzasowy(), wyciag));
    }
}