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

    Dwa ciągi słów oznaczają te same zawody, jeśli są to takie same ciągi słów. 
    Odstępów między słowami nie porównujemy !!!

*/

#include <cstddef>
#include <iostream>
#include <string_view>
#include <regex>

using namespace std; // TO BE DELETED

const string WHITESPACE = " \t";   

inline void communicateError(int &lineNum) {
    cerr << "ERROR " << lineNum << '\n';
}

inline void trimLine(string_view &line) {
    line.remove_prefix(line.find_first_not_of(WHITESPACE));
    // Notice: if a line contains only whitespace, then it won't get trimmed.
}

inline void standarizeEventName(string &eventName){
    // A name is a succession of words separated with 
    // any amount of whitespace [ \t] which shouldn't be compared.
    // This function transforms each whitespace into one space character (' ')
    size_t freePlaceToWrite = 0;
    for (size_t i = 0; i < eventName.size(); i++) {
        if (WHITESPACE.contains(eventName[i])) {
            if (eventName[freePlaceToWrite] == ' ') {
                continue;
            }
            else {
                eventName[freePlaceToWrite] = ' ';
                freePlaceToWrite++;
            }
            
        }
    } 
    

}

void processLine(string_view line, int lineNum){
    trimLine(line);
    cout << line << endl;

    // A new query.
    if (line.at(0) == '?') {
        line.remove_prefix(1);
        trimLine(line);
        // fetch info and check if correct

    }
    else { // New data.
        static const regex pattern(R"(^([1-3])[ \t]+(\d{4,5})[ \t]+([a-zA-Z]+(?:[ \t]+[a-zA-Z]+)*)[ \t]+$)");    
        match_results<string_view::const_iterator> matches;
        if (regex_match(line.begin(), line.end(), matches, pattern)) {
            string place = matches[1].str();
            string year  = matches[2].str();
            string name  = matches[3].str();
            int yearInt  = stoi(year, 0, 10);
            cout << yearInt<<" "<<name<<endl;
            if (year.at(0) == '0' || !(yearInt >= 1800 && yearInt <= 59999)){
                communicateError(lineNum);
                return;
            }
            standarizeEventName(name);
        }
        else {
            communicateError(lineNum);
            return;
        }
    }
}



int main() {
    
    string currentLine;
    int lineNum = 0;

    while (getline(cin, currentLine)) {
        processLine (currentLine, ++lineNum);
    }

    if (lineNum == 0){
        // puste wejscie - co wtedy?
        cerr << "Empty input.\n";
    }
}