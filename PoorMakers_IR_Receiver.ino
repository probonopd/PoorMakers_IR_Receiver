/*
************************************************************************
Poor Maker's Infrared receiver V1.0

Author: AnalysIR
Copyright: AnalysIR

Free to use, provided AnalysIR is credited and a link to http://www.AnalysIR.com/blog is included in the source and any related article or publication.

Contact: via our blog at http://www.analysir.com/blog/contact/

Get your own copy of AnalysIR >>>: http://www.analysir.com/blog/get-analysir/

************************************************************************
*/

//default definition for ISR
#define enableIRrx attachInterrupt(0, rxIR_Interrupt_Handler, FALLING) //set up interrupt handler for IR rx   on pin 2 - demodulated signal

// only one of the following should be uncommented to match your platform
//#define Arduino328 true
#define ArduinoUNO true
//#define ArduinoYun true
//#define ArduinoLeonardo true
//#define ArduinoMega1280 true
//#define ArduinoMega2560 true


#if Arduino328 || ArduinoUNO
  #define enableIRrx attachInterrupt(0, rxIR_Interrupt_Handler, FALLING) //set up interrupt handler for IR rx   on pin 2 - demodulated signal
#elif ArduinoLeonardo || ArduinoYun
//Following must be redefined as interrupt pins are different on Leonardo/Yun
  #define enableIRrx attachInterrupt(1, rxIR_Interrupt_Handler, FALLING) //set up interrupt handler for IR rx   on pin 2 - demodulated signal
#elif ArduinoMega1280 || ArduinoMega2560
  #define enableIRrx attachInterrupt(0, rxIR_Interrupt_Handler, FALLING) //set up interrupt handler for IR rx   on pin 2 - demodulated signal
#endif

// unsigned long uSecs = 0;
volatile unsigned long spaceLen = 0;
volatile unsigned long markLen = 0;
volatile unsigned long thisChange = 0;
volatile unsigned long lastChange = 0;
volatile unsigned long lastChangeSafe = 0; //avoids changes by ISR
volatile unsigned long spaceStart = 0;
volatile unsigned long markStart = 0;
volatile unsigned long period = 0;
volatile unsigned long thisP = 0;
volatile unsigned long lastP = 0;
volatile boolean endMark = false;
unsigned long sigLen = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);

initClockT1(); //init on-board Timer1 cloack @ 16Mhz, no prescaling

enableIRrx; // enable interrupts

}

void loop() {
  while (true) { //endless loop, wait for the action!

    if (markLen > 0) { //High
      Serial.print(", +");
      Serial.print(markLen);
      markLen = 0;

      if (spaceLen < 120000) { //LOW
          Serial.print(", -");
          Serial.print(spaceLen);
        }
        spaceLen = 0;
    }
    
    lastChangeSafe=lastChange; // prevents update of value by ISR
    if ((markStart > 0) && ((micros() - lastChangeSafe) > 200000) && markLen ==0 & spaceLen == 0) {//last mark in sequence
      markLen =  spaceStart - markStart;
      Serial.println(markLen);
      
      Serial.print("Modulation Period: ");
      period=(period*1000)/16; // in nano seconds...better accuracy achieved using nanosecs & integer division
      Serial.print(period);
      Serial.println(" (nano seconds)");
      
      Serial.print("Modulation Frequency: ");
      Serial.print(1000000000/period); // in nano seconds
      Serial.println(" (Hz)");
      Serial.println();

      //reset values for next signal
      markLen = 0;
      markStart = 0;
      spaceStart = 0;
    }
  }
}

void rxIR_Interrupt_Handler(void) { //ISR for IR signals
  //on falling edge so pin 2 will always be LOW here
  // _________|||||||||______________||||||||||||||_______________||||||||||||||||||___________||||||||||||||||     etc
  thisChange = micros(); //capture timestamp only once in ISR

  if ((thisChange - lastChange) > 50) {//start of mark / end of space
    //so calculate previous space & mark lengths
    spaceLen = thisChange - spaceStart ;
    markLen = thisChange - markStart - spaceLen;
    markStart = thisChange;
    //spaceStart=0;

  }
  //repeatedly update this bit until last modulation of mark
  else  spaceStart = thisChange;

  thisP = TCNT1;
  period=thisP-lastP; //capture 
  lastP=thisP;
  lastChange = thisChange;
  
}

void initClockT1(void){ //init 16 bit Timer1 to 16MHz , no prescaling
  TCCR1A = 0;
  TCCR1B = (1<<CS10); //no prescaler = clockfreq/1
  TCNT1  = 0;
}
