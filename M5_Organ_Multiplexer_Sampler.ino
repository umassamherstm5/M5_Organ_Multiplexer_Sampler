#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Namespace.h>
#include <midi_Settings.h>

const int outputPin  =  1;    
const int selectPin1 =  2;  
const int selectPin2 =  3;  
const int selectPin3 =  4;  
const int selectPin4 =  5;  
const int selectPin5 =  6;  
const int selectPin6 =  7;
const int inputPin   =  8;


int buttonState = 0;         

void setup() {
  pinMode(outputPin,  OUTPUT);     
  pinMode(selectPin1, OUTPUT);      
  pinMode(selectPin2, OUTPUT);      
  pinMode(selectPin3, OUTPUT);      
  pinMode(selectPin4, OUTPUT);      
  pinMode(selectPin5, OUTPUT);      
  pinMode(selectPin6, OUTPUT);      

  pinMode(inputPin, INPUT);     
  MIDI.begin();
//  Serial.begin(9600);
  delay (1000);
  
}

void loop(){
  

  static byte note[64];
//  Creates a byte array that holds a value 0-3 for each note. The number in each note value describes the state of the button.
//  0= note delay is false, note sent is false. 1= delay true, sent false. 2= delay true, sent true.

//  Serial.println("setup");
  
  for(int i=7; i<8; i++){
    // Scan through high order bits
    if(i%2==1){
      digitalWrite(selectPin1, HIGH);
    }
    else{
      digitalWrite(selectPin1, LOW);
    }
    if(i%4>1){
      digitalWrite(selectPin2, HIGH);
    }
    else{
      digitalWrite(selectPin2, LOW);
    }
    if(i>3){
      digitalWrite(selectPin3, HIGH);
    }
    else{
      digitalWrite(selectPin3, LOW);
    }
//  Runs through the first set of multiplexers using pins 2-4
//  These are the low order bits

      for(int j=0; j<8; j++){
        if(j%2==1){
          digitalWrite(selectPin4, HIGH);
        }
        else{
          digitalWrite(selectPin4, LOW);
        }
        if(j%4>1){
          digitalWrite(selectPin5, HIGH);
        }
        else{
          digitalWrite(selectPin5, LOW);
        }
        if(j>3){
          digitalWrite(selectPin6, HIGH);
        }
        else{
          digitalWrite(selectPin6, LOW);
        }          
//  Runs through the second level of multiplexers using pins 5-7  

        int a=8*i+j;
        int noteValue=a+29;
//  Int a describes the address lines while int noteValue shifts it to the correct midi note.   
//  The note number starts at 29 (F) and is incremented once each j and eight times each i

        buttonState = digitalRead(inputPin);
//        Serial.print ("Checking address "); Serial.print (i*8+j,BIN); 
//        Serial.print ("->"); Serial.println (buttonState);
          
        if (buttonState == LOW){
          note[a]=0;
        } 
        if (buttonState == HIGH && note[a]==0){
          delay(50);
//  Delays the note to reduce noise
          note[a]=1;
//          Serial.println("Button State High");
        }
        if (buttonState == HIGH && note[a]==1){
          MIDI.sendNoteOn(noteValue,127,1);
          note[a]=2;
//          Serial.print("Note Sent: "); Serial.println(noteValue);
        }
//  Reads the button. If the button is LOW, no note is sent and there was no delay
//  If the buttonState is HIGH, a delay is set and then a note is generated based on which input on the multiplexer was read

        delay(1);
//  Delays the code. Will be removed or reduced in the final implementation        
      }
  }
}
