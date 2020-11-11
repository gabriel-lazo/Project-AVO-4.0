#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>

/*
 * Codigo para Arduino Pro Micro el cual permite leer los sensores: 
 * DS18B20, Read Temp: (Pin 8, Comunicacion 1-Wire), 
 * DHT11, Read Temp y Humedad: (Ping 9, Comunicacion 1-Wire), 
 * BMP180 Read Temp, Altitud, Presion: (Pin 2=SDA, 3=SLC, Comunicacion I2C).
 * El formato de la trama enviada por puerto Serial (USB): NData Data1 Data2 Data3 Data4 ... DataN
 * La trama es Procesada con Node Red para su visualizacion.
 */

#define PIN_SENSOR_DS 8
#define PIN_SENSOR_DHT 9

DHT SensorDHT(PIN_SENSOR_DHT,DHT11);
OneWire oneWire(PIN_SENSOR_DS);
DallasTemperature SensorDS(&oneWire);
Adafruit_BMP085 SensorBMP;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  SensorDHT.begin();
  SensorDS.begin();
  SensorBMP.begin(); 
}

void loop() {
//Read Temperature and Humidity from DHT11 Sensor
  String Temp_DHT = (String)SensorDHT.readTemperature();
  String Humid_DHT = (String)SensorDHT.readHumidity();
 
//Read Temperature from DS18B20 Sensor
  SensorDS.requestTemperatures();
  String Temp_DS = (String)SensorDS.getTempCByIndex(0);
  
//Read Temperature from BMP180 Sensor
  String Temp_BMP = (String)SensorBMP.readTemperature();   //Celcius
  String Pressure_BMP = (String)SensorBMP.readPressure();  //Pascal
  String Altitude_BMP = (String)SensorBMP.readAltitude();  //mts
  int NData = 6;
  String str_Payload;

//Crea un string con el formato: NData Data1 Data2 Data3 ... DataN
  str_Payload += NData;
  str_Payload += " " + Temp_DHT;
  str_Payload += " " + Humid_DHT;
  str_Payload += " " + Temp_DS;
  str_Payload += " " + Temp_BMP;
  str_Payload += " " + Pressure_BMP;
  str_Payload += " " + Altitude_BMP;
  str_Payload += " ";
  
  byte Payload[str_Payload.length()];
  int LengthFrameAPI = 18 + sizeof(Payload);
  int LengthPayload = sizeof(Payload);
  
  Serial.print(str_Payload);
    
  delay(2000);
}
