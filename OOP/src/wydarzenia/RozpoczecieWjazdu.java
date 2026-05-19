package wydarzenia;

import kontenery.KolejkaWydarzen;
import sportowcy.Sportowiec;
import struktura.Wyciag;

// Rejestruje rozpoczęcie wjazdu wyciągiem przez sportowca.
public class RozpoczecieWjazdu extends Wydarzenie {
    private final Wyciag wyciag;

    public RozpoczecieWjazdu(int czas, Sportowiec sportowiec, Wyciag wyciag) {
        super(czas, sportowiec);
        this.wyciag = wyciag;
    }

    @Override
    public void wykonaj(KolejkaWydarzen kopiec) {
        loguj("Rozpoczął wjazd wyciągiem.");
        // Zapisuje w wyciagu informację o kolejnym przejeździe odbytego przez sportowca (inkrementuje licznik)
        wyciag.zarejestrujPrzejazd();
    }
}