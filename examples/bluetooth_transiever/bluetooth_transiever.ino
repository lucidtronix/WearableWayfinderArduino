/* LucidTronix Wearable Wayfinder 
 * Blue Tooth Transeiver
 * written by samwell freeman for LucidTronix
 * For instructions, details and schematic, See:
 * http://www.lucidtronix.com/tutorials/64
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

int old_pot = 0;
const int buffer_size = 18;
char buffer_in[buffer_size];
int char_index = 0;

void setup() {
  Serial1.begin(115200);    	//initialize Serial1
  Serial.begin(115200);    	//initialize Serial1

  for(int i = 0; i < buffer_size; i++) buffer_in[i] = 0;

  ww.initialize();
  ww.draw_string(5, 6, "BLUE", Color(30, 35, 255).color_16(), 3);
  ww.draw_string(15, 36, "TOOTH", Color(30, 35, 255).color_16(), 3);
  ww.draw_string(2, 76, "transiever", Color(30, 135, 235).color_16(), 2);
  
  delay(2500);
  ww.clear_screen();

}

void loop() {   
  if (Serial1.available() > 0){
    char cur_char;
    while((cur_char = Serial1.read()) != -1){
      buffer_in[char_index] = cur_char;
      char_index = ++char_index % buffer_size;
    }
    Serial1.print("received string:");
    Serial1.println(buffer_in);
    tft.fillRect(1, 145, 127, 10, ww.background.color_16());
    ww.draw_string(2, 145, buffer_in, ww.stroke.color_16(), 1);
  }
  
  if (ww.btn_2_pressed()){
    ww.clear_screen();
    Serial1.println("Btn 2 pressed");
    ww.draw_string(5, 106, "Btn 2 pressed", ww.stroke.color_16(), 1);
  }
  
  if (ww.btn_1_pressed()){
    ww.clear_screen();
    Serial1.println("Btn 1 pressed");
    ww.draw_string(5, 106, "Btn 1 pressed", ww.stroke.color_16(), 1);
  }

  int pot =  analogRead(0);
  if(abs(pot-old_pot) > 5) {
    old_pot = pot;
    tft.fillRect(10, 45, 50, 10, ww.background.color_16()); 
    ww.draw_string(5, 36, "Potentiometer", ww.stroke.color_16(), 1);
    ww.print_integer(10, 45, pot, 1);
    Serial1.print("Wheel Reeds:");
    Serial1.println(pot);
  }
}

