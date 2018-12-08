int clockPin = 5; //Pin connected to SRCLK of 74HC595
int latchPin = 6; //Pin connected to RCLK of 74HC595
int dataPin = 7; //Pin connected to SER of 74HC595
//#define DEBUG //Uncomment to enable debug print statements
int antenna1 = 8; //GPIO 8 for WE of the attentuator
int antenna2 = 9; //GPIO 9 for WE of the attentuator
int antenna3 = 10; //GPIO 10 for WE of the attentuator
int pwm1 = 2;
int pwm2 = 3;
int pwm3 = 4;
int packetcount = 0;
int count = 0;
int input = 0;
int i;
int len;
byte inputsize[6];
int current_state[6] = {0,0,0,0,0,0};
int prev_state[6] = {999,999,999,999,999,999}; //initial state value which not are acceppted therefore the states in the very first packet will be set
bool att1_decimal = false; //boolean to check if an attentuation value has a decimal, it is used to set .0 as the decimal if there is not decimal
bool att2_decimal = false; //boolean to check if an attentuation value has a decimal, it is used to set .0 as the decimal if there is not decimal
bool att3_decimal = false; //boolean to check if an attentuation value has a decimal, it is used to set .0 as the decimal if there is not decimal
bool c_pwm1 = false; //adjust the value of PWM1 if it is true
bool c_pwm2 = false; //adjust the value of PWM2 if it is true
bool c_pwm3 = false; //adjust the value of PWM3 if it is true
bool c_att1 = false; //adjust the value of Attentuation1 if it is true
bool c_att2 = false; //adjust the value of Attentuation2 if it is true
bool c_att3 = false; //adjust the value of Attentuation3 if it is true
bool c_i = true;

void setup() {
  analogWriteResolution(12);
  pinMode(pwm1, OUTPUT);
  pinMode(pwm2, OUTPUT);
  pinMode(pwm3, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(antenna1, OUTPUT);
  pinMode(antenna2, OUTPUT);
  pinMode(antenna3, OUTPUT);
  digitalWrite(antenna1, LOW);
  digitalWrite(antenna2, LOW);
  digitalWrite(antenna3, LOW);
  attenuation(0);
  Serial.begin(9600);
  while(!Serial) {
    
  }
  SerialUSB.begin(9600);
}

void loop() {
#ifdef DEBUG
  Serial.println("Start");
  Serial.print(prev_state[0]);
  Serial.print(", ");
  Serial.print(prev_state[1]);
  Serial.print(", ");
  Serial.print(prev_state[2]);
  Serial.print(", ");
  Serial.print(prev_state[3]);
  Serial.print(", ");
  Serial.print(prev_state[4]);
  Serial.print(", ");
  Serial.println(prev_state[5]);
  Serial.println("Lets wait");
#endif
  /* Before wait for a new packet, flush the message buffer */
  SerialUSB.flush();  
  while(SerialUSB.available() <= 0) {
  }
#ifdef DEBUG
  Serial.println("Start Reading");
#endif
  while(SerialUSB.available() > 0) {
    if(count < 6) {
        inputsize[count] = SerialUSB.read();
#ifdef DEBUG
      Serial.print("inputsize: ");
      Serial.println(inputsize[count]);
#endif
      count++;
    }
    else{
      len = inputsize[packetcount];
      int adder = 0;
      for(int i = len-1; i >= 0; i--) {
        if(i == 0) {
            input = SerialUSB.read();
            if((packetcount == 1 || packetcount == 3 || packetcount == 5)){ //check if there are decimals
              if(input == 5) {
                if(packetcount == 1) {
                  att1_decimal = true;  
                }
                else if(packetcount == 3) {
                  att2_decimal = true;   
                }
                else if(packetcount == 5) {
                  att3_decimal = true;  
                }
                else {
#ifdef DEBUG
                  Serial.println("Error: Unexpeceted Decimal Check");
#endif
                }
              }
              else {
                if(packetcount == 1) {
                  att1_decimal = false;
                }
                else if(packetcount == 3) {
                  att2_decimal = false;  
                }
                else if(packetcount == 5) {
                  att3_decimal = false;
                }
                else {
#ifdef DEBUG
                  Serial.println("Error: Unexpeceted Decimal Check");
#endif
                }
              }
            }
            adder += input;
        }
        else if(i == 1) { //multiply by 10 or 100 to make the digit bytes to a int value
          input = SerialUSB.read();
          adder += input * 10;
        }
        else if(i == 2) {
          input = SerialUSB.read();
          adder += input * 100;
        }
        else {
#ifdef DEBUG
          Serial.println("Error: Unexpected Size Input");
#endif
        }
      }      
      current_state[packetcount] = adder;
#ifdef DEBUG
      Serial.print("input: ");
      Serial.println(current_state[packetcount]);
#endif
      packetcount++;
      
    }
  }
  packetcount = 0;
  count = 0;  
#ifdef DEBUG
  Serial.println("Done reading"); 
  /*Check if there are state changes */
  Serial.println("Is it a new packet");
#endif
  for(i = 0; i < 6; i++) {
    if(current_state[i] != prev_state[i]) {
#ifdef DEBUG
      Serial.print("current: ");
      Serial.print(current_state[i]);
      Serial.print(" vs ");
      Serial.print("previous: ");
      Serial.println(prev_state[i]);
#endif
      if(i == 0) {
        c_pwm1 = true;
        c_i = false; 
      }
      if(i == 1) {
        c_att1 = true;
        c_i = false;
      }
      if(i == 2) {
        c_pwm2 = true;
        c_i = false;
      }
      if(i == 3) {
        c_att2 = true;
        c_i = false;
      }
      if(i == 4) {
        c_pwm3 = true;
        c_i = false;
      }
      if(i == 5) {
        c_att3 = true;
        c_i = false;
      }
      }
    }
    if(c_i == false) {
/*If there are some state changes, update the state array*/
#ifdef DEBUG
      Serial.println("New State Change");
#endif
      prev_state[0] = current_state[0];     
      prev_state[1] = current_state[1];
      prev_state[2] = current_state[2];
      prev_state[3] = current_state[3];
      prev_state[4] = current_state[4];
      prev_state[5] = current_state[5];
    }
    
  if(c_i == false) {
#ifdef DEBUG
    Serial.println("Start Modify");
#endif
    if(c_pwm1 == true) {
#ifdef DEBUG
      Serial.println("New PWM1");
      Serial.println(prev_state[0]*11);
#endif
      analogWrite(DAC1, prev_state[0]*11); //alternatively used DAC instead of PWM
      c_pwm1 = false;
    }
    if(c_pwm2 == true) {
#ifdef DEBUG      
      Serial.println("New PWM2");
      Serial.println(prev_state[2]);
#endif
      analogWrite(pwm2, prev_state[2]);
      c_pwm2 = false;
    }
    if(c_pwm3 == true) {
#ifdef DEBUG
      Serial.println("New PWM3");
      Serial.println(prev_state[4]);
#endif
      analogWrite(pwm3, prev_state[4]);
      c_pwm3 = false;
    }
    if(c_att1 == true) {
#ifdef DEBUG
      Serial.println("New Att1");
#endif
      int adjust;
      adjust = prev_state[1];
      Serial.println(att1_decimal);
	  /*If there are no decimals adjust data strcture and add .0*/
      if(att1_decimal == 1) {
        adjust -= 5;
        adjust /= 10;
        adjust = adjust << 1;
        adjust = adjust | 1;
      }
      else{        
        adjust /= 10;
        adjust = adjust << 1;
      }
#ifdef DEBUG
      Serial.print("adjust: ");
      Serial.println(adjust, BIN);
#endif
      attenuation((byte)adjust);
      delay(10);
      toggle_antenna_we(1);
      c_att1 = false;
    }
    if(c_att2 == true) {
#ifdef DEBUG
      Serial.println("New Att2");
#endif
      int adjust;
      adjust = prev_state[3];
      if(att2_decimal == true) {
        adjust -= 5;
        adjust /= 10;
        adjust = adjust << 1;
        adjust = adjust | 1;
      }
      else if (att2_decimal = false){
        adjust /= 10;
        adjust = adjust << 1;
      }
#ifdef DEBUG
      Serial.print("adjust: ");
      Serial.println(adjust, BIN);
#endif
      attenuation((byte)adjust);
      delay(10);
      toggle_antenna_we(2);
      c_att2 = false;
    }
    if(c_att3 == true) {
#ifdef DEBUG
      Serial.println("New Att3");
#endif
      int adjust;
      adjust = prev_state[5];
      if(att3_decimal == true) {
        adjust -= 5;
        adjust /= 10;
        adjust = adjust << 1;
        adjust = adjust | 1;
      }
      else if (att3_decimal = false){
        adjust /= 10;
        adjust = adjust << 1;
      }
#ifdef DEBUG
      Serial.print("adjust: ");
      Serial.println(adjust,BIN);
#endif
      attenuation((byte)adjust);
      delay(10);
      toggle_antenna_we(3);
      c_att3 = false;
    }
    c_i = true;
  }
  else{
#ifdef DEBUG
    Serial.println("No Modifiy Needed");
#endif
  }
}
/* latch a GPIO latch pin used for WE of an antentuator*/
void toggle_antenna_we(int value) {
  if(value == 1) {
    digitalWrite(antenna1, HIGH);
    delay(500);
    digitalWrite(antenna1, LOW);
  }
  else if(value == 2) {
    digitalWrite(antenna2, HIGH);
    delay(500);
    digitalWrite(antenna2, LOW);
  }
  else if(value == 3) {
    digitalWrite(antenna3, HIGH);
    delay(500);
    digitalWrite(antenna3, LOW);
  }
  else {
    
  }
}

/*Sets a 8 bit value to a shfit register*/
void attenuation(byte value) {
  digitalWrite(latchPin, 0);
  shiftOut(value);
  digitalWrite(latchPin, 1); 
}

void shiftOut(byte myDataOut) {
  int i = 0;
  int pinState; 
  
  //initially write 0 to the data and clock pin
  digitalWrite(dataPin, 0);
  digitalWrite(clockPin, 0);
  
  for (i = 7; i >=0; i--) { //iterate 8 bits of a byte data
    digitalWrite(clockPin, 0); //falling clock edges
    
    if (myDataOut & (1 << i)) { //find if i-th bit of a byte data is low or high
      pinState = 1;
    }
    else {
      pinState = 0;
    }
    //Shift a bit per clock-pin cycle    
    digitalWrite(dataPin, pinState);
    digitalWrite(clockPin, 1); //rising clock edge
    digitalWrite(dataPin, 0);
  }
  //Stop shifting
  digitalWrite(clockPin, 0);
}
