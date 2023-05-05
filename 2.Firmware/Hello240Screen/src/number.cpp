#include "number.h"

#include <TJpg_Decoder.h>

//显示白色36*60大小数字
void Number::printfW3660(int numx,int numy,int numn)
{
  switch(numn)
  {
    case 0:
      TJpgDec.drawJpg(numx,numy,imgW_3660_i0, sizeof(imgW_3660_i0));
      break;
    case 1:
      TJpgDec.drawJpg(numx,numy,imgW_3660_i1, sizeof(imgW_3660_i1));
      break;
    case 2:
      TJpgDec.drawJpg(numx,numy,imgW_3660_i2, sizeof(imgW_3660_i2));
      break;
    case 3:
      TJpgDec.drawJpg(numx,numy,imgW_3660_i3, sizeof(imgW_3660_i3));
      break;
    case 4:
      TJpgDec.drawJpg(numx,numy,imgW_3660_i4, sizeof(imgW_3660_i4));
      break;
    case 5:
      TJpgDec.drawJpg(numx,numy,imgW_3660_i5, sizeof(imgW_3660_i5));
      break;
    case 6:
      TJpgDec.drawJpg(numx,numy,imgW_3660_i6, sizeof(imgW_3660_i6));
      break;
    case 7:
      TJpgDec.drawJpg(numx,numy,imgW_3660_i7, sizeof(imgW_3660_i7));
      break;
    case 8:
      TJpgDec.drawJpg(numx,numy,imgW_3660_i8, sizeof(imgW_3660_i8));
      break;
    case 9:
      TJpgDec.drawJpg(numx,numy,imgW_3660_i9, sizeof(imgW_3660_i9));
      break;
    default:
      Serial.println("显示imgW_3660数字错误");
      break;
  }
}
//显示橙色36*60大小数字
void Number::printfO3660(int numx,int numy,int numn)
{
  switch(numn)
  {
    case 0:
      TJpgDec.drawJpg(numx,numy,imgO_3660_i0, sizeof(imgO_3660_i0));
      break;
    case 1:
      TJpgDec.drawJpg(numx,numy,imgO_3660_i1, sizeof(imgO_3660_i1));
      break;
    case 2:
      TJpgDec.drawJpg(numx,numy,imgO_3660_i2, sizeof(imgO_3660_i2));
      break;
    case 3:
      TJpgDec.drawJpg(numx,numy,imgO_3660_i3, sizeof(imgO_3660_i3));
      break;
    case 4:
      TJpgDec.drawJpg(numx,numy,imgO_3660_i4, sizeof(imgO_3660_i4));
      break;
    case 5:
      TJpgDec.drawJpg(numx,numy,imgO_3660_i5, sizeof(imgO_3660_i5));
      break;
    case 6:
      TJpgDec.drawJpg(numx,numy,imgO_3660_i6, sizeof(imgO_3660_i6));
      break;
    case 7:
      TJpgDec.drawJpg(numx,numy,imgO_3660_i7, sizeof(imgO_3660_i7));
      break;
    case 8:
      TJpgDec.drawJpg(numx,numy,imgO_3660_i8, sizeof(imgO_3660_i8));
      break;
    case 9:
      TJpgDec.drawJpg(numx,numy,imgO_3660_i9, sizeof(imgO_3660_i9));
      break;
    default:
      Serial.println("显示imgO_3660数字错误");
      break;
  }
}
//显示白色18*30大小数字
void Number::printfW1830(int numx,int numy,int numn)
{
  switch(numn)
  {
    case 0:
      TJpgDec.drawJpg(numx,numy,imgW_1830_i0, sizeof(imgW_1830_i0));
      break;
    case 1:
      TJpgDec.drawJpg(numx,numy,imgW_1830_i1, sizeof(imgW_1830_i1));
      break;
    case 2:
      TJpgDec.drawJpg(numx,numy,imgW_1830_i2, sizeof(imgW_1830_i2));
      break;
    case 3:
      TJpgDec.drawJpg(numx,numy,imgW_1830_i3, sizeof(imgW_1830_i3));
      break;
    case 4:
      TJpgDec.drawJpg(numx,numy,imgW_1830_i4, sizeof(imgW_1830_i4));
      break;
    case 5:
      TJpgDec.drawJpg(numx,numy,imgW_1830_i5, sizeof(imgW_1830_i5));
      break;
    case 6:
      TJpgDec.drawJpg(numx,numy,imgW_1830_i6, sizeof(imgW_1830_i6));
      break;
    case 7:
      TJpgDec.drawJpg(numx,numy,imgW_1830_i7, sizeof(imgW_1830_i7));
      break;
    case 8:
      TJpgDec.drawJpg(numx,numy,imgW_1830_i8, sizeof(imgW_1830_i8));
      break;
    case 9:
      TJpgDec.drawJpg(numx,numy,imgW_1830_i9, sizeof(imgW_1830_i9));
      break;
    default:
      Serial.println("显示imgO_1830数字错误");
      break;
  }
}
