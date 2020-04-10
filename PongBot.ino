  /*
   * Program for a double pong playing bot for the Arduino Uno
   * 
   * This code assumes you are using the MAX7219 LED Dot Matrix. If you have the standard one, you may need to reprogram.
   * 
   * Pins:
   * 
   * 9 - Piezo
   * 10 - CS (LED Matrix)
   * 11 - CLK (LED Matrix)
   * 12 - DIN (LED Matrix)
   * 
   * Other notes:
   * - The dot matrix is placed with the pins facing upwards.
   * - Yes I know the programming is very messy
   * 
   * Created by: u/Ibbs620
   */
  
  #include "LedControl.h"
  
  LedControl lc=LedControl(12,11,10,1);
  
  int ball[6] = {0,0,16,0,0,0};
  int p1 = 28;
  int p2 = 28;
  int pad1 = 3;
  int pad2 = 3;
  int a[8] = {p1,0,0,0,0,0,0,p2};
  int bx = 2;
  int by = 4;
  int dir[2] = {1,1};
  int it = 0;
  bool hard = false;
  int num;
  int del = 250;
  int prev = 16;
  bool turn = false;
  
  void setup() {
    lc.shutdown(0,false);
    /* Set the brightness to a medium values */
    lc.setIntensity(0,8);
    /* and clear the display */
    lc.clearDisplay(0);
    Serial.begin(9600);
    pinMode(2, INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);
    pinMode(5, INPUT_PULLUP);
    pinMode(6, INPUT_PULLUP);
    pinMode(9, OUTPUT); 
  }
  
  void matrix(int a[8]) {
    for(int i = 0; i < 8; i++){
      lc.setRow(0,i,a[i]);
    }
  }
  
  void loop() {
    noTone(9);
    int s;
    if(by > pad2 && p2 < 224 && turn){
      p2 *= 2;
      pad2++;
    } else if(by < pad2 && p2 > 7 && turn){
      p2 /= 2;
      pad2--;
    }
    if(by > pad1 && p1 < 224 && !turn){
      p1 *= 2;
      pad1++;
    } else if(by < pad1 && p1 > 7 && !turn){
      p1 /= 2;
      pad1--;
    }
    if(bx == 0 && abs(pad1 - by) <= 1){
      turn = true;
      dir[0] *= -1;
      num = random(4);
      tone(9, 1000);
      
      if(pad1 == by && num == 1) {
        hard = true;
        tone(9, 1500);
      }
      else hard = false;
      delay(50);
      noTone(9);   
    } else if(bx == 0 && abs(pad1 - by) == 2 && ! hard && (dir[1] == -1 && by < pad1 || dir[1] == 1 && by > pad1)){
      dir[0] *= -1;
      dir[1] *= -1; 
      tone(9, 1000);
      delay(50);
      noTone(9);
      hard = false;
    }
    
    if(bx == 5 && abs(pad2 - by) <= 1){
      turn = false;
      dir[0] *= -1;
      num = random(4);  
      tone(9, 1000);
      if(pad2 == by && num == 1) {
        hard = true;  
        tone(9, 1500);
      }
      else hard = false;
      delay(50);
      noTone(9);
    } else if(bx == 5 && abs(pad2 - by) == 2 && ! hard && (dir[1] == -1 && by < pad2 || dir[1] == 1 && by > pad2)){
      dir[0] *= -1;
      dir[1] *= -1; 
      tone(9, 1000);
      delay(50);
      noTone(9);
      hard = false;
    }
    
    
    if((ball[bx] >= 128 || ball[bx] <= 1)){
      dir[1] *= -1;
    }
    
    if(dir[0] == 1){
      bx++;
      s = ball[bx];
      ball[bx] = ball[bx - 1];
      ball[bx - 1] = s; 
    } else {
      bx--;
      s = ball[bx];
      ball[bx] = ball[bx + 1];
      ball[bx + 1] = s;   
    }
    
    if(hard) goto cont;
      if(dir[1] == 1){
        ball[bx] /= 2;
        prev = ball[bx];
      } else {
        ball[bx] *= 2;  
      }
    by += dir[1] * -1;
    cont:
    for(int i = 0; i < 6; i++){
      a[i+1] = ball[i];  
    }
    a[0] = p1;
    a[7] = p2;
    
    matrix(a);
    if(it == 0) delay(750);
    it++;
    if(bx == 6 || bx == -1){
      if(bx == 6){
        if(abs(by - pad2) > 1)a[7] = p2 + prev;
        else a[7] = p2;
      }
      if(bx == -1){
        if(abs(by - pad1) > 1)a[0] = p1 + prev;
        else a[0] = p1;   
      }
      for(int i = 0; i < 6; i++) a[i+1] = 0;
      matrix(a);
      tone(9, 1000);
      delay(100);
      tone(9, 900);
      delay(100);
      tone(9, 800);
      delay(100);
      tone(9, 700);
      delay(100);
      tone(9, 600);
      delay(500);
      noTone(9);
      lc.clearDisplay(0);
      delay(500);
      ball[0] = 0;
      ball[1] = 0;
      ball[2] = 16;
      ball[3] = 0;
      ball[4] = 0;
      ball[5] = 0;
      p1 = 28;
      p2 = 28;
      pad1 = 3;
      pad2 = 3;
      for(int i = 1; i < 7; i++ )a[i] = 0;
      a[0] = p1;
      a[7] = p2;
      bx = 2;
      by = 4;
      it = 0;
      del = 250;
      hard = true;
      randomSeed(analogRead(A0));
      num = random(4);
      switch(num){
      case 0:
        dir[0] = 1;
        dir[1] = -1;
        break;
      case 1:
        dir[0] = 1;
        dir[1] = 1;
        break;
      case 2:
        dir[0] = -1;
        dir[1] = -1;
        break;
      case 3:
        dir[0] = -1;
        dir[1] = 1;
        break;  
      }
    } 
    prev = ball[bx];
    delay(50);
  }
