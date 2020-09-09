/*
 * Created by Ian Reichard, 2020/9/9
 * This software comes with NO warranty, use at your own risk.
 */

/*
 * EcoSphere data collector, used to automate logging of relative light data for my ecosphere project in a university bio class.
 * 
 * Hardware:
 * Arduino UNO
 * Micro SD Card
 * Photodiode assembly: Photodiode, 10k Resistor (Creates a voltage divider)
 * SD Card Reader: SPI 
 * RTC (Real Time Clock): I2C
 * Power Source (5V barrel jack for a wall outlet)
 * 
 * Arduino Port | Device
 * 10             SD Card reader Chip Select (CS)
 * 11             SD Card Reader MOSI
 * 12             SD Card Reader MISO
 * 13             SD Card Reader SCK
 * A0             Photodiode Voltage Divider
 * A4             RTC SDA (I2C)
 * A5             RTC SCL (I2C)
 */

//Libraries needed
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"

//Object Declaration for RTC and files
RTC_DS3231 rtc;
File myFile;

const int delayTime = 900000; //Log every 15 minutes (1000*60*15)
const int CSPin = 10; //ChipSelect for SD Card
const int sensorPin = A0; //Sensor pin for light sensor
int sensorValue = 0; //Value for string output
int counter = 0; //Index

//Setup
void setup() {
  rtc.begin(); //Assumes person has set time on RTC prior to this code running
  pinMode(sensorPin, INPUT); 
  SD.begin(CSPin); //TODO implement verification for fs access?
  myFile = SD.open("data.txt",  FILE_WRITE);
  if (myFile) {
    myFile.println("Initial Arduino bootup"); //Print bootup message to file
  }
  myFile.close();
}

void loop() {
  //Get sensor data from photodiode
  sensorValue = analogRead(sensorPin);
  //String conversion - was having problems writing to SD card so converted everything to Strings with DECs
  String inputString = String(sensorValue, DEC);
  String counterString = String(counter, DEC);
  //Get real time clock data
  DateTime now = rtc.now();
  //Combine strings
  String yearOut = String(now.year(), DEC);
  String monthOut = String(now.month(), DEC);
  String dayOut = String(now.day(), DEC);
  String hourOut = String(now.hour(), DEC);
  String minuteOut = String(now.minute(), DEC);
  String secondOut = String(now.second(), DEC);
  //Combine all Strings
  String toWrite = counterString + " " + yearOut + '/' + monthOut + '/' + dayOut + " " + hourOut + ":" + minuteOut + ":" + secondOut + " " + inputString;
  //Log to SD card
  myFile = SD.open("data.txt", FILE_WRITE);
  myFile.println(toWrite);
  myFile.close();
  //Delay 15 minutes TODO make this a bit cleaner with a power saving library
  delay(delayTime);
  //Increment the counter
  counter++;
}
