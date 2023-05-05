#ifndef Page_h
#define Page_h

#include "Arduino.h"
#include <TFT_eSPI.h>

class Page
{
    
protected:
    TFT_eSPI* _tft;

public:
    // ----- Constructor -----
    
    Page(TFT_eSPI& tft);

    /**
     * 显示
    */
    virtual void onStart() {}
    /**
     * 事件循环
     */
    virtual void onLoop() {}
    /**
     * 退出显示
     */
    virtual void onStop() {}
};

#endif