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
};

#endif // SPEAKER_MANAGER_HPP
