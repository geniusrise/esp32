#include "speaker.h"

int bassTab[7] = {1911, 1702, 1516, 1431, 1275, 1136, 1012};

SpeakerManager::SpeakerManager(int speakerPin) : speakerPin(speakerPin)
{
    pinMode(speakerPin, OUTPUT);
    digitalWrite(speakerPin, LOW);
}

void SpeakerManager::begin()
{
    pinMode(speakerPin, OUTPUT);
    digitalWrite(speakerPin, LOW); // Ensure the speaker starts off
}

void SpeakerManager::playSound(uint8_t noteIndex)
{
    if (noteIndex < sizeof(bassTab) / sizeof(bassTab[0]))
    { // Check if noteIndex is within the bassTab range
        for (int i = 0; i < 100; i++)
        {
            digitalWrite(speakerPin, HIGH);
            delayMicroseconds(bassTab[noteIndex]);
            digitalWrite(speakerPin, LOW);
            delayMicroseconds(bassTab[noteIndex]);
        }
    }
}

void SpeakerManager::stopSound()
{
    digitalWrite(speakerPin, LOW); // Immediately stop any ongoing sound
}
