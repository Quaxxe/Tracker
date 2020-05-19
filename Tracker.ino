#include <Adafruit_FONA.h>
#include <SoftwareSerial.h>

#define FONA_RX 2
#define FONA_TX 3
#define FONA_RST 4

SoftwareSerial fonaSS(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;

Adafruit_FONA fona = Adafruit_FONA(FONA_RST);



void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    fonaSerial->begin(4800);

    if (! fona.begin(*fonaSerial)) {
        Serial.println(F("Couldn't find FONA"));
        while(1);
    }
    Serial.println(F("FONA is OK"));

    Serial.println((String)"Type is " + fona.type());

    Serial.println(F("Enabling GPS..."));
    fona.enableGPS(true);
}

void loop() {
    delay(2000);

    float latitude, longitude, speed_kph, heading, altitude;

    // if you ask for an altitude reading, getGPS will return false if there isn't a 3D fix
    boolean gps_success = fona.getGPS(&latitude, &longitude, &speed_kph, &heading, &altitude);

    if (gps_success) {

        Serial.print("GPS lat:");
        Serial.println(latitude, 6);
        Serial.print("GPS long:");
        Serial.println(longitude, 6);
        Serial.print("GPS speed KPH:");
        Serial.println(speed_kph);
        Serial.print("GPS heading:");
        Serial.println(heading);
        Serial.print("GPS altitude:");
        Serial.println(altitude);

    } else {
        Serial.println("Waiting for FONA GPS 3D fix...");
    }

}
