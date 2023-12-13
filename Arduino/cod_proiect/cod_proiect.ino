// 

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    char input = Serial.read();

    if (input == '1') {
      digitalWrite(LED_BUILTIN, HIGH);
    }

    if (input == '0') {
      digitalWrite(LED_BUILTIN, LOW);
    }

    delay(100);
  }
}

