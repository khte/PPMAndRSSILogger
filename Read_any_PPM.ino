/*
Read PPM and RSSI

Main author: https://github.com/Hasi123
Changes made by: Kristian Husum Terkildsen, khte@mmmi.sdu.dk, December '17

The functionality for reading PPM signals have been written by Hasi123.
I have made some changes and added the logging of the RSSI value, in order to suit my needs.

Notes to dev: 
* 

Sources of inspiration:
* https://github.com/Hasi123/read-any-ppm
*/

#define PPM_Pin 3
#define multiplier (F_CPU/8000000)
int ppm[16];
int RSSIInputPin = A0;
int analogValue = 0;

void setup()
{
  Serial.begin(115200);

  pinMode(PPM_Pin, INPUT);
  attachInterrupt(PPM_Pin - 2, read_ppm, CHANGE);

  TCCR1A = 0;  //reset timer1
  TCCR1B = 0;
  TCCR1B |= (1 << CS11);  //set timer1 to increment every 0,5 us
}

void loop()
{
  analogValue = analogRead(RSSIInputPin);

  int count;
  while(ppm[count] != 0){
    Serial.print(ppm[count]);
    Serial.print("  ");
    count++;
  }
  Serial.print(analogValue);
  Serial.println("");
  delay(100);
}

void read_ppm(){
  static unsigned int pulse;
  static unsigned long counter;
  static byte channel;
  static unsigned long last_micros;

  counter = TCNT1;
  TCNT1 = 0;

  if(counter < 710*multiplier){  //must be a pulse if less than 710us
    pulse = counter;
  }
  else if(counter > 1910*multiplier){  //sync pulses over 1910us
    channel = 0;
  }
  else{  //servo values between 710us and 2420us will end up here
    ppm[channel] = (counter + pulse)/multiplier;
    channel++;
  }
}
