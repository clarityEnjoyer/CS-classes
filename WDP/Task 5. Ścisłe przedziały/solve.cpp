#include <bits/stdc++.h>
#include <cstddef>
using namespace std;

#define endl '\n'
#define fi first
#define se second

struct monoQ {  // kolejka MINIMÓW
  deque<pair<int, int>>
      container;  // w zadadaniu obsluguję wspolrzedne z zakresu 0 - 10^9
  void push(int y, int id) {
    while (!container.empty() && container.back().fi <= y) container.pop_back();
    container.push_back({y, id});
  }
  void pop(int id) {
    if (container.front().se == id) container.pop_front();
  }
  int get_best() {
    assert(!container.empty());
    return container.front().fi;
  }
};

struct segment {
  int l, r;
  double jakosc;
};

signed main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int n, U;
  cin >> n >> U;
  vector<pair<int, int>> points(n);
  for (int i = 0; i < n; i++) cin >> points[i].fi >> points[i].se;
  monoQ maxQ, minQ;  // kolejka max'ow bedzie stworzona po prostu biorąc liczby przeciwne
  auto diff = [&maxQ, &minQ]() {
    return abs(-maxQ.get_best() - minQ.get_best());
  };

  // I faza : okreslic zbior kolejnych U-scislych max_przedzialow
  vector<segment> przedzialy;
  minQ.push(points[0].se, 0);
  maxQ.push(-points[0].se, 0);
  int l = 0, r = 0;
  // gąsienica. zalożenie: l<=r, trzymany przedzial to [l,r]
  while (r < n) {
    // l nigdy nie przekroczy r, bo na przedziale dlug. 1 roznica jest 0
    if (diff() > U) {
      // to znaczy ze dystans miedzy skrajnymi y jest za duzy i nie uzyskamy
      // przedzialu U-scislego
      minQ.pop(l);
      maxQ.pop(l);
      l++;
      continue;
    }
    // mam przedzial U-scisly i musze go uczynic maxymalnym
    while (r < n - 1 && diff() <= U) {
      r++;
      minQ.push(points[r].se, r);
      maxQ.push(-points[r].se, r);
    }
    if (r == n - 1 && diff() <= U) {
      przedzialy.push_back(
        {l, r, // konce oraz jakosc...
         1ll * (points[r].fi - points[l].fi) * (points[r].fi - points[l].fi) / (double)(r - l + 1)});
      break;
    }
    przedzialy.push_back(
      {l, r - 1, // konce oraz jakosc...
       1ll * (points[r - 1].fi - points[l].fi) * (points[r - 1].fi - points[l].fi) / (double)(r - l)});
  }

  // II faza : utworzyc z przedzialow rozłączne pokrycie. Priorytezujemy:
  // a) największą jakosc
  // b) dla rownej jakosc, polozenie bardziej na lewo

  // zgodnie z warunkami zadania, nie bedzie sytuacji, zeby jeden calkowicie sie zawieral w drugim

  size_t ile = przedzialy.size();
  vector<segment> pokrycie, original;
  pokrycie.reserve(ile);
  original = przedzialy;
  vector<size_t> ids;

  ids.push_back(0);
  pokrycie.push_back(przedzialy[0]);

  for (size_t i = 1; i < ile; i++) {
    if (przedzialy[i].l <= pokrycie.back().r) {
      while (!pokrycie.empty() && przedzialy[i].l <= pokrycie.back().r) {
        // przecinamy się:)
        if (przedzialy[i].jakosc > pokrycie.back().jakosc) {
          if (przedzialy[i].l <= pokrycie.back().l) {
            // zawiera go (moze istniec tylko jeden taki obciety przedzial z
            // podzialu, ktory bedzie zawierany)
            pokrycie.pop_back();
            ids.pop_back();
          } else {
            segment x = pokrycie.back();
            pokrycie.pop_back();
            x.r = przedzialy[i].l - 1; // popraw koniec "poprzedniego"
            pokrycie.push_back(x);
          }
        } else {
          // gorsza jakosc
          przedzialy[i].l = pokrycie.back().r + 1; // popraw swoj początek
        }
      }
    }
    pokrycie.push_back(przedzialy[i]);
    ids.push_back(i);
  }

  // uzyskanie wyniku  
  size_t rozm_pokrycia = ids.size();
  for (size_t i = 0; i < rozm_pokrycia; i++) {
    for (int j = pokrycie[i].l; j <= pokrycie[i].r; j++) {
      cout << original[ids[i]].l + 1 << " " << original[ids[i]].r + 1 << endl;
    }
  }
}
