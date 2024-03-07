#ifndef SPEAKER_MANAGER_HPP
#define SPEAKER_MANAGER_HPP

#include <Arduino.h>

class SpeakerManager
{
public:
    SpeakerManager(int speakerPin);
    void begin();
    void playSound(uint8_t noteIndex);
    void stopSound();

private:
    int speakerPin;
    static constexpr int bassTab[7] = {1911, 1702, 1516, 1431, 1275, 1136, 1012}; // Bass notes 1~7
};

#endif // SPEAKER_MANAGER_HPP
