;----------------------------------------------------------------------
; ARGUMENTY FUNKCJI:

; A0 i A1 są wskaźnikami na binarną reprezentację odpowiednio liczb A0 i A1.
; rdi := *A0 
; rsi := *A1

; Ak jest wskaźnikiem na miejsce w pamięci, gdzie należy umieścić 
; binarną reprezentację 64n młodszych bitów liczby Ak.
; rdx := *Ak 

; n zawiera liczbę słów typu uint64_t wskazywanych przez A0, A1 i Ak.
; rcx := qword n 

; int64_t k jest indeksem szukanego wyrazu ciągu arytmetycznego; może być ujemny.
; r8  := qword k
;----------------------------------------------------------------------
; WYNIK DZIAŁANIA:

; int128_t jako para 
; {uint64_t lo;
;  int64_t  hi;}
; czyli:
; rax := lo 
; rdx := hi 
;----------------------------------------------------------------------
; KONTEKST:

; System V AMD64 ABI
; architektura cienkokońcówkowa (little-endian)
; Na start mam dostępne 4 czyste rejestry caller-saved:
;   rax, r9, r10, r11
;----------------------------------------------------------------------

section .text
global arithmetic_sequence

arithmetic_sequence:

.init:
    ; rdx bedzie uzywany przez mul
    mov r9, rdx         ; r9 := *Ak

    ; rcx bedzie uzywany przez loop
    mov r10, rcx        ; r10 := n

    ; Przygotowujemy czysty rejestr.
    ; (Zgodnie z architekturą 32 gorne bity też są zerowane.)
    xor r11d, r11d      ; r11 := 0

    ; Za chwilę użyjemy sbb, ale na początku przeniesienie = 0.
    clc                 ; CF := 0

    ; Znajdziemy różnicę A1 - A0 i zapiszemy ją do Ak:
    ; NB rcx póki co wynosi n.

.substraction_loop:
    ; Architektura cienkokońcówkowa wymusza iterowanie od początku tablicy.
    ; Ze wzgledu na uzycie sbb, nie wolno modyfikować CF. Stąd uzycie lea i loop.

    mov rax, [rsi]      ; rax := A1[i]
    sbb rax, [rdi]      ; rax := A1[i] - A0[i] - przeniesienie
    mov [rdx], rax      ; Ak[i] := A1[i] - A0[i]
    lea rsi, [rsi + 8]  ; inkrementuję wskazniki
    lea rdi, [rdi + 8]
    lea rdx,  [rdx + 8]
    loop .substraction_loop

.clear_after_substracting:
    ; Aby zapisac roznicę liczb 64n-bitowych potrzeba 64n + 1 bitów.
    ; Tym ostatnim (NB najstarszym) bitem jest przeniesienie. Zapisujemy je.
    setl r11b          ; ustawia jesli roznica < 0

    ; przywracam stan wskaznikow
    mov rax, r10        ; rax := n
    shl rax, 3          ; rax := n * 8
    sub rsi, rax        ; rsi := *A1
    sub rdi, rax        ; rdi := *A0
    ; wczesniej ustawiono r9  := *Ak
    ; dodatkowo           r8  := k
    ; oraz                r10 := n

.simplify_signed_to_unsigned:
    ; Znajdę znak iloczynu roznicy * k, zapiszę go
    ; , a roznicę oraz k zamienię na liczy dodatnie.
    test r11b, r11b
    jz .simplify_k

.simplify_difference:
    ; Dla dowolnego slowa w U2 zachodzi:
    ; slowo = negacja (slowo) + 1

    ; Ustawiamy licznik pętli.
    mov rcx, r10        ; rcx := n
    ; Ustawiamy CF aby od razu robić (+1)
    stc                 ; CF  := 1  

.negation_loop:
    not qword [r9]          
    adc qword [r9], 0
    lea r9,  [r9 + 8]   ; Przesuwamy wskaźnik
    loop .negation_loop

    ; Przywracamy stan wskaznika Ak.
    mov rax, r10        ; rax := n
    shl rax, 3          ; rax := n * 8
    sub r9, rax         ; r9 := *Ak

.simplify_k:
    test r8, r8 
    jns .multiply       ; Pomijamy, jeśli k >= 0 
    ; Skoro k jest ujemne, odwracam znak.     
    xor r11b, 1         ; r11 = 0 wtw iloczyn >= 0 
    neg r8              ; k := -k

.multiply:
    mov rcx, r10        ; rcx := n 
    xor esi, esi        ; *A1 juz się nie przyda
    ; Od teraz rsi przechowuje przeniesienie (słowo).
.multiplication_loop:
    mov rax, qword [r9] ; Pobierz bieżące słowo
    mul r8              ; Pomnóż słowo przez k 
    add rax, rsi        ; Dodaj przeniesienie do dolnej części wyniku
    adc rdx, 0          ; Suma może wygenerować jeden bit przeniesienia
    mov [r9], rax       ; Zapisz wynik
    mov rsi, rdx        ; Aktualizuj przeniesienie
    lea r9, [r9 + 8]    ; Przesuń wskaznik
    loop .multiplication_loop

    ; Przywracamy stan wskaznika Ak.
    mov rdx, r10        ; rdx := n
    shl rdx, 3          ; rdx := n * 8
    sub r9, rdx         ; r9 := *Ak

.sign_fix_and_addition:
    test r11, r11
    jz .add             ; Dodatni iloczyn, więc nic nie trzeba ruszać
    ; Ustawiamy licznik pętli.
    mov rcx, r10        ; rcx := n
    ; Ustawiamy CF aby od razu robić (+1)
    stc                 ; CF  := 1  

.negation_loop2:
    not qword [r9]          
    adc qword [r9], 0
    lea r9,  [r9 + 8]
    loop .negation_loop2

    ; W tym momencie rsi trzyma jeszcze moje ostatnie przeniesienie.
    not rsi
    adc rsi, 0
    sub r9, rdx         ; r9 := *Ak

.add:
    ; Ustawiamy licznik pętli.
    mov rcx, r10        ; rcx := n
    ; Sprawdzam znak A0 (ostatnie slowo w A0)
    mov r11, [rdi + r10*8 - 8]
    sar r11, 63         ; r11 := rozszerzenie znaku 
    mov rdx, rsi        ; n+1. slowo iloczynu
    sar rdx, 63         ; Rozszerzam jego znak jako słowo n+2
    ; Na początku dodawania przeniesienie = 0
    clc                 ; CF := 0

.addition_loop:
    mov rax, [rdi]      ; rax := A0[i]
    adc [r9], rax       ; Ak[i] := Ak[i] + A0[i] + przeniesienie 
    lea rdi, [rdi + 8]  ; Inkrementuję wskazniki
    lea r9,  [r9 + 8]
    loop .addition_loop

    ; Propaguję przeniesienie i uwzględniam rozszerzenie znaku A0.
    adc rsi, r11 
    mov rax, rsi        ; 'lo' jest gotowe     
    ; Propaguję przeniesienie i uwzględniam rozszerzenie znaku A0 oraz iloczynu.
    adc rdx, r11        ; 'hi' jest gotowe

    ret
