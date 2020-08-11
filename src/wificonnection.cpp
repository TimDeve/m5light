#ifdef TARGET_STACK
#include <M5Stack.h>
#endif
#ifdef TARGET_STICK
#include <M5StickC.h>
#endif

#include <WiFi.h>

#include "lifx.h"
#include "ui.h"
#include "wificonnection.h"

void connectToWiFi(const char *ssid, const char *pwd)
{
    #ifndef TARGET_ATOM
    M5.Lcd.print("Connecting to ");
    M5.Lcd.println(String(ssid) + "\n");
    #endif

    // Delete old config
    WiFi.disconnect(true);

    WiFi.onEvent(wifiEventHandler);

    WiFi.begin(ssid, pwd);

    #ifndef TARGET_ATOM
    M5.Lcd.println("Waiting for connection...");
    #endif
}

void wifiEventHandler(WiFiEvent_t event)
{
    switch (event)
    {
    case SYSTEM_EVENT_STA_GOT_IP:
        setupUdp();

        uiInit();

        break;
    default:
        // Other events; Do nothing
        break;
    }
}
