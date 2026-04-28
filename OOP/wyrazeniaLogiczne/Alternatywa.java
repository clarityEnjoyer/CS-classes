package wyrazeniaLogiczne;


import java.util.List;

// Zmodyfikowana Alternatywa - obsługuje wieloargumentowe wyrażenia (np. pojedynczą klauzulę)
class Alternatywa implements Wyrazenie {
    private final List<Wyrazenie> argumenty;

    public Alternatywa(List<Wyrazenie> argumenty) {
        this.argumenty = argumenty;
    }

    @Override
    public boolean ewaluuj(boolean[] wartosciowanie) {
        for (Wyrazenie w : argumenty) {
            if (w.ewaluuj(wartosciowanie)) {
                return true; // Leniwe wartościowanie - jeśli jeden argument jest prawdą, całe OR to prawda
            }
        }
        return false; // Pusta alternatywa z definicji jest fałszywa
    }
}