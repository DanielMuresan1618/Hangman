//Displaying "HELLO" on LCD for Dragon12+ Trainer Board
//with HCS12 Serial Monitor Program installed. This code is for CodeWarrior IDE
//Modified from Mazidi's book with contribution from Travis Chandler
//On Dragon12+ LCD data pins of D7-D4 are connected to Pk5-Pk2, En=Pk1,and RS=Pk0,


#include <hidef.h>      /* common defines and macros */
#include "mc9s12dg256.h"      /* derivative-specific definitions */
#include <string.h>


#define LCD_DATA PORTK
#define LCD_CTRL PORTK
#define RS 0x01
#define EN 0x02
#define SIZE 10
#define ZGOMOT 50


void COMWRT4(unsigned char);
void DATWRT4(unsigned char);
void MSDelay(unsigned int);
void win();
void afis_init();
void lose();
void afis_vieti();
void resume();

int lives=5;
char spanz[11];//cuvantul ales random

void main(void){

char cuvinte[10][11]={"DANI\0","GEORDANI\0","PORTOCALA\0","CEAPA\0","UIUMIMW\0","DANI\0","GEORDANI\0","PORTOCALA\0","CEAPA\0","UIUMIMW\0"};
char alfabet[27]="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char aux[]="0";
char word_att[11]="";//cuvant incercare
int i,j,sel=0,randd=0,exitt=0,tofind;
/* randd - variabila care se incrementeaza pana sa apasa tasta

                                */



 DDRK = 0xFF;
    DDRH = 0x00;  //port butoane
    DDRB = 0xFF;    //PORTB as output since LEDs are connected to it
    DDRJ = 0x01;    //PTJ as output to control Dragon12+ LEDs
    PTJ=0x0F;        //Allow the LEDs to display data on PORTB pins
    DDRP=0x0F;
    PTP =  0x07;


  afis_vieti();
	  while (!exitt){
		  j=0;
      while ( PTH_PTH0==1) //randd se incrementeaza pana apas tasta dreapta
      if (randd>10)
      randd=0;
      else
		  randd++;

	  if (PTH_PTH0==0)
              while(PTH_PTH0==0)
                j++;
                if (j>ZGOMOT)
					exitt=1;
	  }

	  strcpy (spanz,cuvinte[randd%10]);
       tofind=strlen(spanz); //tofind - nr de liniute
    afis_init();
    for(;;){
    resume();
			if (tofind==0)
				win();
			if (lives==0)
				lose();

			  //apas butonul drept
			   j=0;
			  {MSDelay(1);
              if (PTH_PTH0==0)
              while(PTH_PTH0==0)
                j++;
                if (j>ZGOMOT)
               {
                        COMWRT4(0xC6);
               MSDelay(1);
              if (sel!=27)
                         sel++;
              else
                sel=0;
                DATWRT4(alfabet[sel]);
              }}

			  //apas butonul stang
			  j=0;
			  {MSDelay(1);
                 j=0;
              if (PTH_PTH2==0)
              while(PTH_PTH2==0)
                j++;
                if (j>ZGOMOT){
                COMWRT4(0xC6);
                 MSDelay(1);
              if (sel!=0)
              sel--;
              else
                sel=27;

                DATWRT4(alfabet[sel]);
              }}
			 //apas butonul de validare
			 j=0;
			 {MSDelay(1);
              if (PTH_PTH1==0)
              while(PTH_PTH1==0)
                j++;
                if (j>ZGOMOT){

	if (!strchr(spanz,alfabet[sel])) //daca n-am gasit litera in spanz
	lives--;
		else
	for (i=0;i<=strlen(spanz);i++)
							if(alfabet[sel]==spanz[i])
	//trebuie pozitionat cursorul LCD pe i
		{MSDelay(1);
	COMWRT4(128+i); //128= 0x80, i stie sa puna i unde trebuie
		MSDelay(1);
								DATWRT4(alfabet[sel]);
		tofind--;
								aux[0]=alfabet[sel];//aux converteste char alfabet[sel] in string pentru a putea alipi cu strcat

	if (strchr(word_att,alfabet[sel])) //decrementez vietile chiar daca litera e in cuvant
		lives--;
			else
				strcat(word_att,aux);
			MSDelay(1);
		}
	afis_vieti();
             }
	 }
    }
}
void resume(){
  //dupa actualizarea cuvantului, se revine la pozitia de selectare a literei
        COMWRT4(0xC6);
}

void afis_vieti(){
//functie de afisare pe segmente a vietilor
//segmentele sunt cu catod comun, deci se vor aprinde cu “1” logic
  if (lives==0)
    PORTB = 0x3F;
  else
    if (lives==1)
    PORTB = 0x06;
    else
    if (lives==2)
     PORTB = 0x5B;
    else
    if (lives==3)
    PORTB = 0x4F;
    else
      if (lives==4)
      PORTB = 0x66;
      else
      if (lives==5)
      PORTB = 0x6D;
      else
      PORTB = 0x7D;
}
void afis_init(){
//rutina de initializare pentru display si segmente
  int i;
   COMWRT4(0x33);   //reset sequence provided by data sheet
        MSDelay(1);
        COMWRT4(0x32);   //reset sequence provided by data sheet
        MSDelay(1);
        COMWRT4(0x28);   //Function set to four bit data length
                                         //2 line, 5 x 7 dot format
        MSDelay(1);
        COMWRT4(0x06);  //entry mode set, increment, no shift
        MSDelay(1);
        COMWRT4(0x0E);  //Display set, disp on, cursor on, blink off
        MSDelay(1);
        COMWRT4(0x01);  //Clear display
        MSDelay(1);
        COMWRT4(0x80);
        MSDelay(1);
        for (i=0;i<strlen(spanz);i++){
            MSDelay(1);
            COMWRT4(128+i);
        DATWRT4('_');
      }

        COMWRT4(0xC6); //pozitia unde se vor afisa literele de selectat
        DATWRT4('A');
}
void lose(){
	MSDelay(1);
				COMWRT4(0x80);
				MSDelay(1);
				DATWRT4('A');
				MSDelay(1);
				COMWRT4(0x81);
				MSDelay(1);
				DATWRT4('I');
				MSDelay(1);
				COMWRT4(0x82);
				MSDelay(1);
				DATWRT4(' ');
				MSDelay(1);
				COMWRT4(0x83);
				MSDelay(1);
				DATWRT4('P');
				MSDelay(1);
				COMWRT4(0x84);
				MSDelay(1);
				DATWRT4('I');
				MSDelay(1);
				COMWRT4(0x85);
				MSDelay(1);
				DATWRT4('E');
				MSDelay(1);
				COMWRT4(0x86);
				MSDelay(1);
				DATWRT4('R');
				MSDelay(1);
				COMWRT4(0x87);
				MSDelay(1);
				DATWRT4('D');
				MSDelay(1);
				COMWRT4(0x88);
				MSDelay(1);
				DATWRT4('U');
				MSDelay(1);
				COMWRT4(0x89);
				MSDelay(1);
				DATWRT4('T');

}
void win(){
				MSDelay(1);
				COMWRT4(0x80);
				MSDelay(1);
				DATWRT4('A');
				MSDelay(1);
				COMWRT4(0x81);
				MSDelay(1);
				DATWRT4('I');
				MSDelay(1);
				COMWRT4(0x82);
				MSDelay(1);
				DATWRT4(' ');
				MSDelay(1);
				COMWRT4(0x83);
				MSDelay(1);
				DATWRT4('C');
				MSDelay(1);
				COMWRT4(0x84);
				MSDelay(1);
				DATWRT4('A');
				MSDelay(1);
				COMWRT4(0x85);
				MSDelay(1);
				DATWRT4('S');
				MSDelay(1);
				COMWRT4(0x86);
				MSDelay(1);
				DATWRT4('T');
				MSDelay(1);
				COMWRT4(0x87);
				MSDelay(1);
				DATWRT4('I');
				MSDelay(1);
				COMWRT4(0x88);
				MSDelay(1);
				DATWRT4('G');
				MSDelay(1);
				COMWRT4(0x89);
				MSDelay(1);
				DATWRT4('A');
				MSDelay(1);
				COMWRT4(0x8A);
				MSDelay(1);
				DATWRT4('T');

			}
void COMWRT4(unsigned char command){
        unsigned char x;

        x = (command & 0xF0) >> 2;         //shift high nibble to center of byte for Pk5-Pk2
      LCD_DATA =LCD_DATA & ~0x3C;          //clear bits Pk5-Pk2
        LCD_DATA = LCD_DATA | x;          //sends high nibble to PORTK
        MSDelay(1);
        LCD_CTRL = LCD_CTRL & ~RS;         //set RS to command (RS=0)
        MSDelay(1);
        LCD_CTRL = LCD_CTRL | EN;          //rais enable
        MSDelay(5);
        LCD_CTRL = LCD_CTRL & ~EN;         //Drop enable to capture command
        MSDelay(15);                       //wait
        x = (command & 0x0F)<< 2;          // shift low nibble to center of byte for Pk5-Pk2
        LCD_DATA =LCD_DATA & ~0x3C;         //clear bits Pk5-Pk2
        LCD_DATA =LCD_DATA | x;             //send low nibble to PORTK
        LCD_CTRL = LCD_CTRL | EN;          //rais enable
        MSDelay(5);
        LCD_CTRL = LCD_CTRL & ~EN;         //drop enable to capture command
        MSDelay(15);
  }
 void DATWRT4(unsigned char data){
  unsigned char x;
        x = (data & 0xF0) >> 2;
        LCD_DATA =LCD_DATA & ~0x3C;
        LCD_DATA = LCD_DATA | x;
        MSDelay(1);
        LCD_CTRL = LCD_CTRL | RS;
        MSDelay(1);
        LCD_CTRL = LCD_CTRL | EN;
        MSDelay(1);
        LCD_CTRL = LCD_CTRL & ~EN;
        MSDelay(5);

        x = (data & 0x0F)<< 2;
        LCD_DATA =LCD_DATA & ~0x3C;
        LCD_DATA = LCD_DATA | x;
        LCD_CTRL = LCD_CTRL | EN;
        MSDelay(1);
        LCD_CTRL = LCD_CTRL & ~EN;
        MSDelay(15);
  }
 void MSDelay(unsigned int itime){
    unsigned int i; unsigned int j;
    for(i=0;i<itime;i++)
      for(j=0;j<4000;j++);
 }
