#ifndef ARTNETNODE_H
#define ARTNETNODE_H
/*

Copyright (c) Charles Yarnold charlesyarnold@gmail.com 2015

Copyright (c) 2016 Stephan Ruloff
https://github.com/rstephan

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, under version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <Arduino.h>

#if defined(ARDUINO_ARCH_ESP32) || defined(ESP32)
#include <WiFi.h>
#elif defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WiFi.h>
#else
#error "Architecture not supported!"
#endif

#include <Udp.h>
#include <WiFiUdp.h>
#include <EthernetUdp.h>

#include "OpCodes.h"
#include "NodeReportCodes.h"
#include "StyleCodes.h"
#include "PriorityCodes.h"
#include "ProtocolSettings.h"
#include "PollReply.h"



class Artnetnode
{
public:
  //Artnetnode(const std::shared_ptr<UDP> &Udp);
  //Artnetnode();
  //Artnetnode(UDP * Udp_obj);
  Artnetnode();


  uint8_t beginWiFi(IPAddress local_IP, IPAddress local_Mask, String hostname="");
  uint8_t beginEthernet(IPAddress local_IP, IPAddress local_Mask, String hostname="");
  uint16_t read();

  // Node identity
  void setShortName(const char name[]);
  void setLongName(const char name[]);
  void setName(const char name[]);
  void setNumPorts(uint8_t num);

  void setStartingUniverse(uint16_t startingUniverse);

  // Transmit
  int write(void);
  int write(IPAddress ip);
  void setByte(uint16_t pos, uint8_t value);

  inline void setUniverse(uint16_t universe)
  {
    outgoingUniverse = universe;
  }

  inline void setPhysical(uint8_t port)
  {
    physical = port;
  }

  inline void setLength(uint16_t len)
  {
    dmxDataLength = len;
  }

  // DMX controls
  void enableDMX();
  void disableDMX();
  void enableDMXOutput(uint8_t outputID);
  void disableDMXOutput(uint8_t outputID);

  uint8_t setDMXOutput(uint8_t outputID, uint8_t uartNum, uint16_t attachedUniverse);

  // DMX tick
  void tickDMX(uint32_t time);
  
  // Return a pointer to the start of the DMX data
  inline uint8_t* getDmxFrame(void)
  {
    return artnetPacket + ARTNET_DMX_START_LOC;
  }

  inline void setArtDmxCallback(void (*fptr)(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data)) 
  {
    artDmxCallback = fptr;
  }

private:
  UDP * Udp;
  //EthernetUDP Udp;
  PollReply PollReplyPacket;
  String host;

  // Packet handlers
  uint16_t handleDMX(uint8_t nzs);
  uint16_t handlePollRequest();
  
  // Packet vars
  uint8_t artnetPacket[ARTNET_MAX_BUFFER];
  uint16_t packetSize;
  uint16_t opcode;
  uint8_t sequence;
  uint8_t physical;
  uint16_t outgoingUniverse;
  uint16_t dmxDataLength;
  IPAddress localIP;
  IPAddress localMask;
  IPAddress localBroadcast;

  // Packet functions
  bool isBroadcast();
  uint16_t makePacket(void);

  // DMX settings
  bool DMXOutputStatus;
  uint16_t DMXOutputs[DMX_MAX_OUTPUTS][3];
  uint8_t DMXBuffer[DMX_MAX_OUTPUTS][DMX_MAX_BUFFER];

  uint16_t startingUniverse;

  // DMX tick
  void sendDMX();
  uint8_t* getDmxFrame(uint8_t outputID);
  uint8_t msSinceDMXSend;

  void (*artDmxCallback)(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data);
  static const char artnetId[];
};

#endif
