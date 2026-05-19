package narzedzia;

// Umożliwia łatwe przechodzenie między formatami czasu.
// Zawiera też niezbędne stałe.
public class Zegar {
    // konwertuje: HH:MM:SS -> sekundy
    public static int parsujCzas(String czasWFormacie) {
        String[] fragmenty = czasWFormacie.split(":");
        int godziny = Integer.parseInt(fragmenty[0]);
        int minuty = Integer.parseInt(fragmenty[1]);
        int sekundy = fragmenty.length > 2 ? Integer.parseInt(fragmenty[2]) : 0;

        return godziny * 3600 + minuty * 60 + sekundy;
    }

    // konwertuje: sekundy -> HH:MM:SS
    public static String formatujCzas(int absolutneSekundy) {
        int godziny = absolutneSekundy / 3600;
        int minuty = (absolutneSekundy % 3600) / 60;
        int sekundy = absolutneSekundy % 60;
        return String.format("%02d:%02d:%02d", godziny, minuty, sekundy);
    }

    // Stała równoważna z zakończeniem symulacji.
    public static final int KONIEC_CZASU = parsujCzas("15:00:00");

    // Początek dnia. Start wyciągów.
    public static final int CZAS_OTWARCIA = parsujCzas("09:00:00");
}