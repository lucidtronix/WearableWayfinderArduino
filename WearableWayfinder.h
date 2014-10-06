/*
 *  WearableWayfinder.h
 *  
 *
 *  Created by Samwell Freeman on 6/21/14.
 *  2014 LucidTronix.
 * For instructions details and schematic, See:
 *   http://www.lucidtronix.com/products/11
 *
 */
 
#ifndef WearableWayfinder_h
#define WearableWayfinder_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <MCP79410.h>
#include <HMC5883L.h>
#include <I2C.h>
#include <MMA8453Q.h>
#include <Color.h>

class WearableWayfinder
{
  public:
    WearableWayfinder();
    WearableWayfinder(Adafruit_ST7735 *a_tft);
    void initialize();
    
    void show_time(int ax, int ay);
    void show_time(int ax, int ay, int text_size);
    void show_time(int ax, int ay, DateTime a_time);
    void show_time(int ax, int ay, DateTime a_time, int text_size);
    void set_date_time(const char* date, const char* time);
    void show_date(int ax, int ay);
    DateTime now();
    
    void initialize_compass();
    void show_heading(int ax, int ay);
    int get_heading();
    
    void show_accelerometer(int ax, int ay);
    int accelerometer_x();
    int accelerometer_y();
    int accelerometer_z();
    
    void draw_ray(int ax, int ay, int angle, int radius);
    void draw_orbiter(int ax, int ay, int angle, int radius, int a_size, boolean=false);
    int get_color_from_angle(int angle);
    void draw_numbers(int color);

    void clear_screen(Color=Color(255,255,255));

    void draw_string(int x, int y, String text, uint16_t color, int text_size);
    void draw_string(int x, int y, String text, uint16_t color);
    void draw_string(int x, int y, String text);
    void print_integer(int ax, int ay, int to_print, int text_size, boolean=false);
    void playTone(int tone, int duration);
    void playNote(char note, int duration);
    
    void fill_ellipse(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, uint16_t color);

    
    Color background, stroke, fill;
    int btn1_pin;
    int btn2_pin;
  private:
  
    Adafruit_ST7735 *tft;
    MCP79410 clock;
    HMC5883L compass;
    MMA8453Q accelerometer;
    DateTime cur_time, last_time;
    int speaker_pin;
};


#endif