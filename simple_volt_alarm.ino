/* Simple voltage monitor for battery pack. 
use case: a 2s li-ion goggle pack using a 9v step-up reg for skyzone 04X

uncomment debug stuff to use serial monitor to fine tune using scaling factor.
start with v_scaler = 1, then compute the percentage your off from your input.
*/
#include <Smoothed.h>

#define ledPin 2          // led pin output
#define sensorPin A5      // output from voltage divider
#define R1 4700.0         // R1 value from voltage divider
#define R2 2700.0         // R2 value from voltage divider
#define WarnVoltage 3.15  // average cell voltage when you want led to slow flash
#define CritVoltage 3.02  // average cell voltage when you want led to fast flash
#define resolution 1023.0 // 10 bit resolution of 5v arduino
#define cells 3.0         // number of cells in your pack
#define v_scaler .999      // scale factor for fine tuning.
#define buzpin 9          // buzzer output
#define led_crit 200      // led flash every .2 seconds
#define led_warn 1000     // led flash every second
#define buz_crit 90000    // buzzer every 1.5 minute
#define buz_warn 300000   // buzzer every 5 minutes
#define serial_step 200

unsigned long previousMillis_led = 0;
unsigned long previousMillis_buzz = 0;
unsigned long previousMillis_serial = 0;
int ledState = LOW;

Smoothed <int> smoothCell;

// the setup function runs once when you press reset or power the board
void setup()
{

  pinMode(ledPin, OUTPUT);   // sets led pin to output
  pinMode(sensorPin, INPUT); // sets sensor pin to input
  pinMode(buzpin, OUTPUT);   //sets buzzer pin to output

    Serial.begin(9600); // for Debug

    smoothCell.begin(SMOOTHED_AVERAGE, 100);
}

// the loop function runs over and over again forever
void loop()
{
  smoothCell.add(analogRead(sensorPin));

  float CellsensorValue = (smoothCell.get() / resolution * 5.0 * (R1 + R2) / R2 * v_scaler) / cells; // take read value and convert to average cell value.
  float fullVolt = CellsensorValue * 2;   // for debug
  int raw = analogRead(sensorPin);     // for debug
  unsigned long currentMillis = millis();

  
  

  if (CellsensorValue > WarnVoltage) // if cell value above WarnVoltage, keep led on
  {
    digitalWrite(ledPin, HIGH);
    
  }

  else if (CellsensorValue < WarnVoltage && CellsensorValue > CritVoltage) // if cell value below warn and above critical slow flash
  {

    if (currentMillis - previousMillis_led >= led_warn)
    {

      previousMillis_led = currentMillis;

      if (ledState == LOW)
      {
        ledState = HIGH;
      }
      else
      {
        ledState = LOW;
      }

      digitalWrite(ledPin, ledState);
    }

    if (currentMillis - previousMillis_buzz >= buz_warn)
    {
      previousMillis_buzz = currentMillis;

      tone(9, 200, 500);
    }
  }

  else // if cell value below critical voltage fast flash
  {
    if (currentMillis - previousMillis_led >= led_crit)
    {

      previousMillis_led = currentMillis;

      if (ledState == LOW)
      {
        ledState = HIGH;
      }
      else
      {
        ledState = LOW;
      }
      digitalWrite(ledPin, ledState);
    }

    if (currentMillis - previousMillis_buzz >= buz_crit)
    {
      previousMillis_buzz = currentMillis;

      tone(9, 200, 1000);
    }
  }

  if (currentMillis - previousMillis_serial >= serial_step)
    {
      previousMillis_serial = currentMillis;

      Serial.println(CellsensorValue + String("\t") + fullVolt + String("\t") + raw + String("\t") + smoothCell.get()); // for debug
    }
  
  
}