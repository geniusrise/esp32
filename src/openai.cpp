#include "openai.h"

OpenAI::OpenAI(const char* apiKey)
  : m_apiKey(apiKey)
{
}

String
OpenAI::sendChatCompletion(const String& messages,
                           const char* model,
                           float temperature,
                           int maxTokens,
                           float topP,
                           float frequencyPenalty,
                           float presencePenalty,
                           int n,
                           const char* stop,
                           bool stream)
{
  String endpoint = "/v1/chat/completions";

  DynamicJsonDocument requestDoc(4096);
  requestDoc["model"] = model;

  // Parse messages JSON string
  DynamicJsonDocument messagesDoc(2048);
  deserializeJson(messagesDoc, messages);
  requestDoc["messages"] = messagesDoc;

  requestDoc["temperature"] = temperature;
  if (maxTokens > 0) {
    requestDoc["max_tokens"] = maxTokens;
  }
  requestDoc["top_p"] = topP;
  requestDoc["frequency_penalty"] = frequencyPenalty;
  requestDoc["presence_penalty"] = presencePenalty;
  requestDoc["n"] = n;
  if (stop != nullptr) {
    requestDoc["stop"] = stop;
  }
  requestDoc["stream"] = stream;

  String requestBody;
  serializeJson(requestDoc, requestBody);

  return sendRequest(endpoint.c_str(), requestBody);
}

String
OpenAI::createSpeech(const char* model,
                     const char* input,
                     const char* voice,
                     const char* responseFormat,
                     float speed)
{
  String endpoint = "/v1/audio/speech";

  DynamicJsonDocument requestDoc(1024);
  requestDoc["model"] = model;
  requestDoc["input"] = input;
  requestDoc["voice"] = voice;
  requestDoc["response_format"] = responseFormat;
  requestDoc["speed"] = speed;

  String requestBody;
  serializeJson(requestDoc, requestBody);

  return sendRequest(endpoint.c_str(), requestBody);
}

String
OpenAI::createTranscription(const char* fileName,
                            const char* model,
                            const char* language,
                            const char* prompt,
                            const char* responseFormat,
                            float temperature,
                            const String& timestampGranularities)
{
  String endpoint = "/v1/audio/transcriptions";

  DynamicJsonDocument requestDoc(1024);
  requestDoc["model"] = model;
  if (language != nullptr) {
    requestDoc["language"] = language;
  }
  if (prompt != nullptr) {
    requestDoc["prompt"] = prompt;
  }
  requestDoc["response_format"] = responseFormat;
  requestDoc["temperature"] = temperature;
  requestDoc["timestamp_granularities"] = timestampGranularities;

  String requestBody;
  serializeJson(requestDoc, requestBody);

  return sendAudioRequest(endpoint.c_str(), fileName, requestBody);
}

String
OpenAI::createTranslation(const char* fileName,
                          const char* model,
                          const char* prompt,
                          const char* responseFormat,
                          float temperature)
{
  String endpoint = "/v1/audio/translations";

  DynamicJsonDocument requestDoc(1024);
  requestDoc["model"] = model;
  if (prompt != nullptr) {
    requestDoc["prompt"] = prompt;
  }
  requestDoc["response_format"] = responseFormat;
  requestDoc["temperature"] = temperature;

  String requestBody;
  serializeJson(requestDoc, requestBody);

  return sendAudioRequest(endpoint.c_str(), fileName, requestBody);
}

String
OpenAI::sendRequest(const char* endpoint, const String& requestBody)
{
  HTTPClient http;
  http.begin("https://api.openai.com" + String(endpoint));
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", "Bearer " + String(m_apiKey));

  int httpResponseCode = http.POST(requestBody);
  String response = "";

  if (httpResponseCode == HTTP_CODE_OK) {
    response = http.getString();
  } else {
    Serial.printf("Error: %d\n", httpResponseCode);
  }

  http.end();
  return response;
}

String
OpenAI::sendAudioRequest(const char* endpoint,
                         const char* fileName,
                         const String& requestBody)
{
  HTTPClient http;
  http.begin("https://api.openai.com" + String(endpoint));
  http.addHeader("Authorization", "Bearer " + String(m_apiKey));

  File file = SD.open(fileName, FILE_READ);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return "";
  }

  String boundary = "------------------------" + String(random(0xffff), HEX);
  String contentType = "multipart/form-data; boundary=" + boundary;
  String body = "--" + boundary + "\r\n";
  body += "Content-Disposition: form-data; name=\"file\"; filename=\"" +
          String(fileName) + "\"\r\n";
  body += "Content-Type: application/octet-stream\r\n\r\n";

  http.addHeader("Content-Type", contentType.c_str());

  int httpResponseCode =
    http.POST((uint8_t*)body.c_str(),
              file.size() + requestBody.length() + boundary.length() + 8);

  String response = "";

  if (httpResponseCode == HTTP_CODE_OK) {
    response = http.getString();
  } else {
    Serial.printf("Error: %d\n", httpResponseCode);
  }

  http.end();
  file.close();
  return response;
}