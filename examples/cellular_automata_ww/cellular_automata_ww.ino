/* LucidTronix Wearable Wayfinder Cellular Automata Animation
 * Code by Samwell Freeman August 2014
 * For instructions, details and schematic, See:
 * http://www.lucidtronix.com/tutorials/9
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

const int scanline_size = 20;
uint8_t scanline[scanline_size];
uint8_t nextline[scanline_size];
int cur_rule = 0;
const int num_rules = 8;
boolean rules[num_rules][8] = {
                                {0,1,0,1,1,0,1,0},
                                {0,0,0,1,1,1,1,0},
                                {1,0,1,1,0,1,1,0},
                                {0,1,1,1,1,1,1,0},
                                {0,1,0,1,0,1,1,0},
                                {1,0,0,1,1,0,0,1},
                                {1,1,0,0,0,0,0,1},
                                {0,0,0,1,1,0,1,0}
                              }; 

int cur_y = 0;
int mode = 0;
int num_modes = 4;
unsigned int last_press = 0;

void setup(){
  ww.initialize();
  int bx = 0;
  int by = 0;
  for (int i = 0; i < scanline_size; i++) {
    scanline[i] = 0;
    if ( i == scanline_size/2) scanline[i] = 0x08;
  }
}

void loop(){
  for (int i = 0; i < (scanline_size*8)-1; i++) {
    int left = 0;
    if (i > 0) left = get_bit_from_index(i-1);   // Left neighbor state
    int me = get_bit_from_index(i);       // Current state
    int right = get_bit_from_index(i+1);  // Right neighbor state
    boolean abit = get_bit_from_rules(left,me,right); // Compute next generation state based on ruleset
    set_bit_from_index(i, abit);
    if (mode == 0){
      int cur_r = 15;
      int cur_g = 2;
      int cur_b = random(200, 250);
      ww.background = Color(cur_r, cur_g, cur_b); 
      cur_r = 230;
      cur_g = 62;
      cur_b = 0;
      ww.fill = Color(cur_r, cur_g, cur_b); 
    }
    else if (mode == 1){
      int cur_r =  0;
      int cur_g =  0;
      int cur_b = 0;
      ww.background = Color(cur_r, cur_g, cur_b); 
      cur_r =  random(23,55);
      cur_g =   62;
      cur_b = 230;
      ww.fill = Color(cur_r, cur_g, cur_b); 
    }
    else if (mode == 2){
      int cur_r =  5;
      int cur_g = random(150,222);
      int cur_b = 215;
      ww.background = Color(cur_r, cur_g, cur_b); 
      cur_r = 219;
      cur_g = 200;
      cur_b = 30;
      ww.fill = Color(cur_r, cur_g, cur_b); 
    }
    if (abit) tft.drawPixel(cur_y, i, ww.fill.color_16());
    else tft.drawPixel(cur_y, i, ww.background.color_16());
    if(ww.btn_1_pressed() == HIGH && millis() - last_press > 500 ){
      cur_rule = ++cur_rule%num_rules;
      for (int i = 0; i < scanline_size; i++) {
        nextline[i] = 0;
        if ( i == scanline_size/2) nextline[i] = 0x08;
      }
      last_press = millis();
      break;
    }
    if(ww.btn_2_pressed() == HIGH ){
      ww.clear_screen(ww.background);
      for (int i = 0; i < scanline_size; i++) {
        nextline[i] = 0;
        if ( i == scanline_size/2) nextline[i] = 0x08;
      }
      mode = ++mode%num_modes;
      break;
    }
  }
  cur_y++;
  if (cur_y > 128) cur_y = 0;
  for (int i = 0; i < scanline_size; i++) {
    scanline[i] = nextline[i];
  }
}

boolean get_bit_from_index(int index){
  int byte_index = index / 8;
  int bit_index = 7 - (index % 8);
  byte cur_b = scanline[byte_index];
  if ( (cur_b & (1 << bit_index)) != 0x00 ) return 1;
  else return 0;
}

void set_bit_from_index(int index, boolean abit){
  int byte_index = index / 8;
  int bit_index = 7 - (index % 8);
  byte cur_b = nextline[byte_index];
  if ( abit)  cur_b |= (1 << bit_index);
  else cur_b &= ~(1 << bit_index) ;
  nextline[byte_index] = cur_b;
}

int get_bit_from_rules(int a, int b, int c) {
  if (a == 1 && b == 1 && c == 1) return rules[cur_rule][0];
  if (a == 1 && b == 1 && c == 0) return rules[cur_rule][1];
  if (a == 1 && b == 0 && c == 1) return rules[cur_rule][2];
  if (a == 1 && b == 0 && c == 0) return rules[cur_rule][3];
  if (a == 0 && b == 1 && c == 1) return rules[cur_rule][4];
  if (a == 0 && b == 1 && c == 0) return rules[cur_rule][5];
  if (a == 0 && b == 0 && c == 1) return rules[cur_rule][6];
  if (a == 0 && b == 0 && c == 0) return rules[cur_rule][7];
  return 0;
}

