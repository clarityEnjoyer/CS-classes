/*  
    dwie identyczne linijki liczymy podwojnie

    Dwa ciągi słów oznaczają te same zawody, jeśli są to takie same ciągi słów. 
    Odstępów między słowami nie porównujemy.

    Słowo jest to niepusty napis zawierający niebiałe znaki.

    Rok jest to słowo zawierające zapisaną przy podstawie 10 liczbę o wartości 
    od 1800 do 59999, bez zer wiodących.

    Odstęp jest to niepusty napis zawierający wyłącznie znaki spacji lub tabulatora

    Zawody jest to niepusty ciąg kolejnych słów zawierających 
    tylko duże lub małe litery alfabetu angielskiego, oddzielonych odstępami. 
    
    Linia jest to ciąg słów oddzielonych odstępami, 
    zakończona znakiem nowej linii. Odstęp może też wystąpić na początku 
    i na końcu linii.

    Obsługę wejścia i wyjścia należy zrealizować za pomocą strumieni.

    Dla każdej błędnej linii program wypisuje na standardowe wyjście 
    diagnostyczne słowo ERROR, spację, numer błędnej linii oraz znak nowej linii. 
    Linie są numerowane od 1. 
    
    Wyjątkiem jest ostatnia linia w danych wejściowych. 
    Jeśli nie kończy się znakiem nowej linii, jest to poprawna linia.
*/

#include <iostream>
#include <string_view>

using namespace std; // TO BE DELETED

const string ODSTEP = " \t";   

void trimLine(string_view &line){
    line.remove_prefix(line.find_first_not_of(ODSTEP));
    // Notice: if a line contains only whitespace, then it won't get trimmed.
}

void processLine(string_view line, bool isLastLine, int lineNum){
    trimLine(line);
    cout << line << " " << isLastLine << " " << lineNum << endl;

    // A new query.
    if (line.at(0) == '?') {
        line.remove_prefix(1);
        trimLine(line);
        // fetch info and check if correct

    }
    else { // New data.
        
    }
}



int main() {
    
    string currentLine;
    int lineNum = 0;
    bool isLastLine = false;

    while (getline(cin, currentLine)) {
        if (cin.peek() == EOF) isLastLine = true;
        processLine (currentLine, isLastLine, ++lineNum);
    }

    if (lineNum == 0){
        // puste wejscie - co wtedy?
        cerr << "Empty input.\n";
    }
}