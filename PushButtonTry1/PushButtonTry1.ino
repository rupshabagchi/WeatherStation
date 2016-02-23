// Try 1: lights bulb, waits for 1sec, off 1sec, light 3sec, off 3sec, light 5sec, ..

const int LED = 13; //connect to LED to pin 13
const int BUTTON = 7; // Connect BUTTON to pin 7

int val = 0; // to store the state of the input pin


void setup() {
  pinMode(LED, OUTPUT); // set LED to output
  pinMode(BUTTON, INPUT); // set BUTTON to input

}

void loop() {
  val = digitalRead(BUTTON); // read input value and store 
  
  // check whether input is high (button pressed)
  if (val == HIGH) {
    // light for 1 sec n off for 1 sec
    digitalWrite(LED, HIGH);
    delay(1000);
    digitalWrite(LED, LOW);
    delay(1000);
    
     // light for 3 sec n off for 3 sec
    digitalWrite(LED, HIGH);
    delay(3000);
    digitalWrite(LED, LOW);
    delay(3000);
    
    // light for 5 sec n off for 5 sec
    digitalWrite(LED, HIGH);
    delay(5000);
    digitalWrite(LED, LOW);
    delay(5000);
  } else {
    digitalWrite(LED, LOW);
  }

}
