#include <bits/stdc++.h>

#include "worki.h"

using namespace std;

//forward_list zamiast vectorow, zeby nie ruszac adresow elementow w pamieci i miec rzeczywisty czas staly
struct _memory {
  forward_list < przedmiot * > p;
  forward_list < worek * > w;
  forward_list < worek ** > handles;
}
_MEMORY;

worek * BIURKO = nullptr;
worek ** BIURKO_PTR = nullptr;

int licznik_workow = -1;

void init_biurko() {
  if (licznik_workow < 0) {
    //stworzenie biurka
    BIURKO = new worek;
    BIURKO -> nr = licznik_workow++; // -1
    BIURKO -> ile = 0;
    BIURKO -> parent = nullptr;
    _MEMORY.w.push_front(BIURKO);

    // uchwyt na biurko
    BIURKO_PTR = new worek * ;
    * BIURKO_PTR = BIURKO;
    _MEMORY.handles.push_front(BIURKO_PTR);
  }
}
// ---------------funkcje------------------

przedmiot * nowy_przedmiot() {
  init_biurko();
  przedmiot * p = new przedmiot;
  p -> parent = BIURKO_PTR;
  ( ** BIURKO_PTR).ile++; //kladziemy na biurku
  _MEMORY.p.push_front(p);
  return p;
}

worek * nowy_worek() {
  init_biurko();
  worek * w = new worek;
  w -> nr = licznik_workow++;
  w -> ile = 0;
  w -> parent = BIURKO_PTR;
  //tworzymy uchwyt na worek
  worek ** handle = new worek * ;
  * handle = w;
  w -> self_ptr = handle;

  _MEMORY.w.push_front(w);
  _MEMORY.handles.push_front(handle);
  return w;
}

void wloz(przedmiot * co, worek * gdzie) {
  co -> parent = gdzie -> self_ptr;
  gdzie -> ile++;
}

void wloz(worek * co, worek * gdzie) {
  co -> parent = gdzie -> self_ptr;
  gdzie -> ile += co -> ile;
}

void wyjmij(przedmiot * p) {
  ( ** (p -> parent)).ile--;
  p -> parent = BIURKO_PTR;
}

void wyjmij(worek * w) {
  ( ** (w -> parent)).ile -= w -> ile;
  w -> parent = BIURKO_PTR;
}

int w_ktorym_worku(przedmiot * p) {
  if (!p -> parent || ! * (p -> parent)) return -1;
  return ( ** (p -> parent)).nr;
}

int w_ktorym_worku(worek * w) {
  if (!w -> parent || ! * (w -> parent)) return -1;
  return ( ** (w -> parent)).nr;
}

int ile_przedmiotow(worek * w) {
  return w -> ile;
}

// Funkcja może być wywołana tylko wtedy, gdy 
// worek w znajduje się bezpośrednio na biurku. 
// Następuje wielka zamiana: cała zawartość worka w ląduje na biurku,
// a wszystko, co poza workiem w znajdowało się bezpośrednio na biurku,
// ląduje wewnątrz worka w. 
// Podczas zamiany nie rozpakowujemy zawartości innych worków niż worek w.

void na_odwrot(worek * w) {
  //zamieniam odpowiednio ilosci przedmiotow. w bedzie mialo wszystkie, a biurko dopelnienie starego w 
  int total = ( * BIURKO_PTR) -> ile;
  int bag_items = ( * (w -> self_ptr)) -> ile;

  // 1. worek w dostanie to co było na biurku (Total - Bag)
  ( * (w -> self_ptr)) -> ile = total - bag_items;

  // 2. Biurko ma wszystko
  ( * BIURKO_PTR) -> ile = total;

  //SWAP 1: dzieki temu wszyscy ktorzy mieli wskaznik na biurko
  // beda mieli teraz wskaznik na worek w, i odwrotnie
  swap( * BIURKO_PTR, * w -> self_ptr);
  // SWAP 2: podmienia uchwyty - przekierowanie przyszlych operacji
  swap(BIURKO_PTR, w -> self_ptr);
  // w ma byc na biurku
  w -> parent = BIURKO_PTR;
}

// Kończy i zwalnia pamięć
void gotowe() {
  for (auto h: _MEMORY.handles) delete h;
  for (auto p: _MEMORY.p) delete p;
  for (auto w: _MEMORY.w) delete w;
  _MEMORY.handles.clear();
  _MEMORY.p.clear();
  _MEMORY.w.clear();
}
