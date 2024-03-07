#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "config.h"
#include "wifi.h"
#include "server.h"
#include "audio.h"
#include "flash.h"

class StateMachine
{
public:
    StateMachine();
    void changeState();
    void currentStateAction();
    // More methods for handling states
private:
    ConfigManager configManager;
    WiFiManager wifiManager;
    ServerManager serverManager;
    AudioManager audioManager;
    FlashManager flashManager;
    // Current state variable
};

#endif // STATE_MACHINE_H
