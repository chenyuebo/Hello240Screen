#include "weathernum.h"

#include <TJpg_Decoder.h>

#include "weather/t0.h"
#include "weather/t1.h"
#include "weather/t2.h"
#include "weather/t3.h"
#include "weather/t4.h"
#include "weather/t5.h"
#include "weather/t6.h"
#include "weather/t7.h"
#include "weather/t9.h"
#include "weather/t11.h"
#include "weather/t13.h"
#include "weather/t14.h"
#include "weather/t15.h"
#include "weather/t16.h"
#include "weather/t18.h"
#include "weather/t19.h"
#include "weather/t20.h"
#include "weather/t26.h"
#include "weather/t29.h"
#include "weather/t30.h"
#include "weather/t31.h"
#include "weather/t53.h"
#include "weather/t99.h"


// 显示天气图标
void WeatherNum::printfweather(int numx, int numy, int numw)
{
  if (numw == 00)
  {
    TJpgDec.drawJpg(numx, numy, imgt0, sizeof(imgt0));
  }
  else if (numw == 01)
  {
    TJpgDec.drawJpg(numx, numy, imgt1, sizeof(imgt1));
  }
  else if (numw == 02)
  {
    TJpgDec.drawJpg(numx, numy, imgt2, sizeof(imgt2));
  }
  else if (numw == 03)
  {
    TJpgDec.drawJpg(numx, numy, imgt3, sizeof(imgt3));
  }
  else if (numw == 04)
  {
    TJpgDec.drawJpg(numx, numy, imgt4, sizeof(imgt4));
  }
  else if (numw == 05)
  {
    TJpgDec.drawJpg(numx, numy, imgt5, sizeof(imgt5));
  }
  else if (numw == 06)
  {
    TJpgDec.drawJpg(numx, numy, imgt6, sizeof(imgt6));
  }
  else if (numw == 07 || numw == 8 || numw == 21 || numw == 22)
  {
    TJpgDec.drawJpg(numx, numy, imgt7, sizeof(imgt7));
  }
  else if (numw == 9 || numw == 10 || numw == 23 || numw == 24)
  {
    TJpgDec.drawJpg(numx, numy, imgt9, sizeof(imgt9));
  }
  else if (numw == 11 || numw == 12 || numw == 25 || numw == 301)
  {
    TJpgDec.drawJpg(numx, numy, imgt11, sizeof(imgt11));
  }
  else if (numw == 13)
  {
    TJpgDec.drawJpg(numx, numy, imgt13, sizeof(imgt13));
  }
  else if (numw == 14 || numw == 26)
  {
    TJpgDec.drawJpg(numx, numy, imgt14, sizeof(imgt14));
  }
  else if (numw == 15 || numw == 27)
  {
    TJpgDec.drawJpg(numx, numy, imgt15, sizeof(imgt15));
  }
  else if (numw == 16 || numw == 17 || numw == 28 || numw == 302)
  {
    TJpgDec.drawJpg(numx, numy, imgt16, sizeof(imgt16));
  }
  else if (numw == 18)
  {
    TJpgDec.drawJpg(numx, numy, imgt18, sizeof(imgt18));
  }
  else if (numw == 19)
  {
    TJpgDec.drawJpg(numx, numy, imgt19, sizeof(imgt19));
  }
  else if (numw == 20)
  {
    TJpgDec.drawJpg(numx, numy, imgt20, sizeof(imgt20));
  }
  else if (numw == 29)
  {
    TJpgDec.drawJpg(numx, numy, imgt29, sizeof(imgt29));
  }
  else if (numw == 30)
  {
    TJpgDec.drawJpg(numx, numy, imgt30, sizeof(imgt30));
  }
  else if (numw == 31)
  {
    TJpgDec.drawJpg(numx, numy, imgt31, sizeof(imgt31));
  }
  else if (numw == 53 || numw == 32 || numw == 49 || numw == 54 || numw == 55 || numw == 56 || numw == 57 || numw == 58)
  {
    TJpgDec.drawJpg(numx, numy, imgt53, sizeof(imgt53));
  }
  else
  {
    TJpgDec.drawJpg(numx, numy, imgt99, sizeof(imgt99));
  }
}
