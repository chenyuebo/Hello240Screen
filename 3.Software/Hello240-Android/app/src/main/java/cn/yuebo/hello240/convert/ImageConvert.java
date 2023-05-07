package cn.yuebo.hello240.convert;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;

import cn.yuebo.hello240.utils.ImageUtil;

/**
 * @author yuebo
 * @date 2023/3/17
 */
public class ImageConvert {

    public int width = 240;
    public int height = 240;
    public int scale = 2;
    public int left;
    public int top;
    public Rect src;
    public Rect dst;
    public int quality = 80;
    private Paint paint;

    public ImageConvert() {
        paint = new Paint(Paint.ANTI_ALIAS_FLAG);
        paint.setFilterBitmap(true);
        paint.setDither(true);

        src = new Rect(left, top, left + width * scale, top + height * scale);
        dst = new Rect(0, 0, width, height);
    }

    public byte[] convert(Bitmap bitmap) {
        Bitmap bitmap1 = transformBitmap(bitmap);
        byte[] bytes = ImageUtil.convertJPEG(bitmap1, quality);
        return bytes;
    }

    private Bitmap transformBitmap(Bitmap bitmap) {
        Bitmap b = Bitmap.createBitmap(width, height, Bitmap.Config.RGB_565);
        Canvas canvas = new Canvas();
        canvas.setBitmap(b);
        src.set(left, top, left + width * scale, top + height * scale);
        canvas.drawBitmap(bitmap, src, dst, paint);
        return b;
    }
}
