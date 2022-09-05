

#include <Arduino.h>
#include <U8g2lib.h>

#include "INA226.h"
#include "Wire.h"

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

INA226 INA(0x40);


U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* CS=*/ 10, /* reset=*/ 8);

void setup() {
    u8g2.begin();  
      Wire.begin();
  INA.setMaxCurrentShunt(1, 0.002);
  INA.setAverage(10);
    pinMode(7, OUTPUT);

          digitalWrite(7,LOW);
          //analogReference(INTERNAL);

}

void drawBattery ( int percent ) {
  int base_x = 100;
  int base_y = 7;
  int outline_width = 26;
  int outline_height = 45;

  int inner_v_padding=3;
  int inner_h_padding=3;
  int inner_width = outline_width-inner_h_padding*2;
  int inner_height = outline_height-inner_v_padding*2;
  /*
  int inner_width = 16;
  int inner_height = 36;
  */
  int plus_height = 6;
  int plus_width = 10;
  //outline
  ///Top battery "+"
  int plus_base_x = base_x + outline_width / 2 - plus_width / 2;
  int plus_base_y = base_y - plus_height; 
  u8g2.drawHLine(plus_base_x, plus_base_y,plus_width+1);
  u8g2.drawVLine(plus_base_x,plus_base_y,plus_height);
  u8g2.drawVLine(plus_base_x+plus_width, plus_base_y,plus_height);
  ///Main borders
  u8g2.drawHLine(base_x,base_y,outline_width); //top outline
  u8g2.drawHLine(base_x,outline_height+base_y,outline_width); //bottom outline
  u8g2.drawVLine(base_x,base_y,outline_height); //left outline
  u8g2.drawVLine(base_x+outline_width,base_y,outline_height+1); //right outline
  //inner
  u8g2.drawHLine(base_x+inner_h_padding,base_y+inner_v_padding,inner_width); //top inner
  u8g2.drawHLine(base_x+inner_h_padding,base_y+inner_height+inner_v_padding,inner_width+1); //bottom inner
  u8g2.drawVLine(base_x+inner_v_padding,base_y+inner_v_padding,inner_height); //left inner
  u8g2.drawVLine(base_x+inner_width+inner_h_padding,base_y+inner_v_padding,inner_height); //right inner

  //Fill
  for(int i=percent; i>0;i--) {
      float decimal_percent = i * 0.01;
      float amount_f = inner_height*decimal_percent;
      int amount = floor(amount_f);
      u8g2.drawHLine(base_x+inner_h_padding,
      base_y+inner_height+inner_v_padding - amount,
      inner_width+1);
  }
  // display number
       u8g2.setFont(u8g2_font_u8glib_4_tf);
       char str_percent[3];
        sprintf(str_percent, "%d %%",percent);
       u8g2.drawStr(base_x+outline_width/2-5,base_y+outline_height+10,str_percent);
  
}

void printCharging(int x, int y, bool clear = false) {
  
}
void printUsb(bool enable) {

if(enable) {
  u8g2.setDrawColor(1);
  u8g2.drawFrame(10,40,50,10);
  u8g2.drawStr(12,47,"USB CHARGE");
}
else {
  u8g2.setDrawColor(0);
  u8g2.drawBox(10,40,50,10);
  u8g2.setDrawColor(1);
}
  
}


 static uint32_t oldtime;
            static uint32_t currentTime;
            bool isCharging=false;
                  static bool screenOff=true;

void loop() {
  // put your main code here, to run repeatedly:
      char str_consuming[5];
      char str_hours[25];

     
      //float power = INA.getPower_mW();
      //char str_power[6];
      //dtostrf(power, 4, 2, str_consuming);
      //sprintf(str_consuming,"%s",str_power);
      //sprintf(str_consuming, "123", consuming);
     u8g2.setFont(u8g2_font_u8glib_4_tf);
   
    for (int i =0; i<100; i++){
   
        u8g2.firstPage();
      do{
        
      /*float power = INA.getPower_mW();
      if(power<0) power=0;
      power*=5;
      */
      float power = analogRead(A5);
      /*
       * Датчик струму
       * 518-519 ничего не подклю
       */
       
       if((power<1000) && (isCharging==true)){
        //remove frame
        //set timer for screen
        oldtime=millis();
        //set screen to on
        screenOff=false;
        //set internal status flag
        isCharging=false;
       }
       else if( (power>=1000) && (isCharging==false)) {
        oldtime=millis();
        screenOff=false;
        isCharging=true;
       }
       if(isCharging) {
        
       }
       else {
                printUsb(false);

       }
       currentTime=millis();
       if((currentTime-oldtime)>5000){
        screenOff=true;
       }
       if(screenOff){
        digitalWrite(7,HIGH);
       }
       else {
        digitalWrite(7,LOW);
       }
       
      /*
      if(power<3){
        digitalWrite(7, HIGH);
      }
      else {
        digitalWrite(7,LOW);
      }
      */
      //float power = INA.getPower_mW();
      //float power=12.321;
      String power_string = String(power);
      const char* power_char = power_string.c_str();
      //dtostrf(power, 4, 2, str_consuming);
      
      u8g2.drawStr(0,4,"CONSUMING");
      u8g2.setFont(u8g2_font_t0_11b_tf);
      u8g2.drawStr(20,14,power_char);
      u8g2.setFont(u8g2_font_u8glib_4_tf);
      u8g2.drawStr(0,24,"HOURS TO CHARGE: 5.5 h");
      
      drawBattery(i);
      
        
        } while ( u8g2.nextPage() );
      delay(1000);
    }
  

}
