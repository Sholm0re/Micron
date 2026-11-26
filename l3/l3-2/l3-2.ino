

#include <stdlib.h>
#include <errno.h>

const byte VARIANT = 2;
char line[48];
byte used = 0;
bool dropping = false;
bool sawCR = false;
unsigned long lastByteAt = 0;


bool calcSquareOfSum(int a, int b, int &result) {
  if (a < -90 || a > 90 || b < -90 || b > 90) {
    return false;
  }
  const long sum = long(a) + b;
  const long value = sum * sum;
  if (value < -32768L || value > 32767L) {
    return false;
  }
  result = int(value);
  return true;
}

void evaluate(char *text) {
  char *next = nullptr;
  errno = 0;
  const long rawA = strtol(text, &next, 10);
  if (next == text || errno == ERANGE) {
    Serial.println(F("ERROR_FORMAT_A"));
    return;
  }

  if (*next != ' ' && *next != '\t') {
    Serial.println(F("ERROR_SEPARATOR"));
    return;
  }
  while (*next == ' ' || *next == '\t') {
    ++next;
  }

  char *startB = next;
  errno = 0;
  const long rawB = strtol(startB, &next, 10);
  if (next == startB || errno == ERANGE) {
    Serial.println(F("ERROR_FORMAT_B"));
    return;
  }

  while (*next == ' ' || *next == '\t') {
    ++next;
  }
  if (*next != '\0') {
    Serial.println(F("ERROR_EXTRA"));
    return;
  }

  if (rawA < -90L || rawA > 90L || rawB < -90L || rawB > 90L) {
    Serial.print(F("a=")); Serial.print(rawA);
    Serial.print(F("; b=")); Serial.print(rawB);
    Serial.println(F("; ERROR_RANGE"));
    return;
  }

  const int a = int(rawA);
  const int b = int(rawB);
  int answer = 0;

  
  Serial.print(F("a=")); Serial.print(a);
  Serial.print(F("; b=")); Serial.print(b);

  if (calcSquareOfSum(a, b, answer)) {
    Serial.print(F("; F="));
    Serial.println(answer);
  } else {
    Serial.println(F("; ERROR_RANGE"));
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println(F("Student: Bilas Mykolai | Group: KI-25-1 | Variant: 2"));
  Serial.println(F("READY: enter 'a b' in [-90..90] ending with LF (115200 baud)"));
}

void loop() {
  if ((used > 0 || sawCR) && !dropping && (millis() - lastByteAt >= 2000UL)) {
    used = 0;
    sawCR = false;
    dropping = true;
    Serial.println(F("ERROR_TIMEOUT"));
  }

  byte budget = 16;
  while (budget-- > 0 && Serial.available() > 0) {
    const int raw = Serial.read();
    if (raw < 0) break;
    const char c = char(raw);
    lastByteAt = millis();

    if (c == '\r' && !dropping) {
      if (sawCR) {
        dropping = true;
        Serial.println(F("ERROR_CR"));
      }
      sawCR = true;
      continue;
    }

    if (c == '\n') {
      if (!dropping) {
        line[used] = '\0';
        if (used == 0) {
          Serial.println(F("ERROR_EMPTY"));
        } else {
          evaluate(line);
        }
      }
      used = 0;
      dropping = false;
      sawCR = false;
      continue;
    }

    if (!dropping) {
      if (sawCR) {
        dropping = true;
        Serial.println(F("ERROR_CR"));
        continue;
      }
      if ((raw < 32 && c != '\t') || raw > 126) {
        dropping = true;
        Serial.println(F("ERROR_CHARACTER"));
        continue;
      }
      if (used < sizeof(line) - 1) {
        line[used++] = c;
      } else {
        used = 0;
        dropping = true;
        Serial.println(F("ERROR_LENGTH"));
      }
    }
  }
}