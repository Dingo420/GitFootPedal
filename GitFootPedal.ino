/*Mode of opperation
 Post, And setup hardware
 Show Welcome screen
 Check Encoder Pos
 - Show "Menu Screen" (Select macro)
 - Wait for pedal to be pressed
 - Run program once or until pedal is released... This needs more thought.
 - Go back to "Menu Screen"
Profit!
*/ 


#include <Mouse.h>
#include <Keyboard.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735 Screen
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789 Screen
#include <SPI.h>
#include <Encoder.h>

#define TFT_CS        10
#define TFT_RST        8 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC         9
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
Encoder myEnc(0, 1);// Reverse these to reverse count on encoder CW = Up or CW= Down , i feel like i wired these backwards between my harware and yours, by accedent, 420!

long oldPosition  = -999;
int FootPedalButton = 5 ;
int FootPedalState =0;
int EncoderSwitch = 7;
int EncoderSwitchState =0;

//float p = 3.1415926; // not used old copied code???
uint16_t time;

const unsigned char Dog128_64 [] PROGMEM = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x3f, 0xff, 0xff, 0xf0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x8f, 0x80, 0x0f, 0xcc, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x27, 0xe0, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0xe3, 0x1c, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x63, 0x18, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x63, 0x38, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x73, 0x38, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0x60, 0x00, 0x00, 0x33, 0x30, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x33, 0x30, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x33, 0x30, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x0e, 0x33, 0x31, 0xc0, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x1f, 0x1b, 0x23, 0xe0, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x3f, 0x9b, 0x67, 0xc0, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x07, 0xdb, 0x6f, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0xeb, 0x5e, 0x08, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x1e, 0x7b, 0x79, 0xe0, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x21, 0xa3, 0xb3, 0x30, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf0, 0x80, 0x00, 0x83, 0x80, 0x00, 0x08, 0x3f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf0, 0x80, 0x0e, 0x1f, 0xe1, 0xc0, 0x04, 0x3f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf1, 0x80, 0x06, 0x7f, 0xf3, 0x80, 0x04, 0x3f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf1, 0x80, 0x00, 0xe0, 0x39, 0x30, 0x06, 0x3f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf7, 0x00, 0x01, 0x80, 0x0e, 0x7c, 0x03, 0x3f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x0b, 0x9f, 0xcf, 0x3e, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x17, 0xbf, 0xef, 0x9f, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x77, 0x80, 0x6f, 0xcf, 0x83, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x6f, 0x80, 0x0f, 0xe3, 0xc1, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xc0, 0x1f, 0xf3, 0xc3, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0xff, 0xc0, 0x3f, 0xf9, 0xe3, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0xff, 0xe0, 0x7f, 0xfc, 0xe1, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x03, 0xff, 0xff, 0xff, 0xfc, 0xe1, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x03, 0xff, 0xfb, 0xff, 0xfc, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x03, 0xff, 0xf8, 0xff, 0xfe, 0x70, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfc, 0x03, 0xff, 0xe0, 0x1f, 0xfe, 0x70, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfc, 0x03, 0xff, 0x00, 0x07, 0xfe, 0x70, 0x7f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf8, 0x03, 0xfe, 0x00, 0x09, 0xfe, 0x70, 0x7f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf8, 0x01, 0x80, 0x00, 0x0c, 0xee, 0x70, 0x3f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x01, 0x80, 0x00, 0x0c, 0x0c, 0x71, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x80, 0x84, 0x08, 0x0c, 0x73, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xcc, 0x98, 0x0c, 0xe3, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x7c, 0xf8, 0x08, 0xe3, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x7d, 0xf9, 0x90, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x06, 0x7d, 0xf3, 0x01, 0xc7, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x03, 0x7d, 0xf6, 0x01, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x01, 0x7d, 0xf4, 0x01, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x01, 0xfd, 0xfc, 0x02, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xfd, 0xfc, 0x00, 0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0xfd, 0xf8, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xe0, 0x7d, 0xf8, 0x39, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xf0, 0x7d, 0xf8, 0x78, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xf8, 0x7f, 0xf0, 0xf9, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xf8, 0x7f, 0xf0, 0xf9, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x37, 0xb1, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x37, 0x71, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x13, 0x61, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x11, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x3f, 0xe3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x1f, 0xe3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x80, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x40, 0x17, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa0, 0x2f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

void setup(void) {
  Serial.begin(9600);
  
  Keyboard.begin(); 
  Mouse.begin(); 

  pinMode(FootPedalButton, INPUT_PULLUP); 
  pinMode(EncoderSwitch, INPUT_PULLUP); 

  Serial.print("FOOT PEDAL");
  tft.initR(INITR_MINI160x80_PLUGIN);  // Init ST7735S mini display (non inverted)
  tft.setRotation(3); // set to proper orrientation, as used...
  //tft.setSPISpeed(40000000); // use this if haveing issues????
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ST7735_BLUE);
  //tft.setTextWrap(true);
  tft.setTextSize(2);
  tft.print("Controller");
  delay(1000);
  //tft.drawBitmap(0, 0, Dog128_64, 128, 64, ST77XX_WHITE);
  //delay(3000);
  tft.fillRect(0,0,240,240,ST77XX_BLACK); // clear screen
  
}

void loop() {


// "Check Encoder"
  long newPosition = myEnc.read();

  // cap encoder values
  if ( newPosition > 40 ) {newPosition= 40;myEnc.write(40);}
  if ( newPosition < 0 ) {newPosition= 0;myEnc.write(0);}

  // this sets an oldPosition for some reason? to check direction? because it should only be 1  # a way, either way ,no?  
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
  }

  // check encoder Button state NOT NEEDED, BUT NICE TO KNOW FOR LATER
  //if (digitalRead(7)){EncoderSwitchState=0;}
  //else {EncoderSwitchState=1;}

  // check Foot Pedal State NOT NEEDED, BUT NICE TO KNOW FOR LATER
  //if (digitalRead(FootPedalButton)){FootPedalState=0;} // inverted because of pull-down resistors
  //else {FootPedalState=1;}


 switch (newPosition){
    case 0: case 1: case 2: case 3:// Thanks Arthur
      tft.fillScreen(ST77XX_BLACK);
      tft.setTextColor(ST7735_BLUE);
      tft.setCursor(0,0);
      tft.print("Walk");
      
      //loop for ever untill pedal is pressed OR ENCODER CHANFES to break?
      while (1){
        //wait for state change as to not flash screen
        //check pedal and encoder state
        // 1. break if  encoder changed
        newPosition = myEnc.read();
        if (newPosition >3){break;}

        // 2. run programm if pedal pressed
        while (!digitalRead(FootPedalButton)){ // cant do while , cuz this needs to be cheked again to break?
          tft.fillScreen(ST77XX_GREEN); 
          Keyboard.press(119);//"w"
         //delay(100); // this delays naturally with pedal hold...
          if (digitalRead(FootPedalButton)) {
            tft.fillScreen(ST77XX_RED); 
            
            Keyboard.release(119);//"w"
            delay(100); // this here to "show red and not just flash for a ms"
            
            goto bailout; // this exits, but then runs through all cases.... moved location to just after default and this WORKS !!!!!
            //end work? loop(); wont work, contunue also dosent workwill this send back to start? , Yes , but now also runs next case....?
            //break; 
          }
        }
      } 
  
    case 4: case 5: case 6: case 7:
      tft.fillScreen(ST77XX_BLACK);
      tft.setTextColor(ST7735_BLUE);
      tft.setCursor(0,0);
      tft.print("Two");
     //loop for ever untill pedal is pressed OR ENCODER CHANFES to break?
      while (1){
        //wait for state change as to not flash screen
        //check pedal and encoder state
        // 1. break if  encoder changed
        newPosition = myEnc.read();
        if (newPosition <4) {break;}
        if (newPosition >7) {break;}

        // 2. run programm if pedal pressed
        if (!digitalRead(FootPedalButton)){
          tft.fillScreen(ST77XX_GREEN);
          delay(100);
          break;
        }
      } 

    default:
    bailout:
     tft.fillScreen(ST77XX_BLACK);
    break;
 }


// this is wrong dont look at foot pedal state yet. look at encoder then run program when pressed. duh..... FML
  /*
  if (FootPedalState == 0) {     
    switch (newPosition){

      case 0: case 1: case 2: case 3: // this shit that house taught me , can i make it work? I also feel that i chose a bad way to do this... No comma!!!
        tft.fillScreen(ST77XX_BLACK);
        tft.setTextColor(ST7735_BLUE);
        tft.setCursor(0,0);
        tft.print("Lever");
        
          if (digitalRead(FootPedalButton)){FootPedalState=0;} // inverted because of pull-down resistors
          else {FootPedalState=1;}

          if (FootPedalState = 0){delay(1000);} // trap here , this is probbably bad...? then hopfully break.

        break;
      
      case 4: case 5: case 6: case 7:
        tft.fillScreen(ST77XX_BLACK);
        tft.setTextColor(ST7735_BLUE);
        tft.setCursor(0,0);
        tft.print("BedRock");
        break;
    
      case 8: case 9: case 10: case 11:
        tft.fillScreen(ST77XX_BLACK);
        tft.setTextColor(ST7735_BLUE);
        tft.setCursor(0,0);
        tft.print("sftclk");
        break;
    
      case 12: case 13: case 14: case 15:
        tft.fillScreen(ST77XX_BLACK);
        tft.setTextColor(ST7735_BLUE);
        tft.setCursor(0,0);
        tft.print("tree");
        break;
    
      case 16: case 17: case 18: case 19:
        tft.fillScreen(ST77XX_BLACK);
        tft.setTextColor(ST7735_BLUE);
        tft.setCursor(0,0);
        tft.print("garbage");
        break;
    
      case 20: case 21: case 22: case 23:
        tft.fillScreen(ST77XX_BLACK);
        tft.setTextColor(ST7735_BLUE);
        tft.setCursor(0,0);
        tft.print("backwall");
        break;   
    
      case 24: case 25: case 26: case 27:
        tft.fillScreen(ST77XX_BLACK);
        tft.setTextColor(ST7735_BLUE);
        tft.setCursor(0,0);
        tft.print("cobble");
        break;  
    
    }
  }
  
  if (FootPedalState == 1) {    
    switch (newPosition){

    case 0:
      tft.fillScreen(ST77XX_BLACK);
      
      while (FootPedalState == 1) {
        
        tft.setTextColor(ST7735_GREEN);
        if (digitalRead(FootPedalButton)){FootPedalState=0;} // inverted because of pull-down resistors
        else {FootPedalState=1;}
        tft.setCursor(0,0);
        tft.print("LEVER");
       // Mouse.press(MOUSE_RIGHT);
        //delay(50);
        //Mouse.release(MOUSE_RIGHT);
        //delay(5000);

      }  
      break;

    case 4:
       tft.fillScreen(ST77XX_BLACK);
      
      while (FootPedalState == 1) {
        
        tft.setTextColor(ST7735_GREEN);
        if (digitalRead(FootPedalButton)){FootPedalState=0;} // inverted because of pull-down resistors
        else {FootPedalState=1;}
        tft.setCursor(0,0);
        tft.print("BEDROCK");
        Mouse.press(MOUSE_RIGHT);
        delay(25);
        Mouse.release(MOUSE_RIGHT);
        delay(25);

      }
      break;

    case 8:
      tft.fillScreen(ST77XX_BLACK);
      while (FootPedalState == 1) {
        tft.setTextColor(ST7735_GREEN);
        if (digitalRead(FootPedalButton)){FootPedalState=0;} // inverted because of pull-down resistors
        else {FootPedalState=1;}
        tft.setCursor(0,0);
        tft.print("SHFTCLK");
        //run function
      }
      break;
   
    case 12:
      tft.fillScreen(ST77XX_BLACK);
      while (FootPedalState == 1) {
        tft.setTextColor(ST7735_GREEN);
        if (digitalRead(FootPedalButton)){FootPedalState=0;} // inverted because of pull-down resistors
        else {FootPedalState=1;}
        tft.setCursor(0,0);
        tft.print("TREE");
        //run function
      }
      break;

    case 16:
      tft.fillScreen(ST77XX_BLACK);
      while (FootPedalState == 1) {
        tft.setTextColor(ST7735_GREEN);
        if (digitalRead(FootPedalButton)){FootPedalState=0;} // inverted because of pull-down resistors
        else {FootPedalState=1;}
        tft.setCursor(0,0);
        tft.print("GARBAGE");
        //run function
      }
      break;
       
    case 20:
      tft.fillScreen(ST77XX_BLACK);
      while (FootPedalState == 1) {
        tft.setTextColor(ST7735_GREEN);
        if (digitalRead(FootPedalButton)){FootPedalState=0;} // inverted because of pull-down resistors
        else {FootPedalState=1;}
        tft.setCursor(0,0);
        tft.print("BACKWALL");
        //run function
      }
      break;

    case 24:
      tft.fillScreen(ST77XX_BLACK);
      while (FootPedalState == 1) {
        tft.setTextColor(ST7735_GREEN);
        if (digitalRead(FootPedalButton)){FootPedalState=0;} // inverted because of pull-down resistors
        else {FootPedalState=1;}
        tft.setCursor(0,0);
        tft.print("COBBLE");
        //run function
      }
      break;  
    }
  }

  */

}


void Cobble(){
  Mouse.press();
  delay(600);
  Mouse.release();
}
void BackWall(){
 //
}
void ObsidianMaker(){
 //
  
}
void Garbage(){

  Mouse.press(MOUSE_LEFT);
  delay(10);
  Mouse.move(0, -100, 0);
  Mouse.move(0, -100, 0);
  Mouse.move(0, -100, 0);
  Mouse.move(0, -100, 0);
  delay(10);
  Mouse.move(0, 50, 0);
  //delay(10);
  Mouse.release(MOUSE_LEFT);
  Mouse.move(0, 100, 0);
  Mouse.move(0, 100, 0);
  delay(500);
}
void ShiftClick(){
  //display.println("Click");
  //display.display();
  
  Keyboard.press(KEY_LEFT_SHIFT); 
  delay(25);
  Mouse.click();  
  delay(25);                  //click again
  Keyboard.releaseAll();            //release the shift    

  delay(150);
  //Keyboard.press(KEY_SPACE);
  delay(50);
  //Keyboard.release(KEY_SPACE);
  delay(250);
  
}
void CritHit(){

  //Keyboard.press(KEY_SPACE);
  delay(50);
  //Keyboard.release(KEY_SPACE);
  
  delay(300);

  Mouse.press();
  delay(50);
  Mouse.release();
  delay(420);

}
void BlinkLED(){
    //digitalWrite(OnBoardLED, HIGH);   //Inverted
    delay(50);
    //digitalWrite(OnBoardLED, LOW);    //Inverted
    delay(50);

}
void TreeCutter(){
  //BlinkLED();
  Mouse.press();
  delay(350); 
  // Sharpness5, Efficiency 5 = 350 YES  
  // Sharpness4, Effieiency 5? = 420
  // Basic Dimond Axe 550?
  Mouse.release();
  
  Mouse.move(100, 0, 0);
  //delay(100);
  Mouse.move(100, 0, 0);
  //delay(100);
  Mouse.move(100, 0, 0);
  //delay(100);
  Mouse.move(100, 0, 0);
  //delay(100);
  Mouse.move(100, 0, 0);
  //delay(100);
  Mouse.move(100, 0, 0);
  //delay(100);// DELAY INBETWEEN CHOPS, 100 Works with S5+E5
  //Mouse.release();
  
}
void StepStep(){

    //Keyboard.press(KEY_W);
    delay(100);  // Mac OS-X will not recognize a very short eject press
   // Keyboard.release(KEY_W);
    delay(480);


}
void Walk(){
  Keyboard.press(119);//w
  delay(75);  // Mac OS-X will not recognize a very short eject press
  Keyboard.release(119);//w
  delay(75);// does this help timing???
}
void AFK2B2T(){
  for (int i = 0; i <= random(5, 10); i++) {
   Keyboard.press(32);//Space
   delay(random(50, 100));  // Mac OS-X will not recognize a very short eject press
   Keyboard.release(32);//Space
   delay(random(50, 100));// does this help timing???
  }
  Keyboard.press(115);//s
  delay(2300);  // Mac OS-X will not recognize a very short eject press
  Keyboard.release(115);//s
  delay(5000);// does this help timing???

  for (int i = 0; i <= random(5, 10); i++) {
    Keyboard.press(32);//Space
    delay(random(50, 100));  // Mac OS-X will not recognize a very short eject press
    Keyboard.release(32);//Space
    delay(random(50, 100));// does this help timing???
  }

  
  /*
  //break brick
  //press 2 (50)
   Keyboard.press(50);//1
   delay(50);  // Mac OS-X will not recognize a very short eject press
   Keyboard.release(50);//1
   delay(100);  // Mac OS-X will not recognize a very short eject press
   
   Mouse.press();
   delay(550);
   Mouse.release();
  delay(1000);
  //place brick

  // Press 3 (51)
  //eat
  //press 4 (52)

  */

}
void MobClicker(){
  Mouse.click();
  delay(1250); 
}
