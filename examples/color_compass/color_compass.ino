/* LucidTronix Wearable Wayfinder color compass 
 * Rainbow colored wearable navigator 
 * For instructions, details and schematic, See:
 * http://www.lucidtronix.com/tutorials/30
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

String directions[] = {"North", "North-west", "West", "South-west", "South", "South-east", "East", "North-east"};
const int num_directions = 8;
int old_direction_index = 0;
float old_heading = 0;

void setup(){
  Wire.begin(); // Start the I2C interface.
  ww.initialize();
}

void loop(){
  ww.draw_string(5, 3, "Compass", ww.stroke.color_16(), 2);
  float heading_degrees = ww.get_heading();
  if(abs(heading_degrees - old_heading) > 3 ){
     tft.fillRect(22, 38, 82, 82, ww.background.color_16()); 
  }
  old_heading = heading_degrees;
  ww.draw_ray(64,80, heading_degrees, 40);
  ww.draw_orbiter(64,80, heading_degrees, 22,6);
  ww.draw_orbiter(64,80, heading_degrees, 32,3);
  
  int direction_index = map(heading_degrees, 0, 359, 0, num_directions);
  if(old_direction_index != direction_index){
    tft.fillRect(2, 128, 108, 12, ww.background.color_16()); 
    ww.draw_string(5, 128, directions[direction_index], ww.stroke.color_16(), 1);
  }
  old_direction_index = direction_index; 
  ww.show_heading(10, 145);
  if (ww.btn_1_pressed()){
   tft.fillScreen(ww.background.color_16());  
 }
}

