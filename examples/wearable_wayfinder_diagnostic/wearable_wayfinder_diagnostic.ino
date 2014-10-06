/* LucidTronix Wearable Wayfinder
 * For instructions, details and schematic, See:
 * http://www.lucidtronix.com/tutorials/56
 */

#include <WearableWayfinder.h>
#include <Adafruit_GFX.h>    // Core graphics library
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
int cur_note = 0;
int old_pot = 0;
char notes[] = {'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };

void setup(){  
  Wire.begin(); // Start the I2C interface.
  ww.initialize();
  //ww.set_date_time(__DATE__, __TIME__);
}
void loop(){
  ww.draw_string(5, 4, "Time", ww.stroke.color_16(), 1);
  ww.show_time(10, 15);
  ww.draw_string(5, 36, "Date", ww.stroke.color_16(), 1);
  ww.show_date(10, 45);
  ww.draw_string(5, 66, "Compass", ww.stroke.color_16(), 1);
  ww.show_heading(10, 75);
  ww.draw_string(5, 91, "Accelerometer", ww.stroke.color_16(), 1);
  ww.show_accelerometer(10, 100);
  ww.draw_string(5, 136, "Potentiometer", ww.stroke.color_16(), 1);
  int pot =  analogRead(0);
  if(pot != old_pot) tft.fillRect(10, 145, 50, 10, ww.background.color_16()); 
  ww.print_integer(10, 145, pot, 1);
  old_pot = pot;
  if(digitalRead(ww.btn1_pin) == HIGH){
    ww.playNote(notes[cur_note++], 300);
    if (cur_note == 8) cur_note = 0;
  } 
  if(digitalRead(ww.btn2_pin) == HIGH){
    ww.stroke = Color(random(255), random(255), random(255));
  } 
}  






