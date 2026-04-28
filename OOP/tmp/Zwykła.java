package wyrazenia;

class Zwykła extends Stała {

    private final double wartość;

    protected Zwykła(double wartość) {
        this.wartość = wartość;
    }

    @Override
    protected double wartość() {
        return wartość;
    }

}
