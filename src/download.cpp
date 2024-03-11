#include "util.h"

/**
 * Downloads a file from a given URL and saves it to a specified location on the
 * SD card. Assumes the WiFi connection has been established outside this
 * function.
 *
 * @param url The URL of the file to download.
 * @param path The path on the SD card where the file will be saved.
 * @return True if download and save were successful, false otherwise.
 */
bool
downloadFileToSD(const String& url, const String& path)
{
  // Ensure the SD card is initialized
  if (!SD.begin()) {
    printf("Error: SD card initialization failed");
    return false;
  }

  HTTPClient http;
  http.begin(url);
  int httpCode = http.GET();

  if (httpCode <= 0) {
    Serial.printf("Error: Failed to connect, HTTP error: %s\n",
                  http.errorToString(httpCode).c_str());
    http.end(); // Free resources
    return false;
  }

  if (httpCode != HTTP_CODE_OK) {
    Serial.printf("Error: Unexpected HTTP response: %d\n", httpCode);
    http.end(); // Free resources
    return false;
  }

  // Open file for writing
  File file = SD.open(path, FILE_WRITE);
  if (!file) {
    printf("Error: Failed to open file for writing");
    http.end(); // Free resources
    return false;
  }

  // Stream the file contents from the web and write them to the SD card
  WiFiClient* stream = http.getStreamPtr();
  uint8_t buffer[1024] = { 0 };
  int bytesRead;
  while (http.connected() &&
         (bytesRead = stream->readBytes(buffer, sizeof(buffer))) > 0) {
    file.write(buffer, bytesRead);
  }

  file.close();
  http.end(); // Free resources
  printf("Success: File downloaded and saved to SD card");
  return true;
}
