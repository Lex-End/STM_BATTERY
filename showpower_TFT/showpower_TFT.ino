#include <Adafruit_GFX.h>    // Core graphics library
#include <MCUFRIEND_kbv.h>   // Hardware-specific library
#include <Adafruit_INA219.h>
MCUFRIEND_kbv tft;

#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>

#include <FreeDefaultFonts.h>

#define BLACK   0x0000
#define RED     0xF800
#define GREEN   0x07E0
#define WHITE   0xFFFF
#define GREY    0x8410
#define BLUE    0x02BF
#define YELLOW  0xFFE0
#define ORANGE  0xFB80
Adafruit_INA219 sensor219;
bool usbStatus[16]={};

void printConsumingPower(int watts) {
    int base_y=10;
    showmsgXY(0,base_y-10,1,NULL,"Consuming power");
    tft.fillRect(0,base_y,160,50,BLACK);
    char str_watt[3];
    sprintf(str_watt, "%04i",watts);
    showmsgXY(0,base_y+50, 1, &FreeSevenSegNumFont, str_watt);
    showmsgXY(130,base_y+5,6,NULL,"W");
}

void printUSBA() {
  tft.fillRect(80,175,50,22,RED);
  tft.fillRect(81,176,48,20,0x047F);
  showmsgXY(81,177,2,NULL,"USBA",ORANGE);
}
void removeUSBA(){
  tft.fillRect(80,175,50,22,BLACK);
}
void printUSBC() {
  tft.fillRect(10,175,50,22,RED);
  tft.fillRect(11,176,48,20,0x047F);
  showmsgXY(11,177,2,NULL,"USBC",ORANGE);
}
void removeUSBC(){
  tft.fillRect(10,175,50,22,BLACK);
}

void print220_1() {
  tft.fillRect(10,150,50,22,RED);
  tft.fillRect(11,151,48,20,0x047F);
  showmsgXY(11,152,2,NULL,"220V",ORANGE);
}
void remove220_1(){
  tft.fillRect(10,150,50,22,BLACK);
}

void print220_2() {
  tft.fillRect(80,150,50,22,RED);
  tft.fillRect(81,151,48,20,0x047F);
  showmsgXY(81,152,2,NULL,"220V",ORANGE);
}
void remove220_2(){
  tft.fillRect(80,150,50,22,BLACK);
}

void printChargeTime(){
   int base_y=120;
   tft.fillRect(20,base_y-10,10,100,BLACK);
  showmsgXY(10,base_y-20,2,NULL,"Charging time: 5h");
}
void printCharging() {
  int base_x=10;
  int base_y=120;
  tft.fillRect(base_x,base_y,120,22,RED);
  tft.fillRect(base_x+1,base_y+1,118,20,0x047F);
  showmsgXY(base_x+2+10,base_y+2,2,NULL,"CHARGING",ORANGE);
}
void removeCharging(){
    int base_x=10;
    int base_y=120;
  tft.fillRect(base_x,base_y,120,22,BLACK);
}
void checkUsbStatus(){
  for(int i=0;i<16;i++){
    if(analogRead(A8)-analogRead(A9)>200){
      usbStatus[i]=true;
    }
    else {
      usbStatus[i]=false;
    }
  }
}
void printUsbStatus() {
    for(int i=0;i<16;i++){
      if(usbStatus[i]==true) {
        tft.drawRect(20+10*i,220,10,10,GREEN);
      }
      else {
        tft.drawRect(20+10*i,220,10,10,RED);
      }
    }
}
void printFlag(void) {
  tft.fillScreen(BLACK);
  tft.fillRect(0,0,320,120,BLUE);
  tft.fillRect(0,120,320,120,YELLOW);
}
void setup(void)
{
    Serial.begin(9600);
    uint16_t ID = tft.readID();
    Serial.println("Example: Font_simple");
    Serial.print("found ID = 0x");
    Serial.println(ID, HEX);
    if (ID == 0xD3D3) ID = 0x9481; //force ID if write-only display
    tft.begin(ID);
    tft.setRotation(1);
    tft.fillScreen(BLACK);
    //analogReference(AR_INTERNAL);
    Serial.begin(9600);    
  sensor219.begin();
  printFlag();
      tft.fillScreen(BLACK);
}
void drawBatteryLines(int base_x,int base_y){
  /*
   * drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) { fillRect(x, y, 1, h, color); }
  drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
   */
   //top
   tft.drawFastHLine(base_x,base_y+10,60,BLUE);
   ///top-plus-left
   tft.drawFastVLine(base_x+15,base_y,10,BLUE);
   ///top-plus-top
   tft.drawFastHLine(base_x+15,base_y,30,BLUE);
   ///top-plus-right
   tft.drawFastVLine(base_x+45,base_y,10,BLUE);
   //bottom
   tft.drawFastHLine(base_x,115+base_y,60,BLUE);
   //left
  tft.drawFastVLine(base_x,base_y+10,105,BLUE);
  //right
  tft.drawFastVLine(base_x+60,base_y+10,105,BLUE);
}

void drawBattery (int percent) {
    int base_x=230;
    int base_y=40;
    tft.fillRect(base_x-5,base_y,160,240,BLACK); //clear battery (half screen)
    drawBatteryLines(base_x,base_y);
    tft.fillRect(base_x+5,base_y+110,51,-percent,GREEN);
    char str_percent[3];
    sprintf(str_percent, "%02i",percent);
    showmsgXY(base_x-5, base_y+180, 1, &FreeSevenSegNumFont, str_percent);
    showmsgXY(base_x+65,base_y+145,3,NULL,"%");
}

void loop(void)
{
    
  //float power = 0;
  //power = busVoltage * (current/1000); // Calculate the Power
    //voltage
    /*
    float busVoltage = 0;
    char str_busVoltage[6];
    char str_voltage[4];
    busVoltage = sensor219.getBusVoltage_V();
    Serial.print("voltage:");
    Serial.println(busVoltage);
    dtostrf(busVoltage, 4, 2, str_busVoltage); //dtostrf(float_value, min_width, num_digits_after_decimal, where_to_store_string)
    sprintf(str_voltage, "%s",str_busVoltage);
    showmsgXY(5, 10, 1, NULL, "Voltage");
    showmsgXY(5, 25, 1, NULL,str_voltage);
    
    //current
    float current = 0; // Measure in milli amps
    char str_busCurrent[6];
    char str_current[4];
    current = sensor219.getCurrent_mA();
     Serial.print("current:");
    Serial.println(current);
    dtostrf(current, 4, 2, str_busCurrent);
    sprintf(str_current, "%s",     str_busCurrent);
    showmsgXY(5, 35, 1, NULL, "Current");
    showmsgXY(5, 50, 1, NULL,str_current);
    delay(1000);
    */
    /*
                tft.fillRect(5,10,200,200,BLACK);
                checkUsbStatus();
                printUsbStatus();
                */
                
     for(int i=99; i>0;i=i-5){
          drawBattery(i);
          printConsumingPower(random(1,1000));
          printChargeTime();
          if(i%2==0){
                printCharging();
                if(random(0,2))
                print220_1();
                 if(random(0,2))
                print220_2();  
                                if(random(0,2))
                                printUSBA();
                                if(random(0,2))
                                printUSBC();
          }
          else {
                removeCharging();
                remove220_1();
                remove220_2();
                removeUSBA();
                removeUSBC();

          }
          delay(500);
    }
    

    //}


}
void showmsgXY(int x, int y, int sz, const GFXfont *f, const char *msg){
  showmsgXY(x,y,sz,f,msg,GREEN);
}
void showmsgXY(int x, int y, int sz, const GFXfont *f, const char *msg, int color)
{
    int16_t x1, y1;
    uint16_t wid, ht;
    //tft.drawFastHLine(0, y, tft.width(), WHITE);
    tft.setFont(f);
    tft.setCursor(x, y);
    tft.setTextColor(color);
    tft.setTextSize(sz);
    tft.print(msg);
}

char *i2str(int i, char *buf){
  byte l=0;
  if(i<0) buf[l++]='-';
  boolean leadingZ=true;
  for(int div=10000, mod=0; div>0; div/=10){
    mod=i%div;
    i/=div;
    if(!leadingZ || i!=0){
       leadingZ=false;
       buf[l++]=i+'0';
    }
    i=mod;
  }
  buf[l]=0;
  return buf;
}
