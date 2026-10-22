#include "recursive_graph_collections.h"

#ifdef NDEBUG
  #undef NDEBUG
#endif

#include <cassert>
#include <format>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using std::format;
using std::pair;
using std::string;
using std::unordered_map;
using std::vector;

namespace {
  // Tworzy w kolekcji o danym id klikę o n wierzchołkach i nazwie K_n.
  bool make_n_clique(long id, int n) {
    string name = format("K_{}", n);
    if (!::cxx::rgc_new_graph(id, name.c_str(), n))
      return false;

    for (int x = 0; x < n; x++)
      for (int y = x + 1; y < n; y++)
        assert(::cxx::rgc_graph_add_edge(id, name.c_str(), x, y));

    return true;
  }

  // Tworzy w kolekcji o danym id graf o nazwie dst będący kopią grafu src.
  bool copy_graph(long id, string const &src, string const &dst) {
    auto s = src.c_str(), d = dst.c_str();
    if (::cxx::rgc_count_nodes(id, s) == -1 || !::cxx::rgc_new_graph(id, d, 0))
      return false;

    unordered_map<int, int> new_node;
    for (int x = ::cxx::rgc_first_node(id, s); x != -1; x = ::cxx::rgc_next_node(id, s, x)) {
      new_node[x] = ::cxx::rgc_graph_new_node(id, d);
      assert(new_node[x] != -1);
    }

    for (int x = ::cxx::rgc_first_node(id, s); x != -1; x = ::cxx::rgc_next_node(id, s, x))
      for (int y = ::cxx::rgc_first_edge(id, s, x); y != -1; y = ::cxx::rgc_next_edge(id, s, x, y))
        if (x < y)
          assert(::cxx::rgc_graph_add_edge(id, d, new_node[x], new_node[y]));

    return true;
  }

  bool has_node(long id, char const *s, int x) {
    return ::cxx::rgc_next_node(id, s, x - 1) == x;
  }

  // Tworzy w kolekcji o danym id graf o nazwie result powstały przez zastąpienie
  // każdej krawędzi grafu recipient grafem donor z wyróżnionymi wierzchołkami
  // v_1 i v_2. Grafy pośrednie są usuwane.
  bool replace_all_edges(long id, string const &result, string const &recipient,
                         string const &donor, int v_1, int v_2) {
    auto rec = recipient.c_str();
    auto d = donor.c_str();

    if (::cxx::rgc_count_nodes(id, rec) == -1 || ::cxx::rgc_count_nodes(id, d) == -1)
      return false;
    if (v_1 == v_2 || !has_node(id, d, v_1) || !has_node(id, d, v_2))
      return false;

    // Zastąpienie krawędzi nie zmienia numerów pozostałych wierzchołków,
    // więc krawędzie biorcy można zebrać z góry.
    vector<pair<int, int>> edges;
    for (int x = ::cxx::rgc_first_node(id, rec); x != -1; x = ::cxx::rgc_next_node(id, rec, x))
      for (int y = ::cxx::rgc_first_edge(id, rec, x); y != -1; y = ::cxx::rgc_next_edge(id, rec, x, y))
        if (x < y)
          edges.emplace_back(x, y);

    if (edges.empty())
      return copy_graph(id, recipient, result);

    string current = recipient;
    for (size_t i = 0; i < edges.size(); i++) {
      string next = i + 1 == edges.size() ? result : format("{}_tmp_{}", result, i);
      auto [x, y] = edges[i];
      if (!::cxx::rgc_replace_edge(id, next.c_str(), current.c_str(), x, y, d, v_1, v_2))
        return false;
      if (current != recipient)
        ::cxx::rgc_delete_graph(id, current.c_str());
      current = next;
    }

    return true;
  }

  // Sprawdza poprawność inicjowania obiektów globalnych.
  long test() {
    long id = ::cxx::rgc_new_collection();
    ::cxx::rgc_new_graph(id, "test", 5);
    return id;
  }

  long id = test();
} // koniec anonimowej przestrzeni nazw

int main() {
  assert(id == 0);

  assert(make_n_clique(id, 4));
  assert(make_n_clique(id, 5));
  assert(!make_n_clique(id, 5));

  // Robimy z 4-kliki ścieżkę 0 – 1 – 2 przez usunięcie wierzchołka 3 i krawędzi {0, 2}.
  assert(copy_graph(id, "K_4", "P_3"));
  assert(::cxx::rgc_graph_delete_node(id, "P_3", 3));
  assert(::cxx::rgc_graph_delete_edge(id, "P_3", 0, 2));
  assert(::cxx::rgc_count_nodes(id, "P_3") == 3);
  assert(::cxx::rgc_count_edges(id, "P_3") == 2);

  // Zastępując każdą krawędź 5-kliki ścieżką P_3 o końcach 0 i 2,
  // otrzymujemy 1-podpodziały K_5: każda krawędź 5-kliki jest podzielona
  // jednym nowym wierzchołkiem.
  assert(replace_all_edges(id, "G", "K_5", "P_3", 0, 2));
  assert(::cxx::rgc_size(id) == 5);
  assert(::cxx::rgc_count_nodes(id, "G") == 5 + 10);
  assert(::cxx::rgc_count_edges(id, "G") == 2 * 10);
  assert(::cxx::rgc_graph_level(id, "G") == 1);

  // Wierzchołki 5-kliki nie sąsiadują ze sobą i mają stopień 4,
  // a nowe wierzchołki mają stopień 2 i sąsiadują tylko z wierzchołkami 5-kliki.
  for (int x = ::cxx::rgc_first_node(id, "G"); x != -1; x = ::cxx::rgc_next_node(id, "G", x)) {
    int degree = 0;
    for (int y = ::cxx::rgc_first_edge(id, "G", x); y != -1; y = ::cxx::rgc_next_edge(id, "G", x, y)) {
      assert((x < 5) != (y < 5));
      assert(::cxx::rgc_edge_level(id, "G", x, y) == 1);
      degree++;
    }
    assert(degree == (x < 5 ? 4 : 2));
  }

  // Grafy w kolekcji są uporządkowane leksykograficznie według nazw,
  // a grafy pośrednie zostały usunięte.
  vector<string> names;
  for (char const *name = ::cxx::rgc_first_graph(id); name != nullptr; name = ::cxx::rgc_next_graph(id, name))
    names.emplace_back(name);
  assert((names == vector<string>{"G", "K_4", "K_5", "P_3", "test"}));
  assert(::cxx::rgc_next_graph(id, nullptr) == nullptr);

  ::cxx::rgc_delete_collection(id);
  assert(::cxx::rgc_size(id) == -1);
}
