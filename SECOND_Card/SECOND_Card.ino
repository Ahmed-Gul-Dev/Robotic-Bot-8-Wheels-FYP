#define A 16
#define B 17

#define r1 32
#define r2 33
#define r3 25
#define r4 26
#define r5 27
#define r6 14
#define r7 18
#define r8 19
#define r9  12
#define r10 13
#define r11 15
#define r12 4
#define r13 5
#define r14 21
#define r15 22
#define r16 23

uint32_t pmillis = 0;
int relays[16] = {r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15, r16};

void STOP() {
  for (int i = 0; i < 16; i++) {
    digitalWrite(relays[i], HIGH);
  }
  delay(500);
}

void setup() {
  Serial.begin(115200);
  Serial.println("OK");
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);

  pinMode(A, INPUT);
  pinMode(B, INPUT);
  pinMode(35, INPUT);
  for (int i = 0; i < 16; i++) {
    pinMode(relays[i], OUTPUT);
  }
  STOP();
  digitalWrite(2, HIGH);
  delay(5000);
}

int command = 5, commandtemp = 0;
bool stopC = false;
void loop() {

  if (digitalRead(35) == HIGH) {
    Serial.println("STOP");
    stopC = true; command = 0;
  } else {
    stopC = false;
  }
  if (digitalRead(A) == HIGH && digitalRead(B) == HIGH && stopC == false) {
    Serial.println("Forward");
    command = 1;
  }
  else if (digitalRead(A) == LOW && digitalRead(B) == LOW && stopC == false) {
    Serial.println("Back");
    command = 2;
  }
  else if (digitalRead(A) == HIGH && digitalRead(B) == LOW && stopC == false) {
    Serial.println("RIGHT");
    command = 3;
  }
  else if (digitalRead(A) == LOW && digitalRead(B) == HIGH && stopC == false) {
    Serial.println("Left");
    command = 4;
  }

  if (commandtemp != command) {
    commandtemp = command;
    Serial.println("Execution");
    if (command == 0) {
      STOP();
    } else if (command == 1) {
      BACK();
    } else if (command == 2) {
      FWD();
    } else if (command == 3) {
      LEFT();
    } else if (command == 4) {
      RIGHT();
    }
  }
}

void FWD() {
  STOP();
  digitalWrite(r1, LOW);
  digitalWrite(r3, LOW);
  digitalWrite(r5, LOW);
  digitalWrite(r8, LOW);
  digitalWrite(r15, LOW);
  digitalWrite(r14, LOW);
  digitalWrite(r12, LOW);
  digitalWrite(r10, LOW);
}

void BACK() {
  STOP();
  digitalWrite(r2, LOW);
  digitalWrite(r4, LOW);
  digitalWrite(r6, LOW);
  digitalWrite(r7, LOW);
  digitalWrite(r16, LOW);
  digitalWrite(r13, LOW);
  digitalWrite(r11, LOW);
  digitalWrite(r9, LOW);
}

void LEFT() {
  STOP();
  digitalWrite(r16, LOW);
  digitalWrite(r13, LOW);
  digitalWrite(r11, LOW);
  digitalWrite(r9, LOW);
}

void RIGHT() {
  STOP();
  digitalWrite(r2, LOW);
  digitalWrite(r4, LOW);
  digitalWrite(r6, LOW);
  digitalWrite(r7, LOW);
}

void debug() {
  if (Serial.available() > 0) {
    char Data = Serial.read();
    Serial.println(Data);
    if (Data == '0') {
      STOP();
    }
    else if (Data == '1') { //fwd motor 1
      STOP();
      digitalWrite(r1, LOW);
      digitalWrite(r10, LOW);
    }
    else if (Data == '2') {//rev motor 1
      STOP();
      digitalWrite(r2, LOW);
      digitalWrite(r9, LOW);
    }
    else if (Data == '3') {//fwd motor 2
      STOP();
      digitalWrite(r3, LOW);
      digitalWrite(r12, LOW);
    }
    else if (Data == '4') {//rev motor 2
      STOP();
      digitalWrite(r4, LOW);
      digitalWrite(r11, LOW);
    }
    else if (Data == '5') {
      STOP();
      digitalWrite(r5, LOW);
      digitalWrite(r14, LOW);
    }
    else if (Data == '6') {
      STOP();
      digitalWrite(r6, LOW);
      digitalWrite(r13, LOW);
    }
    else if (Data == '7') {
      STOP();
      digitalWrite(r7, LOW);
      digitalWrite(r15, LOW);
    }
    else if (Data == '8') {
      STOP();
      digitalWrite(r8, LOW);
      digitalWrite(r16, LOW);
    }
  }
}
