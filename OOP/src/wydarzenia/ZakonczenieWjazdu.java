package wydarzenia;

import kontenery.KolejkaWydarzen;
import sportowcy.Sportowiec;
import struktura.Krawedz;
import struktura.Trasa;
import struktura.Wyciag;

// Realizuje wjechanie wyciągiem do węzła.
public class ZakonczenieWjazdu extends Wydarzenie {
    private final Wyciag wyciag; // Tym wyciągiem właśnie wjeżdżaliśmy.

    public ZakonczenieWjazdu(int czas, Sportowiec sportowiec, Wyciag wyciag) {
        super(czas, sportowiec);
        this.wyciag = wyciag;
    }

    @Override
    public void wykonaj(KolejkaWydarzen kolejka) {
        loguj("Zakończył wjazd wyciągiem.");

        // Aktualizacja pozycji - sportowiec dotarł na szczyt krawędzi.
        sportowiec.setAktualnePolozenie(wyciag.getCel());

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