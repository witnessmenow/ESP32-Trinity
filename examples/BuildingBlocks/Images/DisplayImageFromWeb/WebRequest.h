// These are mostly all taken from my Spotify Arduino library
// https://github.com/witnessmenow/spotify-api-arduino

Client *webClient;

int getHttpStatusCode()
{
  char status[32] = {0};
  webClient->readBytesUntil('\r', status, sizeof(status));
  Serial.print(F("Status: "));
  Serial.println(status);

  char *token;
  token = strtok(status, " "); // https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm

  Serial.print(F("HTTP Version: "));
  Serial.println(token);

  if (token != NULL && (strcmp(token, "HTTP/1.0") == 0 || strcmp(token, "HTTP/1.1") == 0))
  {
    token = strtok(NULL, " ");
    if (token != NULL)
    {
      Serial.print(F("Status Code: "));
      Serial.println(token);
      return atoi(token);
    }
  }

  return -1;
}

int makeGetRequest(const char *command, const char *host, int portNumber)
{
  webClient->flush();
  webClient->setTimeout(2000);
  if (!webClient->connect(host, portNumber))
  {
    Serial.println(F("Connection failed"));
    return -1;
  }

  // give the esp a breather
  yield();

  // Send HTTP request
  webClient->print(F("GET "));
  webClient->print(command);
  webClient->println(F(" HTTP/1.0"));

  //Headers
  webClient->print(F("Host: "));
  webClient->println(host);

  webClient->println(F("Cache-Control: no-cache"));

  if (webClient->println() == 0)
  {
    Serial.println(F("Failed to send request"));
    return -2;
  }

  int statusCode = getHttpStatusCode();

  return statusCode;
}

int getContentLength()
{

  if (webClient->find("Content-Length:"))
  {
    int contentLength = webClient->parseInt();
    Serial.print(F("Content-Length: "));
    Serial.println(contentLength);
    return contentLength;
  }

  return -1;
}

void skipHeaders()
{
  // Skip HTTP headers
  if (!webClient->find("\r\n\r\n"))
  {
    Serial.println(F("Invalid response"));
    return;
  }
}

void closeClient()
{
  if (webClient->connected())
  {
    Serial.println(F("Closing client"));
    webClient->stop();
  }
}

// The start of this is parsing the url into something we can use

bool getImage(char *imageUrl, Stream *file)
{
  // ------------------------
  // Step 1: Parse the URL (harder than it sounds :) )
  // ------------------------

  Serial.print(F("Parsing image URL: "));
  Serial.println(imageUrl);
  int totalLength = getContentLength();

  uint8_t lengthOfString = strlen(imageUrl);

  int port = 0;
  uint8_t protocolLength = 0;

  if (strncmp(imageUrl, "https://", 8) == 0)
  {
    Serial.println("is Https");
    port = 443;
    protocolLength = 8;
  } else if (strncmp(imageUrl, "http://", 7) == 0) {
    Serial.println("is Http");
    port = 80;
    protocolLength = 7;
  } else {
    Serial.print("Url not in expected format: ");
    Serial.println(imageUrl);
    Serial.println("(expected it to start with \"https://\" or \"http://\")");
    return false;
  }

  char *pathStart = strchr(imageUrl + protocolLength, '/');
  uint8_t pathIndex = pathStart - imageUrl;
  uint8_t pathLength = lengthOfString - pathIndex;
  char path[pathLength + 1];
  strncpy(path, pathStart, pathLength);
  path[pathLength] = '\0';

  uint8_t hostLength = pathIndex - protocolLength;
  char host[hostLength + 1];
  strncpy(host, imageUrl + protocolLength, hostLength);
  host[hostLength] = '\0';

  Serial.print(F("host: "));
  Serial.println(host);

  Serial.print(F("len:host:"));
  Serial.println(hostLength);

  Serial.print(F("path: "));
  Serial.println(path);

  Serial.print(F("len:path: "));
  Serial.println(strlen(path));

  // ------------------------
  // Step 2: Make the request and read the headers
  // ------------------------
  int statusCode = makeGetRequest(path, host, port);
  Serial.print(F("statusCode: "));
  Serial.println(statusCode);
  if (statusCode == 200)
  {
    int totalLength = getContentLength();

    if (totalLength > 0)
    {
      skipHeaders();
      int remaining = totalLength;

      // ------------------------
      // Step 3: Read the response and save it to your file
      // ------------------------

      // This section of code is inspired by the "Web_Jpg"
      // example of TJpg_Decoder
      // https://github.com/Bodmer/TJpg_Decoder
      // -----------
      uint8_t buff[128] = {0};
      while (webClient->connected() && (remaining > 0 || remaining == -1))
      {
        // Get available data size
        size_t size = webClient->available();

        if (size)
        {
          // Read up to 128 bytes
          int c = webClient->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));

          // Write it to file
          file->write(buff, c);

          // Calculate remaining bytes
          if (remaining > 0)
          {
            remaining -= c;
          }
        }
        yield();
      }
      Serial.println(F("Finished getting image"));
    }
    closeClient();
    return (totalLength > 0); //Probably could be improved!

  } else {
    Serial.println(F("Status Code was not 200"));
    return false;
  }

}
