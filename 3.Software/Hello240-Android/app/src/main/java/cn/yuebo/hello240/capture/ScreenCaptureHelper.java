package cn.yuebo.hello240.capture;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.media.projection.MediaProjectionManager;
import android.os.Build;
import android.util.Log;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

/**
 * @author yuebo
 * @date 2023/3/16
 */
public class ScreenCaptureHelper {

    private static final int REQUEST_CODE = 100;
    private Activity activity;

    public ScreenCaptureHelper(@NonNull Activity activity) {
        this.activity = activity;
    }

    public void startCapture() {
        MediaProjectionManager mMediaProjectionManager = (MediaProjectionManager) activity.getSystemService(Context.MEDIA_PROJECTION_SERVICE);
        Intent intent = mMediaProjectionManager.createScreenCaptureIntent();
        activity.startActivityForResult(intent, REQUEST_CODE);
    }

    public void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        if (requestCode == REQUEST_CODE) {
            if (resultCode == Activity.RESULT_OK) {
                // target29开启录屏必须开启前台服务
                // Caused by: java.lang.SecurityException: Media projections require a foreground service of type ServiceInfo.FOREGROUND_SERVICE_TYPE_MEDIA_PROJECTION
                Intent intent = new Intent(activity, ScreenCaptureService.class);
                intent.putExtra("resultCode", resultCode);
                intent.putExtra("resultData", data);
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                    activity.startForegroundService(intent);
                } else {
                    activity.startService(intent);
                }
            } else {
                Log.d("bobo", "用户取消");
            }
        }
    }

    public void stopCapture() {
        activity.stopService(new Intent(activity, ScreenCaptureService.class));
    }
}
