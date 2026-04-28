package wyrazeniaLogiczne;

import java.util.List;

// Zmodyfikowana Koniunkcja - obsługuje wieloargumentowe wyrażenia (np. całą formułę CNF)
class Koniunkcja implements Wyrazenie {
    private final List<Wyrazenie> argumenty;

    public Koniunkcja(List<Wyrazenie> argumenty) {
        this.argumenty = argumenty;
    }

    @Override
    public boolean ewaluuj(boolean[] wartosciowanie) {
        for (Wyrazenie w : argumenty) {
            if (!w.ewaluuj(wartosciowanie)) {
                return false; // Leniwe wartościowanie - jeśli jedna klauzula jest fałszem, całe AND to fałsz
            }
        }
        return true; // Pusta koniunkcja z definicji jest prawdziwa
    }
}