package struktura;

public class Trasa extends Krawedz {
    // Parametry krawędzi.
    private final int poziomTrudnosci;
    private final int czasPrzejazdu;
    private final double bazowaAtrakcyjnosc;
    private final double odpornosc;

    public Trasa(int id, Wezel zrodlo, Wezel cel, int trudnosc, int czas, double bazowaAtrakcyjnosc, double odpornosc) {
        super(id, zrodlo, cel);
        this.poziomTrudnosci = trudnosc;
        this.czasPrzejazdu = czas;
        this.bazowaAtrakcyjnosc = bazowaAtrakcyjnosc;
        this.odpornosc = odpornosc;
    }

    public int getCzasPrzejazdu() {
        return czasPrzejazdu;
    }

    public int getPoziomTrudnosci() {
        return poziomTrudnosci;
    }

    public double getBazowaAtrakcyjnosc() {
        return bazowaAtrakcyjnosc;
    }

    public double getOdpornosc() {
        return odpornosc;
    }
}