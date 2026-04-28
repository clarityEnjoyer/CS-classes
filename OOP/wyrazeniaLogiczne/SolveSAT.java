package wyrazeniaLogiczne;

import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class SolveSAT {

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        List<Wyrazenie> klauzule = new ArrayList<>();
        int maxZmienna = 0;

        // Budowa drzewa AST z wejścia
        while (scanner.hasNextLine()) {
            String linia = scanner.nextLine().trim();
            if (linia.isEmpty()) {
                continue;
            }

            Scanner liniaScanner = new Scanner(linia);
            List<Wyrazenie> literaly = new ArrayList<>();

            while (liniaScanner.hasNextInt()) {
                int wartosc = liniaScanner.nextInt();
                int numerZmiennej = Math.abs(wartosc);

                // Uaktualniamy najwyższy zaobserwowany indeks zmiennej
                if (numerZmiennej > maxZmienna) {
                    maxZmienna = numerZmiennej;
                }

                Wyrazenie literal = new Zmienna(numerZmiennej);
                if (wartosc < 0) {
                    literal = new Negacja(literal);
                }
                literaly.add(literal);
            }

            if (!literaly.isEmpty()) {
                klauzule.add(new Alternatywa(literaly));
            }
        }

        // Formuła w formacie CNF to koniunkcja (AND) alternatyw (OR)
        Wyrazenie formulaCNF = new Koniunkcja(klauzule);

        // Tablica zmiennych. Indeks 0 jest ignorowany, używamy indeksów 1..maxZmienna.
        // Domyślna wartość w Javie to 'false', co odpowiada ciągowi z samymi zerami.
        boolean[] wartosciowanie = new boolean[maxZmienna + 1];
        boolean czySpelnialna = false;

        // Szukanie pierwszego wartościowania metodą brute-force
        do {
            if (formulaCNF.ewaluuj(wartosciowanie)) {
                czySpelnialna = true;
                break;
            }
        } while (inkrementujWartosciowanie(wartosciowanie));

        if (czySpelnialna) {
            for (int i = 1; i <= maxZmienna; i++) {
                if (wartosciowanie[i]) {
                    System.out.print(" " + i);
                }
            }
            System.out.println();
        } else {
            System.out.println("0");
        }
    }

    private static boolean inkrementujWartosciowanie(boolean[] tab) {
        // Zaczynamy od końca tablicy (od LSB)
        for (int i = tab.length - 1; i >= 1; i--) {
            if (!tab[i]) {
                tab[i] = true;
                return true; // Sukces: nie nastąpiło przepełnienie
            } else {
                tab[i] = false; // "Przeniesienie" do wyższego bitu
            }
        }
        return false; // Nastąpiło przepełnienie - przetestowaliśmy wszystkie wartości
    }
}