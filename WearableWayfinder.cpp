/*
 *  WearableWayfinder.cpp
 *  
 *
 *  Created by Samwell Freeman on 6/21/14.
 *  Copyright 2014 Bdau Bdow. All rights reserved.
 *
 */
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "WearableWayfinder.h"

WearableWayfinder :: WearableWayfinder()
{

}

WearableWayfinder :: WearableWayfinder(Adafruit_ST7735 *a_tft)
{
  tft = a_tft;
  speaker_pin = 6;
  btn1_pin = 10;
  btn2_pin = 11;
  fill = Color(128, 128, 128);
  stroke = Color(0, 0, 0);
  background = Color(255, 255, 255);
}

void WearableWayfinder :: initialize()
{
  pinMode(btn1_pin,INPUT);
  pinMode(btn2_pin,INPUT); 
  pinMode(speaker_pin, OUTPUT); 
  
  Wire.begin(); // Start the I2C interface.

  initialize_compass();
  tft->initR(INITR_GREENTAB); // initialize a ST7735R chip, green tab
    
  clear_screen(background);
  
  tft->setCursor(4, 4);
  draw_string(3, 4, "wearable" , stroke.color_16(), 2);
  draw_string(6, 34, "wayfinder", stroke.color_16(), 2);
  tft->setCursor(30, 110);
  draw_string(30,105, "LucidTroniX", stroke.color_16(), 1);
  delay(1700);

  clear_screen(background);

}

void WearableWayfinder :: show_time(int ax, int ay)
{
  int text_size = 1;
  cur_time = clock.now();
  if (cur_time.hour() > 9) print_integer(ax, ay, cur_time.hour(), text_size, false);  
  else print_integer(ax+7, ay, cur_time.hour(), text_size, false);  
  draw_string(ax+(text_size*11), ay,":", ST7735_MAGENTA);
  if (cur_time.minute() < 10){
    print_integer(ax+(text_size*15),ay, 0, text_size, false);
    print_integer(ax+(text_size*15)+6,ay, cur_time.minute(), text_size, false);
  } else print_integer(ax+(text_size*15),ay, cur_time.minute(), text_size, false);
  if (last_time.minute() != cur_time.minute() ){
   tft->fillRect(ax, ay-2, text_size*42, text_size*10, ST7735_WHITE);  
  }
  draw_string(ax+(text_size*26), ay,":", ST7735_RED); 
  if (cur_time.second() < 10){
    print_integer(ax+(text_size*30),ay, 0, text_size, false);  
    print_integer(ax+(text_size*30)+7,ay, cur_time.second(), text_size, false);  
  }else print_integer(ax+(text_size*30),ay, cur_time.second(), text_size, false);  
  if (last_time.second() != cur_time.second() ){
   tft->fillRect(ax+(text_size*30), ay-2, text_size*12, text_size*10, ST7735_WHITE);  
  }
  last_time = cur_time;
}

void WearableWayfinder :: set_date_time(const char* date, const char* time)
{
  clock.setDateTime(date, time);
}

DateTime WearableWayfinder :: now()
{
  return clock.now();
}

void WearableWayfinder :: clear_screen(Color c)
{
  // clear weird edge pixels
  tft->fillRect(-2, -2, 5, 161, c.color_16());
  tft->fillRect(-2, -2, 131, 5, c.color_16());
  tft->fillScreen(c.color_16());
}

void WearableWayfinder :: initialize_compass()
{
  float scalar = 5.6;
  //not checking errors you should if you have issues.
  compass.SetScale(scalar); // Set the scale of the compass.
  compass.SetMeasurementMode(Measurement_Continuous); // Set the measurement mode to Continuous
}

void WearableWayfinder :: show_time(int ax, int ay, DateTime a_time)
{
  int text_size = 1;
  if (a_time.hour() > 9) print_integer(ax, ay, a_time.hour(), text_size, false);  
  else print_integer(ax+7, ay, a_time.hour(), text_size, false);  
  draw_string(ax+(text_size*11), ay,":", ST7735_MAGENTA);
  if (a_time.minute() < 10){
    print_integer(ax+(text_size*15),ay, 0, text_size, false);
    print_integer(ax+(text_size*15)+6,ay, a_time.minute(), text_size, false);
  } else print_integer(ax+(text_size*15),ay, a_time.minute(), text_size, false);
  
  draw_string(ax+(text_size*26), ay,":", ST7735_RED); 
  if (a_time.second() < 10){
    print_integer(ax+(text_size*30),ay, 0, text_size, false);  
    print_integer(ax+(text_size*30)+7,ay, a_time.second(), text_size, false);  
  }else print_integer(ax+(text_size*30),ay, a_time.second(), text_size, false);
}

void WearableWayfinder :: show_date(int ax, int ay)
{
  int text_size = 1;
  int cur_x = ax;
  int char_width = 6*text_size;
  print_integer(cur_x, ay, cur_time.month(), text_size, false);
  cur_x += cur_time.month()>9 ? (text_size*char_width*2) : (text_size*char_width);
  draw_string(cur_x, ay, "/", stroke.color_16()); 
  cur_x += char_width;
  print_integer(cur_x,ay, cur_time.day(), text_size, false);
  cur_x += cur_time.day()>9 ? (text_size*char_width*2) : (text_size*char_width);

  draw_string(cur_x, ay, "/20", stroke.color_16()); 
  cur_x += char_width*3;
  print_integer(cur_x, ay, cur_time.year(), text_size, false);
}

void WearableWayfinder :: show_heading(int ax, int ay)
{
  tft->fillRect(ax, ay, 20, 10, ST7735_WHITE);
  print_integer(ax, ay, get_heading(), 1, false);
}

int WearableWayfinder :: get_heading()
{
  MagnetometerRaw raw = compass.ReadRawAxis();
  // Retrived the scaled values from the compass (scaled to the configured scale).
  MagnetometerScaled scaled = compass.ReadScaledAxis();
  // Values are accessed like so:
  int MilliGauss_OnThe_XAxis = scaled.XAxis;// (or YAxis, or ZAxis)
  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading = atan2(scaled.YAxis, scaled.XAxis);
  float declinationAngle = 0.00404;
  heading += declinationAngle;
  // Correct for when signs are reversed.
  if(heading < 0) heading += 2*PI;
  // Check for wrap due to addition of declination.
  if(heading > 2*PI) heading -= 2*PI;
  int heading_degrees = (int)(heading * 180/M_PI); 
  return heading_degrees;
}


void WearableWayfinder :: show_accelerometer(int ax, int ay)
{
  tft->fillRect(ax, ay, 50, 40, ST7735_WHITE);
  tft->setCursor(ax, ay);
  tft->print("X:");
  print_integer(ax+15, ay, accelerometer.getX(), 1, false);
  
  tft->setCursor(ax, ay + 10);
  tft->print("Y:");
  print_integer(ax+15, ay+10, accelerometer.getY(), 1, false);
  
  tft->setCursor(ax, ay + 20);
  tft->print("Z:");
  print_integer(ax+15, ay+20, accelerometer.getZ(), 1, false);
}

int WearableWayfinder :: accelerometer_x()
{
  return accelerometer.getX();
}

int WearableWayfinder :: accelerometer_y()
{
  return accelerometer.getY();
}

int WearableWayfinder :: accelerometer_z()
{
  return accelerometer.getZ();
}


void WearableWayfinder :: draw_ray(int ax, int ay, int angle, int radius)
{
  float ray_x = sin(((float)angle*PI)/180.0f)*(float)radius;
  float ray_y = -cos(((float)angle*PI)/180.0f)*(float)radius;
  int c = get_color_from_angle(angle);
  tft->drawLine(ax,ay,ax+ray_x, ay+ray_y, c);
}

void WearableWayfinder :: draw_orbiter(int ax, int ay, int angle, int radius, int a_size, boolean erase)
{
  float ray_x = sin(((float)angle*PI)/180.0f)*(float)radius;
  float ray_y = -cos(((float)angle*PI)/180.0f)*(float)radius;
  int c = get_color_from_angle(angle);
  if(erase) tft->fillCircle(ax+ray_x, ay+ray_y, a_size+2, background.color_16());
  tft->fillCircle(ax+ray_x, ay+ray_y, a_size, c);
}

int WearableWayfinder :: get_color_from_angle(int angle){
  Color cur_color = Color(0,0,0);
  cur_color.convert_hcl_to_rgb((float)angle/360,0.95,0.7);
  return cur_color.color_16(); 
}

void WearableWayfinder :: draw_numbers(int color)
{
  draw_string(56, 24,"12",color,2);
  draw_string(110, 68,"3",color,2);
  draw_string(56, 124,"6",color,2);
  draw_string(12, 68,"9",color,2); 
}

void WearableWayfinder :: playTone(int tone, int duration) 
{
  for (long i = 0; i < duration * 1000L; i += tone * 2){
    digitalWrite(speaker_pin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speaker_pin, LOW);
    delayMicroseconds(tone);
  }
}

void WearableWayfinder :: playNote(char note, int duration) 
{
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };

  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}

void WearableWayfinder :: fill_ellipse(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, uint16_t color)
{
	uint8_t last_y = 0;
	for(float i = -PI/2; i < PI/2; i+= 0.01){
		int cur_y = h*sin(i);
		if (cur_y == last_y) continue;
		int cur_x = w*cos(i);
		tft->drawFastHLine(x0-cur_x, y0-cur_y, cur_x<<1, color);
		last_y = cur_y;
	}
}

void WearableWayfinder :: draw_string(int x, int y, String text)
{
  draw_string(x, y, text, stroke.color_16());
}


void WearableWayfinder :: draw_string(int x, int y, String text, uint16_t color, int text_size)
{
  tft->setTextSize(text_size);
  draw_string(x, y, text, color);
  tft->setTextSize(1);
}

void WearableWayfinder :: draw_string(int x, int y, String text, uint16_t color)
{
  tft->setTextColor(color);
  tft->setCursor(x, y);
  tft->print(text);
}

void WearableWayfinder :: print_integer(int ax, int ay, int to_print, int text_size, boolean hex)
{
  tft->setTextColor(stroke.color_16());
  tft->setTextSize(text_size);
  tft->setCursor(ax, ay);
  if(hex) tft->print(String(to_print, HEX));
  else tft->print(String(to_print));
}