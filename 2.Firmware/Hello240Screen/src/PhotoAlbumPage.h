#ifndef PhotoAlbumPage_H
#define PhotoAlbumPage_H

#include "Page.h"

class PhotoAlbumPage : public Page
{
private:
    int lastTime = 0;
    int photoIndex = 0;

public:
    PhotoAlbumPage(TFT_eSPI &tft) : Page(tft) {}
    void onStart();
    void onLoop();
    void onStop();
};

#endif