
void discardLine() {
  const unsigned long started = millis();
  while (millis() - started < 300UL) {
    if (Serial.available() > 0 && Serial.read() == '\n') return;
  }
}

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(250);
  Serial.println(F("Student: Bilas Mykolai | Group: KI-25-1 | Variant: 2"));
  Serial.println(F("Send: I 7 2 (x3), I 7 (x3), I x (x3) with Newline"));
}

void loop() {
  if (Serial.available() <= 0) return;
  const int mode = Serial.read();
  if (mode == '\r' || mode == '\n' || mode == ' ') return;

  const unsigned long started = millis();
  if (mode == 'I') {
    const long a = Serial.parseInt();
    const long b = Serial.parseInt();
    const unsigned long parseElapsed = millis() - started;

    Serial.print(F("a=")); Serial.print(a);
    Serial.print(F("; b=")); Serial.print(b);
    if (a < -100 || a > 100 || b < -100 || b > 100) {
      Serial.print(F("; ERROR_RANGE"));
    } else if (b == 0) {
      Serial.print(F("; ERROR_DIV0_OR_MISSING"));
    } else {
      Serial.print(F("; a/b=")); Serial.print(a / b);
    }
    Serial.print(F("; parse_ms=")); Serial.println(parseElapsed);
  } else {
    Serial.println(F("ERROR_MODE"));
  }
  discardLine();
}