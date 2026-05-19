package losowanie;

import java.util.Random;

public class Generator {
    // Ukrywamy instancję - nikt z zewnątrz nie musi jej modyfikować
    private final Random random;

    // Konstruktor pozwala na wstrzyknięcie konkretnego ziarna (np. do testów)
    public Generator(long seed) {
        this.random = new Random(seed);
    }

    // Domyślny konstruktor dla normalnego działania programu
    public Generator() {
        this.random = new Random();
    }

    // Zwraca liczbę z przedziału [min, max)
    public int generujLiczbeZPrzedzialu(int min, int max) {
        return random.nextInt(min, max);
    }

    // Losuje liczbę rzeczywistą z przedziału [0,1]
    public double losujZPrzedzialu01() {
        return random.nextDouble();
    }
}