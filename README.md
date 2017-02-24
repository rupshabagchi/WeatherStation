# WeatherStation

The project uses Arduino Uno along with Adafruit C33000 Wifi Shield to sense the environment data and send it to SparkFun database. Analysed the data and made graphs. The data is posted to SparkFun's data stream server system. The data stream on the SparkFun database is then analysed and prepared as graphs using the Analog.io APIs.

This sketch uses an Arduino Uno to POST sensor readings to SparkFun's data logging streams (http://data.sparkfun.com). A post will be initiated whenever pin 3 is connected to ground.

# Motivation
This project is intended to measure the real-time environment variable from multiple sensors in the greenhouse, from where the results can be easily checked and understood.

# Hardware Hookup:
  * These components are connected to the Arduino's I/O pins:
    * D3 - Active-low momentary button (pulled high internally)
    * A0 - Photoresistor (which is combined with a 10k resistor
           to form a voltage divider output to the Arduino).
    * D5 - SPST switch to select either 5V or 0V to this pin.
  * A CC3000 Shield sitting comfortable on top of your Arduino.
  
# Development environment specifics:
    * IDE: Arduino 1.0.5
    * Hardware Platform: RedBoard & CC3000 Shield (v10)
    * Sensors: Grove-Digital Light Sensor, DHT22 Temperature-Humidity Sensor, 

    
# Copyright    
Distributed as-is; no warranty is given.
