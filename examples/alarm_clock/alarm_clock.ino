/* LucidTronix Wearable Wayfinder Color Clock
 * For instructions, details and schematic, See:
 * http://www.lucidtronix.com/tutorials/29
 */

#include <WearableWayfinder.h>
#include <Adafruit_GFX.h> // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <Wire.h>
#include <SPI.h>
#include <Color.h>
#include <HMC5883L.h>
#include <MCP79410.h>
#include <I2C.h>
#include <MMA8453Q.h>

// for leo:
Adafruit_ST7735  tft = Adafruit_ST7735(SS, 9, 8);
WearableWayfinder ww = WearableWayfinder(&tft);

int speakerPin = 6;
int potPin = 0;
int cur_sec = 0;
int cur_min = 0;
int millisecond_six = 0;
unsigned int last_sec = 0;
unsigned int last_press = 0;
int old_val = 0;
char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
int cur_note = 0;

boolean alarm_on = false;
boolean set_minute = false;
int time_y = 4;
int alarm_y = 38;


DateTime cur_time, alarm_time;
void setup(){  
  Wire.begin(); // Start the I2C interface.
  ww.initialize();
  //ww.set_date_time(__DATE__, __TIME__);
  alarm_time = ww.now();
}
void loop(){
  cur_time = ww.now();
  ww.show_time(2, time_y,3);
  ww.show_time(80, alarm_y, alarm_time);
  ww.show_date(30, 130);
  if(set_minute) tft.drawLine(96, alarm_y+8, 106, alarm_y+8,  Color(0,255,0).color_16());
  else {
    tft.drawLine(80, alarm_y+8, 90, alarm_y+8, ww.background.color_16());
    if (alarm_time.hour()>9) tft.drawLine(80, alarm_y+8, 90, alarm_y+8, Color(255,0,0).color_16());
    else tft.drawLine(85, alarm_y+8, 90, alarm_y+8, Color(255,0,0).color_16());
  }
  check_buttons();
  if (alarm_on){
    ww.draw_string(2, alarm_y,"Alarm is ON", ww.stroke.color_16(), 1);
    if (alarm_time.hour() == cur_time.hour()
      && alarm_time.minute() == cur_time.minute()){
         ww.play_note(names[cur_note++], 300);
         if (cur_note == 8) cur_note = 0;
     }
  } else {
    analogWrite(speakerPin, 0);
    ww.draw_string(2, alarm_y,"Alarm is OFF", ww.stroke.color_16(), 1);
  }
}  

void check_buttons(){
 if (ww.btn_1_pressed() && millis() - last_press > 800){
   tft.fillRect(0, alarm_y, 128, 62, ST7735_WHITE);  
   alarm_on = !alarm_on;
   last_press = millis();
 }
 if (ww.btn_2_pressed() && millis() - last_press > 800){
   set_minute = !set_minute;
   tft.drawLine(80, alarm_y+8, 110, alarm_y+8, ST7735_WHITE);
   last_press = millis();
 }
 int val = analogRead(potPin);
 if (abs(val - old_val) > 1){
   tft.fillRect(64, alarm_y, 128, 12, ST7735_WHITE);  
   if(set_minute){
     val = map(val,0,1024,0,60);
     alarm_time = DateTime(cur_time.year(), cur_time.month(), cur_time.day(), 
                          alarm_time.hour(), val, 0 );
   } else { //set hour 
     val = map(val,0,1024,0,24);
     alarm_time = DateTime(cur_time.year(), cur_time.month(), cur_time.day(), 
                          val, alarm_time.minute(), 0 );
   } 
 }
 old_val = analogRead(potPin);
}
