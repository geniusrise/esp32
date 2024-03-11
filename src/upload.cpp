#include "util.h"

/**
 * Uploads a file from the SD card to a specified server location using a
 * multipart/form-data POST request. Assumes the WiFi connection has been
 * established outside this function.
 *
 * @param url The URL of the server endpoint to which the file will be uploaded.
 * @param path The path on the SD card of the file to upload.
 * @param fieldName The name of the form field to be used for the file.
 * @param contentType The MIME type of the file being uploaded.
 * @return True if the file was successfully uploaded, false otherwise.
 */
bool
uploadFileFromSD(WiFiClient& client,
                 const String& url,
                 const String& path,
                 const String& fieldName,
                 const String& contentType)
{
  if (!SD.begin()) {
    Serial.println("SD card initialization failed");
    return false;
  }

  File file = SD.open(path, FILE_READ);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return false;
  }

  if (!client.connect(url.c_str(), 80, 1000)) {
    Serial.println("Connection failed");
    return false;
  }

  // Start constructing the request
  String boundary = "----WebKitFormBoundary7MA4YWxkTrZu0gW";
  String head = "--" + boundary +
                "\r\nContent-Disposition: form-data; name=\"" + fieldName +
                "\"; filename=\"" + path.substring(path.lastIndexOf('/') + 1) +
                "\"\r\nContent-Type: " + contentType + "\r\n\r\n";
  String tail = "\r\n--" + boundary + "--\r\n";

  client.println("POST /upload HTTP/1.1");
  client.println("Host: example.com"); // Change this to your actual host
  client.println("Content-Type: multipart/form-data; boundary=" + boundary);
  client.print("Content-Length: ");
  client.println(file.size() + head.length() + tail.length());
  client.println();
  client.print(head);

  // Stream the file content
  uint8_t buf[512];
  size_t size;
  while ((size = file.read(buf, 512)) > 0) {
    client.write(buf, size);
  }

  client.print(tail);

  // Read the response (optional)
  while (client.connected()) {
    if (client.available()) {
      String line = client.readStringUntil('\n');
      Serial.println(line);
    }
  }

  file.close();
}
