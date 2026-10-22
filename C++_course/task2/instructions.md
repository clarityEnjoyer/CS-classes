# Zadanie 2: Recursive Graph Collections (`rgc`)

## 1. Opis i Reguły Domenowe

Moduł udostępnia kolekcje nazwanych grafów rekurencyjnych z interfejsem dostępnym zarówno z poziomu języka C, jak i C++.

### Grafy, Wierzchołki i Nazwy
- **Wierzchołki:** Numerowane nieujemnymi liczbami całkowitymi od `0` (wierzchołki w grafie o $n$ wierzchołkach początkowo mają numery $0, \dots, n-1$).
- **Nazwy:** Niepusty napis złożony wyłącznie ze znaków `[a-zA-Z0-9_]`. Kolekcje przechowują **kopie nazw**.
- **Porządek:** Grafy w kolekcji są uporządkowane leksykograficznie według kodów ASCII swoich nazw.

### Poziomy Krawędzi i Grafów
- **`LEVEL_MAX`:** Dodatnia stała kompilacji (domyślnie `4`, modyfikowalna flagą kompilatora `-DLEVEL_MAX=...`).
- **Poziom krawędzi:** Liczba całkowita w zakresie $[0, \text{LEVEL\_MAX} - 1]$. Nowo dodana krawędź otrzymuje poziom `0`.
- **Poziom grafu:** Maksymalny z poziomów jego krawędzi lub `0`, jeżeli graf nie ma krawędzi:
  $$\text{graphLevel} = \max(\{ \text{edgeLevel} \mid e \in E \} \cup \{0\})$$

### Procedura Zastępowania Krawędzi (`rgc_replace_edge`)
Operacja polega na zastąpieniu wybranej krawędzi $\{w_1, w_2\}$ w **biorcy** grafem prostym (**dawca** o $k$ wierzchołkach i wyróżnionych końcach $v_1, v_2$):
1. **Kopia:** Tworzymy graf wynikowy jako kopię biorcy (biorca pozostaje niezmieniony). Dawca może być tym samym grafem co biorca.
2. **Usunięcie krawędzi:** Z grafu wynikowego usuwamy krawędź $\{w_1, w_2\}$.
3. **Utożsamienie:** Utożsamiamy wierzchołki $w_1$ i $w_2$ grafu wynikowego z $v_1$ i $v_2$ dawcy.
4. **Nowe wierzchołki:** Pozostałe wierzchołki dawcy $v_3, \dots, v_k$ (w kolejności rosnącej ich numerów) są dodawane do grafu wynikowego jako $k - 2$ nowych wierzchołków $w_3, \dots, w_k$ o kolejnych najmniejszych wolnych numerach w grafie wynikowym.
5. **Krawędzie dawcy:** Dla każdej krawędzi $\{v_i, v_j\}$ w dawcy dodajemy krawędź $\{w_i, w_j\}$ w grafie wynikowym.
6. **Poziom nowych krawędzi:**
   $$\text{poziom} = \max(\text{poziom dawcy}, \text{poziom zastępowanej krawędzi}) + 1$$
7. **Warunki poprawności:** Operacja powiedzie się, jeśli poziom grafu wynikowego jest $<\text{LEVEL\_MAX}$, a łączna liczba wierzchołków nie przekracza `INT_MAX`.

---

## 2. Wymagania Formalne

- **Pliki rozwiązania:** `recursive_graph_collections.h` oraz `recursive_graph_collections.cpp`.
- **Środowisko:** Standard C++23 (dla C++), C23 (dla C). Testowane na maszynie `students`.
- **Zakaz własnych struktur danych:**
  - Należy używać kontenerów i metod biblioteki standardowej C++ (STL).
  - **Nie wolno** definiować własnych struktur, unii ani klas zawierających dane.
- **Pamięć:**
  - Zakaz wycieków pamięci oraz zakaz ręcznego używania `new` / `delete`.
  - Kolekcje muszą przechowywać kopie łańcuchów znaków (nazw), a nie jedynie przekazane wskaźniki.
  - Można założyć brak wyczerpania pamięci (brak konieczności obsługi wyjątku braku pamięci).
- **Interfejs modułu i przestrzenie nazw:**
  - W pliku nagłówkowym przy kompilacji w C++ deklaracje funkcji muszą znaleźć się w przestrzeni nazw `cxx`.
  - W C deklaracje muszą być dostępne w globalnej przestrzeni nazw z konwencją wywołań języka C (`extern "C"`).
  - Wszelkie zmienne globalne i funkcje pomocnicze modułu muszą być ściśle ukryte przed światem zewnętrznym (np. w anonimowej przestrzeni nazw).
  - Nagłówek musi zawierać poprawne strażniki włączeń (`#ifndef`, `#define`, `#endif`).

---

## 3. Notatki z Laboratorium

- **Kompatybilność C / C++:**
  - Użycie `#ifdef __cplusplus` w połączeniu z blokiem `extern "C"` zapewnia zgodne ABI z C i brak dekorowania (*mangling*) nazw dla kompilatora C.
  - W C++ funkcje powinny być umieszczone w przestrzeni nazw `cxx`.
  - Unikać zbędnych lub zagnieżdżonych `namespace` w `namespace`.
- **Obsługa `LEVEL_MAX`:**
  - Wartość stałej powinna być konfigurowalna przez makro kompilacji (`-DLEVEL_MAX=...`), a przy jego braku przyjmować wartość domyślną `4`:
    ```cpp
    #ifdef LEVEL_MAX
        constexpr uint64_t LEVEL_MAX_C = LEVEL_MAX;
    #else
        constexpr uint64_t LEVEL_MAX_C = 4;
    #endif
    ```
  - Dyrektywy preprocesora (`#ifdef`) stosować tylko tam, gdzie są rzeczywiście niezbędne.

---

## 4. Kompilacja i Przykłady Użycia

Przykłady użycia znajdują się w plikach `recursive_graph_collections_example_1.c` oraz `recursive_graph_collections_example_2.cpp`.

```bash
# 1. Kompilacja modułu (wersja domyślna LEVEL_MAX=4 oraz wersja z LEVEL_MAX=16)
g++ -c -Wall -Wextra -O2 -std=c++23 ../rozwiazanie/recursive_graph_collections.cpp -o recursive_graph_collections_4.o
g++ -c -Wall -Wextra -O2 -std=c++23 -DLEVEL_MAX=16 ../rozwiazanie/recursive_graph_collections.cpp -o recursive_graph_collections_16.o

# 2. Kompilacja plików z przykładami (C i C++)
gcc -c -Wall -Wextra -O2 -std=c23 -I../rozwiazanie recursive_graph_collections_example_1.c -o recursive_graph_collections_example_1.o
g++ -c -Wall -Wextra -O2 -std=c++23 -I../rozwiazanie recursive_graph_collections_example_2.cpp -o recursive_graph_collections_example_2.o

# 3. Linkowanie
g++ recursive_graph_collections_example_1.o recursive_graph_collections_4.o -o recursive_graph_collections_example_1
g++ recursive_graph_collections_example_2.o recursive_graph_collections_16.o -o recursive_graph_collections_example_2_a
g++ recursive_graph_collections_16.o recursive_graph_collections_example_2.o -o recursive_graph_collections_example_2_b
```

---

## 5. Kryteria Oceny Jakości Kodu (0–4 pkt)

Ocena jakości kodu jest liczbą całkowitą z przedziału **0–4 pkt**.

### Punkty odejmowane są za:
- **Styl i formatowanie:** niepoprawne spacje, wcięcia, linie $> 80$ znaków, brak komentarzy, magiczne stałe.
- **Własne struktury zamiast STL:** tworzenie własnych klas, struktur lub algorytmów z danymi zamiast wykorzystania STL.
- **Dobór typów:** zły wybór kontenera/typu, brak aliasów typów lub nieintuicyjne nazwy typów (`using` zamiast `typedef`).
- **Nieefektywność i redundancja:**
  - Wielokrotne wyszukiwanie tego samego klucza w mapie (np. `map[key]`).
  - Rozwlekła architektura, powtarzanie kodu, rozpatrywanie nadmiaru przypadków brzegowych.
  - Nieefektywne użycie `std::string` (np. `str != ""` zamiast `!str.empty()`), przechowywanie liczb jako napisów.
  - Nieusuwanie zbędnych danych lub wprowadzanie sztucznych limitów rozmiaru.
- **Architektura globalna i pamięć:**
  - *Static Initialization Order Fiasco* (SIOF) przy obiektach globalnych.
  - Powielanie inicjalizacji strumienia diagnostycznego w każdej funkcji.
  - Potencjalne wycieki pamięci, użycie `new` / `delete`.
- **Pliki nagłówkowe i enkapsulacja:**
  - Błędy w nagłówkach (brak include guardów, brak `#ifdef __cplusplus`, brakujące lub zbędne `#include`).
  - Nieukrycie zmiennych globalnych lub funkcji pomocniczych nienależących do publicznego interfejsu.
  - Błędy w operowaniu przestrzeniami nazw (`namespace`).

### Dodatkowe uwagi (wskazywane jako błędy):
- Przekazywanie dużych obiektów przez wartość zamiast przez stałą referencję (`const &`).
