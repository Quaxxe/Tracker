# Tracker

A GPS-Tracker based on Arduino and Adafruit FONA module.

# Notes

## SIM5320

Do not end commands with `NL CR`, only `CR`!

### Useful AT commands

```
AT+CGPS?
AT+CGPSINFO
AT+CGPSAUTO=1
```

```
AT+CVHU=0
ATI
AT+GSN
AT+CFUN=1

AT+CPSI?
AT+COPS?

AT+CGATT=1
AT+CGSOCKCONT=1,"IP","web"
AT+CIPMODE=1
AT+NETOPEN=,,1

AT+CHTTPSSTART
AT+CHTTPSOPSE="www.dweet.io",443,2
AT+CHTTPSSEND=81
GET /dweet/for/39462395639?temp=761.37&batt=3700 HTTP/1.1
Host: dweet.io

AT+CHTTPSSEND
AT+CHTTPSRECV?
AT+CHTTPSRECV=0
AT+CHTTPSCLSE
AT+CHTTPSSTOP
```

### Links

* [TinyGSM](https://github.com/vshymanskyy/TinyGSM) library
* [Adafruit FONA downloads](https://learn.adafruit.com/adafruit-fona-3g-cellular-gps-breakout/downloads)
* [SIM5320 Downloads](https://simcom.ee/documents/?dir=SIM5320)
* [Working Arduino library](https://github.com/botletics/SIM7000-LTE-Shield)

