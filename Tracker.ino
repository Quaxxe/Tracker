#include <Adafruit_FONA.h>
#include <SoftwareSerial.h>


SoftwareSerial gpsSerial(3, 4); // RX, TX


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  gpsSerial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (gpsSerial.available()) {
    Serial.write(gpsSerial.read());
  }
  if (Serial.available()) {
    gpsSerial.write(Serial.read());
  }
 
}
