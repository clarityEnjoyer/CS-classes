//Aliaksei Papkouski
#ifndef _ZBIOR_ARY_H_
#define _ZBIOR_ARY_H_

#include <stdbool.h>

// extern long long Q;

typedef struct ciag{
  long long p, k;
}ciag;

typedef struct CiagVec{
  ciag *data;
  unsigned size;//jako indeks wskazuje pierwszy "element" za vectorem :)
  unsigned capacity;
}CiagVec;

//zbior_ary to de facto nic innego jak vector ciagów!
typedef struct zbior_ary {
  CiagVec ciagi;//ciagi są posortowane najpierw po rosnącym modulo, a potem po rosnących początkach
}zbior_ary;

void FREE(zbior_ary *X);

/* Najlepiej nie modyfikowac nic ponizej. */

// Funkcja daje w wyniku zbior reprezentujacy ciag arytmetyczny o elemencie poczatkowym a, końcowym b i roznicy q>0, tj. {a,a+q,...,b}.
// Mozesz zalozyc, ze we wszystkich zapytaniach w danym tescie wartosc q bedzie taka sama.
// Mozesz zalozyc, ze ta funkcja zostanie wywolana jako pierwsza.
zbior_ary ciag_arytmetyczny(int a, int q, int b);

// Daje w wyniku zbior zlozony z samego elementu a, tj. {a}.
zbior_ary singleton(int a);

// Daje w wyniku zbior reprezentujacy teoriomnogosciowa sume zbiorow A i B.
zbior_ary suma(zbior_ary A, zbior_ary B);

// Daje w wyniku zbior reprezentujacy czesc wspolna zbiorow A i B.
zbior_ary iloczyn(zbior_ary A, zbior_ary B);

// Daje w wyniku zbior A \ B.
zbior_ary roznica(zbior_ary A, zbior_ary B);

// Daje w wyniku true wtw. gdy liczba b nalezy do zbioru A.
bool nalezy(zbior_ary A, int b);

// Wynikiem funkcji jest liczba elementow w zbiorze A.
unsigned moc(zbior_ary A);

// Wynikiem funkcji jest Ary_q(A), czyli minimalna liczba parami rozlacznych ciagow arytmetycznych o roznicy q, ktorych suma jest zbior A.
unsigned ary(zbior_ary A);

#endif
