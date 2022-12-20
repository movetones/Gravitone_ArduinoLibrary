/****************************************************************************************************************************
  ESP8266_WebSocketServer.ino
  For ESP8266

  Based on and modified from WebSockets libarary https://github.com/Links2004/arduinoWebSockets
  to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52 boards, etc.

  Built by Khoi Hoang https://github.com/khoih-prog/WebSockets_Generic
  Licensed under MIT license

  Originally Created on: 22.05.2015
  Original Author: Markus Sattler
*****************************************************************************************************************************/

#define NODEBUG_WEBSOCKETS

#if !defined(ESP8266)
  #error This code is intended to run only on the ESP8266 boards ! Please check your Tools->Board setting.
#endif

//#define _WEBSOCKETS_LOGLEVEL_     3

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <WebSocketsServer_Generic.h>

#include <Hash.h>

#include <ArduinoJson.h>

ESP8266WiFiMulti WiFiMulti;

WebSocketsServer webSocket = WebSocketsServer(81); 

#define CRLF        "\r\n"

// incoming messages (commands)
#define CMD_APCRED  F("AT+APCRED")    // set credentials for connecting to external AP (needed after device reset)
#define CMD_STACRED F("AT+STCRED")    // set credentials for the ESP to act as an AP (station mode)
#define CMD_SLEEP   F("AT+SLEEP")     // request ESP to sleep
#define CMD_SEND    F("AT+SEND")      // send data
#define CMD_STWSS   F("AT+STWSS")     // start websocket server

// outgoing messages (commands)
#define CMD_READY   F("+READY")       // response from ESP after startup
#define CMD_ACK     F("+OK")          // response from ESP after success etc
#define CMD_CONN    F("+CONN")        // connected to AP (message includes local IP)
#define CMD_RCV     F("+RCV")         // data received from websocket

#define PRINTBUF_MAX (500)
#define SERBUF_MAX (500)

// eventually add more states, 
// have options for different WS server settings, 
// maybe do OTA, pull config from online profile
#define STATE_STARTUP     0
#define STATE_CONN_WAIT   2
#define STATE_START_WSS   3
#define STATE_MAIN_LOOP   4


StaticJsonDocument<500> serDoc;
StaticJsonDocument<500> wsDoc;

int state = 0;
char printBuf[PRINTBUF_MAX];
int  printBufLen = 0;
char serInBuf[SERBUF_MAX];
int  serInBufLen = 0;
bool serInBufReady = false,
     serJsonAvailable = false, 
     wsDataAvailable = false;



bool haveClient = false;
uint8_t clientNum;
unsigned long lastSend = 0;



int wsStatus = 0;

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
  switch (type)
  {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      haveClient = false;
      break;
      
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\ n", num, ip[0], ip[1], ip[2], ip[3], payload);

        if (!haveClient){
          haveClient = true;
          clientNum = num;
          webSocket.sendTXT(num, "Connected");
        } else {
          webSocket.disconnect(num);
        }
      }
      break;
      
    case WStype_TEXT:
      Serial.printf("[%u] get Text: %s\n", num, payload);

      // turn payload into json and send to gravitone over serial

      
      wsDataAvailable = true;


      
      // send message to client
      // webSocket.sendTXT(num, "message here");

      // send data to all connected clients
      // webSocket.broadcastTXT("message here");
      break;
      
    case WStype_BIN:
      Serial.printf("[%u] get binary length: %u\n", num, length);
      hexdump(payload, length);

      // send message to client
      // webSocket.sendBIN(num, payload, length);
      break;

    default:
      break;
  }
}

void setup()
{
  Serial.begin(115200);

  delay(2000);

  Serial.print(CRLF);
  Serial.print(CMD_READY);
  Serial.print(CRLF);

  state = STATE_CONN_WAIT;
}



void loop()
{
  webSocket.loop();

  while( Serial.available() && !serInBufReady && (serInBufLen < SERBUF_MAX) ){
    serInBuf[ serInBufLen ] = Serial.read();
    
    if( (serInBuf[serInBufLen ] == '\n') && (serInBufLen > 1) ){
      if( serInBuf[ serInBufLen - 1 ] == '\r' ){
        serInBufReady = true;
      }
    }
    serInBufLen++;
  }

  if( serInBufReady ){
    DeserializationError error = deserializeJson(serDoc, serInBuf, serInBufLen);
    if ( error ){
      Serial.print("error parsing JSON: ");
      Serial.write(serInBuf, serInBufLen);
      Serial.print(", fstring: ");
      Serial.println(error.f_str());
      serJsonAvailable = false;
    } else {
      //Serial.println("successfully parsed JSON from serial");
      serJsonAvailable = true;
    }
    serInBufReady = false;
    serInBufLen = 0;
  }

  // waiting for credentials to be sent
  if( state == STATE_CONN_WAIT ){

    if( serJsonAvailable ){
      Serial.print("in STATE_CONN_WAIT, we think we got credential data over seral as JSON");


      if( strcmp(serDoc["type"],"wificred") == 0){
  
        const char* ssid = serDoc["ssid"];
        const char* pass = serDoc["pass"];
        
        WiFiMulti.addAP(ssid, pass);
  
        //WiFi.disconnect();
        while (WiFiMulti.run() != WL_CONNECTED)
        {
          Serial.print(".");
          delay(100);
        }
        
        Serial.println();
      
        // print your board's IP address:
        Serial.print("Starting WebSocket server @ ");
        Serial.println(WiFi.localIP());
  
        // assuming the credentials were the only useful thing in the 
        // current decoded json
        serJsonAvailable = false;
  
        // move on to main event loop only if we 
        // receive credentials to connect to
        state = STATE_START_WSS;
      } else {
        Serial.println("didnt detect wifi creds in input string");
        serJsonAvailable = false;
      }
    }
  }

  if( state == STATE_START_WSS ){
    Serial.print("\nStart ESP8266_WebSocketServer on "); Serial.println(ARDUINO_BOARD);
    Serial.println("Version " + String(WEBSOCKETS_GENERIC_VERSION));
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
    
    state = STATE_MAIN_LOOP;
  }

  if( state == STATE_MAIN_LOOP ){
    if( serJsonAvailable ){    
      //Serial.println("In MAIN_LOOP, got some JSON data over serial");

      if( strcmp(serDoc["type"],"data") == 0){
        // fill printBuf with serialized json from the serial input
        printBufLen = serializeJson(serDoc["data"], printBuf, PRINTBUF_MAX);
        webSocket.sendTXT(clientNum, printBuf, printBufLen);
      } else  {

        // parse different commands?
        
      }
      serJsonAvailable = false;
    }
  }
  
  /*
  if( haveClient && (millis() - lastSend > 100) ){
    String quatString = "{\"quat_w\":"+String(random(0,100)/100.0)+",\"quat_x\":"+String(random(0,100)/100.0) + ",\"quat_y\":"+String(random(0,100)/100.0) + ",\"quat_z\":"+String(random(0,100)/100.0) +  + ",\"millis\":"+String(millis()) + "}";
    webSocket.sendTXT(clientNum, quatString);
    lastSend = millis();
  }
  */
}
