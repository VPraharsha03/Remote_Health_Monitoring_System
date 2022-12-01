# Remote Health Monitoring System with IoT

This project utilizes an IoT platform to monitor the body temperature using LM35, heart rate and oxygen level using MAX30100 sensor. Vital signals are captured from the sensors and are processed using Arduino. 
ESP8266 WiFi MCU (which runs the NodeMCU firmware) enables connectivity to the Blynk cloud over WiFi.
A SIM900A GSM module is also installed to send an SMS alert. A panic button is installed in case of emergence, and for sending the reports of the patient to the caretaker. Finally, the obtained results of the project are displayed on the OLED, Blynk App and a message alert is sent.

#### Block Diagram:
![Block Diagram](https://github.com/VPraharsha03/Remote_Health_Monitoring_System/blob/main/Block.png?raw=true)
