#ifndef MIC_MANAGER_H
#define MIC_MANAGER_H

#include <Arduino.h>
#include <FS.h>
#include <HTTPClient.h>
#include <SD.h>
#include <WiFiClient.h>
#include <driver/i2s.h>

void
print_logo();

void
color_printf(const char* format, ...);

bool
downloadFileToSD(const String& url, const String& path);

bool
uploadFileFromSD(WiFiClient& client,
                 const String& url,
                 const String& path,
                 const String& fieldName,
                 const String& contentType);

#endif // MIC_MANAGER_H
