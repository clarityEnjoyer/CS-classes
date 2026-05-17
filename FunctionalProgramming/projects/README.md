# Combinator Reducer

Prosty interpreter uproszczonego języka funkcyjnego opartego na logice kombinatorycznej, napisany w Haskellu. Program wczytuje plik z definicjami kombinatorów, sprawdza ich poprawność strukturalną, a następnie redukuje wyrażenie `main` krok po kroku, stosując strategię redukcji normalnej (najpierw lewe, skrajnie zewnętrzne podwyrażenia).

## Główne cechy projektu

* Własny silnik redukcji krokowej wypisujący kolejne etapy ewaluacji.
* Parsowanie kodu do drzewa składniowego (AST) przy użyciu biblioteki haskell-src.
* Walidacja bezpieczeństwa: wykrywanie powielonych definicji kombinatorów oraz sprawdzanie unikalności parametrów formalnych.
* Bezpieczne środowisko podstawień (rozwiązany problem przechwytywania zmiennych).
* Limit kroków ewaluacji chroniący przed nieskończonym zapętleniem programu.

## Wymagania

* GHC (testowane na wersjach od 9.0.2 do 9.8.2)
* Cabal 3.4 lub nowszy

## Uruchamianie

Projekt obsługiwany jest przez system Cabal. Aby go zbudować i uruchomić, użyj terminala w głównym folderze:

1. Budowa projektu:
   cabal build

2. Uruchomienie programu dla wybranego pliku wejściowego:
   cabal run exe:zadanie2 -- nazwa_pliku.in

3. Wyświetlenie instrukcji (help):
   cabal run exe:zadanie2 -- --help

### Side note
Ten interpreter jest aktualnie rozwijany i do 01.06.2026 powinna powstać dużo bardziej zaawansowana wersja .3 obsługująca konstruktory typów danych.
