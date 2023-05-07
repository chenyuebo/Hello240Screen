package cn.yuebo.hello240.utils;

import android.graphics.Bitmap;
import android.media.Image;

import androidx.annotation.NonNull;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.nio.ByteBuffer;

/**
 * @author yuebo
 * @date 2022/7/9
 */
public class ImageUtil {

    public static Bitmap convertBitmap(@NonNull Image image) {
        long startTime = System.currentTimeMillis();
        int format = image.getFormat();
        int width = image.getWidth();
        int height = image.getHeight();

        Image.Plane[] planes = image.getPlanes();
        // pixelStride：像素步长，有可能是1、有可能是2。它代表的是行内连续两个颜色值之间的距离(步长)。
        int pixelStride = planes[0].getPixelStride();
        // rowStride：“每行数据”的“宽度”，这个跟分辨率的宽度不是同个回事，它是每一行实际存储的宽度
        int rowStride = planes[0].getRowStride();

        Bitmap bitmap = Bitmap.createBitmap(rowStride / pixelStride, height, Bitmap.Config.ARGB_8888);
        ByteBuffer buffer = planes[0].getBuffer();
        bitmap.copyPixelsFromBuffer(buffer);
//        Log.d("bobo", String.format("convertBitmap()耗时=%d(ms) width=%d ,height=%d", System.currentTimeMillis() - startTime, width, height));
        return bitmap;
    }

    /**
     * 图片转换为jpg字节流
     *
     * @param bitmap
     * @param quality
     * @return
     */
    public static byte[] convertJPEG(Bitmap bitmap, int quality) {
        ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
        bitmap.compress(Bitmap.CompressFormat.JPEG, quality, outputStream);
        return outputStream.toByteArray();
    }

    public static void saveImage(Bitmap bitmap, File file) {
        FileOutputStream outputStream = null;
        try {
            outputStream = new FileOutputStream(file);
            bitmap.compress(Bitmap.CompressFormat.JPEG, 100, outputStream);
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            StreamUtil.close(outputStream);
        }
    }
}
