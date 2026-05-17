#ifndef _RSTACK_H
#define _RSTACK_H

#include <stddef.h>
#include <inttypes.h>

typedef struct rstack rstack_t;

typedef struct {
  bool     flag;  // To pole mówi, czy pole value zawiera wynik.
  uint64_t value; // W tym polu jest właściwy wynik.
} result_t;

rstack_t* rstack_new();

// Jeśli wartością rs jest nullptr, funkcja nic nie robi. Po skasowaniu stosu nie należy używać wskaźnika rs.
void      rstack_delete(rstack_t *rs);

// 0 – jeśli operacja zakończyła się sukcesem;
// -1 – jeśli wskaźnik rs ma wartość nullptr lub wystąpił błąd przydzielania pamięci; 
// funkcja ustawia wtedy errno odpowiednio na EINVAL lub ENOMEM
int       rstack_push_value(rstack_t *rs, uint64_t value);

// 0 – jeśli operacja zakończyła się sukcesem;
// -1 – jeśli wskaźnik rs1 lub rs2 ma wartość nullptr lub wystąpił błąd przydzielania pamięci;
//  funkcja ustawia wtedy errno odpowiednio na EINVAL lub ENOMEM
int       rstack_push_rstack(rstack_t *rs1, rstack_t *rs2);

// Funkcja zdejmująca nierekurencyjnie wierzchołek stosu.
// Jeśli wartością rs jest nullptr lub stos jest pusty, funkcja nic nie robi
void      rstack_pop(rstack_t *rs);

// Funkcja sprawdzająca rekurencyjnie, czy stos zawiera liczbę
// true – jeśli wskaźnik rs ma wartość nullptr lub stos nie zawiera liczby;
// false – jeśli stos zawiera liczbę
bool      rstack_empty(rstack_t *rs);

// Funkcja znajdująca rekurencyjnie liczbę, która jest najbliżej wierzchołka stosu
// flag == true oznacza, że pole value zawiera znalezioną liczbę;
// flag == false oznacza, że wskaźnik rs ma wartość nullptr, stos jest pusty lub nie ma takiej liczby.
result_t  rstack_front(rstack_t *rs);

// Funkcja tworząca nowy stos, na którym odłożone są kolejno liczby podane w pliku
// nullptr – jeśli wskaźnik path ma wartość nullptr lub wystąpił błąd; 
// funkcja ustawia wtedy odpowiednio errno; dobranie wartości errno jest częścią zadania.

// Liczby w pliku podane są w zapisie przy podstawie 10. Liczby w pliku oddzielone są białymi znakami.
// Między dwoma liczbami może być więcej niż jeden biały znak. 
// Na początku i końcu pliku może być dowolna liczba białych znaków. 
// Funkcja dokładnie sprawdza poprawność zawartości pliku.
rstack_t* rstack_read(char const *path);

// Jeśli przy zapisywaniu zostanie wykryty cykl, zapisywanie zostaje przerwane.
int       rstack_write(char const *path, rstack_t *rs);

#endif
