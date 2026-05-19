package wydarzenia;

import kontenery.KolejkaWydarzen;
import sportowcy.Sportowiec;
import narzedzia.Zegar;

public abstract class Wydarzenie {
    // Czas, w którym to wydarzenie ma nastąpić.
    protected final int czas;
    // W przypadku zdarzen następujących w tym samym czasie, musimy zachować własność FIFO.
    // Dlatego kolejka dodatkowo ustawi identyfikatory, aby uzywać ich jako tie-breakera.
    protected int identyfikator;

    protected final Sportowiec sportowiec;

    public Wydarzenie(int czas, Sportowiec sportowiec) {
        this.czas = czas;
        this.sportowiec = sportowiec;
    }

    public int getCzas() { return czas; }
    public int getIdentyfikator() { return identyfikator; }
    public void setIdentyfikator(int identyfikator) { this.identyfikator = identyfikator; }

    // Ujednolicone logowanie dla wszystkich klas pochodnych
    protected void loguj(String wiadomosc) {
        if (sportowiec != null && sportowiec.jestMonitorowany()) {
            System.out.println(Zegar.formatujCzas(czas) + " - Sportowiec " + sportowiec.getId() + ": " + wiadomosc);
        }
    }

    // Zdarzenie dostaje dostęp do kopca, żeby móc zaplanować swoją przyszłość!
    public abstract void wykonaj(KolejkaWydarzen kolejkaWydarzen);
}