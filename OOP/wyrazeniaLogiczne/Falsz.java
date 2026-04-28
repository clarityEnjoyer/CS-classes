package wyrazeniaLogiczne;

class Falsz implements Wyrazenie {
    @Override
    public boolean ewaluuj(boolean[] wartosciowanie) {
        return false;
    }
}
