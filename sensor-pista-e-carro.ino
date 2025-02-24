//Este projeto IoT no Wokwi agora inclui a medição de temperatura e umidade,
//consumo de corrente, velocidade e registro de data e hora, oferecendo uma
//visão completa dos dados relevantes para a Fórmula E. Para funcionalidades 
//mais avançadas,  você pode migrar para o ESP32 e adicionar conectividade 
//Wi-Fi para enviar dados para um servidor ou aplicação web.

//Dois sensores em um prototipo com RTC para armazenar dados
//O 1º sensor é voltado a captura de dados da pista, sendo Humidade, 
//Temperatura e de chuva
//Já o 2º sensor é voltado a captura de dados do carro, sendo Rotação Por
//Minuto (RPM) e o consumo de corrente do carro

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <RTClib.h>
#include <EEPROM.h>

#define DHTPIN 2
#define DHTTYPE DHT22
#define RAIN_SENSOR A1 // Pino do sensor de chuva
#define POT_PIN A0
#define ENCODER_DT 3
#define ENCODER_CLK 4

//byte dos emojis
byte triste[8] = {  B00000, B10001,  B00000,  B00000,  B01110,  B10001,  B00000,};
byte sorriso[8] = {  B00000,  B10001,  B00000,  B00000,  B10001,  B01110,  B00000,};
byte serio[8] = { B00000, B10001, B00000, B00000, B11111,  B00000,  B00000,};


//byte da logo
byte logo1[] = { B11111, B01111, B00111, B00111, B00011, B00001, B00001, B00000 };
byte logo2[] = { B11111, B11111, B11111, B11111, B11111, B11111, B11110, B11100 };
byte logo3[] = { B11111, B11111, B11111, B11111, B10111, B00111, B01111, B11111 };

byte logo4[] = { B00000, B10000, B11000, B11100, B11110, B11111, B11111, B11111 };
byte logo5[] = { B01000, B00001, B00001, B00011, B00111, B01111, B11111, B11111 };
byte logo6[] = { B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111 };

#define GREEN_LED 5 // Pino do LED verde
#define YELLOW_LED 6 // Pino do LED amarelo
#define RED_LED 7 // Pino do LED vermelho
#define BUZZER 9 // Buzzer

volatile unsigned long rainCount = 0;
unsigned long lastRainCount = 0;
unsigned long lastRainTime = 0;
float maxRainValue = 60.0; // Valor máximo de mm de chuva
float rainAlertThreshold = 50.0; // Limite de chuva para acionar o alerta
bool alertTriggered = false;
// Diâmetro das rodas em metros
float diameterOfWheels = 0.6; // Supondo um diâmetro médio de 0.6 metros

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS3231 rtc;

volatile int encoderCount = 0;
int lastDTState = LOW;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  dht.begin();
  pinMode(RAIN_SENSOR, INPUT);
  pinMode(POT_PIN, INPUT);
  pinMode(ENCODER_DT, INPUT);
  pinMode(ENCODER_CLK, INPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(ENCODER_DT), readEncoder, CHANGE);

  //Declarando a logo - parte de cima
  lcd.createChar(1, logo1);
  lcd.createChar(2, logo2);
  lcd.createChar(3, logo3);
  
  //Declarando a logo - parte de baixo
  lcd.createChar(4, logo4);
  lcd.createChar(5, logo5);
  lcd.createChar(6, logo6);

  //Exibe a logo no display
  lcd.display();
  lcd.setCursor(2, 0);  
  lcd.write(byte(1));
  lcd.setCursor(3, 0);  
  lcd.write(byte(2));
  lcd.setCursor(4, 0);  
  lcd.write(byte(3));
  lcd.setCursor(2, 1);  
  lcd.write(byte(4));
  lcd.setCursor(3, 1);  
  lcd.write(byte(5));
  lcd.setCursor(4, 1);  
  lcd.write(byte(6));
  lcd.print("G4 Tech");
  delay(3000);
  lcd.clear();

  //Declarando os emojis
  lcd.createChar(0, triste);
  lcd.createChar(1, sorriso);
  lcd.createChar(2, serio);

  if (!rtc.begin()) {
    lcd.setCursor(0, 0);
    lcd.print("Couldn't find RTC");
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
  DateTime now = rtc.now();

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float batteryCurrent = analogRead(POT_PIN) * (5.0 / 1023.0);  // Simulando corrente com potenciômetro
  int speed = encoderCount;
  float rainPotValue = analogRead(RAIN_SENSOR); // Ler o valor do potenciômetro
  float rainValue = map(rainPotValue, 0, 1023, 0, maxRainValue); // Ajustar a quantidade de chuva com base na leitura do potenciômetro
  float speed_kmh = (speed * 3.1416 * diameterOfWheels) / 60.0; // Convertendo RPM para km/h

  if (isnan(h) || isnan(t)) {
    lcd.setCursor(0, 0);
    lcd.print("Erro no sensor");
    Serial.println("Erro no sensor DHT22");
    return;
  }

  // Verificar se o limite de chuva foi atingido
  if (rainValue >= rainAlertThreshold && !alertTriggered) {
    // Acionar alerta
    triggerRainAlert(now);
    // Registrar evento na EEPROM
    saveEventToEEPROM(now);
    alertTriggered = true;
  }

  // Atualizar LEDs de acordo com o status da pista
  updateLEDs(rainValue);

  lcd.clear(); // limpa a tela do display
  lcd.setCursor(4, 0);
  lcd.print(now.year(), DEC);
  lcd.print('/');
  lcd.print(now.month(), DEC);
  lcd.print('/');
  lcd.print(now.day(), DEC);
  lcd.print(' ');
  lcd.setCursor(5,1);
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  lcd.print(now.minute(), DEC);
  lcd.print(':');
  lcd.print(now.second(), DEC);

  Serial.print("Data e Hora: ");
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  delay(1000);

  lcd.clear(); // limpa a tela do display
  lcd.setCursor(0, 0);
  lcd.print("=DADOS DA PISTA=");
  lcd.setCursor(0, 1);
  lcd.print("================");
  delay(1500);
  lcd.clear(); // limpa a tela do display
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Umid: ");
  lcd.print(h);
  lcd.print(" %");

  Serial.print("------DADOS DA PISTA------");
  Serial.print("\nTemp: ");
  Serial.print(t);
  Serial.println(" C");

  Serial.print("Umid: ");
  Serial.print(h);
  Serial.println(" %");

  delay(2000);

  lcd.clear(); // Limpa a tela do display
  lcd.setCursor(0, 0);
  lcd.print("Chuva: ");
  lcd.print(rainValue);
  lcd.print(" mm");

  lcd.setCursor(0, 1);
  if (rainValue == 0){
    lcd.print("Sem chuva  ");
    lcd.setCursor(13, 1);  
    lcd.write(byte(1));
  }
  if (rainValue > 0 && rainValue < 2.5){
    lcd.print("Chuva fraca  ");
    lcd.setCursor(13, 1);
    lcd.write(byte(1));
    delay(500);
    lcd.setCursor(13, 1);
    lcd.write(byte(2));
    delay(500);
    lcd.setCursor(13, 1);
    lcd.write(byte(1));
  }
  if (rainValue >= 2.5 && rainValue <= 10.0){
    lcd.print("Chuva moderada ");
    lcd.write(byte(2));
  }
  if (rainValue >= 11.0 && rainValue < 50.0){
    lcd.print("Chuva forte  ");
    lcd.setCursor(13, 1);
    lcd.write(byte(2));
    delay(500);
    lcd.setCursor(13, 1);
    lcd.write(byte(0));
    delay(500);
    lcd.setCursor(13, 1);
    lcd.write(byte(2));
  }
  if (rainValue >= 50.0){
    lcd.print("Chuva violenta ");
    lcd.write(byte(0));
  }

  Serial.print("Chuva: ");
  Serial.print(rainValue);
  Serial.println(" mm");
  if (rainValue == 0){
    Serial.print("Sem chuva");
  }
  if (rainValue > 0 && rainValue < 2.5){
    Serial.print("Chuva fraca");
  }
  if (rainValue >= 2.5 && rainValue <= 10.0){
    Serial.print("Chuva moderada");
  }
  if (rainValue >= 11.0 && rainValue < 50.0){
    Serial.print("Chuva forte");
  }
  if (rainValue >= 50.0){
    Serial.print("Chuva violenta");
  }

  delay(2000);

  lcd.clear(); // limpa a tela do display
  lcd.setCursor(0, 0);
  lcd.print("=DADOS DA CARRO=");
  lcd.setCursor(0, 1);
  lcd.print("================");
  delay(1500);
  lcd.clear(); // limpa a tela do display
  lcd.print("Corrente: ");
  lcd.print(batteryCurrent, 2);
  lcd.print(" A");

  lcd.setCursor(0, 1);
  lcd.print("Vel: ");
  lcd.print(speed_kmh);
  lcd.print(" km/h");

  Serial.print("\n------DADOS DA CARRO------");
  Serial.print("\nCorrente: ");
  Serial.print(batteryCurrent, 2);
  Serial.println(" A");

  Serial.print("Velocidade: ");
  Serial.print(speed_kmh);
  Serial.println(" km/h");

  delay(2000);
}

void readEncoder() {
  int dtState = digitalRead(ENCODER_DT);
  if (dtState != lastDTState) {
    if (digitalRead(ENCODER_CLK) != dtState) {
      encoderCount++;
    } else {
      encoderCount--;
    }
    lastDTState = dtState;
  }
}

void triggerRainAlert(DateTime time) {
  // Acionar alerta de chuva forte
  digitalWrite(RED_LED, HIGH);
  Serial.println("Alerta: Chuva forte - Prova interrompida");
}

void saveEventToEEPROM(DateTime time) {
  // Salvar evento na EEPROM
  int address = 0; // Endereço de memória na EEPROM
  EEPROM.put(address, time.unixtime());
}

void updateLEDs(float rainValue) {
  if (rainValue < 2.5) {
    // Chuva fraca - Pista liberada
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);
    noTone(BUZZER);
  } else if (rainValue >= 2.5 && rainValue < rainAlertThreshold) {
    // Chuva moderada - Atenção
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    noTone(BUZZER);
  } else {
    // Chuva forte - Pista interrompida
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    tone(BUZZER,392);     //Liga/toca o buzzer
    delay(2500);
    noTone(BUZZER);
  }
}