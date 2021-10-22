#ifndef ROBOT_CONTROL_SRC_WEBSERVERHANDLE_HPP_
#define ROBOT_CONTROL_SRC_WEBSERVERHANDLE_HPP_

#include "Drives.hpp"
#include "../middleware/EnvironmentRecord.hpp"
#include <ESP8266WebServer.h>

class WebserverHandle {
public:

  WebserverHandle(ESP8266WebServer& webserver, const EnvironmentRecord& environmentRecord);

  /**
   * Does handle web client requests to root path.
   */
  void handleRoot();

  void setup();

  /**
   * Should be called regularly.
   */
  void loop();

  struct TargetRequest
  {
    bool isTargetNew = false;
    drives::Counter newDrive = 0;
    bool forward = true;
    drives::Counter newRotate = 0;
    bool clockwise = true;
  };

  TargetRequest flushTargetRequest();

private:
  /**
   * Does calculate the HTML source to be handled to the web client.
   */
  void updateHtmlSource();

  const char * htmlSourceFrontBuffer;
  TargetRequest target;
  ESP8266WebServer& server;
  const EnvironmentRecord& environment;
};

#endif /* ROBOT_CONTROL_SRC_WEBSERVERHANDLE_HPP_ */
