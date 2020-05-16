# Viz3D
TODO:
 - mousePicking (part III)
 - graficzna interpretacja zmiennych
 - algorytm do uogólniania
 - 'KnowHow'
 - optymalizacja (Instancing)


 Update 16.05.2020 (file picking)
 + filePicking - using windowsAPI
 + dane wybranej komórki - muszą istnieć nazwy (przeykład: w 100.txt nazwami są "a", "b", "c"...)
 + do "state_100x100x100" dodałem rozmiary siatki i teraz się ładuje
 + ograniczenie dla wyświetlanych komórek do 100 000 (tymczasowe)

 Update 11.05.2020 (mouse picking - prat II):
 + mousePicking (part II)
 + algorytm do uogólniania - pierwszy pomysł (F2 - na własne ryzyko)
 + podstawowe GUI (wczytywanie pliku, pozycja kamery, dane wybranej komórki)

Update 08.05.2020 (mouse picking - prat I):
 + można kliknąć aby "zaznaczyć" komórki (wybierane ze środka ekranu)

Update 03.05.2020 (Optimization v2.1):
 + nadal zdecydowanie optyamilzacja (100k pełnych komórek naraz)
 + początek GUI
 + pliki projektu (!)
 + więcej siatek (między innymi 'poszarpana' siatka)

Update 29.04.2020 (Optimization v1):
 + zdecydowanie optymalizacja (ok 10k pełnych komórek naraz)

Update 17.04.2020:
 + opisy w plikach naglówkowych (dla folderu 'core')
 + kolory różnych ziaren
 + odczytanie komórki z dowolnej linijki w pliku
 + wygenerowane przez Michała struktury siatek 10x10x10, 20x20x20, 30x30x30 (folder "sample")
 + dwa przykładowe screeny struktury 10_10_10 (folder "sample")

 
Q:
 + Czy jest ograniczenie co do liczby różnych ziaren? (ile bajtów potrzebuję na określenie ID ziarna)
    - "docelowo ziaren może być dość dużo"
 + Czy możemy uznać że siatka nie będzie miała większych rozmiarów? (ile bajtów potrzebuję na określenie ID ziarna)
    - "Po wczytaniu siatki z pliku w programie nie będziemy już zwiększać liczby komórek"
 + Czy jest ograniczenie co do typu/rozmiaru zmiennych? (tych wartości, których nie wykorzystuję do wizualizacji)
    - "Zmienne zawsze będą typu int (0 - n) albo double (0 - n) można odrysowywać dane zmienne za pocą gradientu kolorów."
 + Czy siatka będzie zawsze w postaci prostopadłościanu? (zkładam, że nie)
    - "mogą pojawić się inne figury"
 + Jakie opcje chcemy w GUI?
    - "rotacja, zoom oraz zmiana widoków pomiędzy danymi które mieliśmy w pliku."
 + Czy jakieś dane można uznać za wspólne dla wszystkich komórek w danym ziarnie/
    - "Tylko kolor będzie wspólny"
