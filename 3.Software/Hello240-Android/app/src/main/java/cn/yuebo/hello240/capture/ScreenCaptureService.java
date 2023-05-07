package cn.yuebo.hello240.capture;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.PixelFormat;
import android.hardware.display.DisplayManager;
import android.hardware.display.VirtualDisplay;
import android.media.Image;
import android.media.ImageReader;
import android.media.projection.MediaProjection;
import android.media.projection.MediaProjectionManager;
import android.os.Build;
import android.os.IBinder;
import android.util.Log;

import cn.yuebo.hello240.MainActivity;
import cn.yuebo.hello240.R;
import cn.yuebo.hello240.utils.ImageUtil;
import cn.yuebo.hello240.utils.Utils;

/**
 * 截屏service
 *
 * @author cyb
 */
public class ScreenCaptureService extends Service {

    private static Callback callback;

    public static void setCallback(Callback callback) {
        ScreenCaptureService.callback = callback;
    }

    private MediaProjectionManager mMediaProjectionManager;
    private ImageReader mImageReader;
    private VirtualDisplay mVirtualDisplay;

    @Override
    public void onCreate() {
        Log.d("bobo", "ScreenCaptureService.onCreate()");
        super.onCreate();
        mMediaProjectionManager = (MediaProjectionManager) getSystemService(MEDIA_PROJECTION_SERVICE);
        createNotificationChannel();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.d("bobo", "ScreenCaptureService.onStartCommand()");
        int resultCode = intent.getIntExtra("resultCode", Activity.RESULT_OK);
        Intent resultData = intent.getParcelableExtra("resultData");
        startImageCapture(resultCode, resultData);
        if (callback != null) {
            callback.onStart();
        }
        return START_NOT_STICKY;
    }

    @SuppressLint("WrongConstant")
    private void startImageCapture(int resultCode, Intent resultData) {
        MediaProjection mediaProjection = mMediaProjectionManager.getMediaProjection(resultCode, resultData);
        int width = Utils.getScreenWidth(this);
        int height = Utils.getScreenHeight(this);
        mImageReader = ImageReader.newInstance(width, height, PixelFormat.RGBA_8888, 2);
        mImageReader.setOnImageAvailableListener(new ImageReader.OnImageAvailableListener() {
            @Override
            public void onImageAvailable(ImageReader reader) {
                Image image = reader.acquireLatestImage();
                if (image != null) {
                    Bitmap bitmap = ImageUtil.convertBitmap(image);
                    image.close();
                    if (callback != null) {
                        callback.onImageAvailable(bitmap);
                    }
                }
            }
        }, null);
        mVirtualDisplay = mediaProjection.createVirtualDisplay("screenCapture"
                , width, height, 1
                , DisplayManager.VIRTUAL_DISPLAY_FLAG_AUTO_MIRROR
                , mImageReader.getSurface(), null, null);
    }

    private void createNotificationChannel() {
        Notification.Builder builder = new Notification.Builder(this.getApplicationContext());
        Intent nfIntent = new Intent(this, MainActivity.class);

        builder.setContentIntent(PendingIntent.getActivity(this, 0, nfIntent, PendingIntent.FLAG_IMMUTABLE));
        builder.setLargeIcon(BitmapFactory.decodeResource(this.getResources(), R.drawable.ic_launcher)); // 设置下拉列表中的图标(大图标)
        builder.setSmallIcon(R.drawable.ic_launcher); // 设置状态栏内的小图标
        builder.setContentText("小电视投屏运行中..."); // 设置上下文内容
        builder.setWhen(System.currentTimeMillis()); // 设置该通知发生的时间

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            builder.setChannelId("notification_id");
            // 前台服务notification适配
            NotificationManager notificationManager = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);
            NotificationChannel channel = new NotificationChannel("notification_id", "notification_name", NotificationManager.IMPORTANCE_LOW);
            notificationManager.createNotificationChannel(channel);
        }

        Notification notification = builder.build();
        notification.defaults = Notification.DEFAULT_SOUND; // 设置为默认通知音
        startForeground(110, notification);
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onDestroy() {
        Log.d("bobo", "ScreenCaptureService.onDestroy()");
        try {
            mImageReader.close();
            mVirtualDisplay.release();
        } catch (Exception e) {
            e.printStackTrace();
        }
        if (callback != null) {
            callback.onStop();
        }
        super.onDestroy();
    }

    public interface Callback {
        void onStart();

        void onImageAvailable(Bitmap bitmap);

        void onStop();
    }
}
