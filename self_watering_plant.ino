
const int normalButtonPin = 11; // the pin to switch into normal mode
const int cactusButtonPin = 10; // the pin to switch into cactus mode
const int normalLEDPin = 13; // the pin that lights up for normal mode
const int cactusLEDPin = 2; // the pin that lights up for cactus mode

const int pumpPin = A0; // pin that the motor is attached to
int waterTime = 10000; // duration of watering in milliseconds

int moisture_level = 0; // the measured moisture value from the sensor
const int soilPin = A5; // pin for the moisutre sensor 
const int soilPower = 7; //the pin for moisture sensor power - plugging it into arduino 5V power will errode the sensor

int BASE_VAL = 100; // sets the value under which the plant will be watered
String mode = "Cactus"; // can take vals Cactus and Normal


void setup()
{
  Serial.begin(9600);

  // set cactus as default type to limit overwatering
  pinMode(normalLEDPin, OUTPUT);
  pinMode(cactusLEDPin, OUTPUT);
  digitalWrite(cactusLEDPin, HIGH);
  digitalWrite(normalLEDPin, LOW);

  
  pinMode(normalButtonPin, INPUT);
  pinMode(cactusButtonPin, INPUT);
  pinMode(pumpPin, OUTPUT); // set A0 to an output so we can use it to turn on the transistor
  pinMode(soilPower, OUTPUT);//Set D7 as an OUTPUT
  digitalWrite(soilPower, LOW);//Set to LOW so no power is flowing through the sensor
  pinMode(soilPin, INPUT);
}


void loop()
{ 
  // Turn on the soil meter power and read the data
  digitalWrite(soilPower, HIGH);

  // Write the moisture/mode data to the serial console
  moisture_level = analogRead(soilPin);
  Serial.println("Mode: " + String(mode));
  Serial.println("Water Time: " + String(waterTime));
  Serial.println("Water Trigger Val: " + String(BASE_VAL));
  Serial.println("Moisture Level: " + String(moisture_level));
  Serial.println();
  digitalWrite(soilPower, LOW);//Set to LOW so no power is flowing through the sensor

  // Turn the pump on if the measured moisure value is lower than the base
  if(moisture_level < BASE_VAL){
    activatePump();
  }

  delay(12*3600000UL); // DELAY WILL BE SAME FOR CACTUS AND NORMAL TYPES - 12 HRS
}


// Custom delay function which allows us to get the button input during wait time 
void myDelay(unsigned long duration)
{  
 unsigned long start = millis();

 while (millis() - start <= duration) {
   checkButtons();  // check the buttons 
 }
}


// Read the button values and set mode values accordinly
void checkButtons()
{
  int cactusButtonState = digitalRead(cactusButtonPin);
  int normalButtonState = digitalRead(normalButtonPin);

  if(cactusButtonState == HIGH && normalButtonState == LOW){
     mode = "Cactus";
  }else if(cactusButtonState == LOW && normalButtonState == HIGH){
    mode = "Normal";
  }

  if(mode == "Normal"){
    digitalWrite(normalLEDPin, HIGH);
    digitalWrite(cactusLEDPin, LOW);
    BASE_VAL = 600;
    waterTime = 20000;
  }else if(mode == "Cactus"){
    digitalWrite(cactusLEDPin, HIGH);
    digitalWrite(normalLEDPin, LOW);
    BASE_VAL = 100;
    waterTime = 10000;
  }
}


// Turns on the pump, waits, and turns pump off
void activatePump(){
  digitalWrite(pumpPin, HIGH); // turn on the motor
  delay(waterTime); 
  digitalWrite(pumpPin, LOW);  // turn off the motor
  return;
}
