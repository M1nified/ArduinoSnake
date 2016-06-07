# Temat 2 - Wąż
kier,rok,rok_akad
* MG
* MS
* KP

## Schemat połączeń
![schematic](https://raw.githubusercontent.com/M1nified/ArduinoSnake/master/schematic.png "Schemat połączeń")
## Program
### Opis algorytmu
Działanie programu opiera się głównie na poniższych elementach:
* `LedControl lc` - instancja klasy `LedControl` zawartej w bibliotece `<LedControl.h>`, a odpowiadającej za sterowanie wyświetlanym obrazem
* `int pos[MAXLEN][2]` - tablica zawierająca historię pozycji głowy węża, co pozwala na łatwe śledzenie ogona
* `int curr` - zmienna określająca aktualną długość węża
* `int curr_spot` - zmienna, w której zliczane są kroki węża, aby określić czy pokonał już całą planszę, a co za tym idzie czy może nadal rosnąć
* `void snakeHead(int x,int y)` - funkcja zapisująca kolejne położenia głowy, dokonująca zmian długości oraz rysująca
* `void spiralaDoSrodka()`,`void spiralaOdSrodka()` - funkcje wywołujące `snakeHead` z odpowiednimi argumentami, tak aby wykonać ruch spiralny
* `void mrugnij()` - wyłącza ekran, ustawia wszystkie diody w stan aktywny i wykonuje mrugnięcie poprzez włączenie, a następnie wyczyszczenie wyświetlacza, wszystko w określonych przez `CYCLE_TIME` odstępach czasu.

Do `snakeHead` można przekazywać dowolne współrzędne, przez które chcemy kierować węża. W celu ukrycia węża podaje się współrzędne poza widocznym obszarem, w ten sposób można w prosty sposób dowolnie manipulować ścieżką. Głównym ograniczeniem jest `MAXLEN`, które należy dobrać zależnie od potrzeb oraz wielkości pamięci.   
W funkcji `setup` wypełniana jest odpowiadająca początkowej długości część tablicy `pos`, tak aby nie dodawać warunków w `snakeHead`. Dodatkowo na początku na upewniamy się, że wyświetlacz jest włączony, ustawiamy najniższą jasność oraz czyścimy ekran.
``` c
  lc.shutdown(0, false);
  lc.setIntensity(0, 0);
  lc.clearDisplay(0);
```
Pierwszy argument każdej z tych funkcji oznacza numer wyświetlacza, w tym przypadku `0`, bo jest to pierwszy wyświetlacz.  
W `loop` wykonywane są wszystkie wymagane animacje, kolejno:
1. `spiralaDoSrodka()` - rysowanie do srodka
2. `znikanie()` - chowanie sie w srodku
3. `snakeHead(-1,-1)` - jeszcze jeden raz chowamy weża, tak aby był moment pustego wyświetlacza
4. `spiralaOdSrodka()` - rysowanie na zewnątrz
5. `znikanie()` - chowanie za krawędzią planszy
6. `snakeHead(-1,-1)` - jeszcze jeden punkt do schowania
7. `mrugnij()` - wykonanie pojedyńczego mrugnięcia

### Stałe
* `LENGTH` - określa początkową długość węża
* `GROW_STEP` - określa co ile kroków ma zwiększać się długość ogona
* `CYCLE_TIME` - odstęp między krokami oraz mrugnięciami
* `MAXLEN` - definicja maksymalnej możliwej do zapisania długości węża (na potrzeby tablicy `pos`)
* `DIN`,`CS`,`CLK` - numery odpowiednich pinów

### Pozostałe funkcje
* `void rst()` - przywracanie węża do stanu początkowego
* `void znikanie()` - przechodzi całą długość węża wywołując `snakeHead(-1,-1)` przez co wąż znika

### Budowanie programu
Budowanie oraz wygrywanie kodu można łatwo wykonać z poziomu środowiska Arduino IDE.
#### Kompilacja
Aby poprawnie skompilować program potrzebna jest wspomniana już wcześniej biblioteka `LedControl`. Wchodzi ona w skład bibliotek dostępnych w Library Manager w Arduino.
#### Wgrywanie
Wgrywanie programu na Adafruit Trinket wymaga odpowienich pakietów do Arduino IDE. Wszystkie potrzebne pliki są dostępne pod tym linkiem: https://learn.adafruit.com/adafruit-arduino-ide-setup/arduino-1-dot-6-x-ide#easy-installation Po instalacji wybieramy płytkę `Adafruit Trinket 8MHz` oraz programator `USBtinyISP`. Teraz przytrzymujemy przycisk na płytce i wgrywamy program.
### Kod
``` Arduino
#include <LedControl.h>

#define LENGTH 5
#define GROW_STEP 16
#define CYCLE_TIME 500

//#define TIME //czas wykonania pelnego obiegu
//#define CYCLE_TIME TIME / (1 + 128 + 2*(64/GROW_STEP)) // czas kroku weza
#define MAXLEN 70 //okreslenie maksymalnej dlugosci ogona
//definicje pinow
#define DIN 4
#define CS 2
#define CLK 3
LedControl lc = LedControl(DIN, CLK, CS, 1);
int pos[MAXLEN][2];//historia pozycji glowy weza
int curr = 0;//aktualna dlugosc weza
int curr_spot = 1;//numer pola
void setup()
{
  for(int i=0;i<LENGTH;i++){
    pos[i][0]=-1;
    pos[i][1]=-1;
    curr++;
  }
  lc.shutdown(0, false);
  lc.setIntensity(0, 0);
  lc.clearDisplay(0);
}
void loop()
{
  spiralaDoSrodka();
  znikanie();
  snakeHead(-1,-1);
  spiralaOdSrodka();
  znikanie();
  snakeHead(-1,-1);//schowanie calosci (+1 przy CYCLE_TIME)
  mrugnij();
}
void spiralaDoSrodka(){
  rst();
  for(int i=0;i<4;i++){//clockwise
    for(int j=i;j<8-i;j++){//1
      snakeHead(i,j);
    }
    for(int j=i+1;j<8-i;j++){//2
      snakeHead(j,7-i);
    }
    for(int j=6-i;j>=i;j--){//3
      snakeHead(7-i,j);
    }
    for(int j=6-i;j>i;j--){//4
      snakeHead(j,i);
    }
  }
}
void spiralaOdSrodka(){
  rst();
  for (int i = 3; i >= 0; i--)
  {
    for(int j=7-i;j>i;j--){//3
      snakeHead(7-i,j-1);
    }
    for(int j=7-i;j>i;j--){//4
      snakeHead(j-1,i);
    }
    for(int j=i;j<7-i;j++){//1
      snakeHead(i,j+1);
    }
    for(int j=i;j<7-i;j++){//2
      snakeHead(j+1,7-i);
    }
  }
}
void rst(){
  curr_spot = 1;
  curr = LENGTH;
}
void znikanie(){
  for(int i=0;i<curr;i++){
    snakeHead(-1,-1);
  }
}
void snakeHead(int x, int y){
  //rysowanie ogona
  for(int i=0;i<curr-1;i++){//-1 zeby nie wyjsc poza tablice przy przestawianiu
    lc.setLed(0,pos[i][0],pos[i][1],true);
    //przestawianie ogona
    pos[i][0] = pos[i+1][0];
    pos[i][1] = pos[i+1][1];
  }
  lc.setLed(0,pos[curr-1][0],pos[curr-1][1],true);//to czego nie narysowalismy z powodu -1 wyzej
  //ustawianie glowy
  if(++curr_spot < 64 && curr_spot % GROW_STEP == 0) curr++;
  pos[curr-1][0] = x;
  pos[curr-1][1] = y;
  lc.setLed(0,x,y,true);
  //krok
  delay(CYCLE_TIME);
  lc.clearDisplay(0);
}
void mrugnij(){
  lc.clearDisplay(0);
  lc.shutdown(0,true);
  for(int i=0;i<8;i++){
    for(int j=0;j<8;j++){
      lc.setLed(0,i,j,true);
    }
  }
  delay(CYCLE_TIME);
  lc.shutdown(0,false);
  delay(CYCLE_TIME);
  lc.clearDisplay(0);
  delay(CYCLE_TIME);
}

```
