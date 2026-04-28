package wyrazenia;

class Zero extends Stała {

    @Override
    protected double wartość() {
        return 0;
    }

    @Override
    public Wyrażenie plus(Wyrażenie wyrażenie) {
        return wyrażenie;
    }

    @Override
    protected Wyrażenie odwróconyPlus(Wyrażenie wyrażenie) {
        return wyrażenie;
    }

    @Override
    protected Wyrażenie odwróconeRazy(Wyrażenie wyrażenie) {
        return this;
    }

    @Override
    public Wyrażenie razy(Wyrażenie wyrażenie) {
        return this;
    }

}
