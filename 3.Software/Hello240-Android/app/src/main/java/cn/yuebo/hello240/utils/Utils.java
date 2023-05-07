package cn.yuebo.hello240.utils;

import android.content.Context;
import android.graphics.Point;
import android.view.WindowManager;

/**
 * @author yuebo
 * @date 2023/3/16
 */
public class Utils {

    public static int getScreenWidth(Context context) {
        WindowManager windowManager = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
        Point size = new Point();
        windowManager.getDefaultDisplay().getRealSize(size);
        return size.x;
    }

    /**
     * 获取屏幕真实高度，包含系统状态栏
     *
     * @param context
     * @return
     */
    public static int getScreenHeight(Context context) {
        WindowManager windowManager = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
        Point size = new Point();
        windowManager.getDefaultDisplay().getRealSize(size);
        return size.y;
    }

    public static int parseInt(String s, int fallback) {
        try {
            return Integer.parseInt(s);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return fallback;
    }
}
