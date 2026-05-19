package wydarzenia;

import kontenery.KolejkaWydarzen;
import kontenery.KopiecWydarzen;
import sportowcy.Sportowiec;
import struktura.Krawedz;
import struktura.Trasa;
import struktura.Wyciag;

// Rejestruje przybycie sportowca na stok (pierwsze wydarzenie dla niego).
public class Przybycie extends Wydarzenie {

    public Przybycie(int czas, Sportowiec sportowiec) {
        super(czas, sportowiec);
    }

    @Override
    public void wykonaj(KolejkaWydarzen kolejka) {
        loguj("Przybył do ośrodka narciarskiego.");

        // Narciarz od razu podejmuje pierwszą decyzję w węźle startowym
        Krawedz nastepnyRuch = sportowiec.wybierzKolejnyRuch(this.czas);

        // Sportowiec albo zdecydował się zjechać trasą, albo wjechać wyciągiem.
        if (nastepnyRuch instanceof Trasa)
            kolejka.wstawWydarzenie(new RozpoczecieZjazdu(this.czas, sportowiec, (Trasa) nastepnyRuch));
         else if (nastepnyRuch instanceof Wyciag)
            kolejka.wstawWydarzenie(new UstawienieSieWKolejce(this.czas, sportowiec, (Wyciag) nastepnyRuch));
    }
}