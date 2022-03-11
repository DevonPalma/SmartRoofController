#ifndef _WEMO_DATA_H_
#define _WEMO_DATA_H_

#include <SPI.h>
#include <Ethernet.h>
#include <mac.h>

const int WEMO_COUNT = 5;
const int WEMO_PORT = 49153;
const char *WEMO_IPS[5] = {"192.168.1.4", "192.168.1.7", "192.168.1.6", "192.168.1.8", "192.168.1.16"};


class WemoWrapper {
    EthernetClient client;
    bool wemoStates[WEMO_COUNT];

    void _sendTurnOnPacket(int wemoIndex) {
      String data1;
      data1 += "<?xml version=\"1.0\" encoding=\"utf-8\"?><s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><s:Body><u:SetBinaryState xmlns:u=\"urn:Belkin:service:basicevent:1\"><BinaryState>1</BinaryState></u:SetBinaryState></s:Body></s:Envelope>"; // Use HTML encoding for comma's
      if (client.connect(WEMO_IPS[wemoIndex], WEMO_PORT)) {
        client.println("POST /upnp/control/basicevent1 HTTP/1.1");
        client.println("Content-Type: text/xml; charset=utf-8");
        client.println("SOAPACTION: \"urn:Belkin:service:basicevent:1#SetBinaryState\"");
        client.println("Connection: keep-alive");
        client.print("Content-Length: ");
        client.println(data1.length());
        client.println();
        client.print(data1);
        client.println();
      }

      if (client.connected()) {
        client.stop();
      }
    }

    void _sendTurnOffPacket(int wemoIndex) {
      String data1;
      data1 += "<?xml version=\"1.0\" encoding=\"utf-8\"?><s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><s:Body><u:SetBinaryState xmlns:u=\"urn:Belkin:service:basicevent:1\"><BinaryState>0</BinaryState></u:SetBinaryState></s:Body></s:Envelope>"; // Use HTML encoding for comma's
      if (client.connect(WEMO_IPS[wemoIndex], WEMO_PORT)) {
        client.println("POST /upnp/control/basicevent1 HTTP/1.1");
        client.println("Content-Type: text/xml; charset=utf-8");
        client.println("SOAPACTION: \"urn:Belkin:service:basicevent:1#SetBinaryState\"");
        client.println("Connection: keep-alive");
        client.print("Content-Length: ");
        client.println(data1.length());
        client.println();
        client.print(data1);
        client.println();
      }

      if (client.connected()) {
        client.stop();
      }
    }

  public:
    WemoWrapper() {
      Serial.printf("Setting up wemo, please wait\n");
      if (!Ethernet.begin(mac)) {
        Serial.printf("Failed to configure ethernet using DHCP\n");
        while (true);
      }

      Serial.print("My IP address: ");
      byte thisbyte;
      for (thisbyte = 0; thisbyte < 3; thisbyte++) {
        Serial.printf("%i.", Ethernet.localIP()[thisbyte]);
      }
      Serial.printf("%i\n", Ethernet.localIP()[thisbyte]);
    }

    void turnOn(int wemoIndex) {
      if (wemoStates[wemoIndex]) {
        Serial.printf("WEMO > Couldn't turn on %i, already on\n", wemoIndex);
      }
      wemoStates[wemoIndex] = true;
      Serial.printf("WEMO > Turning on %i \n", wemoIndex);

      _sendTurnOnPacket(wemoIndex);
    }

    void turnOff(int wemoIndex) {
      if (wemoStates[wemoIndex]) {
        Serial.printf("WEMO > Couldn't turn off %i, already off\n", wemoIndex);
      }
      wemoStates[wemoIndex] = true;
      Serial.printf("WEMO > Turning off %i \n", wemoIndex);

      _sendTurnOffPacket(wemoIndex);
    }
};
#endif
