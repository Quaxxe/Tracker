#include "Adafruit_FONA.h"
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>

#define SIMCOM_3G

#define FONA_RX 2
#define FONA_TX 3
#define FONA_RST 4
#define FONA_PWRKEY 5
const int chipSelect = 10;



//variable that defines how long to wait
const unsigned int waitSdwrite = 5000;    //5 secoonds until saving data to SD card
const unsigned long waitServersend = 30 * 1000; //15 minutes until sending data to server


SoftwareSerial fonaSS(FONA_TX, FONA_RX);

Adafruit_FONA_3G fona = Adafruit_FONA_3G(FONA_RST);

char imei[16] = {0}; // MUST use a 16 character buffer for IMEI!

File dataFile;


void setup() {
    pinMode(FONA_RST, OUTPUT);
    digitalWrite(FONA_RST, HIGH); // Default state

    pinMode(FONA_PWRKEY, OUTPUT);
    digitalWrite(FONA_PWRKEY, LOW);
    delay(2000);
    digitalWrite(FONA_PWRKEY, HIGH);

    Serial.begin(115200);
    Serial.println(F("Starting"));
    fonaSS.begin(4800);

    if (! fona.begin(fonaSS)) {
        Serial.println(F("Couldn't find FONA"));
        while (1);
    }
    Serial.println(F("FONA is OK"));

    uint8_t imeiLen = fona.getIMEI(imei);
    if (imeiLen > 0) {
        Serial.print(F("Module IMEI: ")); Serial.println(imei);
    }

    Serial.println(F("Enabling GPS..."));
    fona.enableGPS(true);

    // Set modem to full functionality
    fona.setFunctionality(1); // AT+CFUN=1

    //fona.setNetworkSettings(F("web")); // For Telering
    fona.setNetworkSettings(F("internet.telekom")); // For Telekom

    // wait a little bit for connection
    delay(1000);

    fona.enableGPRS(true);

    Serial.print("Initializing SD card...");

    // see if the card is present and can be initialized:
    if (!SD.begin(chipSelect)) {
        Serial.println("Card failed, or not present");
        // don't do anything more:
        while (1);
    }
    Serial.println("card initialized.");

    dataFile = SD.open("datalog.txt", FILE_WRITE);

}

void loop() {
    char buf[100];
    char latBuff[16];
    char lonBuff[16];
    //time variables
    static unsigned long lastSdwritten;     //variable to save time difference to save data on SD card
    static unsigned long lastServersent;     //variable to save time difference to send data to server
    delay(1000);

    // fona.getNetworkInfo();

    float latitude, longitude, speed_kph, heading, altitude, second;
    uint8_t hour, minute, month, day;
    uint16_t year;

    // if you ask for an altitude reading, getGPS will return false if there isn't a 3D fix
    boolean gps_success = fona.getGPS(&latitude, &longitude, &speed_kph, &heading, &altitude, &year, &month, &day, &hour, &minute, &second);
    
    
    if (! gps_success) {
        Serial.println("Waiting for FONA GPS 3D fix...");
        return;
    }

        
    // Format the floating point numbers as needed
    dtostrf(latitude, 1, 6, latBuff); // float_val, min_width, digits_after_decimal, char_buffer
    dtostrf(longitude, 1, 6, lonBuff);

    // Construct the appropriate URL's and body, depending on request type
    // Use IMEI as device ID for this example

    //get data every 5 seconds to save it
    if (millis() > waitSdwrite + lastSdwritten) {
        Serial.print(F("Writing to SD"));
        lastSdwritten = millis();

        sprintf(buf, "%02d:%02d:%02d;%s;%s\n",
                hour, minute, second, latBuff, lonBuff);

        dataFile.write(buf);
        dataFile.flush(); //only for testing every 10s
        Serial.println(F("...done"));
    }


    //send data every 15 minuets to server
    if (millis() > waitServersend + lastServersent) {
        //Serial.print(F("Sending to Server"));
        lastServersent = millis();

        // Create char buffers for the floating point numbers for sprintf
        // Make sure these buffers are long enough for your request URL


        // GET request
        sprintf(buf, "GET /dweet/for/%s?lat=%s&lon=%s HTTP/1.1\r\nHost: dweet.io\r\n\r\n", imei, latBuff, lonBuff);
        Serial.print(buf);
        /*if (!fona.postData("www.dweet.io", 443, "HTTPS", buf)) // Server, port, connection type, URL
             Serial.println(F("Failed to complete HTTP/HTTPS request..."));*/
        Serial.println(F("...done"));

    }

}
