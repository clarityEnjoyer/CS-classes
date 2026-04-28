package wyrazenia;

public class Iloczyn extends Operacja {

    Iloczyn(Wyrażenie lewy, Wyrażenie prawy) {
        super(lewy, prawy);
    }

    @Override
    protected double oblicz(double wartośćLewego, double wartośćPrawego) {
        return wartośćLewego * wartośćPrawego;
    }

    @Override
    protected String operator() {
        return "*";
    }

    @Override
    public Wyrażenie pochodna() {
        Wyrażenie pochodnaLewego = lewy().pochodna();
        Wyrażenie pochodnaPrawego = prawy().pochodna();
        return pochodnaLewego.razy(prawy()).plus(lewy().razy(pochodnaPrawego));
    }

}
