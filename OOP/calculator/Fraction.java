package calculator;

public class Fraction {
    private int numerator, denominator;//denominator is > 0

    public Fraction(int numerator, int denominator) {
        this.numerator   = numerator;
        this.denominator = denominator;
    }

    public Fraction(int numerator) {
        this(numerator,1);
    }

    public Fraction() {
        this(0,1);
    }

    public Fraction(Fraction other) {
        this.numerator   = other.numerator;
        this.denominator = other.denominator;
    }

    private int gcd (final int _a, final int _b){
        int a = Math.abs(_a);
        int b = Math.abs(_b);
        while (b>0){
            int t = a % b;
            a = b;
            b = t;
        }
        return a;
    }

    private void normalise(){
        if (denominator <  0) {
            numerator   *= -1;
            denominator *= -1;
        }
        int x = gcd(numerator,denominator);
        numerator   /=x;
        denominator /=x;
    }

    public void add (Fraction x){
        int _numerator   = this.numerator * x.denominator + this.denominator * x.numerator;
        int _denominator = this.denominator * x.denominator;
        this.numerator   = _numerator;
        this.denominator = _denominator;
        normalise();
    }

    public void substract (Fraction x){
        int _numerator   = this.numerator * x.denominator - this.denominator * x.numerator;
        int _denominator = this.denominator * x.denominator;
        this.numerator   = _numerator;
        this.denominator = _denominator;
        normalise();
    }

    public void multiply (Fraction x){
        int _numerator   = this.numerator * x.numerator;
        int _denominator = this.denominator * x.denominator;
        this.numerator   = _numerator;
        this.denominator = _denominator;
        normalise();
    }

    public void divide (Fraction x){
        int _numerator   = this.numerator * x.denominator;
        int _denominator = this.denominator * x.numerator;
        this.numerator   = _numerator;
        this.denominator = _denominator;
        normalise();
    }


    @Override
    public String toString(){
        normalise();
        StringBuilder fractionAsText = new StringBuilder();
        fractionAsText.append(numerator);
        if (denominator > 1)
            fractionAsText.append("/").append(denominator);
        return fractionAsText.toString();
    }

}
