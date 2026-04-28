package wyrazenia;

public class Suma extends Operacja {

    Suma(Wyrażenie lewy, Wyrażenie prawy) {
        super(lewy, prawy);
    }

    @Override
    protected double oblicz(double wartośćLewego, double wartośćPrawego) {
        return wartośćLewego + wartośćPrawego;
    }

    @Override
    protected String operator() {
        return "+";
    }

    @Override
    public Wyrażenie pochodna() {
        Wyrażenie pochodnaLewego = lewy().pochodna();
        Wyrażenie pochodnaPrawego = prawy().pochodna();
        return pochodnaLewego.plus(pochodnaPrawego);
    }

}
