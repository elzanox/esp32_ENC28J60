/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com
*********/


#include <SPI.h>
#include <EthernetENC.h>
#include "PubSubClient.h"
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xAD };

char server[] = "www.google.com";    // name address for Google (using DNS)
#define CLIENT_ID       "JEZIEL_78233"
#define INTERVAL        3000 // 3 sec delay between publishing

IPAddress ip(192, 168, 2, 77);
IPAddress myDns(192, 168, 2, 1);

EthernetClient client;
PubSubClient mqttClient;
long previousMillis;

unsigned long beginMicros, endMicros;
unsigned long byteCount = 0;
bool printWebData = true;  // set to false for better speed measurement

TaskHandle_t Task1;
TaskHandle_t Task2;

// LED pins
const int led1 = 2;
const int led2 = 4;
int counter;
void setup() {
  Serial.begin(115200);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
    Task1code,   /* Task function. */
    "Task1",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    NULL,      /* Task handle to keep track of created task */
    0);          /* pin task to core 0 */
  delay(500);

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
    Task2code,   /* Task function. */
    "Task2",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    NULL,      /* Task handle to keep track of created task */
    1);          /* pin task to core 1 */
  delay(500);


  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip, myDns);
  } else {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }
  mqttClient.setClient(client);
  mqttClient.setServer("180.210.205.235", 1883);
  Serial.println(F("MQTT client configured"));
  previousMillis = millis();
  // give the Ethernet shield a second to initialize:


  // if you get a connection, report back via serial:

  beginMicros = micros();
}

//Task1code: blinks an LED every 1000 ms
void Task1code( void * pvParameters ) {
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  for (;;) {
    Serial.println("core 0 running");

  }
}

//Task2code: blinks an LED every 700 ms
void Task2code( void * pvParameters ) {
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  for (;;) {
    Serial.println("core 1 running");
//    auto link = Ethernet.linkStatus();
//    Serial.print("Link status: ");
//    switch (link) {
//      case Unknown:
//        Serial.println("Unknown");
//        break;
//      case LinkON:
//        Serial.println("ON");
//        break;
//      case LinkOFF:
//        Serial.println("OFF");
//        break;
//    }
//    delay(1000);
    
    if (millis() - previousMillis > INTERVAL) {
      sendData();
      previousMillis = millis();
    }
    mqttClient.loop();
  }
}
void sendData() {
  if (mqttClient.connect(CLIENT_ID)) {
    mqttClient.publish("/MYTEST", "hola_esp32");

  }
}
void loop() {

}
