#include <Arduino.h>
#include "speaker.h"
#include "driver/gpio.h"
#include "util.h"


int SPEAKER = 18;
int count = 0;

SpeakerManager speakerManager = SpeakerManager(SPEAKER);

void setup()
{
    // Initialize the state machine, which will also initialize all other managers
    print_logo();
}

void loop()
{

    color_printf("------ ------ ------ ------ ------ ------ ------ ------ ------\n");
    color_printf("Main: Start loop function\n");
    color_printf("------ ------ ------ ------ ------ ------ ------ ------ ------\n");

    speakerManager.playSound(count);
    count += 1;
    if (count > 7) {
        count = 0;
    }
    delay(500);
}
