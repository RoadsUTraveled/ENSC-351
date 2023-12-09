#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

const int N1 = 25;
const int N2 = 26;
const int N3 = 27;


BluetoothSerial SerialBT;
int cmd = 0;


void setup() {
  Serial.begin(115200);
  SerialBT.begin("BBGBLU"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
 
  pinMode(N1, OUTPUT);
  pinMode(N2, OUTPUT);
  pinMode(N3, OUTPUT);

  digitalWrite(N1,LOW);
  digitalWrite(N2,LOW);
  digitalWrite(N3,LOW);
}

void loop() {
 
  if (SerialBT.available()) {
    cmd = SerialBT.read();
    //speed = SerialBT.readString();
  }
 
  if (cmd == 0) {
    digitalWrite(N1,LOW);
    digitalWrite(N2,LOW);
  }
  if (cmd == 1) {
    digitalWrite(N1,HIGH);
    digitalWrite(N2,LOW);
  }
  if (cmd == 2) {
    digitalWrite(N1,LOW);
    digitalWrite(N2,HIGH);
  }
  if (cmd == 3) {
    digitalWrite(N1,HIGH);
    digitalWrite(N2,HIGH);
  }
  if (cmd == 4){
    digitalWrite(N3,HIGH);
  }
  if (cmd == 5){
    digitalWrite(N3,LOW);
  }

  
  delay(20);
}
