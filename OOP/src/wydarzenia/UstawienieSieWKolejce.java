package wydarzenia;

import kontenery.KolejkaWydarzen;
import sportowcy.Sportowiec;
import struktura.Wyciag;

// Rejestruje się w kolejce do wyciągu przez sportowca.
public class UstawienieSieWKolejce extends Wydarzenie {
    private final Wyciag wyciag;

    public UstawienieSieWKolejce(int czas, Sportowiec sportowiec, Wyciag wyciag) {
        super(czas, sportowiec);
        this.wyciag = wyciag;
    }

    @Override
    public void wykonaj(KolejkaWydarzen kolejka) {
        loguj("Ustawił się w kolejce do wyciągu.");
        wyciag.dodajDoKolejki(sportowiec);

        // Jeśli wyciąg sobie "spał" (nie marnował zasobów), budzimy go!
        if (!wyciag.czyZaplanowanyOdjazd()) {
            wyciag.setZaplanowanyOdjazd(true);

            // Obliczamy, kiedy w rzeczywistości nadjedzie krzesełko.
            int czasNastepnegoKrzesla = wyciag.obliczNastepneKrzeselko(this.czas);

            // Planujemy odjazd wyciągu.
            kolejka.wstawWydarzenie(new OdjazdWyciagu(czasNastepnegoKrzesla, wyciag));
        }
    }
}