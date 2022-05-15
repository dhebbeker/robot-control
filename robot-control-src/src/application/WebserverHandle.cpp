#include "WebserverHandle.hpp"
#include "../utils/array.hpp"
#include "../utils/algorithm.hpp"
#include "board.hpp"
#include <assert.h>
#include <cstddef>
#include <SimplyAtomic.h>

static constexpr char titleStart[] = "start";
static constexpr char titleStop[] = "stop";
static constexpr std::uint16_t unicodeIdStart = 0x25B6;
static constexpr std::uint16_t unicodeIdStop = 0x23F9;

static constexpr char htmlSourceTemplate[] =
    "<!DOCTYPE html>\n"
    "<html lang=\"en\">\n"
    "  <head>\n"
    "    <title>Robot Control</title>\n"
    "    <meta charset=\"utf-8\" />\n"
    "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.6\">\n"
    "    <meta http-equiv=\"refresh\" content=\"5\">\n"
    "  </head>\n"
    "  <body>\n"
    "  <main>\n"
    "  <form method=\"post\" action=\"/\">\n"
    "    <fieldset disabled>\n"
    "      <legend>Direct control</legend>\n"
    "      <table>\n"
    "      <tbody style=\"text-align:center; vertical-align:middle;\">\n"
    "        <tr><td></td><td><button type=\"submit\" name=\"forwards\" value=\"10\" title=\"+37,70mm\">&#8593;</button></td><td></td></tr>\n"
    "        <tr><td><button type=\"submit\" name=\"left\" value=\"5\" title=\"-86,4&deg;\">&#8634;</button></td><td>&#x1F916;</td><td><button type=\"submit\" name=\"right\" value=\"5\" title=\"+86,4&deg;\">&#8635;</button></td></tr>\n"
    "        <tr><td></td><td><button type=\"submit\" name=\"backwards\" value=\"10\" title=\"-37,70mm\">&#8595;</button></td><td></td></tr>\n"
    "      </tbody>\n"
    "      </table>\n"
    "    </fieldset>\n"
    "    <fieldset>\n"
    "      <legend>Follow wall</legend>\n"
    "      <button type=\"submit\" name=\"%sFW\" title=\"%s following wall\">&#x%hX;</button>\n"
    "    </fieldset>\n"
    "  </form>\n"
    "  <img style=\"max-width:90vw; max-height:100vh; border-style:solid;\" src=\"https://david.hebbeker.info/robot-control.php?positions=%s\" alt=\"Outline of driven route\" />\n"
    "  </main>\n"
    "  </body>\n"
    "</html>\n"
    "";

void WebserverHandle::handleRoot() {
  TargetRequest newTarget;
  board::setDebugLed(LOW);
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
  target = newTarget;
  const char * pointerToHtml = nullptr;
  ATOMIC()
  {
    pointerToHtml = htmlSourceFrontBuffer;
  }
  server.send(200, "text/html", pointerToHtml);
  board::setDebugLed(HIGH);
}

void WebserverHandle::loop() {
  updateHtmlSource();
}

WebserverHandle::WebserverHandle(ESP8266WebServer& webserver,
    const EnvironmentRecord &environmentRecord) :
    htmlSourceFrontBuffer(htmlSourceTemplate), server(webserver), environment(
        environmentRecord) {
}

WebserverHandle::TargetRequest WebserverHandle::flushTargetRequest() {
  TargetRequest oldTarget;
  ATOMIC()
  {
    oldTarget = target;
  }
  target = TargetRequest { };
  return oldTarget;
}

void WebserverHandle::setup() {
  updateHtmlSource();
}

void WebserverHandle::updateHtmlSource() {
  constexpr std::size_t maxCharPerPosition = (5+1)*2; // when serializing the position, the number of characters maximum used per position
  constexpr std::size_t positionsStringMaxLength = maxCharPerPosition*EnvironmentRecord::numberOfPositions+1;
  constexpr std::size_t extraCharactersForFollowWallButton = 2*utils::max(size(titleStart), size(titleStop)) + 2*utils::max(sizeof(unicodeIdStart), sizeof(unicodeIdStop));

  static char htmlSourceBackBufferA[size(htmlSourceTemplate) + extraCharactersForFollowWallButton + positionsStringMaxLength] = {0};
  static char htmlSourceBackBufferB[size(htmlSourceTemplate) + extraCharactersForFollowWallButton + positionsStringMaxLength] = {0};

  char * const backBuffer = (htmlSourceFrontBuffer == htmlSourceBackBufferA) ? htmlSourceBackBufferB : htmlSourceBackBufferA;
  char positionStringBuffer[positionsStringMaxLength] = { 0 };
  std::size_t charPos = 0;
  for(std::size_t i=0; i<=environment.positionIndex; i++)
  {
    const int writtenCharacters = snprintf(&(positionStringBuffer[charPos]), maxCharPerPosition+1, "%i;%i;", environment.positions[i].x, environment.positions[i].y);
    assert(writtenCharacters>0);
    charPos += writtenCharacters;
  }
  const int writtenCharacters = snprintf(
                                         backBuffer,
                                         size(htmlSourceBackBufferA),
                                         htmlSourceTemplate,
                                         titleStart,
                                         titleStart,
                                         unicodeIdStart,
                                         positionStringBuffer);
  assert(writtenCharacters>0);
  htmlSourceFrontBuffer = backBuffer;
}
