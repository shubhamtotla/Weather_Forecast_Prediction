
#include "DHT.h"

#define DHTPIN 9     // what pin we're connected to
#include <SFE_BMP180.h>
#include <Wire.h>
#define ALTITUDE 213.0 // Altitude of VELLORE in meters
int led = 8;

SFE_BMP180 pressure;

#define DHTTYPE DHT22   // DHT 11 

// Initialize DHT sensor for normal 16mhz Arduino
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600); 
  Serial.println("Sensors Reading!");
  dht.begin();
  pinMode(led, OUTPUT);
    
  if (!pressure.begin())
  {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.

    Serial.println("BMP180 init fail\n\n");
    
  }
}

void loop() {
  // Wait a few seconds between measurements.
  
  char status;
  double T,P,p0,a;


  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  status = pressure.startTemperature();
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) ) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index
  // Must send in temp in Fahrenheit!
  

  Serial.print(","); 
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print(",");  
  Serial.print(t);
  Serial.print(" C ");
  Serial.print(",");
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Function returns 1 if successful, 0 if failure.

      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          // Print out the measurement:
          
          Serial.print(P,2);
          Serial.print(" mb, ");
          Serial.print(",");
          Serial.print(P*0.0295333727,2);
          Serial.print(" inHg");
          Serial.print(",");

          // The pressure sensor returns abolute pressure, which varies with altitude.
          // To remove the effects of altitude, use the sealevel function and your current altitude.
          // This number is commonly used in weather reports.
          // Parameters: P = absolute pressure in mb, ALTITUDE = current altitude in m.
          // Result: p0 = sea-level compensated pressure in mb
          p0 = pressure.sealevel(P,ALTITUDE); // we're at 1655 meters (Boulder, CO)
          Serial.print(p0,2);
          Serial.print(" mb, ");
          Serial.print(",");
          Serial.print(p0*0.0295333727,2);
          Serial.println(" inHg");
          Serial.println("\n");

          
         
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    
  }

  delay(6000);
  
}
