#define STEP1 15
#define DIR1  14
#define STEP2 3
#define DIR2  4
#define STEP3 1
#define DIR3  2


void setup() {
  pinMode(STEP1, OUTPUT);
  pinMode(DIR1, OUTPUT);
  pinMode(STEP2, OUTPUT);
  pinMode(DIR2, OUTPUT);
  pinMode(STEP3, OUTPUT);
  pinMode(DIR3, OUTPUT);
}

void loop() {
  coarseControl(1000, 1024, 0);
  delay(5000);
}


void coarseControl(int timePerStep, int steps, int dir){

  digitalWrite(DIR1, dir);
  digitalWrite(DIR2, dir);
  digitalWrite(DIR3, dir);

  for (int i = 0; i < steps; i++) {
    digitalWrite(STEP1, HIGH);
    digitalWrite(STEP2, HIGH);
    digitalWrite(STEP3, HIGH);

    delayMicroseconds(timePerStep);

    digitalWrite(STEP1, LOW);
    digitalWrite(STEP2, LOW);
    digitalWrite(STEP3, LOW);

    delayMicroseconds(timePerStep);
  }
}