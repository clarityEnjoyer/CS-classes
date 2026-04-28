package calculator;

import java.util.Scanner;

public class Instance {
    public Instance(){
        for (int i = 0; i < variables.length; i++) {
            variables[i] = new Fraction();
        }
    }

    private Fraction [] variables = new Fraction['z' - 'a' + 1];

    private Fraction getVariable(String variable) {
        return variables[variable.charAt(0) - 'a'];
    }

    private void setVariable (String variable, Fraction newValue) {
        variables[variable.charAt(0) - 'a'] = newValue;
    }

    private boolean isVariable (String s) {
        return s.length()==1 && (s.charAt(0) >= 'a' && s.charAt(0) <= 'z');
    }

        //moge na bieząco wykonkywac operacje.
        //traktuję ze wczytuję liczby calkowite

    private boolean isAssignment (String command){
        return (command.length()>1 && command.charAt(2)=='=');
    }

    public void executeCommand (String _command) {
        Scanner command = new Scanner(_command);
        if (isAssignment(_command))
            executeAssignment(command);
        else
            System.out.println(executeExpression(command));
    }

    private Fraction executeExpression (Scanner expression){
        String chunk      = expression.next();
        Fraction fraction = valueOf(chunk);
        Fraction value;
        String operator;

        while(expression.hasNext()){
            operator = expression.next();
            chunk    = expression.next();
            value    = valueOf(chunk);
            switch (operator) {
                case "+":
                    fraction.add(value);
                    break;
                case "-":
                    fraction.substract(value);
                    break;
                case "*":
                    fraction.multiply(value);
                    break;
                case "/":
                    fraction.divide(value);
                    break;
                default:
                    assert (true);
            }
        }
        return fraction;
    }

    private Fraction valueOf(String chunk) {
        Fraction value;
        if (isVariable(chunk))
            value = new Fraction(getVariable(chunk));
        else
            value = new Fraction(Integer.parseInt(chunk));
        return value;
    }

    private void executeAssignment (Scanner expression){
        String chunk      = expression.next();
        Fraction fraction = getVariable(chunk);
        expression.next();
        Fraction rightHandSide = executeExpression(expression);
        setVariable(chunk, rightHandSide);
    }
}
