package wyrazenia;

class Jeden extends Stała {

    @Override
    protected double wartość() {
        return 1;
    }

    @Override
    public Wyrażenie razy(Wyrażenie wyrażenie) {
        return wyrażenie;
    }

    @Override
    protected Wyrażenie odwróconeRazy(Wyrażenie wyrażenie) {
        return wyrażenie;
    }

}
