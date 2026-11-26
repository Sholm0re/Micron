
const byte VARIANT = 2;


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

void setup() {
  Serial.begin(9600);
  Serial.print(F("VARIANT="));
  Serial.println(VARIANT);

  
  const int tests[][2] = {
    {3, 4},      
    {-90, 90},    
    {90, 90},     
    {91, 0},      
    {0, -91}      
  };

  const byte count = sizeof(tests) / sizeof(tests[0]);
  for (byte i = 0; i < count; ++i) {
    const int a = tests[i][0];
    const int b = tests[i][1];
    int answer = 0;

    Serial.print(F("test="));
    Serial.print(i + 1);
    Serial.print(F("; a="));
    Serial.print(a);
    Serial.print(F("; b="));
    Serial.print(b);

    if (calcSquareOfSum(a, b, answer)) {
      Serial.print(F("; F="));
      Serial.println(answer);
    } else {
      Serial.println(F("; ERROR_RANGE"));
    }
  }
}

void loop() {}