#include "rstack.h"
#include <asm-generic/errno-base.h>
#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h> 

// ****** DEFINICJE KOLORÓW UŻYWANE W TRIAL DELETION ********
#define COLOR_BLACK  0 // W użyciu lub wyczyszczony
#define COLOR_PURPLE 1 // Podejrzany o bycie odizolowanym cyklem
#define COLOR_GRAY   2 // W trakcie analizy Trial Deletion
#define COLOR_WHITE  3 // Zidentyfikowany śmieć
// **********************************************************

// ********************* POCZĄTEK DEFINICJI STRUKTUR *************************************

// Nasze stosy mają przechowywać liczby lub inne stosy. 
// Implementuje to struktura element.
typedef struct element{
  bool isNum;
  uint64_t number;
  rstack_t* reference;
}element;

// Implementuje dynamicznie powiększaną tablicę 'element'ów.
typedef struct Vector{
  element *data;
  size_t size;
  size_t capacity;
}Vector;

// Jest tożsamy stosowi z treści zadania.
// Użycie wektora umożliwia bardziej efektywną i wygodną implementację.
typedef struct _stack{
  Vector *elements;
}_stack;

// Implementuje referencję na stos.
struct rstack{
  _stack* pointer;
  unsigned int counter;  // licznik wystąpień
  unsigned int lastTime; // znacznik dla przejść dfs
  uint8_t color;         // znacznik dla Trial Deletion
};

// ********************* KONIEC DEFINICJI STRUKTUR *************************************



// Forward deklaracja głównej funkcji sprzątającej
void process_buffer(void);



// ****************** POCZĄTEK FUNKCJI IMPLEMENTUJĄCYCH VECTOR <element> ******************

// Dodaje element do wektora.
bool v_add(Vector* V, element e) {
  if (V -> size >= V -> capacity) {
    size_t new_cap = V -> capacity * 2;
    element* new_data = (element*) realloc(V->data, new_cap * sizeof(element));
    
    if (new_data == nullptr) {
      // Spróbujmy odzyskać nieco pamięci.
      process_buffer();
      new_data = (element*) realloc(V->data, new_cap * sizeof(element));
      // Wciąż niewystarcza nam pamięci.
      if (new_data == nullptr) return false;
    }
    
    V -> capacity = new_cap;
    V -> data = new_data;
  }
  *(V -> data + V -> size++) = e;
  return true;
}

// Tworzy nowy wektor.
Vector* v_constructor() {
  // Tworzymy nowe opakowanie wektora.
  Vector *V = (Vector*) malloc(sizeof(Vector));
  if (V == nullptr) {
    // Spróbujmy odzyskać nieco pamięci.
    process_buffer();
    V = (Vector*) malloc(sizeof(Vector));
    // Wciąż niewystarcza nam pamięci.
    if (V == nullptr) return nullptr;
  }
  
  // Tworzymy tablicę przechowującą elementy wektora.
  V -> data = (element*) malloc(sizeof(element));
  if (V -> data == nullptr) {
    process_buffer();
    V -> data = (element*) malloc(sizeof(element));
    if (V -> data == nullptr) {
      free(V);
      return nullptr;
    }
  }
  
  V -> capacity = 1;
  V -> size     = 0;
  return V;
}

// Usuwa i zwraca ostatni element wektora.
element v_pop(Vector *V){
  assert((V -> size > 0));
  V -> size--;
  return V -> data[V -> size];
  // MOZE REALOKACJA?
}

// Pomocnicza funkcja dla GC i destrukcji iteracyjnej
bool push_ref(Vector* V, rstack_t* ref) {
  element e = {false, 0, ref};
  return v_add(V, e);
}

// Niszczy instancję wektora.
void v_destructor(Vector *V){
  free(V -> data);
  free(V);
}

// ****************** KONIEC FUNKCJI IMPLEMENTUJĄCYCH VECTOR <element> ******************



// Implementuje listę kandydatów na członków odizolowanych cykli.
// Będzie ona zwalniana albo w przypadku braku pamięci programu (np. nieudany malloc), w celu jej odzyskania
// , albo gdy zostanie zgromadzona zbyt duża liczba kandydatów.
static Vector* suspect_buffer = nullptr;
const size_t SUSPECT_BUFFER_SIZE_LIMIT = 1000;

// Szybkie usuwanie z bufora w czasie O(1) (podmiana z ostatnim elementem)
void remove_from_suspects(rstack_t *rs) {
  if (!suspect_buffer) return;
  for (size_t i = 0; i < suspect_buffer->size; i++) {
    if (suspect_buffer->data[i].reference == rs) {
      suspect_buffer->data[i] = suspect_buffer->data[suspect_buffer->size - 1];
      suspect_buffer->size--;
      return;
    }
  }
}

// Ze względu na to, że 'suspect_buffer' jest zwalniany okresowo
// , należy się zabezpieczyć przed wyciekiem na końcu programu.
// Do realizacji tego używam funkcji 'atexit()'
static bool is_atexit_registered = false;

void flush_global_buffer(void) {
  if (suspect_buffer) {
    process_buffer(); // Ostatnie sprzątanie
    v_destructor(suspect_buffer);
    suspect_buffer = nullptr;
  }
}

void init_gc() {
  if (!is_atexit_registered) {
    suspect_buffer = v_constructor();
    atexit(flush_global_buffer);
    is_atexit_registered = true;
  }
}

// Dodaje do listy kandydatów na członków odizolowanych cykli.
void push_to_suspects(rstack_t* rs) {
  init_gc();
  if (suspect_buffer) {
    push_ref(suspect_buffer, rs);
    // Wyzwalacz objętościowy GC
    if (suspect_buffer->size >= SUSPECT_BUFFER_SIZE_LIMIT) {
      process_buffer();
    }
  }
}


// ****************** POCZĄTEK ALGORYTMU TRIAL DELETION ******************
void mark_gray(rstack_t *root) {
  if (root->color != COLOR_PURPLE) return;
  
  Vector *stack = v_constructor();
  if (!stack) return; 
  push_ref(stack, root);

  while (stack->size > 0) {
    rstack_t *curr = v_pop(stack).reference;
    if (curr->color != COLOR_GRAY) {
      curr->color = COLOR_GRAY;
      Vector *children = curr->pointer->elements;
      for (size_t i = 0; i < children->size; i++) {
        if (!children->data[i].isNum) {
          children->data[i].reference->counter--;
          push_ref(stack, children->data[i].reference);
        }
      }
    }
  }
  v_destructor(stack);
}

void scan_black(rstack_t *root) {
  Vector *stack = v_constructor();
  if (!stack) return;
  push_ref(stack, root);

  while (stack->size > 0) {
    rstack_t *curr = v_pop(stack).reference;
    curr->color = COLOR_BLACK;
    Vector *children = curr->pointer->elements;
    for (size_t i = 0; i < children->size; i++) {
      if (!children->data[i].isNum) {
        children->data[i].reference->counter++;
        if (children->data[i].reference->color != COLOR_BLACK) {
          push_ref(stack, children->data[i].reference);
        }
      }
    }
  }
  v_destructor(stack);
}

void scan(rstack_t *root) {
  Vector *stack = v_constructor();
  if (!stack) return;
  push_ref(stack, root);

  while (stack->size > 0) {
    rstack_t *curr = v_pop(stack).reference;
    if (curr->color == COLOR_GRAY) {
      if (curr->counter > 0) {
        scan_black(curr);
      } else {
        curr->color = COLOR_WHITE;
        Vector *children = curr->pointer->elements;
        for (size_t i = 0; i < children->size; i++) {
          if (!children->data[i].isNum) {
            push_ref(stack, children->data[i].reference);
          }
        }
      }
    }
  }
  v_destructor(stack);
}

// Zmienia nazwę i przeznaczenie z collect_white na gather_white
void gather_white(rstack_t *root, Vector *to_free_global) {
  if (root->color != COLOR_WHITE) return;

  Vector *stack = v_constructor();
  if (!stack) return;
  push_ref(stack, root);

  while (stack->size > 0) {
    rstack_t *curr = v_pop(stack).reference;
    
    if (curr->color == COLOR_WHITE) {
      curr->color = COLOR_BLACK; 
      // Zamiast niszczyć, odkładamy bezpiecznie na stos do usunięcia
      push_ref(to_free_global, curr);
      
      Vector *children = curr->pointer->elements;
      for (size_t i = 0; i < children->size; i++) {
        if (!children->data[i].isNum) {
          rstack_t *child = children->data[i].reference;
          // Dodajemy na stos tylko jeśli dziecko nadal jest BIAŁE
          if (child->color == COLOR_WHITE) {
            push_ref(stack, child);
          }
        }
      }
    }
  }
  v_destructor(stack);
}

static bool gc_running = false;

void process_buffer(void) {
  // Zabezpieczenie przed wywołaniem GC wewnątrz GC
  if (gc_running || !suspect_buffer || suspect_buffer->size == 0) return;
  gc_running = true;

  // Faza 1: Mark
  for (size_t i = 0; i < suspect_buffer->size; i++) {
    rstack_t* curr = suspect_buffer->data[i].reference;
    if (curr->color == COLOR_PURPLE) mark_gray(curr);
  }
  
  // Faza 2: Scan
  for (size_t i = 0; i < suspect_buffer->size; i++) {
    rstack_t* curr = suspect_buffer->data[i].reference;
    scan(curr);
  }
  
  // FAZA 3: Zbieranie WSZYSTKICH węzłów ze wszystkich cykli bez zwalniania pamięci
  Vector *to_free_global = v_constructor();
  if (to_free_global) {
    for (size_t i = 0; i < suspect_buffer->size; i++) {
      rstack_t* curr = suspect_buffer->data[i].reference;
      if (curr->color == COLOR_WHITE) {
        gather_white(curr, to_free_global);
      }
    }
    
    // FAZA 4: zwolnienie pamięci
    for (size_t i = 0; i < to_free_global->size; i++) {
      rstack_t *curr = to_free_global->data[i].reference;
      
      v_destructor(curr->pointer->elements);
      free(curr->pointer);
      free(curr);
    }
    v_destructor(to_free_global);
  }
  
  // Bezpiecznie opróżniamy bufor podejrzanych
  suspect_buffer->size = 0; 
  gc_running = false;
}

// ****************** KONIEC ALGORYTMU TRIAL DELETION ******************

// Iteracyjne, kaskadowe usuwanie stosów z licznikiem == 0 omijające logikę GC w celu przyśpieszenia działania
void release_node(rstack_t *root) {
  Vector *stack = v_constructor(); // Jest to nasz stos niszczenia.
  if (!stack) return; // CZY NIE POWINNISMY JAKOS WYWALIC PROGRAMU?
  push_ref(stack, root); // Z założenia mamy zniszczyć root.
  
  // Weźmy dowolny stos, który ma być zniszczony.
  // Wtedy licznik każdego stosu, który był na niego odłożony musi zostać zmniejszony o jeden.
  // Jeśli zaś spadnie do zera, to i on będzie musiał zostać zniszczony.
  while(stack->size > 0) {
    rstack_t *curr = v_pop(stack).reference;
    Vector *children = curr->pointer->elements;
    
    for (size_t i = 0; i < children->size; i++) {
      if (!children->data[i].isNum) {
        rstack_t* child = children->data[i].reference;
        child->counter--;
        
        if (child->counter == 0) {
          // Dziecko zmarło natychmiast - dodajemy do stosu niszczenia.
          push_ref(stack, child);
        } else if (child->color != COLOR_PURPLE) {
          // Dziecko żyje - podejrzewamy cykl.
          child->color = COLOR_PURPLE;
          push_to_suspects(child);
        }
      }
    }
    // Zabezpieczenie przed Dangling Pointer w buforze GC
    if (curr->color == COLOR_PURPLE) {
      remove_from_suspects(curr);
    }

    // Niszczymy
    v_destructor(curr->pointer->elements);
    free(curr->pointer);
    free(curr);
  }
  v_destructor(stack);
}

// Tworzy nowy stos.
rstack_t* rstack_new(){
  _stack* newStackPointer = (_stack*)malloc(sizeof(_stack));
  if (newStackPointer == nullptr){
    process_buffer();
    newStackPointer = (_stack*)malloc(sizeof(_stack));
    if (newStackPointer == nullptr) { errno = ENOMEM; return nullptr; }
  }

  rstack_t* newStackReference = (rstack_t*)malloc(sizeof(rstack_t));
  if (newStackReference == nullptr){
    process_buffer();
    newStackReference = (rstack_t*)malloc(sizeof(rstack_t));
    if (newStackReference == nullptr) { 
      errno = ENOMEM; free(newStackPointer); return nullptr; 
    }
  }

  newStackPointer -> elements = v_constructor();
  if (newStackPointer -> elements == nullptr){
    errno = ENOMEM;
    free(newStackPointer);
    free(newStackReference);
    return nullptr;
  }
  
  newStackReference -> pointer  = newStackPointer;
  newStackReference -> counter  = 1;
  newStackReference -> lastTime = 0;
  newStackReference -> color    = COLOR_BLACK;
  
  init_gc(); // Upewniamy się, że atexit zadziała!
  return newStackReference;
}

void rstack_delete(rstack_t *rs){
  if (rs == nullptr) return;
  rs->counter--;
  
  if (rs->counter == 0) {
    release_node(rs);
  } else if (rs->color != COLOR_PURPLE) { 
    // Mamy kandydata na członka odizolowanego cyklu.
    rs->color = COLOR_PURPLE;
    push_to_suspects(rs);
  }
}

int rstack_push_value(rstack_t *rs, uint64_t value){
  if (rs == nullptr){
    errno = EINVAL;
    return -1;
  }
  element e = {true,value,nullptr}; 
  if (v_add(rs -> pointer->elements, e) == false){
    errno = ENOMEM;
    return -1;
  }
  return 0;
}

int rstack_push_rstack(rstack_t *rs1, rstack_t *rs2){
  if (rs1 == nullptr || rs2 == nullptr){
    errno = EINVAL;
    return -1;
  }
  element e = {false,0,rs2}; 
  if (v_add(rs1 -> pointer->elements, e) == false){
    errno = ENOMEM;
    return -1;
  }
  rs2 -> counter++;
  return 0;
}

void rstack_pop(rstack_t *rs){
  if (rs == nullptr) return;

  Vector *v = rs -> pointer -> elements;
  if (v -> size == 0) return;

  element deletedElem = v_pop(v);

  // Usunęliśmy liczbę - już nie ma nic więcej do roboty.
  if (deletedElem.isNum == true) return;

  rstack_t *deleted = deletedElem.reference;
  deleted->counter--;
  
  if (deleted->counter == 0) {
    release_node(deleted);
  } else if (deleted->color != COLOR_PURPLE) {
    // Mamy kandydata na członka odizolowanego cyklu.
    deleted->color = COLOR_PURPLE;
    push_to_suspects(deleted);
  }
  
}

unsigned int timer = 0;
unsigned int startTime;
bool isStartSet;

// Znajduje rekurencyjnie liczbę, która jest najbliżej wierzchołka stosu.
// W przypadku zacyklenia, po prostu ignoruje cykl i szuka dalej. Używa dfs.
result_t rstack_front(rstack_t *rs){
  result_t result = {false};
  if (rs == nullptr) return result;

  Vector  *v_Container = rs -> pointer -> elements;
  element *v           = v_Container -> data;
  size_t sizeOfV       = v_Container -> size;

  if (sizeOfV == 0) return result;

  bool iSetStart = false;
  if (isStartSet == false){
    startTime  = timer++;
    iSetStart  = true;
    isStartSet = true;
  }

  rs -> lastTime = timer++;

  for (int i = sizeOfV - 1; i >= 0; i--){
    if (v[i].isNum){
      result.flag = true;
      result.value = v[i].number;
      if (iSetStart) isStartSet = false;
      return result;
    }
    
    if (v[i].reference -> lastTime > startTime) continue;
    
    result_t recursiveResult = rstack_front(v[i].reference);

    if (recursiveResult.flag) {
      if (iSetStart) isStartSet = false;
      return recursiveResult;
    }
  }
  if (iSetStart) isStartSet = false;
  return result; 
}

// Jeśli na stosie jest odłożona (rekurencyjnie lub nie) jakaś liczba
// , to w szczególności istnieje liczba najbliższa jego wierzchołka.
bool rstack_empty(rstack_t *rs){
  if (rs == nullptr) return true;
  result_t result = rstack_front(rs);
  return !result.flag;
}

// Tworzy nowy stos, na którym odłożone są kolejno liczby podane w pliku.
// Sprawdza poprawność wejścia. 
// Realizacja poprzez skaner tokenów znak po znaku.
rstack_t* rstack_read(char const *path) {
  // 1. Walidacja argumentu i otwarcie pliku
  if (path == nullptr) {
    errno = EINVAL;
    return nullptr;
  }

  FILE *file = fopen(path, "r");
  // fopen automatycznie ustawia odpowiednie errno (np. ENOENT, EACCES)
  if (file == NULL) return nullptr;

  // 2. Alokacja stosu, na którym będziemy zapisywać liczby z pliku
  rstack_t *rs = rstack_new();
  if (rs == nullptr) {
    // rstack_new ustawiło errno na ENOMEM
    int saved_errno = errno;
    fclose(file);
    errno = saved_errno;
    return nullptr;
  }

  // 3. Inicjalizacja
  // UINT64_MAX ma tylko 20 cyfr, więc 32 znaki na pewno wystarczą
  char buf[32];
  int i = 0;
  int c;
  bool error_occurred = false;

  //  4. Główna pętla parsowania pliku znak po znaku
  while ((c = fgetc(file)) != EOF) {
    if (isspace(c)) {
      if (i > 0) {
        // Zakończyliśmy wczytywać słowo
        buf[i] = '\0';
        
        // Nie powinno być liczb ujemnych.
        if (buf[0] == '-') {
          error_occurred = true;
          errno = EINVAL;
          break;
        }

        char *endptr;
        // Resetujemy errno przed wywołaniem funkcji konwertującej
        // , bo zwraca ona status poprzez errno.
        errno = 0; 
        uint64_t value = strtoull(buf, &endptr, 10);

        // Walidacja: czy całe słowo było liczbą i czy nie ma przepełnienia (ERANGE)
        if (*endptr != '\0' || errno == ERANGE) {
          error_occurred = true;
          errno = EINVAL;
          break;
        }

        // Próba odłożenia na stos
        if (rstack_push_value(rs, value) == -1) {
          // errno jest już ustawione przez rstack_push_value
          error_occurred = true;
          break;
        }

        // Reset bufora dla kolejnej liczby
        i = 0; 
      }
    } else {
      // Zbyt długa liczba
      if (i >= 31) {
        error_occurred = true;
        errno = EINVAL;
        break;
      }
      // Poprawny znak
      buf[i++] = (char)c;
    }
  }

  // 5. Przetworzenie ewentualnego ostatniego słowa (jeśli plik nie kończy się spacją/nową linią)
  if (!error_occurred && i > 0) {
    buf[i] = '\0';
    if (buf[0] == '-') {
      error_occurred = true;
      errno = EINVAL;
    } else {
      char *endptr;
      errno = 0;
      uint64_t value = strtoull(buf, &endptr, 10);

      if (*endptr != '\0' || errno == ERANGE) {
        error_occurred = true;
        errno = EINVAL;
      } else {
        if (rstack_push_value(rs, value) != 0) {
          error_occurred = true;
        }
      }
    }
  }

  // 6. Sprawdzenie błędów odczytu z samego nośnika
  if (!error_occurred && ferror(file)) {
    error_occurred = true;
    errno = EIO; // Błąd wejścia/wyjścia (I/O)
  }

  // 7. Protokół awaryjny (Cleanup)
  if (error_occurred) {
    // rstack_delete lub fclose mogą wewnątrz wywołać kod, który nadpisze errno!
    int saved_errno = errno;
    rstack_delete(rs);
    fclose(file);
    errno = saved_errno;
    return nullptr;
  }

  // 8. Sukces – zamykamy plik i upewniamy się, że zamknięcie się powiodło
  if (fclose(file) != 0) {
    int saved_errno = errno;
    rstack_delete(rs);
    errno = saved_errno;
    return nullptr;
  }

  return rs;
}

// **************************** WYPISYWANIE ******************************************

// Pomocnicza funkcja rekurencyjna do zapisywania i precyzyjnego wykrywania cykli
// Zwraca:
//  0 -> Sukces (zapisano wszystko w tej gałęzi)
//  1 -> Wykryto cykl (należy natychmiast przerwać zapisywanie na wszystkich szczeblach)
// -1 -> Błąd braku pamięci (ENOMEM)
// -2 -> Błąd zapisu do pliku (EIO)
static int write_dfs(FILE *file, rstack_t *rs, Vector *visited) {
  // 1. Detekcja Cyklu (Sprawdzamy ścieżkę aktywną)
  for (size_t i = 0; i < visited->size; i++) {
    if (visited->data[i].reference == rs) {
      return 1; // Cykl wykryty! Zwracamy sygnał do natychmiastowego przerwania
    }
  }

  // 2. Oznaczamy stos jako "w trakcie odwiedzania"
  element e = {false, 0, rs};
  if (!v_add(visited, e)) {
    return -1; // ENOMEM
  }

  Vector *v = rs->pointer->elements;
  
  // 3. Iteracja od spodu stosu do wierzchołka (zgodnie z kolejnością odkładania)
  for (size_t i = 0; i < v->size; i++) {
    if (v->data[i].isNum) {
      // Zapisujemy liczbę i znak nowej linii. PRIu64 gwarantuje brak zer wiodących dla uint64_t.
      if (fprintf(file, "%" PRIu64 "\n", v->data[i].number) < 0) {
        v_pop(visited); // Sprzątamy wektor przed ucieczką
        return -2; // Błąd wejścia/wyjścia (I/O)
      }
    } else {
      // Wchodzimy rekurencyjnie w pod-stos
      int res = write_dfs(file, v->data[i].reference, visited);
      
      // Jeśli otrzymaliśmy jakikolwiek kod inny niż sukces (np. cykl lub błąd IO)
      if (res != 0) {
        v_pop(visited); // Sprzątamy i natychmiast kaskadowo przerywamy pętlę
        return res; 
      }
    }
  }

  // 4. Skończyliśmy przetwarzać ten stos - zdejmujemy go ze ścieżki aktywnej
  v_pop(visited);
  return 0;
}

int rstack_write(char const *path, rstack_t *rs) {
  if (path == nullptr || rs == nullptr) {
    errno = EINVAL;
    return -1;
  }

  FILE *file = fopen(path, "w");
  if (file == NULL) {
    // fopen automatycznie ustawia odpowiednie errno (EACCES, ENOENT itp.)
    return -1;
  }

  // Alokujemy wektor przechowujący naszą ścieżkę odwiedzin
  Vector *visited = v_constructor();
  if (visited == nullptr) {
    int saved_errno = ENOMEM;
    fclose(file);
    errno = saved_errno;
    return -1;
  }

  // Uruchamiamy silnik zapisujący
  int res = write_dfs(file, rs, visited);

  v_destructor(visited);

  // Analiza wyników zwracanych przez silnik
  if (res == -1) {
    int saved_errno = ENOMEM;
    fclose(file);
    errno = saved_errno;
    return -1;
  } else if (res == -2) {
    int saved_errno = EIO; 
    fclose(file);
    errno = saved_errno;
    return -1;
  }
  // Uwaga: Jeśli res == 1 (Wykryto cykl), polecenie nakazuje "przerwać zapisywanie".
  // Funkcja zadziałała prawidłowo zgodnie z obroną przed cyklami, więc traktujemy to
  // jako cichy sukces w wykonaniu polecenia (przerywa, ale nie wywala błędu API).
  // Podobnie gdy res == 0 (całkowity sukces).

  if (fclose(file) != 0) {
    return -1;
  }

  return 0; // Sukces!
}
