#include <Arduino.h>
#include <U8g2lib.h>
#include <WiFi.h>

U8G2_SSD1315_128X64_NONAME_1_SW_I2C screen(U8G2_R0, /* SCK serial clock */ 22, /* SDA serial data */ 23, /* reset= */ U8X8_PIN_NONE); // Constructor for the OLED screen driver

const char* osname = "SLKLNK";
const char* bconspmname = "Free Wifi ";
int top_button = 15;  // TOP button
int mid_button = 0;     // MIDDLE button
int bottom_button = 16; // BOTTOM button
bool inBeaconSpam = false;
const char* targetssid = "SOME SSID"; 
const char* targetpass = "SOME PASS";



void bootscreen(){
  screen.firstPage(); // Output first framebuffer
  do {  
    screen.setFont(u8g2_font_ncenB14_tr); // Set the font
    screen.drawStr(10, 15, "----------"); // Draw characters at desired x,y position
    screen.drawStr(10, 33, osname);
    screen.drawStr(10, 52, "Version 1.0");
    screen.drawStr(10, 64, "----------");
    
  } while ( screen.nextPage() ); // Until all framebuffers are filled — 64/4 = 16 parts
}   

void wifi_deuath_all(){
    WiFi.begin(targetssid, targetpass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Awaiting wifi connection . .  ");
    }
    Serial.println("[Debug] -> Wifi connected");
    // TODO: scan for other devices, then send deauth packets
}

void ble_deauth(){} // Not implemented yet

void beacon_spam(){
  Serial.println("[Debug] -> Starting beacon spam ");
  screen.firstPage(); 
  do {  
    screen.setFont(u8g2_font_6x10_tf); 
    screen.drawStr(5, 15, "SLKLNK V.1");  
    screen.drawStr(5, 33, "Spamming WIFI beacon:");
    screen.drawStr(40, 52, bconspmname);
    screen.drawStr(10, 64, "----------------------------------");
    WiFi.softAP(bconspmname); // TODO: Randomize MAC address for more unique APs
  } while ( screen.nextPage() ); 
}

void startupscreen(){
  screen.firstPage(); 
  do {  
    screen.setFont(u8g2_font_ncenB14_tr);
    screen.drawStr(10, 15, osname); 
    screen.setFont(u8g2_font_3x5im_mr);
    screen.drawStr(10, 28, "----------------------");
    screen.drawStr(10, 33, "  1. SSID SPAM");
    screen.drawStr(10, 40, "  2. WIFI Deauth");
    screen.drawStr(10, 45, "  3. BLE Deauth");
    screen.drawStr(10, 50, "----------------------");
  } while ( screen.nextPage() );
}


void setup() {
  screen.begin(); // Initialize the U8G2 screen object
  Serial.begin(9600);
  bootscreen();
  delay(3000); // Not needed at all but imo a startup screen makes it cooler :) 
  pinMode(top_button, INPUT_PULLUP);  // TOP button
  pinMode(mid_button, INPUT_PULLUP);    // MIDDLE button
  pinMode(bottom_button, INPUT_PULLUP); // BOTTOM button
  startupscreen();
}

void loop() {
  if(digitalRead(top_button) == 0 && !inBeaconSpam){
    inBeaconSpam = true;
  }
  if(inBeaconSpam){
    beacon_spam();
  }
}
