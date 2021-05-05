#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <PCF8574.h>
#include <DHTesp.h>
#include "wifi_ap.hpp"

#include "board.hpp"
#include "Drives.hpp"

static ESP8266WebServer server(80);
static struct
{
  bool isTargetNew = false;
  drives::Counter newDrive = 0;
  bool forward = true;
  drives::Counter newRotate = 0;
  bool clockwise = true;
} newTarget;

constexpr auto htmlSource = 
    "<!DOCTYPE html>\n"
    "<html lang=\"en\">\n"
    "  <head>\n"
    "    <title>Robot Control</title>\n"
    "    <meta charset=\"utf-8\" />\n"
    "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.6\">\n"
    "  </head>\n"
    "  <body>\n"
    "  <main>\n"
    "  <form method=\"post\" action=\"/\">\n"
    "    <table>\n"
    "    <tbody align=center valign=middle>\n"
    "      <tr><td></td><td><button type=\"submit\" name=\"forwards\" value=\"10\" title=\"+37,70mm\">&#8593;</button></td><td></td></tr>\n"
    "      <tr><td><button type=\"submit\" name=\"left\" value=\"5\" title=\"-86,4&deg;\">&#8634;</button></td><td>&#x1F916;</td><td><button type=\"submit\" name=\"right\" value=\"5\" title=\"+86,4&deg;\">&#8635;</button></td></tr>\n"
    "      <tr><td></td><td><button type=\"submit\" name=\"backwards\" value=\"10\" title=\"-37,70mm\">&#8595;</button></td><td></td></tr>\n"
    "    </tbody>\n"
    "    </table>\n"
    "  </form>\n"
    "  </main>\n"
    "  </body>\n"
    "</html>";

static void handleRoot() 
{
  digitalWrite(board::debugLed, LOW);
  if(server.hasArg("forwards"))
  {
    newTarget.newDrive = server.arg("forwards").toInt();
    newTarget.forward = true;
    newTarget.isTargetNew = true;
    Serial.printf("Got forwards by %u!\n", newTarget.newDrive);
  }
  if(server.hasArg("backwards"))
  {
    newTarget.newDrive = server.arg("backwards").toInt();
    newTarget.forward = false;
    newTarget.isTargetNew = true;
    Serial.printf("Got backwards by %u!\n", newTarget.newDrive);
  }
  if(server.hasArg("left"))
  {
    newTarget.newRotate = server.arg("left").toInt();
    newTarget.clockwise = false;
    newTarget.isTargetNew = true;
    Serial.printf("Got left by %u!\n", newTarget.newRotate);
  }
  if(server.hasArg("right"))
  {
    newTarget.newRotate = server.arg("right").toInt();
    newTarget.clockwise = true;
    newTarget.isTargetNew = true;
    Serial.printf("Got right by %u!\n", newTarget.newRotate);
  }
  server.send(200, "text/html", htmlSource);
  digitalWrite(board::debugLed, HIGH);
}

void setup()
{
  Serial.begin(115200);
  delay(100);
  Serial.flush();
  delay(100);
  Serial.printf("\n begin program '%s'\n", __FILE__);
  Wire.begin(board::sda, board::scl);
  board::ioExpander1.init();
  pinMode(board::debugLed, OUTPUT);
  digitalWrite(board::debugLed, LOW);
  drives::leftDrive.init();
  drives::rightDrive.init();

  Serial.printf("connect to wifi %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");

  server.begin();
  Serial.printf("webserver has IP %s\n", WiFi.localIP().toString().c_str());
  server.on("/", handleRoot);
}

void loop()
{
  server.handleClient();
  //Serial.printf("left: \t%3u, right: \t%3u\n", drives::leftDrive.counter, drives::rightDrive.counter);
  if(drives::leftDrive.isIdle && drives::rightDrive.isIdle && newTarget.isTargetNew)
  {
    if(newTarget.newDrive!=0)
    {
      drives::driveCounter(newTarget.newDrive, drives::cruiseSpeed, !newTarget.forward);
    }
    else if(newTarget.newRotate!=0)
    {
      drives::rotateCounter(newTarget.newRotate, drives::cruiseSpeed, newTarget.clockwise);
    }
    newTarget = { };
  }
}
