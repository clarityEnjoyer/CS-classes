package wyrazeniaLogiczne;

class Prawda implements Wyrazenie {
    @Override
    public boolean ewaluuj(boolean[] wartosciowanie) {
        return true;
    }
}