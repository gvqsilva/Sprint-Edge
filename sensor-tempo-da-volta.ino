#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>

RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int buttonPin = 2; // Pino digital para o botão
volatile unsigned long lastTime = 0;
unsigned long bestTime = 0;
unsigned long worstTime = 0;
unsigned long currentTime;
unsigned long lapTime;
volatile bool lapStarted = false;
int lapCounter = 1; // Contador de voltas

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), toggleLap, FALLING);

  Serial.begin(9600);
  
  lcd.init(); // Inicializar o LCD
  lcd.backlight();

  if (!rtc.begin()) {
    Serial.println("Não foi possível encontrar o RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC perdeu energia, ajustando o tempo!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  lcd.setCursor(0, 0);
  lcd.print("Timer Pronto");
}

void loop() {
  if (lapStarted) {
    currentTime = millis();
    lapTime = currentTime - lastTime;

    // Mostrar o tempo da volta no LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Volta:");
    lcd.print(lapTime / 1000.0);
    lcd.print("s");

    delay(200); // Pequeno atraso para evitar piscar rapidamente na tela
  }
}

void toggleLap() {
  if (lapStarted) {
    // Parar o cronômetro
    lapStarted = false;
    lastTime = 0;

    // Atualizar o melhor tempo se necessário
    if (bestTime == 0 || lapTime < bestTime) {
      bestTime = lapTime;
    }

    // Atualizar o pior tempo se necessário
    if (lapTime > worstTime) {
      worstTime = lapTime;
    }

    // Incrementar o contador de voltas
    lapCounter++;

    // Exibir a melhor e pior volta no Monitor Serial
    Serial.print("Melhor tempo: Volta ");
    Serial.print(getBestLap());
    Serial.print(" - ");
    Serial.print(bestTime / 1000.0);
    Serial.print("s   Pior tempo: Volta ");
    Serial.print(getWorstLap());
    Serial.print(" - ");
    Serial.print(worstTime / 1000.0);
    Serial.println("s");
  } else {
    // Iniciar uma nova volta
    lapStarted = true;
    lastTime = millis();
  }
}

// Retorna o número da melhor volta
int getBestLap() {
  return (bestTime == 0) ? 0 : lapCounter - 1;
}

// Retorna o número da pior volta
int getWorstLap() {
  return (worstTime == 0) ? 0 : lapCounter - 1;
}