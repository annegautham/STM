#define STEP_PIN 15  // Connect to A4988 STEP
#define DIR_PIN  14  // Connect to A4988 DIR

void setup() {
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
}

void loop() {
  digitalWrite(DIR_PIN, HIGH);  // Set direction (HIGH or LOW)

  int steps = 1000;  // Number of steps per loop (adjust as needed)

  for (int i = 0; i < steps; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(500);     // Step pulse width (adjust speed here)
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(500);
  }

  delay(500);  // Wait 1 second before repeating
}
