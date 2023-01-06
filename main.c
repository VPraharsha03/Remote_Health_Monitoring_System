#include <Wire.h>
#include <SoftwareSerial.h>
#include "MAX30100_PulseOximeter.h"
#define REPORTING_PERIOD_MS     1000
const int lm35_pin = A0;
PulseOximeter pox;
uint32_t tsLastReport = 0;
int led = 13;
int pb = 12;

void onBeatDetected()
{    
  Serial.println("Beat!");
}

SoftwareSerial mySerial(9, 10); //Tx, Rx

void setup()
{
  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  delay(100);
  pinMode(led,OUTPUT);
  pinMode(pb,INPUT);
     
  Serial.print("Initializing pulse oximeter..");
 
  // Initialize the PulseOximeter instance
  // Failures are generally due to an improper I2C wiring, missing power supply
  // or wrong target chip
  if (!pox.begin()) 
  {
    Serial.println("FAILED");
    for(;;);
  } 
  else 
  {
    Serial.println("SUCCESS");
  }
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
 
  // Register a callback for the beat detection
  pox.setOnBeatDetectedCallback(onBeatDetected);

}

void loop()
{
  if (Serial.available()>0)
  {
    switch(Serial.read())
    {
      case 's':
        SendMessage();
        break;
      case 'r':
        RecieveMessage();
        break;
    }
  }

 if (mySerial.available() > 0)
 {
   Serial.write(mySerial.read());

   // Make sure to call update as fast as possible
   pox.update();
   if (millis() - tsLastReport > REPORTING_PERIOD_MS) 
   { 
     //Serial.print("Heart rate:");
     Serial.print(pox.getHeartRate());
     Serial.print("bpm / SpO2:");
     Serial.print(pox.getSpO2());
     Serial.println("%");
     int temp_adc_val;
     float temp_val;
     float tempf;
     temp_adc_val = analogRead(lm35_pin);  /* Read Temperature */
     temp_val = (temp_adc_val * 4.88); /* Convert adc value to equivalent voltage */
     temp_val = (temp_val/10); /* LM35 gives output of 10mv/°C */
     Serial.print("Temperature = ");
     Serial.print(temp_val);
     Serial.print(" Degree Celsius\n"); 
     tempf = ( temp_val * 1.8 ) + 32; // Converting to Fahrenheit 
     Serial.print("Fahrenheit ");
     Serial.println(tempf);
     tsLastReport = millis();
   }

  if(temp_val > 37.5)
  {
    mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
    delay(1000);  // Delay of 1000 milli seconds or 1 second
    mySerial.println("AT+CMGS=\"+919182451223\"\r"); // Replace x with mobile number
    delay(1000);
    mySerial.println("High Fever Detected");// The SMS text you want to send
    delay(100);
    mySerial.println((char)26);// ASCII code of CTRL+Z
    delay(1000);
  }
        
  if(digitalRead(pb)==HIGH)
  {
    pox.update();
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) 
    {
      Serial.print("Heart rate:");
      Serial.print(pox.getHeartRate());
      
      Serial.print("bpm / SpO2:");
      Serial.print(pox.getSpO2());
      Serial.println("%");
      tsLastReport = millis();
    }
        
    
    Serial.println("Emergency Alert.Support needed");
    digitalWrite(led,HIGH);  
    int temp_adc_val;
    float temp_val;
    float  tempf;
    temp_adc_val = analogRead(lm35_pin);  /* Read Temperature */
    temp_val = (temp_adc_val * 4.88); /* Convert adc value to equivalent voltage */
    temp_val = (temp_val/10); /* LM35 gives output of 10mv/°C */
    Serial.print("Temperature = ");
    Serial.print(temp_val);
    tempf=( temp_val *1.8)+32; // Converting to Fahrenheit 
    Serial.print("Fahrenheit ");
    Serial.println(tempf);
    Serial.print(" Degree Celsius\n"); 


  
    mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
    delay(1000);  // Delay of 1000 milli seconds or 1 second
    mySerial.println("AT+CMGS=\"+919182451223\"\r"); // Replace x with mobile number
    delay(1000);
    mySerial.println("Emergency Health Alert");// The SMS text you want to send
    mySerial.println("Temperature = ");// The SMS text you want to send
    mySerial.println(temp_val);// The SMS text you want to send

    mySerial.println("Fahrenheit = ");// The SMS text you want to send
    mySerial.println(tempf);// The SMS text you want to send

    mySerial.print(" SPO2 = ");// The SMS text you want to send
    mySerial.println(pox.getSpO2());// The SMS text you want to send

    mySerial.print("BPM = ");// The SMS text you want to send
    mySerial.println(pox.getHeartRate());// The SMS text you want to send

    //pox.getHeartRate()
    delay(100);
    mySerial.println((char)26);// ASCII code of CTRL+Z
    delay(1000);  
  }
  else
  {
    digitalWrite(led,LOW);
  }
 }
 delay(2000);
}
