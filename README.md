Hardware Setup
==============

components
----------

 - Robot System Robby RP5 ROBOT (CCRP5)
 - NodeMCU version 3

### robot base CCRP5

The robot base is the "Robot System Robby RP5 ROBOT (CCRP5)" by Conrad Electronic GmbH. It is a mobile robot with chain drive. Its main processor is a C-Control/BASIC microcomputer. It was originally distributed under the order code `19 03 33`, EAN: `4016138222968`.

Resources:

 - [product website](https://www.conrad.de/de/p/roboter-robby-rp5-190333.html) with firmware, manuals, schematics and more for download
 - [Wiki article at RN-Wissen.de](https://rn-wissen.de/wiki/index.php/CCRP5) with detailed description of the circuits

As copyright or other restrictions may apply to the resources, I will not distribute them. I can only hope they will remain available.

### NodeMCU

NodeMCU is a simple board. Its main component is a ESP8266 system on a chip. It contains a Wi-Fi microchip with microcontroller capability. This simplifies to develop programs which can easily connect to the internet.

It can easily be programmed with the Arduino IDE. Board support packages for NodeMCU are available.

system design
-------------

The CCRP5 has two microcontrollers:

 - subsystem IC2 `MC68HC705KJ1`
 - main controller IC7 `MC68HC05B6`

The CCRP5 is designed such as, the subsystem is only programmed by the manufacturer. There is no in-system programming interface. All applications shall be written for the main controller.

The main controller should be programmed with a proprietary development environment CCEW32D. Programs must be written in CCBASIC, which is a dialect of the BASIC programming language. The applications are based on a firmware `P5DRIV.S19`. CCBASIC is a low level language. The documentation is available to a limited extent.

This projects aims to control the hardware of the CCRP5 without the CCRP5 microcontrollers.
Thus an additional "piggyback" PCBA is installed onto the basis CCRP5 PCBA which is tapping into the signals from the CCRP5.
The piggyback PCBA includes the NodeMCU board.

connections
-----------

### Piggyback PCBA

The schematics of the piggyback PCBA are described in `hardware/robot-control-circuit/robot-control-circuit.sch`.

### Basis PCBA

The original schematics can be retrieved at the above mentioned resources.

Some modifications were made to the CCRP5's PCBA.
The signals `D/A 1` and `D/A 2` have been disconnected from the pins of the CCRP5's main microcontroller (IC7) because the pins are configured as drain (referencing to CCRP5's schematics).
This interferes with the control of the NodeMCU and causes an 80mA current.
The signals have been disconnected by un-soldering and lifting of the microcontroller's pins.

Also the IO-expander `I11` has been disconnected in order to be able to control the current for the IR-LEDs `D3` and `D4`.

Software
========

The source code is written in C++. It is compiled and programmed with the [Arduino IDE](https://www.arduino.cc/en/Main/Software). The [board support package for the ESP8266 chip](https://github.com/esp8266/Arduino) must be installed.
The [Arduino library VL53L1](https://github.com/stm32duino/VL53L1) must be installed.
The [SimplyAtomic library](https://github.com/wizard97/SimplyAtomic) must be installed.
