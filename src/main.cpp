#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>

#include <Adafruit_NeoPixel.h>

#include "index.h"
#include "favicon.h"

#include "../config.h"

MDNSResponder mdns;

ESP8266WiFiMulti wiFiMulti;

ESP8266WebServer server(80);
WebSocketsServer webSocket(81);

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);

const int colorWhite = strip.Color(0, 0, 0, 255);

enum Animation
{
  NONE,
  RAINBOW,
  FADE
};

struct StripStatus
{
private:
  int _color = colorWhite;

public:
  byte brightness = 255;
  int hue = 0;
  bool enabled = true;
  bool isWhite = true;
  Animation anim = Animation::NONE;

  void setHue(int h)
  {
    hue = h % 360;
    isWhite = false;
    _color = colorHue(hue);
  }

  int colorHue(int hue)
  {
    double hue_d = (hue % 360) / 360.0;
    return strip.ColorHSV(hue_d * 65535);
  }

  void setWhite(bool white)
  {
    isWhite = white;
    if (white)
      anim = Animation::NONE;
  }

  int color()
  {
    return isWhite ? colorWhite : _color;
  }

  String asJson()
  {
    String json = "";
    json += "{";
    json += "\"enabled\": ";
    json += (enabled ? "true" : "false");

    json += ", ";
    json += "\"hue\": ";
    json += hue;

    json += ", ";
    json += "\"brightness\": ";
    json += brightness;

    json += ", ";
    json += "\"isWhite\": ";
    json += isWhite;

    json += ", ";
    json += "\"anim\": ";
    json += anim;

    json += "}";
    return json;
  }
};

StripStatus stripStatus;

void updateStripStatus()
{
  strip.setBrightness(stripStatus.enabled ? stripStatus.brightness : 0);
  if (stripStatus.enabled)
  {
    if (stripStatus.anim != Animation::RAINBOW)
    {
      strip.fill(stripStatus.color());
    }
    else
    {
      for (int i = 0; i <= LED_COUNT; i++)
      {
        strip.setPixelColor(i, stripStatus.colorHue(stripStatus.hue + i));
      }
    }
  }
  strip.show();
}

inline void setStripWhite()
{
  stripStatus.setWhite(true);
  updateStripStatus();
}

inline void setStripHue(int hue)
{
  stripStatus.setHue(hue);
  updateStripStatus();
}

inline void setStripBrightness(byte brightness)
{
  stripStatus.brightness = brightness;
  updateStripStatus();
}

inline void setStripEnabled(bool enabled)
{
  stripStatus.enabled = enabled;
  updateStripStatus();
}

void handleJsonMessage(String message)
{

  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, message);
  if (error)
  {
    Serial.print("deserializeJson failed: ");
    Serial.println(error.f_str());
    return;
  }

  if (doc.containsKey("enabled"))
  {
    Serial.println("setting 'enabled'");
    stripStatus.enabled = doc["enabled"];
  }
  if (doc.containsKey("hue"))
  {
    Serial.println("setting 'hue'");
    int hue = doc["hue"];
    Serial.println(hue);
    stripStatus.setHue(hue);
  }
  if (doc.containsKey("brightness"))
  {
    Serial.println("setting 'brightness'");
    byte brightness = doc["brightness"];
    Serial.println(brightness);
    stripStatus.brightness = brightness;
  }
  if (doc.containsKey("brightness+"))
  {
    Serial.println("adding 'brightness'");
    int brightness = doc["brightness+"];
    Serial.println(brightness);
    int stripBrightness = stripStatus.brightness;
    int newBright = stripBrightness + brightness;
    if (newBright < 20)
    {
      newBright = 20;
    }
    else if (newBright > 255)
    {
      newBright = 255;
    }
    stripStatus.brightness = newBright;
  }
  if (doc.containsKey("isWhite"))
  {
    Serial.println("setting 'isWhite'");
    bool isWhite = doc["isWhite"];
    Serial.println(isWhite);
    stripStatus.setWhite(isWhite);
  }
  if (doc.containsKey("anim"))
  {
    Serial.println("setting 'anim'");
    int anim = doc["anim"];
    Serial.println(anim);
    stripStatus.anim = static_cast<Animation>(anim);
  }

  updateStripStatus();

  String buf = stripStatus.asJson();
  webSocket.broadcastTXT(buf);
}

void handleRoot()
{
  Serial.println("handle 'root' request");
  server.send(200, "text/html", MAIN_page);
}

void handleIcon()
{
  Serial.println("handle 'icon' request");
  server.send(200, "image/svg+xml", FAVICON);
}

void handleLed()
{
  Serial.println("handle 'LED' request");
  handleJsonMessage(server.arg("plain"));
  server.send(200);
}

void handleNotFound()
{
  Serial.println("handle 'not found' request");
  server.send(404, "text/plain", "What are you doing here?");
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
  Serial.println("websocket event");
  char *charPayload = (char *)payload;

  switch (type)
  {
  case WStype_DISCONNECTED:
    Serial.printf("[%u] Disconnected!\r\n", num);
    break;
  case WStype_CONNECTED:
  {
    IPAddress ip = webSocket.remoteIP(num);
    Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\r\n", num, ip[0], ip[1], ip[2], ip[3], payload);
    String status = stripStatus.asJson();
    Serial.println("Current Status: ");
    Serial.println(status);
    webSocket.sendTXT(num, status);
  }
  break;
  case WStype_TEXT:
  {
    Serial.printf("[%u] get Text: %s\r\n", num, payload);

    handleJsonMessage(charPayload);
  }
  break;
  default:
    Serial.printf("Invalid WStype: %d\r\n", type);
    break;
  }
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Serial connected");
  strip.begin();
  strip.setBrightness(255);
  strip.fill(strip.Color(10, 10, 255));
  strip.show();
  delay(50);
  Serial.println("NeoPixel started");

  wiFiMulti.addAP(ssid, password);

  Serial.print("Connecting to WiFi");

  while (wiFiMulti.run() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (mdns.begin(mdnsHostname, WiFi.localIP()))
  {
    Serial.println("MDNS responder started");
    mdns.addService("http", "tcp", 80);
    mdns.addService("ws", "tcp", 81);
    
    Serial.print("Connect to http://");
    Serial.print(mdnsHostname);
    Serial.print(".local or http://");
  }
  else
  {
    Serial.println("MDNS startup failed");
    Serial.print("Connect to http://");
  }

  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handleRoot);
  server.on("/favicon.svg", HTTP_GET, handleIcon);
  server.on("/LED", HTTP_POST, handleLed);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  setStripEnabled(true);
}

uint16_t frame = 0;

void loop()
{
  mdns.update();
  webSocket.loop();
  server.handleClient();

  frame += 1;

  if (stripStatus.enabled && frame >= 100)
  {
    frame = 0;
    switch (stripStatus.anim)
    {
    case NONE:
      break;
    case RAINBOW:
      stripStatus.setHue(stripStatus.hue + 1);
      updateStripStatus();
      break;

    case FADE:
      stripStatus.setHue(stripStatus.hue + 1);
      updateStripStatus();
      break;
    }
  }
}
