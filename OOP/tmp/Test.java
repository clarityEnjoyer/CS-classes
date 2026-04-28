import wyrazenia.Stała;
import wyrazenia.Wyrażenie;
import wyrazenia.Zmienna;

public class Test {

    public static void main(String[] args) {
        Wyrażenie a =
                Stała.oWartości(2).razy(
                        Zmienna.instancja().plus(
                                Stała.oWartości(5))).plus(
                        Stała.oWartości(7));
        System.out.println("a = " + a);
        Wyrażenie ap = a.pochodna();
        System.out.println("ap = " + ap);
        System.out.println(ap.toInfixString());
        System.out.println(a.wartość(10));
        System.out.println(a.całka(0, 1));
        Wyrażenie b = Stała.oWartości(5);
        System.out.println(b.całka(0, 10));
        Wyrażenie c = Stała.oWartości(0).plus(Zmienna.instancja());
        System.out.println(c);
        Wyrażenie f = Zmienna.instancja().plus(Stała.oWartości(0));
        System.out.println("f = " + f);
    }

}
