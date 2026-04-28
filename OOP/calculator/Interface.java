package calculator;

import java.util.Scanner;

public class Interface {
    public static void main(String[] args) {
        Instance calculator = new Instance();
        Scanner script = new Scanner(System.in);
        while (script.hasNextLine()){
            String newCommand = script.nextLine();
            calculator.executeCommand(newCommand);
        }
    }
}
