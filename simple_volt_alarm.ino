
const int ledPin = 1;
const int sensorPin = A3;
const float R1 = 4700.0;
const float R2 = 2700.0;
const float WarnVoltage = 3.15;
const float CritVoltage = 3.0;
const float resolution = 1023.0;
const float cells = 2.0;
const float v_offset = 0.3;


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 1 as an output.
  pinMode(1, OUTPUT);
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  
  float CellsensorValue = (analogRead(sensorPin) / resolution * 5.0 * (R1+R2) / R2 - v_offset) / cells; // take read value and convert to average cell value.

   
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

  Serial.println(CellsensorValue);
  delay(1); 
}