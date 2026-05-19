# Symulator Ośrodka Narciarskiego

#### *ZAAWANSOWANA SYMULACJA ZDARZEŃ DYSKRETNYCH (DES)*
-------
## Architektura i Główne Założenia

Projekt został zaprojektowany z naciskiem na czystość obiektową (OOP), niezawodność stanu (State Integrity) oraz wysoką wydajność przetwarzania czasu.

* **Silnik DES (Discrete Event Simulation):** Pętla główna nie opiera się na tykającym zegarze (ticks), lecz na własnej implementacji kopca binarnego (`KopiecWydarzen`), pełniącego rolę kolejki priorytetowej dla zdarzeń. Rozwiązuje to problem pustych cykli CPU i gwarantuje chronologiczne przetwarzanie łańcuchów przyczynowo-skutkowych.
  
* **Smart Sleep (Zarządzanie zasobami):** Wyciągi implementują mechanikę inteligentnego usypiania. Puste krzesełka nie zalewają kolejki zdarzeń bezużytecznymi obiektami, co drastycznie optymalizuje zużycie pamięci (zapobieganie *Event Queue Flooding*).
  
* **Decyzyjność Agentów:** Każdy narciarz (`Sportowiec`) autonomicznie ewaluuje ścieżki w grafie, wykorzystując zdefiniowane funkcje kosztu (dopasowanie poziomu, stan trasy, zużycie) oraz algorytm losowy (współczynnik spontaniczności $\epsilon$). Porównywanie wag odbywa się z uwzględnieniem bezpiecznego marginesu błędu (`EPSILON`) dla obliczeń zmiennoprzecinkowych.
  
* **Polimorficzna Struktura Grafu:** Węzły komunikują się z krawędziami abstrakcyjnymi (`Krawedz`). Logika domenowa specyficzna dla Tras i Wyciągów jest szczelnie ukryta w podklasach.
  
* **Zero-Dependency:** Zgodnie ze specyfikacją akademicką, projekt został zrealizowany bez użycia zaawansowanych struktur danych ze standardowej biblioteki `java.util` (poza narzędziem wejścia `Scanner`). Wszystkie kontenery (np. wektory, bufor cykliczny, kopiec) zostały zaimplementowane od zera.


## Format Wejścia / Wyjścia

* Program czyta zestandaryzowane dane ze standardowego wejścia (spójne bloki Węzłów, Tras, Wyciągów oraz Grup Sportowców). 
* Silnik symuluje jeden dzień działania ośrodka (od godziny 09:00:00 do godziny 15:00:00 horyzontu decyzyjnego), dbając o spójność trwających przejazdów aż do zamknięcia (16:00:00).

* Wynikiem działania programu są logi śledzonych sportowców oraz globalne statystyki eksploatacji infrastruktury.


---
_Autor: Aleks Popkowski_
