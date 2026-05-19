package wydarzenia;

import kontenery.KolejkaWydarzen;
import sportowcy.Sportowiec;
import struktura.Trasa;

// Rejestruje rozpoczęcie zjazdu trasą przez sportowca.
public class RozpoczecieZjazdu extends Wydarzenie {

    private final Trasa trasa;

    public RozpoczecieZjazdu(int czas, Sportowiec sportowiec, Trasa trasa) {
        super(czas, sportowiec);
        this.trasa = trasa;
    }

    @Override
    public void wykonaj(KolejkaWydarzen kolejka) {
        loguj("Rozpoczyna zjazd trasą.");

        //Zapisuje dla trasy informację o kolejnym zjeździe odbytym przez sportowca (inkrementuje licznik)
        trasa.zarejestrujPrzejazd();

        // Obliczamy czas trwania zjazdu
        int czasPrzejazdu = trasa.getCzasPrzejazdu();

        // Generujemy wydarzenie w przyszłości
        int czasZakonczenia = this.czas + czasPrzejazdu;
        kolejka.wstawWydarzenie(new ZakonczenieZjazdu(czasZakonczenia, sportowiec, trasa));
    }
}