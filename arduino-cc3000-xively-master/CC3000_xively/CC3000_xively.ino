/*************************************************** 
  This is a sketch to use the CC3000 WiFi chip & Xively
  
  Written by Marco Schwartz for Open Home Automation
 ****************************************************/

// Libraries
#include <Adafruit_CC3000.h>
#include <SPI.h>
#include "DHT.h"
#include <avr/wdt.h>

// Define CC3000 chip pins
#define ADAFRUIT_CC3000_IRQ   3
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10

// DHT sensor
#define DHTPIN 7
#define DHTTYPE DHT11

// Create CC3000 instances
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                                         SPI_CLOCK_DIV2); // you can change this clock speed
Adafruit_CC3000_Client client;                                                                            
// DHT instance
DHT dht(DHTPIN, DHTTYPE);

// WLAN parameters
#define WLAN_SSID       "ZyXEL NBG-418N v2"
#define WLAN_PASS       "MSHYD36443"
#define WLAN_SECURITY   WLAN_SEC_WPA2

char server[] = "https://weatherstation.firebaseio.com/temperature";   

unsigned long lastConnectionTime = 0;       
boolean lastConnected = false;              
const unsigned long postingInterval = 60000;

float temperature;  
int reading;  
int lm35Pin = 5;
float referenceVoltage;

void setup(void)
{
  // Initialize
  Serial.begin(9600);
  
  Serial.println(F("Initializing WiFi chip..."));
  if (!cc3000.begin())
  {
    Serial.println(F("Couldn't begin()! Check your wiring?"));
    while(1);
  }
  
  // Connect to WiFi network
  Serial.print(F("Connecting to WiFi network ..."));
  cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY);
  Serial.println(F("done!"));
  
  // Wait for DHCP to complete
  Serial.println(F("Request DHCP"));
  while (!cc3000.checkDHCP())
  {
    delay(100);
  }
 
}

void loop(void)
{
 // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only:
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  // if there's no net connection, but there was one last time
  // through the loop, then stop the client:
  if (!client.connected() && lastConnected) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }

  // if you're not connected, and ten seconds have passed since
  // your last connection, then connect again and send data:
  if(!client.connected() && (millis() - lastConnectionTime > postingInterval)) {
    //reading = analogRead(lm35Pin);
    temperature = 23;
    sendData(temperature);
  }
  // store the state of the connection for next time through
  // the loop:
  lastConnected = client.connected();
  } 
 
 
 /**************************** 
  cc3000.printIPdotsRev(ip);
  Serial.println(F(""));
  
  // Get data & transform to integers
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  int temperature = (int) t;
  int humidity = (int) h;

  // Prepare JSON for Xively & get length
  int length = 0;

  // JSON data
  String data = "";
  data = data + "\n" + "{\"version\":\"1.0.0\",\"datastreams\" : [ "
  + "{\"id\" : \"Temperature\",\"current_value\" : \"" + String(temperature) + "\"},"
  + "{\"id\" : \"Humidity\",\"current_value\" : \"" + String(humidity) + "\"}]}";
  
  // Get length
  length = data.length();
 
  // Reset watchdog
  wdt_reset();
  
  // Check connection to WiFi
  Serial.print(F("Checking WiFi connection ..."));
  if(!cc3000.checkConnected()){while(1){}}
  Serial.println(F("done."));
  wdt_reset();
  
  
  // Ping Xively server
   Serial.print(F("Pinging firebase server ..."));
  if(!cc3000.ping(ip, 2)){while(1){}}
  Serial.println(F("done."));
  wdt_reset();
  
  
  // Send request
  Adafruit_CC3000_Client client = cc3000.connectTCP(ip, 80);
  if (client.connected()) {
    Serial.println(F("weatherstation.firebaseio.com."));
   
    // Send headers
    Serial.print(F("Sending headers "));
    client.fastrprint(F("PUT /v2/feeds/"));
    client.fastrprint(feedID);
    client.fastrprintln(F(".json HTTP/1.0"));
    Serial.print(F("."));
    client.fastrprintln(F("Host: weatherstation.firebaseio.com"));
    Serial.print(F("."));
    client.fastrprint(F("X-ApiKey: "));
    client.fastrprintln(API_key);
    Serial.print(F("."));
    client.fastrprint(F("Content-Length: "));
    client.println(length);
    Serial.print(F("."));
    client.fastrprint(F("Connection: close"));
    Serial.println(F(" done."));
    // Reset watchdog
    wdt_reset();
    
    // Send data
    Serial.print(F("Sending data ..."));
    client.fastrprintln(F(""));    
    sendData(client,data,buffer_size);  
    client.fastrprintln(F(""));
    Serial.println(F("done."));
    
    // Reset watchdog
    wdt_reset();
    
  } else {
    Serial.println(F("Connection failed"));    
    return;
  }
  
  // Reset watchdog
  wdt_reset();
  
  Serial.println(F("Reading answer ..."));
  while (client.connected()) {
    while (client.available()) {
      char c = client.read();
      Serial.print(c);
    }
  }
  
  // Reset watchdog
  wdt_reset();
   
  // Close connection and disconnect
  client.close();
  Serial.println(F("Closing connection"));
  
  // Reset watchdog & disable
  wdt_reset();
  wdt_disable();
  
  // Wait 10 seconds until next update
  wait(10000);
  
}

// Send data chunk by chunk
void sendData(Adafruit_CC3000_Client& client, String input, int chunkSize) {
  
  // Get String length
  int length = input.length();
  int max_iteration = (int)(length/chunkSize);
  
  for (int i = 0; i < length; i++) {
    client.print(input.substring(i*chunkSize, (i+1)*chunkSize));
    wdt_reset();
  }  
}

// Wait for a given time using the watchdog
void wait(int total_delay) {
  
  int number_steps = (int)(total_delay/5000);
  wdt_enable(WDTO_8S);
  for (int i = 0; i < number_steps; i++){
    delay(5000);
    wdt_reset();
  }
  wdt_disable();
}
*/

// this method makes a HTTP connection to the server:
void sendData(int thisData) {
  Serial.println("now in sendData function....");
  // if there's a successful connection:
  String JsonData = "{\"temperature\": {\"value\": ";
  JsonData = JsonData + thisData;
  JsonData = JsonData + "}}";
  if (client.connect(server, 80)) {
    Serial.println("connecting...");
    // send the HTTP PUT request:
    client.println("POST /api/temperatures HTTP/1.1");
    client.println("Host: https://weatherstation.firebaseio.com");
    client.println("User-Agent: Arduino/1.0");
    client.println("Accept: application/json");
    client.print("Content-Length: ");
    client.println(JsonData.length());

    // last pieces of the HTTP PUT request:
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.println();
    client.println(JsonData);
  } 
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }
   // note the time that the connection was made or attempted:
  lastConnectionTime = millis();
}
