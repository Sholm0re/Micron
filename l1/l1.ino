
const byte VARIANT = 2;
const byte LED_A_PIN = 8;   
const byte LED_B_PIN = 9;   
const byte BUTTON_PIN = 2;  

void setLeds(bool a, bool b) {
  digitalWrite(LED_A_PIN, a ? HIGH : LOW);
  digitalWrite(LED_B_PIN, b ? HIGH : LOW);
}

void showStep(bool a, bool b, unsigned long durationMs) {
  setLeds(a, b);
  delay(durationMs);
}

void runPressedCycle() {
  showStep(false, true, 600);   
  showStep(false, false, 400);  
}

void setup() {
  pinMode(LED_A_PIN, OUTPUT);
  pinMode(LED_B_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  setLeds(false, false);

  Serial.begin(9600);
  Serial.print(F("VARIANT="));
  Serial.println(VARIANT);
}

void loop() {
  const bool pressed = (digitalRead(BUTTON_PIN) == LOW);
  if (pressed) {
    Serial.println(F("PRESSED; CYCLE_START"));
    runPressedCycle();
  } else {
    setLeds(false, false);
    Serial.println(F("RELEASED"));
    delay(100);
  }
}