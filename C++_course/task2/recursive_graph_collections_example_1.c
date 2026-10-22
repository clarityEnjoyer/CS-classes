#include "recursive_graph_collections.h"

#ifdef NDEBUG
  #undef NDEBUG
#endif

#include <assert.h>
#include <stdio.h>

int main() {
  long id = rgc_new_collection();
  assert(id == 0);

  // Graf G_0 jest to ścieżka o wierzchołkach 0 – 1 – 2.
  assert(rgc_new_graph(id, "G_0", 3));
  assert(rgc_graph_add_edge(id, "G_0", 0, 1));
  assert(rgc_graph_add_edge(id, "G_0", 1, 2));

  // Graf G_{k+1} powstaje z G_k przez zastąpienie krawędzi wychodzącej z 0
  // samym grafem G_k o wyróżnionych końcach 0 i 2. Wynik nadal jest ścieżką,
  // a każde zastąpienie podnosi poziom grafu o 1.
  char name[16], next[16];
  int k = 0, n = 3;
  for (;; k++, n = 2 * n - 2) {
    snprintf(name, sizeof name, "G_%d", k);
    snprintf(next, sizeof next, "G_%d", k + 1);
    int x = rgc_first_edge(id, name, 0);
    if (!rgc_replace_edge(id, next, name, 0, x, name, 0, 2))
      break;
    assert(rgc_count_nodes(id, next) == 2 * n - 2);
    assert(rgc_count_edges(id, next) == 2 * n - 3);
    assert(rgc_graph_level(id, next) == k + 1);
    assert(rgc_edge_level(id, next, 1, 2) == 0);
  }

  // Dalej się nie da zastępować, bo poziom przekroczyłby maksymalny.
  assert(k == rgc_max_level());
  assert(rgc_graph_level(id, name) == rgc_max_level());
  assert(rgc_count_nodes(id, next) == -1);
  assert(rgc_size(id) == k + 1);

  // Krawędź {1, 2} ma nadal poziom 0, więc można ją zastąpić dawcą o poziomie
  // niewiększym niż rgc_max_level() - 1, np. grafem G_0, ale już nie grafem G_k.
  assert(!rgc_replace_edge(id, "Q", name, 1, 2, name, 0, 2));
  assert(rgc_replace_edge(id, "Q", name, 1, 2, "G_0", 0, 2));
  assert(rgc_edge_level(id, "Q", 1, 2) == -1);
  assert(rgc_graph_level(id, "Q") == rgc_max_level());

  rgc_delete_collection(id);
  assert(rgc_size(id) == -1);
}
