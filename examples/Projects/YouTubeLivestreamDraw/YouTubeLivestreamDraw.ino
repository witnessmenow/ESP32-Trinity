/*******************************************************************
    Allows YouTube live stream chat to draw on a 64x64 LED matrix.

    - "!colour x y" is the draw command
    - supported colour: red, green, blue, black

    For use with my I2S Matrix Shield.

    Parts:
    ESP32 Mini Kit (ESP32 D1 Mini) * - https://s.click.aliexpress.com/e/_AYPehO (pick the CP2104 Drive version)
    ESP32 I2S Matrix Shield (From my Tindie) = https://www.tindie.com/products/brianlough/esp32-i2s-matrix-shield/

 *  * = Affiliate

    If you find what I do useful and would like to support me,
    please consider becoming a sponsor on Github
    https://github.com/sponsors/witnessmenow/


    Written by Brian Lough
    YouTube: https://www.youtube.com/brianlough
    Tindie: https://www.tindie.com/stores/brianlough/
    Twitter: https://twitter.com/witnessmenow
 *******************************************************************/

// ----------------------------
// Standard Libraries
// ----------------------------

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif

#include <WiFiClientSecure.h>

// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------

#include <YouTubeLiveStream.h>
// Library for interacting with YouTube Livestreams

// Only available on Github
// https://github.com/witnessmenow/youtube-livestream-arduino

#define ARDUINOJSON_DECODE_UNICODE 1

#include <ArduinoJson.h>
// Library used for parsing Json from the API responses

// Search for "Arduino Json" in the Arduino Library manager
// https://github.com/bblanchon/ArduinoJson

#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
// This is the library for interfacing with the display

// Can be installed from the library manager (Search for "ESP32 MATRIX DMA")
// https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA

//------- Replace the following! ------

char ssid[] = "SSID";         // your network SSID (name)
char password[] = "password"; // your network password

// You need 1 API key per roughly 2 hours of chat you plan to monitor 
// So you can pass in just one:

#define YT_API_TOKEN "AAAAAAAAAABBBBBBBBBBBCCCCCCCCCCCDDDDDDDDDDD"

// Or you can pass in an array of keys, 2 keys gives 4 hours, 3 == 6 etc (See Github readme for info)

//#define NUM_API_KEYS 2
//const char *keys[NUM_API_KEYS] = {"AAAAAAAAAABBBBBBBBBBBCCCCCCCCCCCDDDDDDDDDDD", "AAAAAAAAAABBBBBBBBBBBCCCCCCCCCCCEEEEEEEEEE"};


//#define CHANNEL_ID "UC8rQKO2XhPnvhnyV1eALa6g" //Bitluni's trash
//#define CHANNEL_ID "UCSJ4gkVC6NrvII8umztf0Ow" //Lo-fi beats (basically always live)
#define CHANNEL_ID "UCu94OHbBYVUXLYPh4NBu10w" // Unexpected Maker

#define LED_PIN LED_BUILTIN

//------- ---------------------- ------

// -------------------------------------
// -------   Matrix Config   ------
// -------------------------------------

const int panelResX = 64;   // Number of pixels wide of each INDIVIDUAL panel module.
const int panelResY = 32;   // Number of pixels tall of each INDIVIDUAL panel module.
const int panel_chain = 1;  // Total number of panels chained one to another.

MatrixPanel_I2S_DMA *dma_display = nullptr;

uint16_t myBLACK = dma_display->color565(0, 0, 0);
uint16_t myWHITE = dma_display->color565(255, 255, 255);
uint16_t myRED = dma_display->color565(255, 0, 0);
uint16_t myGREEN = dma_display->color565(0, 255, 0);
uint16_t myBLUE = dma_display->color565(0, 0, 255);

WiFiClientSecure client;

#ifdef NUM_API_KEYS
YouTubeLiveStream ytVideo(client, keys, NUM_API_KEYS);
#else
YouTubeLiveStream ytVideo(client, YT_API_TOKEN);
#endif


unsigned long requestDueTime;               //time when request due
unsigned long delayBetweenRequests = 5000; // Time between requests (5 seconds)

unsigned long ledBlinkDueTime;               //time when blink is due
unsigned long delayBetweenBlinks = 1000; // Time between blinks (1 seconds)

LiveStreamDetails details;
char liveChatId[YOUTUBE_LIVE_CHAT_ID_CHAR_LENGTH];
char videoId[YOUTUBE_VIDEO_ID_LENGTH];
bool haveVideoId = false;
bool haveLiveChatId = false;

bool ledState = false;
bool isBlinking = false;

char lastMessageReceived[YOUTUBE_MSG_CHAR_LENGTH];

void setup() {

  HUB75_I2S_CFG mxconfig(
    panelResX,   // Module width
    panelResY,   // Module height
    panel_chain  // Chain length
  );

  mxconfig.gpio.e = 18;

  // May or may not be needed depending on your matrix
  // Example of what needing it looks like:
  // https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA/issues/134#issuecomment-866367216
  mxconfig.clkphase = false;

  //mxconfig.driver = HUB75_I2S_CFG::FM6126A;

  // Display Setup
  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->begin();
  dma_display->setBrightness8(100); //0-255
  dma_display->clearScreen();
  dma_display->fillScreen(myBLACK);

  liveChatId[0] = '\0';
  videoId[0] = '\0';
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, ledState);

  // Set WiFi to 'station' mode and disconnect
  // from the AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Connect to the WiFi network
  Serial.print("\nConnecting to WiFi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);


  //TODO: Use certs
  client.setInsecure();

}

void drawChatPixel(char *msg) {

  // Expect messages to be in the form "!colour x y"
  char *token;
  token = strtok(msg, " "); // https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm

  // Token should now == "!colour"
  Serial.print("Potential Colour");
  Serial.println(token);

  uint16_t colour;
  int x;
  int y;

  if (token != NULL) {
    if (strcmp(token, "!red") == 0) {
      colour = myRED;
    } else if (strcmp(token, "!blue")  == 0) {
      colour = myBLUE;
    } else if (strcmp(token, "!green")  == 0) {
      colour = myGREEN;
    } else if (strcmp(token, "!black")  == 0) {
      colour = myBLACK;
    } else {
      //Not a valid colour
      return;
    }

    token = strtok(NULL, " ");
    // Token should now == x
    if (token != NULL) {
      x = atoi(token);
      if (x >= panelResX || x < 0)
        return;
      token = strtok(NULL, " ");
      // Token should now == y
      if (token != NULL) {
        y = atoi(token);
        if (y >= panelResY || y < 0)
          return;
      } else {
        //Not Valid
        return;
      }
    } else {
      //Not Valid
      return;
    }

    //If we are here, its probably valid.
    dma_display->drawPixel(x,y,colour);
  }
}

bool processMessage(ChatMessage chatMessage, int index, int numMessages) {

  switch (chatMessage.type)
  {
    case yt_message_type_text:

      //See is it a command
      if ( chatMessage.displayMessage[0] == '!')
      {
        Serial.print("Received command from ");
        Serial.println(chatMessage.displayName);
        drawChatPixel((char *) chatMessage.displayMessage);
      } 
      break;
    case yt_message_type_superChat:
    case yt_message_type_superSticker:
      // You could potentially lock off events behind tiers or values
      if (chatMessage.tier >= 0) {
        isBlinking = true;
        Serial.print("SuperChat/Sticker from: ");
        Serial.print(chatMessage.displayName);
        return false; // This will stop processing messages
      }
      break;
    default:
      Serial.print("Unknown Message Type: ");
      Serial.println(chatMessage.type);
  }

  // return false from this method if you want to
  // stop parsing more messages.
  return true;
}


// This gets the video ID of a live stream on a given channel
void getVideoId() {
  // This is the official way to get the videoID, but it
  // uses too much of your daily quota.
  //haveVideoId = ytVideo.getLiveVideoId(CHANNEL_ID, videoId, YOUTUBE_VIDEO_ID_LENGTH);

  haveVideoId = ytVideo.scrapeIsChannelLive(CHANNEL_ID, videoId, YOUTUBE_VIDEO_ID_LENGTH);
  if (haveVideoId) {
    Serial.println("Channel is live");
    Serial.print("Video ID: ");
    Serial.print(videoId);
  } else {
    Serial.println("Channel does not seem to be live");
  }
}

// This gets the Live Chat ID of a live stream on a given Video ID.
// This is needed to get chat messages.
void getLiveChatId() {
  haveLiveChatId = false;

  details = ytVideo.getLiveStreamDetails(videoId);
  if (!details.error) {
    if (details.isLive) {
      Serial.print("Chat Id: ");
      Serial.println(details.activeLiveChatId);
      strncpy(liveChatId, details.activeLiveChatId, sizeof(liveChatId));
      liveChatId[sizeof(liveChatId) - 1] = '\0';
      haveLiveChatId = true;
    } else {
      Serial.println("Video does not seem to be live");
      haveVideoId = false;
    }
  } else {
    Serial.println("Error getting Live Stream Details");
  }
}

void loop() {
  if (millis() > requestDueTime) {
    if (!haveVideoId) {
      //Don't have a video ID, so we'll try get one.
      getVideoId();
    }

    if (haveVideoId && !haveLiveChatId) {
      // We have a video ID, but not a liveChatId
      getLiveChatId();
    }

    if (haveLiveChatId) {
      // The processMessage Callback will be called for every message found.
      ChatResponses responses = ytVideo.getChatMessages(processMessage, liveChatId); 
      if (!responses.error) {
        Serial.println("done");
        Serial.print("Polling interval: ");
        Serial.println(responses.pollingIntervalMillis);

        requestDueTime = millis() + responses.pollingIntervalMillis;
      } else if (!responses.isStillLive) {
        //Stream is not live any more.
        haveLiveChatId = false;
        haveVideoId = false;
        requestDueTime = millis() + delayBetweenRequests;
      } else {
        Serial.println("There was an error getting Messages");
        requestDueTime = millis() + delayBetweenRequests;
      }
    } else {
      requestDueTime = millis() + delayBetweenRequests;
    }
  }
}
