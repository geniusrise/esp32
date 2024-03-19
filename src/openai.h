#ifndef OPENAI_H
#define OPENAI_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <SD.h>

class OpenAI
{
public:
  OpenAI(const char* apiKey);
  String sendChatCompletion(const String& messages,
                            const char* model = "gpt-3.5-turbo",
                            float temperature = 1.0,
                            int maxTokens = 0,
                            float topP = 1.0,
                            float frequencyPenalty = 0.0,
                            float presencePenalty = 0.0,
                            int n = 1,
                            const char* stop = nullptr,
                            bool stream = false);
  String createSpeech(const char* model,
                      const char* input,
                      const char* voice,
                      const char* responseFormat = "mp3",
                      float speed = 1.0);
  String createTranscription(const char* fileName,
                             const char* model,
                             const char* language = nullptr,
                             const char* prompt = nullptr,
                             const char* responseFormat = "json",
                             float temperature = 0.0,
                             const String& timestampGranularities = "segment");
  String createTranslation(const char* fileName,
                           const char* model,
                           const char* prompt = nullptr,
                           const char* responseFormat = "json",
                           float temperature = 0.0);

private:
  const char* m_apiKey;
  String sendRequest(const char* endpoint, const String& requestBody);
  String sendAudioRequest(const char* endpoint,
                          const char* fileName,
                          const String& requestBody);
};

#endif // OPENAI_H