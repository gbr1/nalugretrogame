//header per i font
#include <font4x6.h>
#include <font6x8.h>
#include <font8x8.h>
#include <font8x8ext.h>
#include <fontALL.h>

//librerie per il TVout
#include <TVout.h>
#include <video_gen.h>

//logo windows
#include "wl.h" 
//immagine Bill Gates
#include "bg.h"
//immagine pinguino
#include "tux.h"

//limiti schermo
#define xmax 120
#define ymax 96

//oggetto TV
TVout TV;

int pinr = A0;    //pin random
int pina = A1;    //pin controller
int pinb = A3;    //pin button

//posizione GNU
byte xg = 5;
byte yg = 0;

byte xb = 92;      //x bullet
byte yb = 0;      //y bullet

byte bstep=1;      //bullet step

long score=0;

//impostazioni
void setup() {
  TV.begin(PAL,xmax,ymax);      //or NTSC
  TV.select_font(font6x8);
  pinMode(pina,INPUT);
  pinMode(pinb,INPUT);
  randomSeed(analogRead(0));
  yb=random(1,94);
  title();						//funzione titolo
  counter();					//conteggio
  intro();						//schermata iniziale
}


void loop(){
  yg=map(analogRead(pina),0,1023,7,88);			//legge il potenziometro e mappa sullo schermo
  cBULLET();									//aggiorna il proiettile
  checkCRASH();									//controlla se c'è stato contatto
  checkSCORE();									//controlla il punteggio
  TV.clear_screen();							//cancella schermo
  drawGNU(xg,yg);								//desegna GNU
  drawBULLET(xb,yb);							//disegna proiettile
  TV.bitmap(94,35,wl);							//disegna il logo di Windows
  TV.delay_frame(1);							//aggiornamento ad un frame
}

//draw GNU spaceship
void drawGNU(byte x,byte y){
  TV.draw_line(x,y-6,x+3,y-6,WHITE);
  TV.draw_line(x-3,y-2,x,y-2,WHITE);
  TV.draw_line(x-3,y+2,x,y+2,WHITE);
  TV.draw_line(x,y+6,x+3,y+6,WHITE);
  TV.draw_line(x,y-5,x,y-3,WHITE);
  TV.draw_line(x-3,y-1,x-3,y+1,WHITE);
  TV.draw_line(x,y+3,x,y+5,WHITE);
}

//draw bullet
void drawBULLET(byte x, byte y){
  TV.draw_line(x-2,y,x+3,y,WHITE);
  TV.set_pixel(x+2,y-1,WHITE);
  TV.set_pixel(x+2,y+1,WHITE);
}

//bullet coordinates
void cBULLET(){          
  if (xb>5){
    xb=xb-bstep;
  }
  else{
    xb=100;
    yb=random(1,94);
    score=score+bstep;
  }
}

//check crash
void checkCRASH(){
  if ((xb<=8)&&(yb<=(yg+8))&&(yb>=(yg-8))){
    CRASH();
  }
}
  
//crash!!! -> loser
void CRASH(){
  TV.clear_screen();
  TV.select_font(font4x6);
  TV.print(0,0,"You are");
  TV.print(0,8,"a LOSER...");
  TV.print(0,18,"LINUX USER!");
  TV.print(0,30,"score:");
  TV.print(0,38,score);
  TV.bitmap(46,10,bg);
  TV.print(0,65,"press button");
  while(!button()){}
  boardrst();
} 

//title
void title(){
  for (int i=0; i<40; i++){
    TV.clear_screen();
    TV.select_font(font8x8);
    TV.print(40,i,"NaLUG");
    TV.delay_frame(2);
  }
  int i=0;
  while(!button()){
    if (i<120){
      TV.clear_screen();
      TV.draw_rect(33,20,20,2,WHITE,WHITE);
      TV.draw_rect(65,20,20,2,WHITE,WHITE);
      TV.draw_rect(54,30,10,2,WHITE,WHITE);
      TV.draw_rect(33,10,2,9,WHITE,WHITE);
      TV.draw_rect(83,10,2,9,WHITE,WHITE);
      TV.draw_rect(51,23,2,9,WHITE,WHITE);
      TV.draw_rect(65,23,2,9,WHITE,WHITE);
      TV.select_font(font8x8);
      TV.print(40,40,"NaLUG");
      if(i%2==1){
        TV.select_font(font4x6);
        TV.print(37,90,"press button");
      }
      drawGNU(i,70);
      TV.delay_frame(2);
      i++;
    }
    else{
      i=0;
    } 
  }
}

//button pressed -> true
boolean button(){
  return digitalRead(pinb);
}

//counter
void counter(){
  for(int i=5;i>=0;i--){
    TV.clear_screen();
    TV.select_font(font8x8);
    TV.print(55,50,i);
    TV.delay(1000);
  }  
}

//begin the game
void intro(){
  for(int i=0;i<xg;i++){
    yg=map(analogRead(pina),0,1023,7,88);
    TV.clear_screen();
    drawGNU(i,yg);
    TV.delay_frame(2);
  }
  for(int i=94;i>50;i--){
    yg=map(analogRead(pina),0,1023,7,88);
    TV.clear_screen();
    drawGNU(xg,yg);
    TV.bitmap(i,35,wl);
    TV.delay_frame(1);
  }
  for(int i=50;i<94;i++){
    yg=map(analogRead(pina),0,1023,7,88);
    TV.clear_screen();
    drawGNU(xg,yg);
    TV.bitmap(i,35,wl);
    TV.delay_frame(1);
  }     
}

//reset back to the title
void boardrst(){
  score=0;
  xb=92;
  yb=0;
  bstep=1;
  title();
  counter();
  intro();
}

//win
void WIN(){
  TV.clear_screen();
  TV.select_font(font8x8);
  TV.print(12,42,"FATAL ERROR");
  TV.draw_rect(7,40,98,12,WHITE,INVERT);
  TV.delay(3000);
  TV.clear_screen();
  TV.bitmap(39,15,tux);
  TV.select_font(font8x8);
  TV.print(1,5,"GOOD JOB!");
  TV.select_font(font4x6);
  TV.print(1,25,"score:");
  TV.print(1,33,score);
  TV.print(1,85,"press button");
  TV.draw_rect(0,0,120,96,WHITE,INVERT);
  while(!button()){}
  boardrst();
}

//check win
void checkSCORE(){
  if (score<10){
    bstep=1;
  }
  else{
    if (score<20){
      bstep=2;
    }
    else{
      if (score<40){
        bstep=3;
      }
      if (score<60){
        bstep=4;
      }
      
      else{
        if (score<80){
          bstep=5;
        }
        
        else{
          if (score<95){
            bstep=6;
          }
        }
        
      }
    }
  } 
  if (score>94){
    WIN();
  }
}


  

  
