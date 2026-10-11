1. Wstęp – styl kodowania

Kluczowymi czynnikami decydującymi o jakości programu komputerowego są poprawność i szybkość działania. Istnieje jednak jeszcze jeden istotny wymiar, często niedostrzegany przez początkujących programistów: jest nim czytelność pisanego kodu.

Czytelność pisanego kodu ma decydujący wpływ na jakość i szybkość tworzenia programów. Dobrze napisane programy są łatwe w rozwijaniu i konserwacji, co z kolei umożliwia ich szybszy rozwój. W ostatecznym rozrachunku czytelność kodu wpływa na wymienione wcześniej kluczowe miary jakości programu: poprawność i szybkość działania.

Dzięki jasnej i przejrzystej strukturze w dobrze napisanym kodzie łatwo znajduje się i naprawia błędy. Dzięki minimalizowaniu zależności między elementami programu trudniej jest wygenerować nowe błędy przez zmianę starych fragmentów. Wiele praktyk skupia się na dzieleniu kodu na spójne, niezależne fragmenty, co z kolei niejednokrotnie pozwala znaleźć nowe możliwości optymalizacji.

Póki programy są krótkie, może się wydawać, że czytelność programu nie ma znaczenia – w przypadku kilkuset linii kodu łatwo jest opanować cały program, szczególnie gdy jest się jego autorem. Ten sposób myślenia jest niebezpieczną pułapką, w którą często wpadają nawet zaawansowane drużyny programistyczne. Problemy z czytelnością kodu szybko dają się we znaki, gdy zwiększa się złożoność pisanego programu, a w szczególności gdy program:

    pisze się dłużej niż jeden dzień,
    ma więcej niż kilkaset linii,
    składa się więcej niż z jednego pliku źródłowego,
    jest pisany przez więcej niż jedną osobę.

W przypadku źle napisanego kodu, wraz ze wzrostem złożoności projektu, programiści coraz więcej czasu spędzają na czytaniu i analizie istniejącego kodu, niż na pisaniu nowego. Zdarza się, że modyfikacja fragmentu kodu skutkuje nieoczekiwanymi zachowaniami innych części programu. Często poziom skomplikowania niektórych fragmentów jest na tyle duży, że wprowadzenie jakiejkolwiek zmiany okazuje się niemożliwe bez przepisania znaczącej części całego programu. Wiele projektów ogromnie spowalnia lub nawet upada na tym etapie. Wszystkim tym problemom można zapobiec poprzez utrzymywanie odpowiedniej jakości kodu od samego początku.

Zasady związane z pisaniem dobrego kodu możemy podzielić na dwie grupy: zasady dotyczące stylu kodowania oraz dobre praktyki mające na celu tworzenie czystego kodu.

Styl kodowania jest to formalizm określający sposób edycji kodu – odstępy, nawiasowanie, wcięcia i tym podobne elementy. Często firmy i drużyny programistyczne ustalają własne zasady, jednak istnieją pewne konwencje powszechnie stosowane na całym świecie, które to konwencje postaramy się tutaj przedstawić. Po ustaleniu stylu kodowania, należy bezwzględnie go przestrzegać. Wiele narzędzi programistycznych pomaga utrzymać wybrany styl. Należą do nich dobre IDE oraz niezależne programy, jak linuksowy indent lub skrypt c style check. Celem stosowania ustalonego stylu jest zwiększenie czytelności kodu oraz spójności, dzięki czemu cały program wygląda jak dzieło jednego programisty.

Czystym kodem nazywamy kod, który jest łatwy do:

    zrozumienia (czytelny),
    testowania,
    zmodyfikowania.

Mówiąc krótko, czysty kod minimalizuje czas spędzany na jego rozwój i konserwację. Programistom jest go łatwo zrozumieć i wprowadzać zmiany. Czas spędzony na czytaniu kodu jest minimalizowany w stosunku do czasu spędzanego na pisaniu.

Praktyki związane z tworzeniem czystego kodu są pewnego rodzaju radami, czy też heurystykami mającymi zastosowanie w większości przypadków. Zdarzają się jednak sytuacje, w których odstąpienie od danej reguły pozwala na stworzenie lepszego kodu. Do podanych reguł należy stosować podejście „każda zasada może zostać złamana, ale żadnej nie wolno zignorować”.

W dalszej części dokumentu omawiamy wpływ stylu dostarczonych programów na ocenę oraz przedstawiamy reguły dotyczące stylu kodowania i dobre praktyki tworzenia czystego kodu.
2. Czytelność programów a ocena

Tworzenie czytelnego kodu jest kluczową umiejętnością, jaką powinien opanować każdy programista. Praktyka pokazuje, że jeśli programista nie wyrobi w sobie nawyku pisania czytelnego kodu źródłowego na samym początku nauki programowania, to później trudno będzie mu się przestawić na czytelny styl pisania. Programistom często wydaje się, że tworzenie czytelnego kodu wymaga dużych nakładów dodatkowej pracy, jednak dobry kod jest inwestycją oszczędzającą w ostatecznym rozrachunku ogromne ilości czasu.

W związku z powyższym czytelność programów będzie miała wpływ na wystawiane oceny zadań zaliczeniowych. Dla każdego zadania będziemy podawać szczegółowe informacje dotyczące wpływu jakości kodu na ocenę.
3. Styl kodowania

Styl kodowania określa sposób zapisu programów. Często zespoły programistów ustalają własne zasady. Istotne jest, aby po wybraniu określonego stylu, bezwzględnie się do niego stosować. Powoduje to, że programy stają się czytelne oraz spójne – cały program wygląda jak praca jednego programisty.
3.1. Szerokość linii

Dobrym zwyczajem jest pisanie linii nie dłuższych niż 80 znaków. Terminale tekstowe wciąż są w powszechnym użyciu, a nawet w środowisku graficznym, gdy źródła nie są szersze niż 80 znaków, można wyświetlić kilka plików obok siebie. Jeżeli instrukcja jest dłuższa niż 80 znaków, trzeba ją jakoś podzielić na wiele linii. Na przykład wywołanie funkcji można przepisać w taki sposób, żeby kolejne argumenty znalazły się w oddzielnych liniach.

Reguła 80 znaków w linii pochodzi jeszcze z czasów kart perforowanych, kiedy to jedna karta kodowała jedną linię programu i mieściła właśnie 80 znaków. Reguła ta ma też inne uzasadnienie. Okazuje się, że wzrok najmniej się męczy, gdy jedna linia tekstu zawiera ok. 66 znaków drukarskich – i taką zasadę stosuje się w typografii.
3.2. Odstępy i wcięcia

Stosowanie odstępów i wcięć ma kluczowe znaczenie dla czytelności programu. Dobrze sformatowany program pozwala natychmiast zorientować się w jego strukturze.
3.2.1. Wcięcia

Wcięcia służą do wyróżniania bloków programu (ciało funkcji, ciało pętli itp.). Każdy blok powinien być wcięty o jeden raz więcej niż otaczający go kod.

void aboutIndents() {
    bool indentsAreCool = true;
    int i;

    printf("\tI'm main function block, so I have one indent.\n");

    for (i = 0; i < 2; i++) {
        printf("\t\tYeah! Two indents as I'm for loop block.\n");

        if (i % 2 == 1) {
            printf("\t\t\tThree indents - how cool is that?\n");
        }
    }

    printf("\tAgain one indent.\n");

    if (indentsAreCool) {
        printf("\t\tLast but not least - two indents!\n");
    }

    printf("\tBye!\n");
}

Zazwyczaj używa się wcięć wielkości dwóch lub czterech spacji. Wcięcia można robić z wykorzystaniem spacji lub tabulacji. Toczy się nieustająca debata nad wyższością jednej koncepcji nad drugą (zainteresowanym polecamy wyszukać hasło „spaces vs tabs”).

Należy zwrócić uwagę, że różne edytory w różny sposób wyświetlają tabulację. Może to być zarówno zaleta, jak i wada. Dobre edytory pozwalają dostosować rozmiar wyświetlanych tabulacji, więc programista może dostosować rozmiar wcięć do własnych preferencji, nie zmieniając kodu (jest to istotne przy pracy z systemami kontroli wersji). Z drugiej strony otwarcie kodu z tabulacjami (a zwłaszcza takiego, w którym miesza się użycie spacji i tabulacji) w gorszym lub nieskonfigurowanym edytorze może spowodować, że plik stanie się kompletnie nieczytelny.

Najważniejsze jest to, aby wcięcia były obecne oraz aby konsekwentnie trzymać się wybranego rozmiaru wcięć – niedopuszczalne jest korzystanie z różnego rodzaju wcięć w obrębie jednego projektu.
3.2.2. Odstępy pionowe

Odstępy pionowe służą do oddzielania spójnych logicznie fragmentów kodu, podobnie jak akapity w tekście. Aby oddzielić jeden fragment od innego, należy używać jednej pustej linii.

Zarówno sekcje programu, jak i deklaracje funkcji powinny być oddzielone od siebie pustą linią.

Również wewnątrz bloków kodu warto wprowadzać pionowe odstępy między spójnymi jego fragmentami.

double average(int numbers[], int size) {
    int i;
    double average;
    double sum;

    for (i = 0; i < size; i++) {
        sum += numbers[i];
    }

    average = sum / size;

    return average;
}

W powyższym kodzie zostały wyróżnione następujące sekcje:

    deklaracje zmiennych,
    obliczenie sumy elementów tablicy,
    obliczenie średniej,
    zwrócenie wyniku.

W takim kodzie na pierwszy rzut oka widać składowe. Jeśli chcielibyśmy go np. zmodyfikować, aby obliczać średnią kwadratów, natychmiast znajdziemy miejsce do modyfikacji. Porównajmy to z poniższym kodem.

double average(int numbers[], int size) {
    int i;
    double average;
    double sum;
    for (i = 0; i < size; i++) {
        sum += numbers[i];
    }
    average = sum / size;
    return average;
}

3.2.3. Odstępy poziome

Odstępy poziome pozwalają wyróżnić składowe wewnątrz linii kodu. Należy je stosować w następujących sytuacjach:

    dookoła operatorów dwuargumentowych, czyli operatorów przypisania, operatorów arytmetycznych, logicznych i porównujących,
    po przecinku oddzielającym argumenty w wywołaniu funkcji,
    po dyrektywie #include,
    po słowach kluczowych if, for, while, do, else, switch, case,
    przed otwierającym nawiasem klamrowym,
    przed nawiasem okrągłym otwierającym i po nawiasie okrągłym zamykającym stosowanym w celu zmiany priorytetu operacji.

Ostępy poziome dzielą linię na logiczne części. Spójne fragmenty tekstu interpretujemy naturalnie jako związane ze sobą atomowe fragmenty kodu.

Dodawanie spacji w nieodpowiednich miejscach zaburza czytelność kodu. Nie należy dodawać spacji w następujących sytuacjach:

    dookoła nawiasu otwierającego przy deklarowaniu i wywołaniu funkcji,
    przed nawiasem zamykającym listę argumentów funkcji oraz warunek w instrukcjach if, for i while,
    przed przecinkiem,
    przed średnikiem,
    dookoła nawiasu kwadratowego otwierającego,
    przed nawiasem kwadratowym zamykającym,
    dookoła kropki i operatora ->,
    miedzy jednoargumentowym operatorem - a wyrażeniem, którego dotyczy ta operacja,
    między operatorem ++ i -- a wyrażeniem, którego dotyczy ta operacja,
    po operatorach referencji i dereferencji (& i *),
    przed zamykającym nawiasem klamrowym,
    po nawiasie okrągłym otwierającym i przed nawiasem okrągłym zamykającym stosowanym w celu zmiany priorytetu operacji.

3.3. Nazwy wielowyrazowe

Jest kilka konwencji zapisu nazw składających z więcej niż jednego słowa:

    laczenie_wyrazow_podkreslnikiem_male_litery – stosowana zwykle w nazwach zmiennych, funkcji i typów,
    LACZENIE_WYRAZOW_PODKRESLNIKIEM_DUZE_LITERY – stosowana zwykle w nazwach stałych i makr,
    KonwencjaZwanaCamelCase – stosowana zwykle w nazwach typów,
    camelCasePisanaMalaLitera – stosowana zwykle w nazwach zmiennych i funkcji.

Można też spotkać inne konwencje. W bibliotece standardowej języka C nazwy typów mają przyrostek _t, np. size_t, time_t, a w kodzie źródłowym jednego z popularnych systemów operacyjnych używa się notacji węgierskiej.
3.4. Struktura programu

Typowy plik (nazywany jednostką translacji) w języku C składa się z kilku sekcji. Poniżej przedstawiamy ich standardową kolejność, razem z używanymi słowami kluczowymi:

    lista dołączanych bibliotek – dyrektywy #include,
    makrodefinicje – dyrektywy #define,
    definicje wszystkich typów – struct, typedef,
    deklaracje zmiennych globalnych,
    deklaracje lokalnych funkcji pomocniczych,
    definicje lokalnych funkcji pomocniczych,
    definicje funkcji eksportowanych przez tę jednostkę translacji, np. funkcja main().

Nie wszystkie wymienione sekcje muszą wystąpić.

Jeśli program składa się z wielu jednostek translacji, to prawie każdy plik z rozszerzeniem .c ma odpowiadający mu plik nagłówkowy, z rozszerzeniem .h, deklarujący funkcje i ewentualnie zmienne eksportowane przez tę jednostkę translacji. Wyjątkiem jest plik definiujący funkcję main(). Mogą też istnieć pliki nagłówkowe bez odpowiadających im plików .c. Plik nagłówkowy może też zawierać definicje funkcji i zmiennych.
3.5. Przykłady formatowania kodu
3.5.1. Deklaracje zmiennych

Zmienne wskaźnikowe należy deklarować w następujący sposób:

typ *nazwa;
typ *nazwa1, *nazwa2, nazwa3; // Uwaga: zmienna nazwa3 nie jest wskaźnikiem!

3.5.2. Instrukcje sterujące

Poniższy przykład pokazuje, jak powinny być sformatowane poszczególne instrukcje sterujące.

if (warunek) {
    instrukcja1;
    instrukcja2;
}
else {
    instrukcja3;
}

if (warunek1) {
    instrukcja1;
}
else if (warunek2) {
    instrukcja2;
    instrukcja3;
}
else {
    instrukcja4;
    instrukcja5;
}

for (wyrażenie1; wyrażenie2; wyrażenie3) {
    instrukcja1;
    instrukcja2;
    instrukcja3;
}

while (warunek) {
    instrukcja;
}

do {
    instrukcja1;
    instrukcja2;
    instrukcja3;
} while (warunek);

W instrukcjach if, for, while nie zawsze trzeba stosować nawiasy klamrowe. Poprawna jest konstrukcja:

for (wyrażenie1; wyrażenie2; wyrażenie3)
    for (wyrażenie4; wyrażenie5; wyrażenie6)
        if (warunek)
            instrukcja;

Częstym błędem jest edycja takiej instrukcji i zapomnienie o dodaniu nawiasów. Dobrą praktyką jest stosowanie nawiasów klamrowych nawet wtedy, gdy nie są one wymagane, ale od tej reguły można czasem odstąpić. Bezwzględnie należy jednak stosować nawiasy klamrowe we wszystkich gałęziach konstrukcji if-else, jeśli co najmniej jedna gałąź ich wymaga.
4. Dobre praktyki pisania czystego kodu

W tym rozdziale omówimy praktyki, pozwalające na tworzenie czytelnego, łatwego w utrzymaniu kodu. Od wielu reguł zawartych w tym rozdziale można odstąpić w uzasadnionych przypadkach, jednak zawsze należy mieć je na uwadze i robić takie odstępstwa świadomie.
4.1. Nazewnictwo
4.1.1. Polski czy angielski?

Zachęcamy do stosowania angielskich nazw.

Warto zwrócić uwagę, że instrukcje w językach programowania są nazywane po angielsku. Również zewnętrzne biblioteki korzystają z angielskiego nazewnictwa. W związku z tym stosowanie angielskich nazw we własnym kodzie zwiększa spójność kodu i czytelność.

Dodatkowo, program z angielskimi nazwami staje się zrozumiały dla większej liczby osób.
4.1.1. Długie, wyczerpujące nazwy

Najpopularniejszym błędem robionym przez programistów jest stosowanie mało mówiących nazw funkcji i zmiennych. Nie należy obawiać się tworzenia długich, wyczerpujących nazw. Opisowe nazwy skutecznie dokumentują kod i przyspieszają nawigację po nim. Zdecydowanie lepiej nazwać funkcję getPatientDescription(char* patientName) niż get(char* s).

Częstym argumentem podnoszonym przeciwko długim nazwom jest konieczność ich pisania za każdym razem, gdy chcemy ich użyć. Współcześnie, gdy niemal każdy edytor ma mniej lub bardziej zaawansowaną funkcję autouzupełniania ten argument jest chybiony, a zysk z korzystania z dobrych nazw jest ogromny.

W trakcie pisania kodu często wydaje nam się, że jest oczywiste, co dana funkcja robi. Zwykle parę dni później okazuje się, że tak wcale nie jest, dlatego zawsze warto zastanowić się nad odpowiednim nazwaniem zmiennej czy funkcji. Wczesne dobranie jak najtrafniejszego identyfikatora, który możliwie zwięźle i trafnie określa funkcję procentuje w przyszłości.

Nie należy stosować skrótów (np. poprzez usuwanie samogłosek ze słowa). Zazwyczaj takie skróty szybko stają się enigmatyczne i okazują się nie lepszą nazwą niż np. a15. Wyjątkiem są tu nazwy odpowiadające konwencjom matematycznym, czyli np. i, j oraz k dla indeksów lub x, y oraz z dla współrzędnych. Użycie nazw m lub n na oznaczenie ilości pewnego rodzaju obiektów bywa już kłopotliwe.

Spójrzmy na przykład poniższej funkcji:

function main() {
    struct Patient *p = load("patients");
    sort(p);
    printP(p);
}

Jesteśmy w stanie się domyślić, że wczytujemy listę pacjentów, porządkujemy według jakiegoś klucza i wypisujemy informacje o nich na wyjście. Bez wczytania się w treści użytych funkcji nie jesteśmy jednak w stanie stwierdzić, jaki jest klucz sortowania ani jakie informacje wypisujemy.

Porównajmy to z kodem poniżej:

#define PATIENTS_FILE_NAME "patients"

function main() {
    struct Patient *patientsList = loadPatientsListFromFile(PATIENTS_FILE_NAME);
    sortPatientsByOperationPriority(patientsList);
    printPatientsNamesAndDescriptions(patientsList);
}

Dzięki dobremu nazwaniu funkcji natychmiast dowiadujemy się, że program wypisuje opisy przypadków pacjentów uporządkowane według pilności ich operacji. Oprócz tego wiemy, skąd dana lista jest pobierana – z pliku o nazwie patients. Jak widać na powyższym przykładzie, dobre nazewnictwo dokumentuje kod i powoduje, że wywołania kolejnych funkcji składają się w opis jego działania.
4.2. Stałe

Wszystkie nietypowe stałe w programie powinny być nazwane.

Nazywanie stałych dokumentuje kod. Oprócz tego zmniejsza ryzyko popełnienia błędu – jeśli używamy danej stałej w kilku miejscach w programie, mamy pewność, że wszędzie jest użyta poprawna wartość.
4.3. Funkcje

Głównym zastosowaniem funkcji jest wydzielanie powtarzanych wielokrotnie fragmentów kodu (tzw. reguła DRY). Nie tylko poprawia czytelność programu, ale zmniejsza też ryzyko wystąpienia błędu. Jeśli korzystamy z podobnego fragmentu kodu w kilku miejscach w programie i znajdziemy w takim fragmencie błąd, możemy łatwo pominąć któryś fragment przy wprowadzaniu poprawek.

Funkcji możemy również używać do dokumentowania kodu – długi blok możemy podzielić na mniejsze, odpowiednio nazwane funkcje. Takie podejście ułatwia rozumienie i nawigację po kodzie. Nie należy jednak przesadzać z rozdrabnianiem kodu.

Podział na funkcje zdecydowanie zwiększa wydajność pracy. Przy dobrze napisanym kodzie w każdym momencie programista skupia się na małym, niezależnym fragmencie programu. Dzięki temu liczba „podręcznych zmiennych” przechowujących informacje o tym, co dzieje się w powiązanych częściach kodu, drastycznie się zmniejsza, co minimalizuje wysiłek programisty i pozytywnie wpływa na efektywność pracy.

Każda funkcja powinna realizować jeden cel o ustalonym poziomie abstrakcji. Program komputerowy realizuje pewne zadanie. Takie zadanie składa się z mniejszych podzadań, a te podzadania znów mogą składać się z jeszcze mniejszych podzadań. Podział na funkcje powinien odzwierciedlać podział na zadania. Ideałem jest, gdy funkcja jest tak napisana, żeby mogła być przeniesiona do innego programu, który jej potrzebuje, i działać bez konieczności wprowadzania jakichkolwiek poprawek.

Patrząc na treść funkcji main(), powinniśmy być w stanie szybko zrozumieć, jakie kroki wykonuje program. Dopiero, kiedy chcemy zrozumieć, jak jest to zrobione, możemy zajrzeć do definicji używanych funkcji. Tutaj rozumowanie jest podobne – funkcja powinna wywoływać inne funkcje o większym stopniu szczegółowości.

Funkcje operujące na danym poziomie abstrakcji nie powinny realizować zadań z innych poziomów abstrakcji. Operacje na niższym poziomie powinny być wydzielone do kolejnych funkcji. Zawsze powinniśmy wołać funkcje z poziomu o jeden wyższego od tego, w którym się „znajdujemy”. Przykładowy podział na poziomy abstrakcji może wyglądać następująco: logika programu → operacje na napisach → operacje na pojedynczych znakach.

Funkcje nie powinny mieć efektów ubocznych, czyli nie powinny zmieniać stanu programu. Oznacza to, że wielokrotne wywołanie danej funkcji dla tych samych danych powinno zawsze dać ten sam efekt. Oczywiście istnieją funkcje, w przypadku których jest to niemożliwe, na przykład funkcja operująca na zewnętrznym pliku. Są to jednak szczególne przypadki.

Odpowiednie pisanie funkcji pozwala z czasem na tworzenie własnych bibliotek – ogólnych fragmentów kodu, z których można korzystać w innych projektach, co znacząco przyspiesza ich realizację.

Funkcje powinny być krótkie. Większość funkcji powinna się mieścić w przedziale od trzech, do sześciu linii. Taki limit może wydawać się bardzo restrykcyjny, ale jeśli stosujemy odpowiedni podział na abstrakcje, często naturalnie kończymy z krótkimi funkcjami. Należy zwrócić uwagę, aby podział na funkcje był sensowny. Nie należy dzielić funkcji „na siłę”, aby sztucznie zmniejszyć liczbę linii – takie praktyki potrafią zmniejszyć jedynie czytelność kodu. Bezwzględnie jednak należy przestrzegać reguły, aby całą definicję funkcji dało się objąć spojrzeniem, co dawniej oznaczało, że mieściła się ona na ekranie monitora, który wyświetlał co najwyżej ok. 50 linii.

Warto zwrócić uwagę, że każda instrukcja sterująca (if, for, while itp.) wprowadza nowy poziom abstrakcji, więc wydzielanie treści ich bloków od funkcji jest zazwyczaj dobrym pomysłem. Dodatkowo, jeśli w instrukcji korzystamy ze skomplikowanego warunku, warto również ten warunek wydzielić do osobnej funkcji w ramach dokumentacji.

Im mniej argumentów ma funkcja, tym lepiej. Zwykle, jeśli funkcja ma więcej niż trzy argumenty, traci ona na czytelności. Często nadmierny wzrost liczby argumentów funkcji jest znakiem, że należy zdefiniować nowy typ strukturalny.
4.4. Zmienne globalne

Należy unikać stosowania zmiennych globalnych. Łatwo utracić kontrolę nad tym, co dzieje się w programie ze zmienną globalną. Stosowanie zmiennych globalnych jest często powodem powstawania błędów wynikających z przypadkowego ich użycia w programie. Dodatkowo funkcje operujące na zmiennych globalnych naruszają zasadę braku efektów ubocznych.
4.5. Instrukcja skoku (goto)

Jest to instrukcja pochodząca z języków niższego poziomu niż C, używana w nich m.in. do implementowania pętli. Nieodpowiedzialne korzystanie z instrukcji skoku potrafi łatwo doprowadzić do nadmiernego skomplikowania przepływu wywołania programu (tzw. spaghetti code). Czasem wygodnie jest skorzystać z tej instrukcji do wyjścia z zagnieżdżonych pętli. Nieuzasadnione użycie goto będziemy traktować jako błąd.
4.6. Komentarze

Im kod wymaga mniej komentarzy, tym lepiej. Głównym celem komentarzy jest dokumentowanie kodu. Stosowanie dobrego nazewnictwa i podziału na funkcje pozwala w ogromnym stopniu zmniejszyć zapotrzebowanie na komentarze. Nie oznacza to, że komentarze są zbędne. Często trafnie dobrany komentarz w odpowiednim miejscu jest niezastąpiony.

Komentarzy należy przede wszystkim używać w przypadku niebanalnych fragmentów kodu. Mogą one zawierać nazwę wykorzystanego algorytmu albo link do dokumentacji lub źródła wykorzystanego kodu. Bezwzględnie należy umieścić komentarz w miejscu poprawieniu niebanalnego błędu. Fakt, że taki błąd wystąpił, świadczy, że dany fragment programu był nieoczywisty dla programisty, który go napisał, i może być nieoczywisty dla programisty, który w przyszłości będzie rozwijał program.

Nie należy tworzyć komentarzy, które nic nie wnoszą, np.:

// Zmienne globalne
int global_counter;

// Funkcja main
int main() {
    ...
    global_counter++; // Zwiększenie globalnego licznika
    ...
}

Niedopuszczalne jest również zostawianie wykomentowanych fragmentów kodu. Często w trakcie pracy wykomentowuje się fragmenty kodu, aby go tymczasowo wyłączyć. Jednak po dokonaniu zmian taki fragment kodu należy usunąć, jeśli jest już niepotrzebny. Wykomentowany kod często zostaje w projektach, ponieważ po dłuższym czasie nikt nie wie, czy taki kod jest potrzebny i czy można go usunąć. Dzięki stosowaniu systemów kontroli wersji nie trzeba bać się o utratę usuniętego kodu.
5. Przykład

Poniżej znajduje się przykład programu napisanego zgodnie z wyżej opisanymi regułami.

/* Program znajduje wszystkie liczby pierwsze nie przekraczające podanej
 * wartości, używając Sita Eratostenesa. */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Funkcja daje w wyniku liczbę całkowitą wczytaną z wejścia.
// Jeśli wystąpił błąd, powoduje zakończenie programu z kodem 1.
static int readPrimeNumbersUpperLimit() {
  int limit;

  printf("Enter the upper limit of prime numbers you are looking for:\n");
  if (scanf("%d", &limit) != 1)
    exit(1);

  return limit;
}

// Funkcja wykreśla z tablicy liczby będące wielokrotnością danej liczby.
//  primes - tablica opisująca znalezione liczby pierwsze
//  size   - rozmiar tej tablicy
//  p      - dana liczba pierwsza
static void unmarkPrimeMultiples(bool primes[], int size, int p) {
  int n = p;

  // Taka postać warunku pętli zapobiega przepełnieniu arytmetyki.
  while (n < size - p) {
    n += p;
    primes[n] = false;
  }
}

// Funkcja wyznacza liczby pierwsze, stosując algorytm Sita Eratostenesa.
// Po wykonaniu tej funkcji liczba n jest pierwsza wtedy i tylko wtedy,
// gdy primes[n] == true.
//  primes - tablica opisująca znalezione liczby pierwsze
//  size   - rozmiar tej tablicy
static void eratosthenesSieve(bool primes[], int size) {
  // Dwa pierwsze elementy tablicy nie są używane, ale takie rozwiązanie
  // sprawia, że tablicę primes łatwo się indeksuje.
  primes[0] = primes[1] = false;
  for (int i = 2; i < size; ++i)
    primes[i] = true;

  for (int i = 2; i <= size / 2; ++i)
    if (primes[i])
      unmarkPrimeMultiples(primes, size, i);
}

// Funkcja wypisuje znalezione liczby pierwsze.
//  primes - tablica opisująca znalezione liczby pierwsze
//  size   - rozmiar tej tablicy
static void printFoundPrimeNumbers(bool primes[], int size) {
  printf("Found prime numbers:\n");
  for (int i = 0; i < size; ++i)
    if (primes[i])
      printf("%d\n", i);
}

int main() {
  // Dodajemy jedynkę, bo tablica jest indeksowana od 0 do tableSize - 1.
  int tableSize = readPrimeNumbersUpperLimit() + 1;

  // Tablica opisująca znalezione liczby pierwsze.
  bool isPrime[tableSize];

  eratosthenesSieve(isPrime, tableSize);
  printFoundPrimeNumbers(isPrime, tableSize);

  return 0;
}
