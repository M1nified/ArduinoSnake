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
