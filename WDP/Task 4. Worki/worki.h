#ifndef WORKI_H
#define WORKI_H

struct worek {
  int nr, ile; //numer worka i liczba przedmiotow w srodku (bezposrednio lub posrednio)
  worek ** parent, ** self_ptr;
  //'parent' wskazuje miejsce w pamieci, 
  // w ktorym znajduje sie wskaznik na worek w ktorym sie znajduje dany obiekt
  //-------------
  //stąd '*parent' to wskaznik na worek - aktualny rodzic danego obiektu
  //self_ptr to miejsce w pamieci, w ktorym znajduje sie wskaznik na ten worek
};

struct przedmiot {
  worek ** parent;
};

// Nowy przedmiot na biurku
przedmiot * nowy_przedmiot();

// Nowy worek na biurku; otrzymuje kolejny numer, począwszy od 0.
worek * nowy_worek();

// Wkłada przedmiot co do worka gdzie.
// Założenie: co i gdzie leżą na biurku.
void wloz(przedmiot * co, worek * gdzie);

// Wkłada worek co do worka gdzie.
// Założenie: co i gdzie leżą na biurku.
void wloz(worek * co, worek * gdzie);

// Wyjmuje przedmiot p z worka i kładzie na biurku.
// Założenie: Przedmiot p był w worku leżącym na biurku.
void wyjmij(przedmiot * p);

// Wyjmuje worek w z worka i kładzie na biurku.
// Założenie: Worek w był w worku leżącym na biurku.
void wyjmij(worek * w);

// Wynik: numer worka, w którym znajduje się przedmiot p (-1 jeśli na biurku).
int w_ktorym_worku(przedmiot * p);

// Wynik: numer worka, w którym znajduje się worek w (-1 jeśli na biurku).
int w_ktorym_worku(worek * w);

// Wynik: liczba przedmiotów zawartych (bezpośrednio i pośrednio) w worku w
int ile_przedmiotow(worek * w);

// Cała zawartość worka w ląduje na biurku, a wszystko, co poza workiem w
// znajdowało się bezpośrednio na biurku, ląduje wewnątrz worka w.
void na_odwrot(worek * w);

// Kończy i zwalnia pamięć
void gotowe();

#endif
