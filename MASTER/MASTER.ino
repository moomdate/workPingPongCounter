#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>

File myFile;
SoftwareSerial BTserial(11, 12);
int BluePin[] = {22, 24, 26, 28, 30, 32, 34};
int GreenPin[] = {36, 38, 40, 42, 44, 46, 48};
int RedPin[] = {23, 25, 27, 29, 31, 33, 35};
int Digit_common[] = {37, 39, 41, 43, 45, 47};
int state = 0;
int max_ = 6;
int freq = 800;
int freq3 = 300;
int freq2 = 300;
int Score_red = 0;
int Set_red = 0;
int Score_blue = 0;
int Set_blue = 0;
int game_o = 11;
int diw = 0;
//score for save to sd card
int SetRed[] = {0, 0, 0, 0, 0};
int SetBlue[] = {0, 0, 0, 0, 0};
int Score_MatRed[] = {0, 0, 0, 0, 0};
int Score_MatBlue[] = {0, 0, 0, 0, 0};

int chipSelect = 13;
int setGameCount = 0; //count game set 
int present = 0;
void setup() {
  Serial.begin(9600);
  BTserial.begin(38400);
  for (int i = 0; i < sizeof(BluePin) / 2; i++) {
    pinMode(BluePin[i], OUTPUT);
    pinMode(GreenPin[i], OUTPUT);
    pinMode(RedPin[i], OUTPUT);
  }
  for (int i = 0; i < sizeof(Digit_common) / 2; i++) {
    pinMode(Digit_common[i], OUTPUT);
  }
  for (int i = 0; i < sizeof(Digit_common) / 2; i++) {
    digitalWrite(Digit_common[i], HIGH);
  }
  for (int i = 0; i < sizeof(BluePin) / 2; i++) {
    digitalWrite(BluePin[i], LOW);
    digitalWrite(GreenPin[i], LOW);
    digitalWrite(RedPin[i], LOW);
  }
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  pinMode(8, INPUT_PULLUP);
  pinMode(9, OUTPUT);
  pinMode(10, INPUT_PULLUP);
  digitalWrite(9, 0);
  led_d(2);
  checkSD();
  while(digitalRead(8)==1){
     RedTeam(1);
    Set_(9, 9);
    BlueTeam(1);
   }
  //  endGameWriteScore();
}
void led_d(int digit) {
  if (digit == 2) {
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW); // red l
    digitalWrite(7, HIGH);
    digitalWrite(6, LOW); // red r
  } else if (digit == 1) {
    digitalWrite(5, HIGH);
    digitalWrite(4, LOW); //blue l
    digitalWrite(7, LOW);
    digitalWrite(6, HIGH); // red r
  }
}
void loop() {
  // Serial.println();
  if (digitalRead(2) == 1) {
    led_d(1);
  } else if (digitalRead(3) == 1) {
    led_d(2);
  }
  RedTeam(Score_red);
  Set_(Set_red, Set_blue);
  BlueTeam(Score_blue);
  if (BTserial.available() > 0) {
    state = BTserial.read();
  }
  if (state == 248) {
    //  Serial.println("RED");
    state = 0;
    if (digitalRead(2) == 1) {
      Score_blue++;
      present = 1;
    } else {
      Score_red++;
      present = 0;
    }
    led_d(1);
  }
  else if (state == 128) {
    //Serial.println("BLACK");
    state = 0;
    if (digitalRead(3) == 1) {
      Score_blue++;
      present = 1;
    } else {
      Score_red++;
      present = 0;
    }
    led_d(2);
  }
  else if (digitalRead(10) == 0) {
    while (digitalRead(10) == 0) { delay(10); }
      if (present == 1) {
        Score_blue--;
      }else {
        Score_red--;
      }
  }
  if (Score_red >= 10 && Score_blue >= 10) {
    if (Score_blue <= Score_red - 2) {
      setSetRed();
    } else if (Score_red <= Score_blue - 2) {
      SetSetBlue();
    }
  }
  else if (Score_red == game_o && Score_blue < 10) {
    setSetRed();
  }
  else if (Score_blue == game_o && Score_red < 10) {
    SetSetBlue();
  }
}
void setSetRed() {
  Set_red++;
  SetBlue[setGameCount] = 0;
  SetRed[setGameCount] = 1;

  Score_MatRed[setGameCount] = Score_red;
  Score_MatBlue[setGameCount] = Score_blue;

  setGameCount++;
  if (Set_red >= 3) {
    endGameWriteScore();
    while (1) {
      RedTeam(99);
    }
  }
  int i = 300;
  while (i--) {
    RedTeam(Score_red);
    Set_(Set_red, Set_blue);
    BlueTeam(Score_blue);
  }
  Score_red = 0; Score_blue = 0;
}
void SetSetBlue() {
  Set_blue++;
  SetBlue[setGameCount] = 1;
  SetRed[setGameCount] = 0;

  Score_MatRed[setGameCount] = Score_red;
  Score_MatBlue[setGameCount] = Score_blue;
  setGameCount++;
  if (Set_blue >= 3) {
    endGameWriteScore();
    while (1) {
      BlueTeam(99);
    }
  }
  int i = 300;
  while (i--) {
    RedTeam(Score_red);
    Set_(Set_red, Set_blue);
    BlueTeam(Score_blue);
  }
  Score_red = 0; Score_blue = 0;

}
void RedTeam(int number) {
  if (number >= 10) {
    int aaa = number / 10;
    int bbb = number % 10;
    digitalWrite(Digit_common[0], LOW);
    digitalWrite(Digit_common[1], HIGH);
    Red(aaa);
    digitalWrite(Digit_common[0], HIGH);
    digitalWrite(Digit_common[1], LOW);
    Red(bbb);
  } else {
    digitalWrite(Digit_common[0], LOW);
    digitalWrite(Digit_common[1], HIGH);
    for (int i = 0; i < sizeof(BluePin) / 2; i++) {
      if (i == 0 || i == 1 || i == 2 || i == 3 || i == 4 || i == 5) {
        digitalWrite(RedPin[i], HIGH);
        delayMicroseconds(freq);
        digitalWrite(RedPin[i], LOW);
      }
    }
    digitalWrite(Digit_common[0], HIGH);
    digitalWrite(Digit_common[1], LOW);
    Red(number);
  }
}
void BlueTeam(int number) {
  if (number >= 10) {
    int aaa = number / 10;
    int bbb = number % 10;
    digitalWrite(Digit_common[4], LOW);
    digitalWrite(Digit_common[5], HIGH);
    Blue(aaa);
    digitalWrite(Digit_common[4], HIGH);
    digitalWrite(Digit_common[5], LOW);
    Blue(bbb);
  } else {
    digitalWrite(Digit_common[4], LOW);
    digitalWrite(Digit_common[5], HIGH);
    for (int i = 0; i < sizeof(BluePin) / 2; i++) {
      if (i == 0 || i == 1 || i == 2 || i == 3 || i == 4 || i == 5) {
        digitalWrite(BluePin[i], HIGH);
        delayMicroseconds(freq2);
        digitalWrite(BluePin[i], LOW);
      }
    }
    digitalWrite(Digit_common[4], HIGH);
    digitalWrite(Digit_common[5], LOW);
    Blue(number);
  }
}
void Red(int number) { //display 7-seg red
  for (int i = 0; i < sizeof(BluePin) / 2; i++) {
    if ((i == 0 && (number == 2 || number == 3 || number == 5 || number == 6 || number == 7 || number == 8 || number == 9 || number == 0)) || (i == 1 && (number == 0 || number == 1 || number == 2 || number == 3 || number == 4 || number == 7 || number == 8 || number == 9))) {
      digitalWrite(RedPin[i], HIGH);
      delayMicroseconds(freq);
      digitalWrite(RedPin[i], LOW); //a b
    } else if ((i == 2 && (number == 0 || number == 1 || number == 3 || number == 4 || number == 5 || number == 6 || number == 7 || number == 8 || number == 9)) || (i == 3 && (number == 0 || number == 2 || number == 3 || number == 5 || number == 6 || number == 8 || number == 9))) {
      digitalWrite(RedPin[i], HIGH);
      delayMicroseconds(freq);
      digitalWrite(RedPin[i], LOW); //c d
    } else if ((i == 4 && (number == 0 || number == 2 || number == 6 || number == 8)) || (i == 6 && (number == 2 || number == 3 || number == 4 || number == 5 || number == 6 || number == 8 || number == 9))) {
      digitalWrite(RedPin[i], HIGH);
      delayMicroseconds(freq);
      digitalWrite(RedPin[i], LOW); //e g
    }
    else if ((i == 5 && (number == 0 || number == 4 || number == 5 || number == 6 || number == 8 || number == 9))) {
      digitalWrite(RedPin[i], HIGH);
      delayMicroseconds(freq);
      digitalWrite(RedPin[i], LOW); //f
    }
  }
}
void Blue(int number) {
  for (int i = 0; i < sizeof(BluePin) / 2; i++) {
    if ((i == 0 && (number == 2 || number == 3 || number == 5 || number == 6 || number == 7 || number == 8 || number == 9 || number == 0)) || (i == 1 && (number == 0 || number == 1 || number == 2 || number == 3 || number == 4 || number == 7 || number == 8 || number == 9))) {
      digitalWrite(BluePin[i], HIGH);
      delayMicroseconds(freq2);
      digitalWrite(BluePin[i], LOW); //a b
    } else if ((i == 2 && (number == 0 || number == 1 || number == 3 || number == 4 || number == 5 || number == 6 || number == 7 || number == 8 || number == 9)) || (i == 3 && (number == 0 || number == 2 || number == 3 || number == 5 || number == 6 || number == 8 || number == 9))) {
      digitalWrite(BluePin[i], HIGH);
      delayMicroseconds(freq2);
      digitalWrite(BluePin[i], LOW); //c d
    } else if ((i == 4 && (number == 0 || number == 2 || number == 6 || number == 8)) || (i == 6 && (number == 2 || number == 3 || number == 4 || number == 5 || number == 6 || number == 8 || number == 9))) {
      digitalWrite(BluePin[i], HIGH);
      delayMicroseconds(freq2);
      digitalWrite(BluePin[i], LOW); //e g
    }
    else if ((i == 5 && (number == 0 || number == 4 || number == 5 || number == 6 || number == 8 || number == 9))) {
      digitalWrite(BluePin[i], HIGH);
      delayMicroseconds(freq2);
      digitalWrite(BluePin[i], LOW); //f
    }
  }
}
void Set_(int number, int number2) { //display number first gigit and second digit 
  // int aaa = number / 10;
  // int bbb = number % 10;
  digitalWrite(Digit_common[2], LOW);
  digitalWrite(Digit_common[3], HIGH);
  green(number);
  digitalWrite(Digit_common[2], HIGH);
  digitalWrite(Digit_common[3], LOW);
  green(number2);
}
void green(int number) { //display number to 7-seg light green
  for (int i = 0; i < sizeof(BluePin) / 2; i++) {
    if ((i == 0 && (number == 2 || number == 3 || number == 5 || number == 6 || number == 7 || number == 8 || number == 9 || number == 0)) || (i == 1 && (number == 0 || number == 1 || number == 2 || number == 3 || number == 4 || number == 7 || number == 8 || number == 9))) {
      digitalWrite(GreenPin[i], HIGH);
      delayMicroseconds(freq3);
      digitalWrite(GreenPin[i], LOW); //a b
    } else if ((i == 2 && (number == 0 || number == 1 || number == 3 || number == 4 || number == 5 || number == 6 || number == 7 || number == 8 || number == 9)) || (i == 3 && (number == 0 || number == 2 || number == 3 || number == 5 || number == 6 || number == 8 || number == 9))) {
      digitalWrite(GreenPin[i], HIGH);
      delayMicroseconds(freq3);
      digitalWrite(GreenPin[i], LOW); //c d
    } else if ((i == 4 && (number == 0 || number == 2 || number == 6 || number == 8)) || (i == 6 && (number == 2 || number == 3 || number == 4 || number == 5 || number == 6 || number == 8 || number == 9))) {
      digitalWrite(GreenPin[i], HIGH);
      delayMicroseconds(freq3);
      digitalWrite(GreenPin[i], LOW); //e g
    }
    else if ((i == 5 && (number == 0 || number == 4 || number == 5 || number == 6 || number == 8 || number == 9))) {
      digitalWrite(GreenPin[i], HIGH);
      delayMicroseconds(freq3);
      digitalWrite(GreenPin[i], LOW); //f
    }
  }
}
void checkSD() //check sd card
{
  pinMode(SS, OUTPUT);

  if (!SD.begin(chipSelect)) {
    //Serial.println("initialization failed!");
    return;
  }
  //Serial.println("initialization done.");
}
void endGameWriteScore() { //save data to sd card after end mat
  myFile = SD.open("SCORE.csv", FILE_WRITE);
  if (myFile) {
    //  Serial.print("Writing to test.txt...");
    myFile.println("RedScore,RedWin,BlueWin,BlueScore.");
    for (int i = 0; i < 5; i++) {
      myFile.print(Score_MatRed[i]);
      myFile.print(",");
      myFile.print(SetRed[i]);
      myFile.print(",");
      myFile.print(SetBlue[i]);
      myFile.print(",");
      myFile.println(Score_MatBlue[i]);
    }
    // Serial.print("End");
    myFile.close();
  } else {
    //Serial.println("error opening test.txt");
  }
}

