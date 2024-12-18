#include <Arduino.h>
#include <esp_http_server.h>
#include <set>
#include "Camera.h"
#include "WifiNetwork.h"
#include "TaskScheduler.h"
#include "WebServer.h"
#include "Preferences.h"
#include "Chip.h"

#define Boundary "frame"

WebServer server(80);
Preferences preferences;
std::set<String> settings = {
        "jpegQuality",
        "frameSize",
        "stationSsid",
        "stationPass",
        "appSsid",
        "appPass"
};

void setup() {
    Serial.begin(115200);
    preferences.begin("app");
    WifiNetwork network;
    Camera camera(
            (framesize_t)preferences.getString("frameSize", String(FRAMESIZE_UXGA)).toInt(),
            preferences.getString("jpegQuality", "5").toInt()
    );

    network.connect({
            .stationPass = preferences.getString("stationPass", ""),
            .stationSsid = preferences.getString("stationSsid", ""),
            .appPass = preferences.getString("appPass", "adminadmin"),
            .appSsid = preferences.getString("appSsid", String(Chip::id())),
    });

    server.on("/stream", HTTP_GET, [&camera]() {
        server.client().write("HTTP/1.1 200 OK\r\n");
        server.client().write("Access-Control-Allow-Origin: *\r\n");
        server.client().write((String("Content-Type: multipart/x-mixed-replace;boundary=") + Boundary + "\r\n").c_str());
        auto boundary = "\r\n" + String("--") + Boundary + "\r\n";
        while (true) {
            if (!server.client().connected()) {
                return;
            }
            camera.onFrame([&boundary](camera_fb_t *f) {
                server.client().write(boundary.c_str());
                server.client().write("Content-Type: image/jpeg\r\n");
                server.client().write((String("Content-Length: ") + f->len + "\r\n\r\n").c_str());
                server.client().write((char *) f->buf, f->len);
            });
        }
    });

    server.on("/settings", HTTP_GET, []() {
        for (const auto &setting : settings) {
            auto val = server.arg(setting);
            if (!val.isEmpty()) {
                preferences.putString(setting.c_str(), val.c_str());
            }
        }
        server.send(200, "text/plain", "OK");
    });

    server.on("/restart", HTTP_GET, []() {
        ESP.restart();
    });

    server.begin();
    Serial.println(WiFi.localIP());
    Serial.println(WiFi.softAPIP());
}

void loop() {
    server.handleClient();
}
