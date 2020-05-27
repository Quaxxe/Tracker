#include "Adafruit_FONA.h"
#include <SoftwareSerial.h>

#define SIMCOM_3G

#define FONA_RX 2
#define FONA_TX 3
#define FONA_RST 4
#define FONA_PWRKEY 5

SoftwareSerial fonaSS(FONA_TX, FONA_RX);

Adafruit_FONA_3G fona = Adafruit_FONA_3G(FONA_RST);

char imei[16] = {0}; // MUST use a 16 character buffer for IMEI!


void setup() {
    pinMode(FONA_RST, OUTPUT);
    digitalWrite(FONA_RST, HIGH); // Default state

    pinMode(FONA_PWRKEY, OUTPUT);
    digitalWrite(FONA_PWRKEY, LOW);
    delay(2000);
    digitalWrite(FONA_PWRKEY, HIGH);

    Serial.begin(115200);
    fonaSS.begin(4800);

    if (! fona.begin(fonaSS)) {
        Serial.println(F("Couldn't find FONA"));
        while(1);
    }
    Serial.println(F("FONA is OK"));

    Serial.println((String)"Type is " + fona.type());

    uint8_t imeiLen = fona.getIMEI(imei);
    if (imeiLen > 0) {
        Serial.print("Module IMEI: "); Serial.println(imei);
    }

    Serial.println(F("Enabling GPS..."));
    fona.enableGPS(true);

    // Set modem to full functionality
    fona.setFunctionality(1); // AT+CFUN=1

    fona.setNetworkSettings(F("web")); // For Telering

    // wait a little bit for connection
    delay(1000);

    fona.enableGPRS(true);
}

void loop() {
    delay(5000);

    // fona.getNetworkInfo();

    float latitude, longitude, speed_kph, heading, altitude;

    // if you ask for an altitude reading, getGPS will return false if there isn't a 3D fix
    boolean gps_success = fona.getGPS(&latitude, &longitude, &speed_kph, &heading, &altitude);

    if (gps_success) {

        // Serial.print("GPS lat:");
        // Serial.println(latitude, 6);
        // Serial.print("GPS long:");
        // Serial.println(longitude, 6);
        // Serial.print("GPS speed KPH:");
        // Serial.println(speed_kph);
        // Serial.print("GPS heading:");
        // Serial.println(heading);
        // Serial.print("GPS altitude:");
        // Serial.println(altitude);

    } else {
        Serial.println("Waiting for FONA GPS 3D fix...");
        return;
    }


    // Create char buffers for the floating point numbers for sprintf
    // Make sure these buffers are long enough for your request URL
    char URL[150];
    char latBuff[16];
    char lonBuff[16];

    // Format the floating point numbers as needed
    dtostrf(latitude, 1, 3, latBuff); // float_val, min_width, digits_after_decimal, char_buffer
    dtostrf(longitude, 1, 3, lonBuff);

    // Construct the appropriate URL's and body, depending on request type
    // Use IMEI as device ID for this example

    // GET request
    sprintf(URL, "GET /dweet/for/%s?lat=%s&lon=%s HTTP/1.1\r\nHost: dweet.io\r\n\r\n", imei, latBuff, lonBuff);

    if (!fona.postData("www.dweet.io", 443, "HTTPS", URL)) // Server, port, connection type, URL
        Serial.println(F("Failed to complete HTTP/HTTPS request..."));

}
