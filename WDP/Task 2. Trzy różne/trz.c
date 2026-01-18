#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define max(a, b)(a < b ? b : a)
#define min(a, b)(a < b ? a : b)

typedef struct motel {
    int typ, dist;
}
motel;

signed main() {

    int n;
    if (scanf("%d", & n)) {};
    motel * motele = (motel * ) malloc((unsigned) n * sizeof(motel));
    //dane na wejściu są posortowane wg dist (zgodnie z treścią zadania).

    int * typy = (int * ) malloc(3 * sizeof(int)), id_typy = 0; // do sprawdzenia czy są przynajmniej 3 typy moteli
    for (int i = 0; i < n; i++) {
        if (scanf("%d %d", & motele[i].typ, & motele[i].dist)) {};

        if (id_typy > 2) continue;
        int good = 1;
        for (int j = 0; j < id_typy; j++) {
            if (typy[j] == motele[i].typ) {
                good = 0;
                break;
            }
        }
        if (good) typy[id_typy++] = motele[i].typ;
    }

    if (id_typy <= 2) { //to znaczy, że nie ma trzech różnych typów moteli - nie ma co liczyc
        printf("%s\n", "0 0");
        free(motele);
        free(typy);
        return 0;
    }
    int najdalsza = 0;

    // Znajdź do 3 kandydatów z lewej (pierwsze unikalne typy)
    int cand_L[3] = {-1,-1,-1};
    int cnt_L = 0;
    for (int k = 0; k < n && cnt_L < 3; k++) {
        int known = 0;
        for (int j = 0; j < cnt_L; j++)
            if (motele[cand_L[j]].typ == motele[k].typ) known = 1;
        if (!known) cand_L[cnt_L++] = k;
    }

    // Znajdź do 3 kandydatów z prawej (ostatnie unikalne typy)
    int cand_R[3] = {-1,-1,-1};
    int cnt_R = 0;
    for (int k = n - 1; k >= 0 && cnt_R < 3; k--) {
        int known = 0;
        for (int j = 0; j < cnt_R; j++)
            if (motele[cand_R[j]].typ == motele[k].typ) known = 1;
        if (!known) cand_R[cnt_R++] = k;
    }

    // Główna pętla iterująca środkowy element B
    for (int i = 1; i < n - 1; i++) {
        // Sprawdź kombinacje z kandydatami z lewej (A)
        for (int l = 0; l < cnt_L; l++) {
            int idxA = cand_L[l];
            if (idxA >= i) continue; // A musi być przed B
            if (motele[idxA].typ == motele[i].typ) continue; // Różne typy A i B

            // Sprawdź kombinacje z kandydatami z prawej (C)
            for (int r = 0; r < cnt_R; r++) {
                int idxC = cand_R[r];
                if (idxC <= i) continue; // C musi być za B
                if (motele[idxC].typ == motele[i].typ) continue; // Różne typy C i B
                if (motele[idxA].typ == motele[idxC].typ) continue; // Różne typy A i C

                // Mamy poprawną trójkę, liczymy
                int d1 = motele[i].dist - motele[idxA].dist;
                int d2 = motele[idxC].dist - motele[i].dist;
                int val = min(d1, d2);
                if (val > najdalsza) najdalsza = val;
            }
        }
    }

    int pierwszy = 0, drugi, trzeci = n - 1;
    /*teraz znajdziemy najmnieszą 
    na zasadzie AAAA BBBBB CCCC
    pierwszy, drugi i trzeci będą w kolejnosci wskazywac motele o roznych typach
    pierwszy - ostatnie A
    trzeci - pierwsze C
    drugi przeiteruje przez kazde B*/
    pierwszy = 0;
    drugi = 1;
    while (motele[pierwszy].typ == motele[drugi].typ) drugi++;
    trzeci = drugi + 1;
    while (motele[trzeci].typ == motele[drugi].typ) trzeci++;
    while (motele[pierwszy].typ == motele[pierwszy + 1].typ) pierwszy++;

    int najblizsza = INT_MAX;
    while (trzeci < n) {
        while (drugi < trzeci) {
            if (motele[trzeci].typ != motele[pierwszy].typ) najblizsza = min(najblizsza, max(motele[drugi].dist - motele[pierwszy].dist, motele[trzeci].dist - motele[drugi].dist));
            drugi++;
        }
        //w tym momencie drugi juz wskazuje na to samo co trzeci, czyli muszę przesunąć pierwszy i trzeci
        pierwszy = drugi - 1;
        while (trzeci < n && motele[trzeci].typ == motele[drugi].typ) trzeci++;
    }

    printf("%d %d\n", najblizsza, najdalsza);

    free(typy);
    free(motele);
}
