/**
   Agentuino SNMP Agent Library Prototyping...

   Copyright 2010 Eric C. Gionet <lavco_eg@hotmail.com>

   @Edit: M. Pivovarsky <miroslav.pivovarsky@gmail.com>

*/

#include "Streaming.h"         // Include the Streaming library
#include <Ethernet.h>          // Include the Ethernet library
#include <SPI.h>
#include "Agentuino.h"
#include "MIB.h"
#include "Variable.h"
#include <DHT.h>
#include <DHT_U.h>
IPAddress address;

static byte mac[] = {0xDE, 0xAD, 0xBE, 0xFF, 0xFE, 0xEF};
static byte ip[] = {172, 16, 0, 80};
static byte gateway[] = {172, 16, 0, 154};
static byte subnet[] = {255, 255, 255, 0};
static byte dns[] = {10, 0, 0, 10};
static byte RemoteIP[4] = {10, 0, 0, 22}; // The IP address of the host that will receive the trap

// Set up DHT sensor
#define DHTTYPE DHT11
#define DHTPIN 8
DHT dht(DHTPIN, DHTTYPE);
void setup() {
  dht.begin();
  Serial.begin(9600);
  Serial.println("CPU Start");

  pinMode(6, INPUT);

  //Ethernet.begin(mac);
  Ethernet.begin(mac, ip, dns, gateway, subnet);

  IPAddress address = Ethernet.localIP();
  for (uint8_t i = 0; i <= 4; i++) {
    my_IP_address[i] = address[i];
    Serial.print(my_IP_address[i]);
    Serial.print(".");
  }
  Serial.println("");

  api_status = Agentuino.begin();

  if (api_status == SNMP_API_STAT_SUCCESS) {

    Agentuino.onPduReceive(pduReceived);

    delay(10);

    Serial << F("SNMP Agent Initalized...") << endl;

    return;
  }

  delay(10);

  Serial << F("SNMP Agent Initalization Problem...") << status << endl;
}

void loop() {
  // listen/handle for incoming SNMP requests
  Agentuino.listen();
  // Grab values from DHT and float them
  int hum = dht.readHumidity();
  int cel = dht.readTemperature();
  int temp_f = dht.readTemperature(true);

   
    Agentuino.Trap(temp_f, RemoteIP, locUpTime);
  Serial.println("Sent trap");
  delay(5000);
}
