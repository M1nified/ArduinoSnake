#include "LedControl.h"
#define BUTTON 0
#define time 100 // czas kroku weza
#define len 5 //okreslenie dlugosci ogona
//definicje pinow
#define DIN 4
#define CS 2
#define CLK 3
LedControl lc = LedControl(DIN, CLK, CS, 1);
int pos[len][2];//historia pozycji glowy weza
int curr = 0;//aktualna dlugosc weza
void setup()
{
  //pinMode(BUTTON,INPUT);
  //digitalWrite(BUTTON,HIGH);
  // the zero refers to the MAX7219 number, it is zero for 1 chip
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, 0); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen
  //INTRO :)
  for(int i=0;i<8;i++){
    for(int j=0;j<8;j++){
      lc.setLed(0,i,j,true);
      delay(30);
    }
  }
  blink(5);
  lc.clearDisplay(0);
}
void loop()
{
  //if(digitalRead(BUTTON)){
  spiralaDoSrodka();
  znikanie();
  spiralaOdSrodka();
  znikanie();
  //}else{
  //zagubiony();
  //}
}
void spiralaDoSrodka(){
  for(int i=0;i<4;i++){
    for(int j=i;j<8-i;j++){
      snakeHead(i,j);
    }
    for(int j=i+1;j<8-i;j++){
      snakeHead(j,7-i);
    }
    for(int j=6-i;j>=i;j--){
      snakeHead(7-i,j);
    }
    for(int j=6-i;j>i;j--){
      snakeHead(j,i);
    }
  }
}
void spiralaOdSrodka(){
  for (int i = 3; i >= 0; i--)
  {
    for (int j = i + 1; j < 7 - i; j++)
    {//gorna krawedz
      snakeHead(i, j);
    }
    for (int j = i; j < 7 - i; j++)
    {//prawa krawedz
      snakeHead(j, 7 - i);
    }
    for (int j = 7 - i; j >= i; j--)
    {
      snakeHead(7 - i, j);
    }
    for (int j = 7 - i - 1; j >= i; j--)
    {
      snakeHead(j, i);
    }
  }
}
void znikanie(){
  //znikanie weza
  for(int i=0;i<curr;i++){
    snakeHead(-1,-1);
  }
}
void snakeHead(int x, int y){
  //rysowanie ogona
  for(int i=0;i<curr;i++){
    lc.setLed(0,pos[i][0],pos[i][1],true);
    if(i<curr-1){//przestawianie ogona
      pos[i][0] = pos[i+1][0];
      pos[i][1] = pos[i+1][1];
    }
  }
  //ustawianie glowy
  if(curr<len)curr++;
  pos[curr-1][0] = x;
  pos[curr-1][1] = y;
  lc.setLed(0,x,y,true);
  //krok
  delay(time);
  lc.clearDisplay(0);
}
void blink(int num){
  for(int i=0;i<num;i++){
    delay(300);
    lc.shutdown(0,true);
    delay(300);
    lc.shutdown(0,false);
  }
}
