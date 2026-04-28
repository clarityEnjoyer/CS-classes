package wyrazeniaLogiczne;

class Negacja implements Wyrazenie {
    private final Wyrazenie wyrazenie;

    public Negacja(Wyrazenie wyrazenie) {
        this.wyrazenie = wyrazenie;
    }

    @Override
    public boolean ewaluuj(boolean[] wartosciowanie) {
        return !wyrazenie.ewaluuj(wartosciowanie);
    }
}