
#define ledPin 7
#define sensorPin A3
#define R1 4700.0
#define R2 2700.0
#define WarnVoltage 3.15
#define CritVoltage 3.05
#define resolution 1023.0
#define cells 2.0
#define v_offset 0.5


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 1 as an output.
  pinMode(ledPin, OUTPUT);
  //pinMode(sensorPin, INPUT);

  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  
  float CellsensorValue = (analogRead(sensorPin) / resolution * 5.0 * (R1+R2) / R2 - v_offset) / cells; // take read value and convert to average cell value.
  float fullVolt = CellsensorValue *2;
   
  if (CellsensorValue > WarnVoltage){    // if cell value above warnvoltage, keep led on
        digitalWrite(ledPin, HIGH);    
                      
    }

    else if (CellsensorValue < WarnVoltage && CellsensorValue > CritVoltage){   // iff cell value below warn and above critical slow flash
        digitalWrite(ledPin, HIGH);   
        delay(1000);                      
        digitalWrite(ledPin, LOW);    
        delay(1000); 
    }

    else {                        // if cell value below critical voltage fast flash
        digitalWrite(ledPin, HIGH);   
        delay(200);                       
        digitalWrite(ledPin, LOW);    
        delay(200); 
    }

  Serial.println(CellsensorValue + String("\t") + fullVolt);
  //Serial.print("\t");
  //Serial.println(fullVolt);
  delay(1); 
}