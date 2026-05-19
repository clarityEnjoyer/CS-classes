package wydarzenia;

import kontenery.KolejkaWydarzen;
import sportowcy.Sportowiec;
import struktura.Krawedz;
import struktura.Trasa;
import struktura.Wyciag;

// Realizuje zjechanie trasą do węzła.
public class ZakonczenieZjazdu extends Wydarzenie {
    private final Trasa trasa; // Tą trasą właśnie zjeżdżaliśmy.

    public ZakonczenieZjazdu(int czas, Sportowiec sportowiec, Trasa trasa) {
        super(czas, sportowiec);
        this.trasa = trasa;
    }

    @Override
    public void wykonaj(KolejkaWydarzen kolejka) {
        loguj("Zakończył zjazd trasą o trudności " + trasa.getPoziomTrudnosci());

        // Aktualizacja pozycji - sportowiec dotarł na dół trasy.
        sportowiec.setAktualnePolozenie(trasa.getCel());

        // Nowa decyzja
        Krawedz nastepnyRuch = sportowiec.wybierzKolejnyRuch(this.czas);
        // Sportowiec albo zdecydował się zjechać trasą, albo wjechać wyciągiem,
        // albo nastąpił już koniec czasu i nastepnyRuch = null!
        if (nastepnyRuch instanceof Trasa)
            kolejka.wstawWydarzenie(new RozpoczecieZjazdu(this.czas, sportowiec, (Trasa) nastepnyRuch));
        else if (nastepnyRuch instanceof Wyciag)
            kolejka.wstawWydarzenie(new UstawienieSieWKolejce(this.czas, sportowiec, (Wyciag) nastepnyRuch));
    }
}