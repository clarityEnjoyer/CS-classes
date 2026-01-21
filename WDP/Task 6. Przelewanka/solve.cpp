#include <bits/stdc++.h>
#include <cstddef>
#include <unordered_set>
using namespace std;

using Cups = vector<int>;

struct VectorHash{
  // Source - https://stackoverflow.com/a/72073933
  // Posted by see
  // Retrieved 2026-01-21, License - CC BY-SA 4.0
  size_t operator()(const vector<int>& vec) const {
        size_t seed = vec.size();
        for(auto &x : vec) {
            // Cast to unsigned for bitwise magic to work safely
            uint32_t v = (uint32_t)x; 
            v = ((v >> 16) ^ v) * 0x45d9f3b;
            v = ((v >> 16) ^ v) * 0x45d9f3b;
            v = (v >> 16) ^ v;
            seed ^= v + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};


signed main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int n; cin>>n;
  vector<int>x(n),y(n);
  for (int i=0; i<n; i++)cin>>x[i]>>y[i];
  //warunki STOP
  int ready=0, cnt=0;
  for (int i=0; i<n; i++){
    if (y[i]==0)ready++;
    else if (y[i]==x[i])cnt++;
  }
  if (cnt+ready==n){
    //wszystkie byly albo 0 albo full, czyli moge od razu jako wynik dac liczbę kubków z y==x
    cout<<cnt;
    return 0;
  }

  if (ready==0&&cnt==0){
    //nie ma ani jednej "zapasowej"
    cout<<-1;
    return 0;
  }
  int GCD = x[0];
  for (int i=0; i<n; i++)GCD=gcd(GCD,x[i]);
  for (int i=0; i<n; i++){
    assert(GCD>0);
    if (y[i]%GCD>0){
      cout<<-1;
      return 0;
    }
  }

  /*
  stan:=

  akutalny zbior wysokosci wody w szklankach

  ruch: przelej full empty

  memo: czy odwiedzilem dany stan?
  */
  unordered_set<Cups,VectorHash>visited;
  queue<Cups>q1,q2;
  int moves=0;

  auto tryVisit = [&visited, &y, &moves](const Cups &move) -> bool {
    // visited.insert returns {iterator, bool inserted}
    // If inserted is true, it means it wasn't there before
    if (visited.insert(move).second) { 
        if (move == y) { //to jest poszukiwany stan
            cout << moves;
            exit(0);// wynik znaleziony, konczymy
        }
        return true;
    }
    return false;
  };

  Cups start(n,0);//poczatkowe wypelnienie wynosi 0
  visited.insert(start);
  q1.push(start);

  //bfs po kolejnych warstwach stanow
  while(!q1.empty()){
    moves++;//oznacza numer kolejnej warstwy bfs
    
    while(!q1.empty()){
      Cups cups = q1.front();
      q1.pop();
      Cups move = cups;
      for (int i = 0; i < n; i++){
        int water_level = move[i];

        //WYLEJ
        if (water_level>0){
          move[i]=0;
          if (tryVisit(move)){
            q2.push(move);
          }
          move[i]=water_level;//cofnij zmianę
        }

        //WYPELNIJ
        if (water_level<x[i]){
          move[i]=x[i];
          if (tryVisit(move)){
            q2.push(move);
          }
          move[i]=water_level;//cofnij zmianę
        }

        for (int j = i+1; j<n; j++){
          // PRZELEJ. i<j
          // najpierw z i do j
          int delta = min(move[i], x[j]-move[j]);
          //nie moge przelac wiecej niz mój akt. poziom wody
          move[i] -= delta;
          move[j] += delta;
          if (tryVisit(move)){
            q2.push(move);
          }
          //cofamy zmiany
          move[i] += delta;
          move[j] -= delta;

          //-----
          //teraz z j do i
          delta = min(move[j], x[i]-move[i]);
          move[i] += delta;
          move[j] -= delta;
          if (tryVisit(move)){
            q2.push(move);
          }
          move[i] -= delta;
          move[j] += delta;
        }
      }
    }
    swap(q1,q2);
  }

cout<<-1;

}
