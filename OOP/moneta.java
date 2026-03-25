package moneta;

import java.util.Random;


public class Moneta {

    private static final Random RANDOM  = new Random();
    public static final int _N = 10;
    public static final String ORZEL = "Orzel";
    public static final String RESZKA = "Reszka";

    public static void main( String [] args){
            for (int i = 0; i < _N; i++){
                boolean b = RANDOM.nextBoolean();
                extracted(b);
            }
            System.out.println();
        }

    private static void extracted(boolean b) {
        String s;
        if (b) {
            s = ORZEL;
        }
        else {
            s = RESZKA;
        }
        System.out.print(" " + s);
    }

}
