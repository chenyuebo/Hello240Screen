#include "Page.h"

Page::Page(TFT_eSPI& tft)
{
    this->_tft = &tft;
}