#include "speaker.h"

SpeakerManager::SpeakerManager(int speakerPin) : speakerPin(speakerPin)
{
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
