
/*
  Code that allow you to test dual rs-458, one to read incoming data and one to write data

*/
int incomingByte = 0;

int wSpeed = 0;
int a = true;

void setup() {
  Serial1.begin(115200);
  Serial2.begin(115200);
  pinMode(PC13, OUTPUT);
  pinMode(PB7, OUTPUT);

  digitalWrite(PC13, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(100);              // wait for a second
  digitalWrite(PC13, HIGH);    // turn the LED off by making the voltage LOW
  delay(100);
  digitalWrite(PC13, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(100);              // wait for a second
  digitalWrite(PC13, HIGH);    // turn the LED off by making the voltage LOW
  delay(100);
  digitalWrite(PC13, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(100);              // wait for a second
  digitalWrite(PC13, HIGH);    // turn the LED off by making the voltage LOW
  delay(500);

  digitalWrite(PB7, HIGH);    //mx485 write mode
}


void loop() {

  if (a) {
    wSpeed = wSpeed + 130; 
    if (wSpeed >= 28000) { //280.00
      a = false;
    }
  } else {
    wSpeed = wSpeed - 130;
    if (wSpeed <= 0) {
      a = true;
    }
  }

  sendDataMessage(0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  delay(40);

}

void sendDataMessage(int b0, int b1, int b2, int b3, int b4, int b5, int b6,
                     int b7, int b8, int b9, int b10) {

  Serial1.write(b0); //B0// reciverID (to who is send)
  Serial1.write(b1); //B1// senderID (who sends the message)
  Serial1.write(b2); //B2// dataID [(0x00)

  String fSpeed = convertIntTo16bitHex(wSpeed);
 
  Serial1.write(convertStringHexTo8bInt(fSpeed.substring(4, 8))); //B4// d1
  Serial1.write(convertStringHexTo8bInt(fSpeed.substring(0, 4))); //B3// d0

  Serial1.write(b5); //B5// d2
  Serial1.write(b6); //B6// d3
  Serial1.write(b7); //B7// d4
  Serial1.write(b8); //B8// d5
  Serial1.write(b9); //B9// d6
  Serial1.write(b10); //B10// d7

  float ercSum = b0 + b1 + b2 + convertStringHexTo8bInt(fSpeed.substring(4, 8)) + convertStringHexTo8bInt(fSpeed.substring(0, 4)) + b5 + b6 + b7 + b8 + b9 + b10;

  String ercSum_P1 = convertIntTo16bitHex (ercSum);
  int ercSum_P2 = convertStringHexTo8bInt(ercSum_P1.substring(0, 4));
  int ercSum_P3 = convertStringHexTo8bInt(ercSum_P1.substring(4, 8));
  Serial1.write(ercSum_P2); //B11// erc0
  Serial1.write(ercSum_P3); //B12// erc1
}


String convertIntTo16bitHex (int n) {
  float n0 = (float) n / 16.0f;
  float n1 = (float) n0 / 16.0f;
  float n2 = (float) n1 / 16.0f;
  float n3 = (float) n2 / 16.0f;

  int h0 = round(n) % 16;
  int h1 = int(n0) % 16;
  int h2 = int(n1) % 16;
  int h3 = int(n2) % 16;

  return "0x" + String(hexFormating(h3)) + String(hexFormating(h2)) + "0x" + String(hexFormating(h1)) + String(hexFormating(h0)) ;
  //0x320x10
}

char hexFormating(int eToHex) {
  char a = ' ';
  switch (eToHex) {
    case 0:
      a = '0';
      break;
    case 1:
      a = '1';
      break;
    case 2:
      a = '2';
      break;
    case 3:
      a = '3';
      break;
    case 4:
      a = '4';
      break;
    case 5:
      a = '5';
      break;
    case 6:
      a = '6';
      break;
    case 7:
      a = '7';
      break;
    case 8:
      a = '8';
      break;
    case 9:
      a = '9';
      break;
    case 10:
      a = 'A';
      break;
    case 11:
      a = 'B';
      break;
    case 12:
      a = 'C';
      break;
    case 13:
      a = 'D';
      break;
    case 14:
      a = 'E';
      break;
    case 15:
      a = 'F';
      break;
    default:
      break;
  }
  return a;
}

int convertStringHexTo8bInt(String stringHex) {

  int value = 0;

  String stringHexTrimg0 = stringHex.substring(3, 4);
  String stringHexTrimg1 = stringHex.substring(2, 3);

  if (stringHexTrimg0.equals("A")) {
    value = value + 10;
  } else if (stringHexTrimg0.equals("B")) {
    value = value + 11;
  } else if (stringHexTrimg0.equals("C")) {
    value = value + 12;
  } else if (stringHexTrimg0.equals("D")) {
    value = value + 13;
  } else if (stringHexTrimg0.equals("E")) {
    value = value + 14;
  } else if (stringHexTrimg0.equals("F")) {
    value = value + 15;
  } else {
    value = value + stringHexTrimg0.toInt();
  }

  if (stringHexTrimg1.equals("A")) {
    value = value + (10 * 16);
  } else if (stringHexTrimg1.equals("B")) {
    value = value + (11 * 16);
  } else if (stringHexTrimg1.equals("C")) {
    value = value + (12 * 16);
  } else if (stringHexTrimg1.equals("D")) {
    value = value + (13 * 16);
  } else if (stringHexTrimg1.equals("E")) {
    value = value + (14 * 16);
  } else if (stringHexTrimg1.equals("F")) {
    value = value + (15 * 16);
  } else {
    value = value + (stringHexTrimg1.toInt() * 16);
  }

  return value;
}





