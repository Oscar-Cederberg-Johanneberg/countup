/*inkluderar olika bibliotek (music.h är egengjord).*/
#include "LedControl.h"
#include "U8glib.h"
#include "music.h"

U8GLIB_SSD1306_128X64 oled(U8G_I2C_OPT_NONE); 

/* pin 12 är kopplad till DataIn
   pin 11 är kopplad till CLK
   pin 10 är kopplad till LOAD*/

LedControl lc = LedControl(12, 11, 10, 1);

#define BTN_PIN 2

unsigned long delaytime = 1;

void setup() {
  Serial.begin(9600);
  lc.shutdown(0, false);
  /* ljusstyrkan till medium */
  lc.setIntensity(0, 8);
  /* tömmer displayen */
  lc.clearDisplay(0);

 pinMode(buzzer, OUTPUT); // Set buzzer - pin 3 as an output
 pinMode(BTN_PIN, INPUT_PULLUP);
  
}
/*Våra integers för nedräknaren.* -1 innebär att den är släckt.*/
int s0 = 0;
int s1 = 0;
int s2 = -1;
int s3 = 0;
int s4 = 0;
int s5 = -1;
int s6 = 2;
int s7 = 0;
int i = 0;
bool continue_loop = false;
bool btn_debounce = false;

void draw(void) {
  /* Ändrar fonten till Helvetica storlek 10*/
  oled.setFont(u8g_font_helvB10);

  /*Texten som står på displayen*/
  oled.drawStr(6, 14, "Press the button");
  oled.drawStr(6, 27, ("to " + String(continue_loop ? "stop" : "start") + " the timer").c_str());
  oled.drawStr(33, 55, (String(s7) + String(s6) + ":" + String(s4) + String(s3) + ":" + String(s1) + String(s0)).c_str());
}

/*Här är vår fantastiska loop. Det är denna som styr nedräkningen.*/
void loop() {

  check_btn();
  // Refresh picture
  oled.firstPage();  
  do {
    draw();
   
  } while(oled.nextPage());

  /*Det som händer är att när siffran innan når 0 ska siffran efter minska med ett. Delayen i slutet är till för att det ska upplevas som sekunder.*/
  while (continue_loop) {
    check_btn();
    if (s6 == 0 && s4 == 0 && s3 == 0 && s1 == 0 && s0 == 0) {
      s7 -= 1;
      if (s7 < 0)
        s7 = 6;
    }

    if (s4 == 0 && s3 == 0 && s1 == 0 && s0 == 0) {
      s6 -= 1;
      if (s6 < 0)
        s6 = 9;
    }

    if (s3 == 0 && s1 == 0 && s0 == 0) {
      s4 -= 1;
      if (s4 < 0)
        s4 = 5;
    }

    if (s1 == 0 && s0 == 0) {
      s3 -= 1;
      if (s3 < 0)
        s3 = 9;
    }

    if (s0 == 0) {
      s1 -= 1;
      if (s1 < 0)
        s1 = 9;
      if (s0 == 0)
        s0 = 9;
    } else {
      s0 -= 1;
    }
    delay(10);

/*Vilka siffror på 7segment 8 digit displayen som ska lysa.*/
    lc.setChar(0, 0, s0 , false);
    lc.setChar(0, 1, s1 , false);
    lc.setChar(0, 2, s2 , false);
    lc.setChar(0, 3, s3 , false);
    lc.setChar(0, 4, s4 , false);
    lc.setChar(0, 5, s5 , false);
    lc.setChar(0, 6, s6 , false);
    lc.setChar(0, 7, s7 , false);

    if ((s0 + s1 + s3 + s4 + s6 + s7) == 0)
      continue_loop = false;
     
 }
 /*Vår belönande music som spelas när man klickar på knappen.*/
while (continue_loop == false && i > 1) {
        musicsetup();
        i -= 1;
 }
}
/*En hjälpfunktion åt vår knapp som används för att starta andra funktioner.*/
void check_btn(){
  if (digitalRead(BTN_PIN) == LOW) {
    if (!btn_debounce) {
     btn_debounce = true;
      continue_loop = !continue_loop;
          i = 2; 

    }
  } else
    btn_debounce = false;
}
