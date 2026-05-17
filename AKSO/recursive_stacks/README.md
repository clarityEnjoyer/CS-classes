📚 librstack - Zaawansowany Stos Rekurencyjny (C)librstack to napisana w języku C wysokowydajna biblioteka implementująca abstrakcyjną strukturę danych: stos rekurencyjny. W odróżnieniu od klasycznych stosów, librstack pozwala na odkładanie na stosie nie tylko 64-bitowych liczb całkowitych (uint64_t), ale również referencji do innych stosów, co umożliwia tworzenie złożonych grafów skierowanych, w tym głębokich rekurencji i cykli.Projekt kładzie szczególny nacisk na bezpieczeństwo pamięci (Memory Safety) w środowiskach nienadzorowanych, implementując własny mechanizm odśmiecania (Garbage Collection) oparty na algorytmie Trial Deletion.✨ Główne funkcjonalnościPolimorfizm elementów: Jeden stos może przechowywać mieszankę wartości liczbowych oraz wskaźników do innych stosów.Architektura Zero-Overhead dla prostych przypadków: Dzięki zastosowaniu wzorca Leniwej Inicjalizacji (Lazy Initialization), wewnętrzny Garbage Collector włącza się i alokuje pamięć wyłącznie w momencie wykrycia potencjalnego cyklu. Zwykłe operacje na stosie nie ponoszą żadnego kosztu narzutu (overhead).Wbudowany Garbage Collector: Algorytm w locie śledzi referencje i wyłapuje tzw. "odizolowane wyspy" (cykle stosów), zapobiegając wyciekom pamięci bez konieczności angażowania programisty.Bezpieczna Serializacja / Deserializacja: Funkcje rstack_write oraz rstack_read pozwalają na bezpieczny zapis zrzutu pamięci stosów do pliku. Zaimplementowano w nich zaawansowany algorytm DFS z wbudowaną detekcją cykli w czasie rzeczywistym.🏗 Architektura Memory ManagementBiblioteka opiera się na hybrydowym podejściu do zarządzania pamięcią:Reference Counting (Zliczanie referencji): Szybka ścieżka (Fast Path) realizowana w czasie $O(1)$. Jeśli licznik referencji stosu spada do zera, jest on natychmiast kaskadowo niszczony (release_node).Trial Deletion (Wykrywanie cykli): Ścieżka wolna (Slow Path). Stosy, których licznik spada, ale jest $>0$, oznaczane są jako potencjalnie zacyklowane (kolor FIOLETOWY) i trafiają do globalnego bufora. Gdy bufor przekroczy próg (1000 elementów) lub program zostanie zamknięty (przez bezpieczny hook atexit), uruchamiana jest wielofazowa analiza grafu:Mark: Symulacja usunięcia referencji i zmiana kolorów na SZARY.Scan: Poszukiwanie zewnętrznych wskaźników do grafu. Jeśli takich nie ma, cykl oznacza się jako śmieć (BIAŁY).Collect: Bezpieczne zebranie martwych węzłów z pominięciem problemów typu Dangling Pointer i usunięcie zwalnianej pamięci.🚀 Budowanie biblioteki (Build)Biblioteka do budowy wymaga środowiska Linux, narzędzia make oraz kompilatora GCC wspierającego standard gnu23.Bash# Sklonuj repozytorium

# Zbuduj współdzieloną bibliotekę (librstack.so)
make all

# Wyczyść pliki tymczasowe po budowie
make clean
Domyślnie make tworzy plik librstack.so (flaga -shared, -fPIC) przygotowany do dynamicznego linkowania z Twoim kodem.📖 Przykłady użycia (Quick Start)1. Podstawowe operacjeC#include "rstack.h"
#include <stdio.h>

int main() {
    rstack_t *rs1 = rstack_new();
    rstack_t *rs2 = rstack_new();

    // Dodanie wartości
    rstack_push_value(rs1, 42);
    rstack_push_value(rs1, 1337);

    // Zagnieżdżanie stosu w stosie
    rstack_push_rstack(rs2, rs1);

    // Zwalnianie pamięci (spadek licznika do 0 wywoła kaskadowe zniszczenie)
    rstack_delete(rs2); // Usunie rs2, co zmniejszy referencję do rs1
    rstack_delete(rs1); // Ostatecznie zniszczy rs1
    return 0;
}
2. Tworzenie i zwalnianie cykliPoniższy kod wygenerowałby wyciek pamięci w standardowym systemie zliczania referencji, ale librstack obsłuży go w tle dzięki GC:Crstack_t *rs1 = rstack_new();
rstack_t *rs2 = rstack_new();

rstack_push_rstack(rs1, rs2);
rstack_push_rstack(rs2, rs1); // Powstał zamknięty cykl!

rstack_delete(rs1); // Licznik != 0 (rs2 nadal trzyma rs1) -> ląduje w buforze GC
rstack_delete(rs2); // GC samoczynnie usunie ten graf na koniec procesu lub po przekroczeniu bufora.
🧪 Testowanie i NiezawodnośćKod biblioteki został zaprojektowany z myślą o skrajnych scenariuszach pamięciowych. Wbudowany w zestaw Makefile system testów sprawdza zachowanie biblioteki przy awariach malloc (OOM).Uruchomienie weryfikatora testów:Kompilacja odbywa się z włączeniem function wrapping (-Wl,--wrap=malloc itd.). Oznacza to, że każda alokacja jest precyzyjnie śledzona.Biblioteka przechodzi w 100% czysto testy wycieków Valgrind Memcheck:Bashvalgrind --leak-check=full --show-leak-kinds=all ./rstack_example
Oczekiwany rezultat: 0 bytes in 0 blocks are still reachable / 0 errors from 0 contexts.📜 LicencjaProjekt dystrybuowany na licencji MIT. Zobacz plik LICENSE w celu uzyskania szczegółowych informacji.
