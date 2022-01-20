/* Simple voltage monitor for battery pack. 
use case: a 2s li-ion goggle pack using a 9v step-up reg for skyzone 04X

uncomment debug stuff to use serial monitor to fine tune using scaling factor.
start with v_scaler = 1, then compute the percentage your off from your input.
*/

#define ledPin 7          // led pin output
#define sensorPin A3      // output from voltage divider
#define R1 4700.0         // R1 value from voltage divider
#define R2 2700.0         // R2 value from voltage divider
#define WarnVoltage 3.15  // average cell voltage when you want led to slow flash
#define CritVoltage 3.02  // average cell voltage when you want led to fast flash
#define resolution 1023.0 // 10 bit resolution of 5v arduino
#define cells 2.0         // number of cells in your pack
#define v_scaler .96      // scale factor for fine tuning.
#define buzpin 9          // buzzer output
#define led_crit 200     // led flash every .2 seconds
#define led_warn 1000     // led flash every second
#define buz_crit 60000   // buzzer every 1 minute
#define buz_warn 180000   // buzzer every 3 minutes
unsigned long led_time_now = 0;
unsigned long buzz_time_now = 0;
unsigned long previousMillis_led = 0;
unsigned long previousMillis_buzz = 0;
int ledState = LOW;

// the setup function runs once when you press reset or power the board
void setup()
{

  pinMode(ledPin, OUTPUT);   // sets led pin to output
  pinMode(sensorPin, INPUT); // sets sensor pin to input
  pinMode(buzpin, OUTPUT);   //sets buzzer pin to output

  //Serial.begin(9600); // for Debug
}

// the loop function runs over and over again forever
void loop()
{

  float CellsensorValue = (analogRead(sensorPin) / resolution * 5.0 * (R1 + R2) / R2 * v_scaler) / cells; // take read value and convert to average cell value.
  //float fullVolt = CellsensorValue * 2;   // for debug
  //int raw = analogRead(sensorPin);        // for debug
  unsigned long currentMillis_led = millis();
  unsigned long currentMillis_buzz = millis();

  if (CellsensorValue > WarnVoltage) // if cell value above WarnVoltage, keep led on
  {
    digitalWrite(ledPin, HIGH);
  }

  else if (CellsensorValue < WarnVoltage && CellsensorValue > CritVoltage) // if cell value below warn and above critical slow flash
  {

    if (currentMillis_led - previousMillis_led >= led_warn)
    {

      previousMillis_led = currentMillis_led;

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

    if (currentMillis_buzz - previousMillis_buzz >= buz_warn)
    {
      previousMillis_buzz = currentMillis_buzz;
      tone(9, 200, 1000);
    }
  }

  else // if cell value below critical voltage fast flash
  {
    if (currentMillis_led - previousMillis_led >= led_crit)
    {

      previousMillis_led = currentMillis_led;

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

    if (currentMillis_buzz - previousMillis_buzz >= buz_crit)
    {
      previousMillis_buzz = currentMillis_buzz;
      tone(9, 200, 1000);
    }
  }
  //Serial.println(CellsensorValue + String("\t") + fullVolt + String("\t") + raw); // for debug
}