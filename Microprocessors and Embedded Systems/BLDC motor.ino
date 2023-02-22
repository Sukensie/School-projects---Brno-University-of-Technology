#include <ESP32Servo.h>
//#include "ESC.h"

#define LED 2
#define ESC_PIN 16
#define MOTOR_PIN 17

Servo esc;
int val;
int loopCnt = 0; 
int speed = 1200; //inicializace minimální rychlosti (pokud by bylo menší číslo, přejde motor do kalibračního režimu) [mikrosekundy]

void setup() {
  Serial.begin(9600); //nastavení komunikace s deskou a nastavení počtu baudů
  esc.attach(ESC_PIN); //připojení esc k pinu 16

  pinMode(LED,OUTPUT); //nastavení indikační LED diody jako výstupní zařížení
  pinMode(MOTOR_PIN,INPUT_PULLDOWN); //nastavení portu motoru jako pulldown rezistor
}

void loop() {
  
  //nastavení kalibrace motoru pouze při prvním proběhnutí smyčky
  //https://dratek.cz/docs/produkty/1/1056/1498208081.pdf
  if(loopCnt == 0)
  {
    calibrateMotor();
  }
 
  delay(100);

  //pokud je signál optické závory přerušený
  if(digitalRead(MOTOR_PIN) == LOW)
  {
    increaseSpeed();
  }
  else
  {
    decreaseSpeed();
  }

  esc.write(speed); //nastavení aktuální rychlosti
  loopCnt++;
}


void calibrateMotor() {
  val = 1050;
  delay(500);
  esc.writeMicroseconds(val);
}

void increaseSpeed() {
  digitalWrite(LED,HIGH); //rozsvícení indikační led diody

  Serial.println(speed); //debug výpis rychlosti motoru
  
  //pokud není motor na maximální rychlosti, dochází k jejímu zvyšování
  //2500 je v knihovně zadefinováno jako maximální pulsní šířka
  if(speed < 2495)
  {
    speed = speed + 25;
  }
}

void decreaseSpeed() {
  digitalWrite(LED,LOW); //zasnutí indikační led diody

  Serial.println(speed); //debug výpis rychlosti motoru

  //pokud není motor na minimální rychlosti, dochází k jejímu snižování
  //od 1200 ms se už začně něco dít, ale teoreticky by šlo snižovat až po kalibračnních 1050
  if(speed > 1200)
  {
    speed = speed - 25;
  }
}