package wyrazeniaLogiczne;

class Zmienna implements Wyrazenie {
    private final int indeks;

    public Zmienna(int indeks) {
        if (indeks < 0) {
            throw new IllegalArgumentException("Indeks zmiennej nie może być ujemny.");
        }
        this.indeks = indeks;
    }

    @Override
    public boolean ewaluuj(boolean[] wartosciowanie) {
        if (indeks >= wartosciowanie.length) {
            throw new IndexOutOfBoundsException("Brak wartościowania dla zmiennej o indeksie " + indeks);
        }
        return wartosciowanie[indeks];
    }
}