package cn.yuebo.hello240.utils;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.provider.Settings;

/**
 * @author yuebo
 * @date 2023/4/23
 */
public class PermissionUtils {

    /**
     * 是否有悬浮窗权限
     *
     * @param context
     * @return
     */
    public static boolean canDrawOverlays(Context context) {
        boolean canDraw = false;
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            return Settings.canDrawOverlays(context);
        }
        return canDraw;
    }

    /**
     * 打开悬浮窗权限设置页
     *
     * @param context
     */
    public static void requestOverlaysPermission(Activity context, int requestCode) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            Intent intent = new Intent();
            intent.setAction(Settings.ACTION_MANAGE_OVERLAY_PERMISSION);
            intent.setData(Uri.parse("package:" + context.getPackageName()));
            context.startActivityForResult(intent, requestCode);
        } else {
            Intent intent = new Intent();
            intent.setAction(Settings.ACTION_APPLICATION_DETAILS_SETTINGS);
            intent.setData(Uri.parse("package:" + context.getPackageName()));
            context.startActivityForResult(intent, requestCode);
        }
    }
}
