//będziemy rekurencyjnie sprawdzać ile warstw jest w punkcie p w origami o indeksie k, odbijając punkt według prostych zgięć
//[moje pierwsze rozwiązanie, odbijało figury explicite i tworzyło ich zbiory, ale było zbyt nieprzyjemne i nieefektywne]

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define max(a,b) (a<b ? b : a)
#define min(a,b) (a<b ? a : b)

const double epsilon = 1e-6;

 typedef struct{
    double x,y;
 }Point;

 void read_point(Point *p){
    if (scanf("%lf%lf", &p->x, &p->y)) {}
 }

typedef struct{//origami to figura bazowa albo zgiete wzdluz prostej origami :)
    int type;//0 rectangle, 1 circle, 2 LINE 
    int link;//indeks origami, które zostało zgięte (jeśli istnieje)
    Point p1, p2; // for rectangle: p1 = bottom-left, p2 = top-right; for circle: p1 = center, p2.x = r
}Origami;

void reflect_point(Point *p, Point *l1, Point *l2, Point *res){//odbija punkt p względem prostej przechodzącej przez l1 i l2 (w kierunku od l1 do l2)
    double A = l2->y - l1->y;
    double B = l1->x - l2->x;
    double C = l2->x * l1->y - l1->x * l2->y;

    double D = (A * p->x + B * p->y + C) / (A * A + B * B);

    res->x = p->x - 2 * A * D;
    res->y = p->y - 2 * B * D;
}

double cross(Point *p, Point *l1, Point *l2){//iloczyn wektorowy
    //jesli jest ujemny to punkt p jest na prawo od linii l
    double val = (l2->x - l1->x)*(p->y - l1->y) - (l2->y - l1->y)*(p->x - l1->x);
    return val;
}

// Pomocnicza funkcja do sprawdzania, czy punkt jest w prostokącie
int is_inside_rect(Point p, Point p1, Point p2) {
    return (p.x >= p1.x - epsilon && p.x <= p2.x + epsilon &&
            p.y >= p1.y - epsilon && p.y <= p2.y + epsilon);
}

// Pomocnicza dla koła (zakładam p1=środek, p2.x = promień)
int is_inside_circle(Point p, Point center, double r) {
    double dx = p.x - center.x;
    double dy = p.y - center.y;
    return (dx*dx + dy*dy <= r*r + epsilon);
}

int count_layers(Origami *origami, int k, Point *p) {
    // 1. Baza rekurencji: Figury podstawowe
    if (origami[k].type == 0) { // Prostokąt
        return is_inside_rect(*p, origami[k].p1, origami[k].p2) ? 1 : 0;
    }
    else if (origami[k].type == 1) { // Koło
        return is_inside_circle(*p, origami[k].p1, origami[k].p2.x) ? 1 : 0;
    }

    // 2. Krok rekurencyjny: Zgięcie (Typ 2)
    double cp = cross(p, &origami[k].p1, &origami[k].p2);

    if (cp < -epsilon) return 0;
    else if (cp > epsilon) {
        // Punkt jest po lewej stronie linii zgięcia
        // Musimy sprawdzić warstwy, które tu były oryginalnie...
        int layers = count_layers(origami, origami[k].link, p);
        
        // ...ORAZ warstwy, które przyleciały tu z prawej strony (odbite).
        Point reflected;
        reflect_point(p,&origami[k].p1, &origami[k].p2, &reflected);
        layers += count_layers(origami, origami[k].link, &reflected);
        return layers;
    } 
    else return count_layers(origami, origami[k].link, p);
    // Punkt leży idealnie na linii zgięcia. Traktujemy jak stronę bez odbicia
}

signed main(){
    unsigned n, q;
    if (scanf("%u%u", &n,&q)) {}
    Origami *origami = (Origami*) malloc (sizeof(Origami)*n);

    char c; 
    for (unsigned i = 0; i < n; i++){
        if (scanf(" %c", &c)) {}
        if (c=='P'){
            read_point(&origami[i].p1);
            read_point(&origami[i].p2);
            origami[i].type = 0;
            // brak poprzednika
            continue;
        }
        if (c=='K'){
            read_point(&origami[i].p1); // Wczytaj środek
            // Wczytaj TYLKO promień do p2.x
            if(scanf("%lf", &origami[i].p2.x)) {} 
            origami[i].p2.y = 0; // p2.y nie jest używane w kole, zerujemy dla porządku
            origami[i].type = 1;
            // brak poprzednika
            continue;
        }
        if (c=='Z'){
            int index;
            if (scanf("%d", &index)) {}
            index--;
            read_point(&origami[i].p1);
            read_point(&origami[i].p2);
            origami[i].type = 2;
            origami[i].link = index;
        }
    }

    //pora na obsługę zapytań: ile jest warstw w punkcie p w origami o indeksie k
    for (unsigned i = 0; i < q; i++){
        Point p; 
        int k;
        if (scanf("%d", &k)) {}
        read_point(&p);
        printf("%d\n", count_layers(origami, k-1, &p));
    }

    return 0;
}
