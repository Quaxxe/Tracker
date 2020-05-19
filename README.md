# Tracker

A GPS-Tracker based on Arduino and Adafruit FONA module.

# Notes

## SIM5320

### Useful AT commands

```
AT+CGPS?
AT+CGPSINFO
AT+CGPSAUTO=1
```

```
AT+CGATT=1
AT+CGDCONT=1,"IP","web","0.0.0.0"
AT+CGACT=1,1
AT+CGPADDR=1
AT+CPSI?
AT+NETOPEN
AT+IPADDR
```

### Links

* [TinyGSM](https://github.com/vshymanskyy/TinyGSM) library
* [Adafruit FONA downloads](https://learn.adafruit.com/adafruit-fona-3g-cellular-gps-breakout/downloads)

