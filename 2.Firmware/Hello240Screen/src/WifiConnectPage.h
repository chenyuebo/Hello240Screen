#ifndef WifiConnectPage_h
#define WifiConnectPage_h

#include "Page.h"

class WifiConnectPage : public Page
{
public:
    WifiConnectPage(TFT_eSPI &tft) : Page(tft) {}
    void onStart();
    void onLoop();
};

#endif