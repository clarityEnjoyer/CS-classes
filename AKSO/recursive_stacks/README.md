# Zaawansowany Stos Rekurencyjny (C)

Wydajna biblioteka w języku C implementująca stos, który potrafi przechowywać zarówno liczby (`uint64_t`), jak i wskaźniki na inne stosy. Pozwala to na budowanie złożonych struktur i grafów, w tym tworzenie głębokich cykli.

## Dlaczego warto? (Główne funkcje)

* **Wbudowany Garbage Collector (Trial Deletion):** Samodzielnie radzi sobie z cyklami referencji. Nie musisz martwić się o wycieki pamięci – biblioteka sama posprząta "odizolowane wyspy", które na siebie wskazują.
* **Leniwa inicjalizacja (Lazy Init):** GC włącza się i alokuje pamięć *tylko wtedy*, gdy faktycznie wykryje potencjalny cykl. Jeśli używasz stosu w prosty sposób, masz absolutne zero narzutu wydajnościowego.
* **Zapis i odczyt:** Bezpieczne zrzucanie całego stanu stosu do pliku z wbudowaną detekcją cykli w czasie rzeczywistym.
* **Czysty Valgrind:** Kod przechodzi rygorystyczne testy pamięci (0 wycieków).

## Kompilacja

Projekt korzysta z `make` i kompilatora GCC (standard `gnu23`).

```bash
# Budowa współdzielonej biblioteki (librstack.so)
make all

# Czyszczenie plików tymczasowych
make clean
```
```cpp
#include "rstack.h"

int main() {
    rstack_t *rs1 = rstack_new();
    rstack_t *rs2 = rstack_new();

    rstack_push_value(rs1, 42);   // Odkładanie liczby
    rstack_push_rstack(rs2, rs1); // Odkładanie stosu rs1 na stos rs2

    // Tworzenie zamkniętego cyklu.
    // W normalnym C byłby tu wyciek pamięci, ale nasz GC sam to posprząta!
    rstack_push_rstack(rs1, rs2); 

    rstack_delete(rs1);
    rstack_delete(rs2);
    
    return 0;
}
```
