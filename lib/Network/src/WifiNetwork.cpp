#include "WifiNetwork.h"
#include <WiFi.h>

IPAddress local_ip(192,168,1,100);
IPAddress gateway(192,168,1,100);
IPAddress subnet(255,255,255,0);

void WifiNetwork::connect(const Credentials& c) {
    WiFi.mode(WIFI_MODE_APSTA);
    WiFi.setSleep(false);
    WiFi.softAPConfig(local_ip, gateway, subnet);
    WiFi.begin(c.stationSsid.c_str(), c.stationPass.c_str());
    WiFi.softAP(c.appSsid.c_str(), c.appPass.c_str());
    auto attempt = 0;
    while (!connected()) {
        if (attempt > 20) {
            return;
        }
        attempt++;
        delay(500);
    }
}

bool WifiNetwork::connected() {
    return WiFi.isConnected();
}