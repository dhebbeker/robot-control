#include "board.hpp"
#include "Drives.hpp"
#include "wifi_ap.hpp"
#include "library_extension.hpp"
#include <assert.h>
#include <ESP8266WebServer.h>
#include <atomic>

static ESP8266WebServer server(80);

static struct
{
  bool isTargetNew = false;
  drives::Counter newDrive = 0;
  bool forward = true;
  drives::Counter newRotate = 0;
  bool clockwise = true;
} newTarget;

static constexpr std::size_t numberOfPositions = 50;
static Position positions[numberOfPositions] = { {0,0} };
static std::size_t positionIndex = 0;

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
		"    <table>\n"
		"    <tbody align=center valign=middle>\n"
		"      <tr><td></td><td><button type=\"submit\" name=\"forwards\" value=\"10\" title=\"+37,70mm\">&#8593;</button></td><td></td></tr>\n"
		"      <tr><td><button type=\"submit\" name=\"left\" value=\"5\" title=\"-86,4&deg;\">&#8634;</button></td><td>&#x1F916;</td><td><button type=\"submit\" name=\"right\" value=\"5\" title=\"+86,4&deg;\">&#8635;</button></td></tr>\n"
		"      <tr><td></td><td><button type=\"submit\" name=\"backwards\" value=\"10\" title=\"-37,70mm\">&#8595;</button></td><td></td></tr>\n"
		"    </tbody>\n"
		"    </table>\n"
		"  </form>\n"
		"  <img style=\"max-width:90vw; max-height:100vh;\" src=\"https://david.hebbeker.info/robot-control.php?positions=%s\" />\n"
		"  </main>\n"
		"  </body>\n"
		"</html>";


constexpr std::size_t maxCharPerPosition = (5+1)*2; //!< when serializing the position, the number of characters maximum used per position
constexpr std::size_t positionsStringMaxLength = maxCharPerPosition*numberOfPositions+1; 

static char htmlSourceBackBufferA[size(htmlSourceTemplate) + positionsStringMaxLength] = {0};
static char htmlSourceBackBufferB[size(htmlSourceTemplate) + positionsStringMaxLength] = {0};
static std::atomic<const char *> htmlSourceFrontBuffer(htmlSourceTemplate);

void updateHtmlSource()
{
	char * const backBuffer = (htmlSourceFrontBuffer.load() == htmlSourceBackBufferA) ? htmlSourceBackBufferB : htmlSourceBackBufferA;
	char positionStringBuffer[positionsStringMaxLength] = { 0 };
	std::size_t charPos = 0;
	for(std::size_t i=0; i<=positionIndex; i++)
	{
	  const int writtenCharacters = snprintf(&(positionStringBuffer[charPos]), maxCharPerPosition+1, "%i;%i;", positions[i].x, positions[i].y);
	  assert(writtenCharacters>0);
	  charPos += writtenCharacters;	  
	}
	const int writtenCharacters = snprintf(backBuffer, size(htmlSourceBackBufferA), htmlSourceTemplate, positionStringBuffer);
	assert(writtenCharacters>0);
	htmlSourceFrontBuffer = backBuffer;
}

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
	server.send(200, "text/html", htmlSourceFrontBuffer.load());
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
  board::ioExpander1.interruptMode(MCP23017InterruptMode::Separated);
  board::ioExpander1.interrupt(MCP23017Port::B, FALLING);
  
  // initialize pins
  pinMode(board::debugLed, OUTPUT);
  digitalWrite(board::debugLed, LOW);
  pinMode(board::ioExpanderIntB, INPUT_PULLUP);
  pinMode(board::ioExpanderIntAInv, INPUT);
  pinMode(board::leftBumper, INPUT_PULLUP);
  pinMode(board::rightBumper, INPUT_PULLUP);
  
  drives::LeftDrive::init();
  drives::RightDrive::init();
  
  attachInterrupt(board::ioExpanderIntB, drives::stopDrives, FALLING);

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
	updateHtmlSource();
}

void loop()
{
	server.handleClient();
	//Serial.printf("left: \t%3u, right: \t%3u\n", drives::LeftDrive::counter, drives::RightDrive::counter);
	if(drives::LeftDrive::isIdle && drives::RightDrive::isIdle)
	{
		const Position newPositionCandidate = drives::flushCurrentPosition();
		if(positions[positionIndex] != newPositionCandidate)
		{
			positions[++positionIndex] = newPositionCandidate;
			positionIndex %= numberOfPositions;
	    updateHtmlSource();
		}
		if(newTarget.isTargetNew)
		{
      const bool bumperIsPressed = digitalRead(board::leftBumper) == LOW || digitalRead(board::rightBumper) == LOW;
			if(newTarget.newDrive!=0 && (!newTarget.forward || !bumperIsPressed))
			{
			  drives::driveCounter(newTarget.newDrive, drives::cruiseSpeed, !newTarget.forward);
			  newTarget = { };
			}
			else if(newTarget.newRotate!=0 && !bumperIsPressed)
			{
			  drives::rotateCounter(newTarget.newRotate, drives::cruiseSpeed, newTarget.clockwise);
			  newTarget = { };
			}
		}
	}
}
