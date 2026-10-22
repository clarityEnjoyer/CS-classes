#ifndef _RECURSIVE_GRAPH_COLLECTIONS_H
#define _RECURSIVE_GRAPH_COLLECTIONS_H

// Tworzy nową, pustą kolekcję nazwanych grafów rekurencyjnych. Wynikiem jest
// identyfikator tej kolekcji. Każde wywołanie zwraca nowy identyfikator,
// począwszy od 0, a skończywszy na LONG_MAX. Jeśli wyczerpią się
// identyfikatory, zwraca -1.
long rgc_new_collection();

// Jeżeli istnieje kolekcja nazwanych grafów rekurencyjnych o identyfikatorze
// id, usuwa ją, a w przeciwnym przypadku niczego nie robi.
void rgc_delete_collection(long id);

// Jeśli istnieje kolekcja o identyfikatorze id, wskaźnik name jest różny od
// nullptr i wskazuje na poprawną nazwę grafu niewystępującą w kolekcji, wartość
// n jest nieujemna, tworzy w tej kolekcji nowy graf o podanej nazwie,
// n wierzchołkach i pustym zbiorze krawędzi. Jeśli n jest dodatnie, to
// wierzchołki są numerowane od 0 do n - 1. Wynikiem funkcji jest true, jeśli
// graf został utworzony, a false w przeciwnym przypadku.
bool rgc_new_graph(long id, char const *name, int n);

// Jeśli istnieje kolekcja o identyfikatorze id, wskaźnik name jest różny od
// nullptr i w kolekcji jest graf o nazwie name, usuwa go, a w przeciwnym
// przypadku niczego nie robi.
void rgc_delete_graph(long id, char const *name);

// Jeśli istnieje kolekcja o identyfikatorze id, wskaźnik name jest różny od
// nullptr i w kolekcji jest graf o nazwie name o mniej niż INT_MAX
// wierzchołkach, dodaje do niego nowy, izolowany wierzchołek. Nowy wierzchołek
// otrzymuje najmniejszy nieujemny numer niewystępujący w grafie. Wynikiem
// funkcji jest numer nowego wierzchołka, jeśli został utworzony, a -1
// w przeciwnym przypadku.
int rgc_graph_new_node(long id, char const *name);

// Jeśli istnieje kolekcja o identyfikatorze id, wskaźnik name jest różny od
// nullptr i w kolekcji jest graf o nazwie name mający wierzchołek o numerze x,
// usuwa ten wierzchołek wraz ze wszystkimi krawędziami, do których należy.
// Numery pozostałych wierzchołków się nie zmieniają. Wynikiem funkcji jest
// true, jeśli wierzchołek został usunięty, a false w przeciwnym przypadku.
bool rgc_graph_delete_node(long id, char const *name, int x);

// Jeśli istnieje kolekcja o identyfikatorze id, wskaźnik name jest różny od
// nullptr i w kolekcji jest graf o nazwie name z różnymi wierzchołkami
// o numerach x i y niepołączonymi krawędzią, dodaje do grafu krawędź {x, y}
// z poziomem 0. Wynikiem funkcji jest true, jeśli krawędź została dodana,
// a false w przeciwnym przypadku.
bool rgc_graph_add_edge(long id, char const *name, int x, int y);

// Jeśli istnieje kolekcja o identyfikatorze id, wskaźnik name jest różny od
// nullptr i w kolekcji jest graf o nazwie name mający krawędź {x, y}, usuwa
// tę krawędź. Wynikiem funkcji jest true, jeśli krawędź została usunięta,
// a false w przeciwnym przypadku.
bool rgc_graph_delete_edge(long id, char const *name, int x, int y);

// Jeśli istnieje niepusta kolekcja o identyfikatorze id, wynikiem funkcji jest
// wskaźnik na nazwę pierwszego grafu w tej kolekcji, a nullptr w przeciwnym
// przypadku.
char const * rgc_first_graph(long id);

// Jeśli istnieje kolekcja o identyfikatorze id, wskaźnik name jest różny od
// nullptr i w kolekcji istnieje graf następny w kolejności po grafie o nazwie
// name, wynikiem funkcji jest wskaźnik na nazwę tego grafu, a nullptr
// w przeciwnym przypadku.
char const * rgc_next_graph(long id, char const *name);

// Jeśli istnieje kolekcja o identyfikatorze id, wskaźnik name jest różny od
// nullptr i w kolekcji jest graf o nazwie name mający co najmniej jeden
// wierzchołek, wynikiem funkcji jest najmniejszy występujący w nim numer
// wierzchołka, a -1 w przeciwnym przypadku.
int rgc_first_node(long id, char const *name);

// Jeśli istnieje kolekcja o identyfikatorze id, wskaźnik name jest różny od
// nullptr i w kolekcji jest graf o nazwie name, który ma wierzchołek
// o numerze większym niż x, wynikiem funkcji jest najmniejszy numer takiego
// wierzchołka, a -1 w przeciwnym przypadku.
int rgc_next_node(long id, char const *name, int x);

// Jeśli istnieje kolekcja o identyfikatorze id, wskaźnik name jest różny od
// nullptr i w kolekcji jest graf o nazwie name, w którym występuje wierzchołek
// o numerze x mający sąsiada, wynikiem funkcji jest najmniejszy numer sąsiada
// tego wierzchołka, a -1 w przeciwnym przypadku.
int rgc_first_edge(long id, char const *name, int x);

// Jeśli istnieje kolekcja o identyfikatorze id, wskaźnik name jest różny od
// nullptr i w kolekcji jest graf o nazwie name, w którym występuje wierzchołek
// o numerze x mający sąsiada o numerze większym niż y, wynikiem funkcji jest
// najmniejszy większy od y numer sąsiada wierzchołka x, a -1 w przeciwnym
// przypadku.
int rgc_next_edge(long id, char const *name, int x, int y);

// Jeśli istnieje kolekcja o identyfikatorze id, wskaźniki result_name,
// recipient_name i donor_name są różne od nullptr, result_name wskazuje na
// poprawną nazwę grafu i grafu o takiej nazwie nie ma w kolekcji, w kolekcji
// jest biorca o nazwie recipient_name, w kolekcji jest dawca o nazwie
// donor_name, biorca ma krawędź {w_1, w_2}, dawca ma dwa różne wierzchołki
// v_1 i v_2, poziom grafu wynikowego będzie mniejszy niż LEVEL_MAX, liczba
// wierzchołków grafu wynikowego nie przekroczy INT_MAX, tworzy w kolekcji graf
// wynikowy o nazwie result_name powstały za pomocą opisanej wyżej procedury
// zastępowania. Wynikiem funkcji jest true, jeśli graf został utworzony,
// a false w przeciwnym przypadku.
bool rgc_replace_edge(long id, char const *result_name,
                      char const *recipient_name, int w_1, int w_2,
                      char const *donor_name, int v_1, int v_2);

// Jeśli istnieje kolekcja o identyfikatorze id, wskaźnik name jest różny od
// nullptr i w kolekcji jest graf o nazwie name mający krawędź {x, y},
// wynikiem funkcji jest poziom tej krawędzi, a -1 w przeciwnym przypadku.
ssize_t rgc_edge_level(long id, char const *name, int x, int y);

// Jeśli istnieje kolekcja o identyfikatorze id, wskaźnik name jest różny od
// nullptr i w kolekcji jest graf o nazwie name, wynikiem funkcji jest poziom
// tego grafu, a -1 w przeciwnym przypadku.
ssize_t rgc_graph_level(long id, char const *name);

// Wynikiem funkcji jest maksymalny dopuszczalny poziom krawędzi, czyli
// LEVEL_MAX - 1.
ssize_t rgc_max_level();

// Jeśli istnieje kolekcja o identyfikatorze id, wskaźnik name jest różny od
// nullptr i w kolekcji jest graf o nazwie name, wynikiem funkcji jest liczba
// wierzchołków tego grafu, a -1 w przeciwnym przypadku.
ssize_t rgc_count_nodes(long id, char const *name);

// Jeśli istnieje kolekcja o identyfikatorze id, wskaźnik name jest różny od
// nullptr i w kolekcji jest graf o nazwie name, wynikiem funkcji jest liczba
// krawędzi tego grafu, a -1 w przeciwnym przypadku.
ssize_t rgc_count_edges(long id, char const *name);

// Jeśli istnieje kolekcja o identyfikatorze id, wynikiem funkcji jest liczba
// grafów w tej kolekcji, a -1 w przeciwnym przypadku.
ssize_t rgc_size(long id);

#endif
