#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 10, 9, 8, 7);


void TaskReadTemperature(void *pvParameters);
void TaskRelayLedBlink(void *pvParameters);
void TaskHotBlink(void *pvParameters);

volatile float temperature = 0.0;

const int relayPin = 4;
const int tempControlLED = 2;

void setup() {
  pinMode(relayPin, OUTPUT);
  pinMode(tempControlLED, OUTPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);

  //Setup tarefa 1 "Template"
  xTaskCreate(
    TaskReadTemperature,
    "ReadTemperature",
    128,
    NULL,
    3,
    NULL
  );
  
  //Setup tarefa 2 "RelayLEDBlink"
  xTaskCreate(
    TaskRelayLedBlink,
    "RelayLedBlink",
    128,
    NULL,
    3,
    NULL
  );
  
  //Setup tarefa 3 "HotBlink"
  xTaskCreate(
    TaskHotBlink,
    "HotBlink",
    128,
    NULL,
    3,
    NULL
  );
}

void loop() {
}

// Leitura Temperatura e visualização do display LCD
void TaskReadTemperature(void *pvParameters) {
  (void) pvParameters;

  float sensorValue = 0.0;
  for (;;) {
    sensorValue = -10.0 + (rand() % 51); 
    temperature = sensorValue;
    
    vTaskDelay(2000 / portTICK_PERIOD_MS); 
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print(" C");
  }
}

//Usa o Relay para piscar o LED a cada 500ms
void TaskRelayLedBlink(void *pvParameters) {
  (void) pvParameters;
  
  for (;;) {
    digitalWrite(relayPin, HIGH); 
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(relayPin, LOW); 
    vTaskDelay(500 / portTICK_PERIOD_MS); 
  }
}

//Tarefa para acionar o LED quando o sensor de temperatura detectar 26°C, delay de 1 segundo após acionamento 
void TaskHotBlink(void *pvParameters) {
  (void) pvParameters;
  
  for (;;) {
    if(temperature >= 26.0) {
      digitalWrite(tempControlLED, HIGH);
    } else {
      digitalWrite(tempControlLED, LOW);
    }
    
    vTaskDelay(1000 / portTICK_PERIOD_MS); 
  }
}
