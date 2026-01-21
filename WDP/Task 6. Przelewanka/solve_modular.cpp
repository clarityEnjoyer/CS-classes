#include <bits/stdc++.h>
#include <cstddef>
#include <unordered_set>
using namespace std;

using Cups = vector<int>;


struct VectorHash {
  // Source - https://stackoverflow.com/a/72073933
  // Posted by see
  // Retrieved 2026-01-21, License - CC BY-SA 4.0
  size_t operator()(const vector<int>& vec) const {
    size_t seed = vec.size();
    for (auto& x : vec) {
      uint32_t v = (uint32_t)x;
      v = ((v >> 16) ^ v) * 0x45d9f3b;
      v = ((v >> 16) ^ v) * 0x45d9f3b;
      v = (v >> 16) ^ v;
      seed ^= v + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
  }
};


// Sprawdza szybkie warunki brzegowe (czy cel jest już osiągnięty lub niemożliwy
// w trywialny sposób)
bool CheckTrivialCases(int n, const Cups& x, const Cups& y) {
  // warunki STOP
  int ready = 0, cnt = 0;
  for (int i = 0; i < n; i++) {
    if (y[i] == 0)
      ready++;
    else if (y[i] == x[i])
      cnt++;
  }


  if (cnt + ready == n) {
    // wszystkie byly albo 0 albo full, czyli moge od razu jako wynik dac liczbę
    // kubków z y==x
    cout << cnt;
    return true;  // Zakończ działanie (sukces)
  }


  if (ready == 0 && cnt == 0) {
    // nie ma ani jednej "zapasowej"
    cout << -1;
    return true;  // Zakończ działanie
  }

  return false;  // Kontynuuj
}

bool CheckGcdConstraints(int n, const Cups& x, const Cups& y) {
  int common_divisor = x[0];
  for (int i = 0; i < n; i++) common_divisor = gcd(common_divisor, x[i]);

  for (int i = 0; i < n; i++) {
    if (y[i] % common_divisor > 0) {
      cout << -1;
      return false;
    }
  }
  return true;  // Warunek spełniony
}

void SolveBfs(int n, const Cups& x, const Cups& y) {
  /*
  stan:=

  akutalny zbior wysokosci wody w szklankach

  ruch: przelej/wypełnij/opróżnij

  memo: czy odwiedzilem dany stan?
  */
  unordered_set<Cups, VectorHash> visited;
  queue<Cups> q1, q2;
  int moves = 0;

  // Funkcja pomocnicza do sprawdzania stanów
  auto TryVisit = [&](const Cups& move) -> bool {
    // visited.insert zwraca {iterator, bool inserted}
    // jezeli inserted jest true, to znaczy ze wczesniej nie odwiedzilismy
    if (visited.insert(move).second) {
      if (move == y) {   // to jest poszukiwany stan
        cout << moves;
        exit(0);  // wynik znaleziony, konczymy
      }
      return true;
    }
    return false;
  };

  Cups start(n, 0);  // poczatkowe wypelnienie wynosi 0
  visited.insert(start);
  q1.push(start);

  // bfs po kolejnych warstwach stanow
  while (!q1.empty()) {
    moves++;  // oznacza numer kolejnej warstwy bfs

    while (!q1.empty()) {
      Cups cups = q1.front();
      q1.pop();
      Cups move = cups;

      for (int i = 0; i < n; i++) {
        int water_level = move[i];

        // WYLEJ
        if (water_level > 0) {
          move[i] = 0;
          if (TryVisit(move)) {
            q2.push(move);
          }
          move[i] = water_level;  // cofnij zmianę
        }

        // WYPELNIJ
        if (water_level < x[i]) {
          move[i] = x[i];
          if (TryVisit(move)) {
            q2.push(move);
          }
          move[i] = water_level;
        }

        // Operacje przelewania między kubkami
        for (int j = i + 1; j < n; j++) {
          // PRZELEJ. i<j
          // najpierw z i do j
          int delta = min(move[i], x[j] - move[j]);
          // nie moge przelac wiecej niz mój akt. poziom wody
          move[i] -= delta;
          move[j] += delta;
          if (TryVisit(move)) {
            q2.push(move);
          }
          // cofamy zmiany
          move[i] += delta;
          move[j] -= delta;

          //-----
          // teraz z j do i
          delta = min(move[j], x[i] - move[i]);
          move[i] += delta;
          move[j] -= delta;
          if (TryVisit(move)) {
            q2.push(move);
          }
          move[i] -= delta;
          move[j] += delta;
        }
      }
    }
    swap(q1, q2);
  }

  cout << -1;
}

signed main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int n;
  cin >> n;
  Cups x(n), y(n);
  for (int i = 0; i < n; i++) cin >> x[i] >> y[i];

  // 1. Sprawdzenie przypadków trywialnych (rozwiązane od razu lub niemożliwe)
  if (CheckTrivialCases(n, x, y) || !CheckGcdConstraints(n, x, y)) 
    return 0;

  // 2. Uruchomienie właściwego algorytmu
  SolveBfs(n, x, y);

  return 0;
}
