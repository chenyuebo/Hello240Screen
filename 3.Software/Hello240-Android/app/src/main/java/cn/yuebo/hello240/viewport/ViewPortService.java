package cn.yuebo.hello240.viewport;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.graphics.PixelFormat;
import android.os.Build;
import android.os.IBinder;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;

import androidx.annotation.Nullable;

import cn.yuebo.hello240.R;
import cn.yuebo.hello240.utils.Utils;

/**
 * 投屏区域预览悬浮窗
 *
 * @author yuebo
 * @date 2023/3/22
 */
public class ViewPortService extends Service {

    WindowManager windowManager;
    WindowManager.LayoutParams layoutParams;
    View view;

    private int screenWidth;
    private int screenHeight;
    private int viewPortWidth;
    private int viewPortHeight;

    public static ViewPortChangeListener viewPortChangeListener;

    public static void setViewPortChangeListener(ViewPortChangeListener viewPortChangeListener) {
        ViewPortService.viewPortChangeListener = viewPortChangeListener;
    }

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        Log.d("bobo", "ViewPortService.onCreate()");
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        screenWidth = Utils.getScreenWidth(this);
        screenHeight = Utils.getScreenHeight(this);
        viewPortWidth = intent.getIntExtra("width", 200);
        viewPortHeight = intent.getIntExtra("height", 200);
        if (windowManager != null && view != null) {
            layoutParams.width = viewPortWidth;
            layoutParams.height = viewPortHeight;
            windowManager.updateViewLayout(view, layoutParams);
        } else {
            // 添加悬浮窗View
            addView();
            //
            view.setOnTouchListener(new View.OnTouchListener() {

                float x, y;
                int lpx, lpy;

                @Override
                public boolean onTouch(View v, MotionEvent event) {
                    int action = event.getAction();
                    if (action == MotionEvent.ACTION_DOWN) {
                        x = event.getRawX();
                        y = event.getRawY();
                        lpx = layoutParams.x;
                        lpy = layoutParams.y;
                    } else if (action == MotionEvent.ACTION_MOVE) {
                        int moveX = (int) (event.getRawX() - x);
                        int moveY = (int) (event.getRawY() - y);
                        layoutParams.x = lpx + moveX;
                        layoutParams.y = lpy + moveY;
                        if (layoutParams.x < 0) {
                            layoutParams.x = 0;
                        } else if (layoutParams.x + viewPortWidth > screenWidth) {
                            layoutParams.x = screenWidth - viewPortWidth;
                        }
                        if (layoutParams.y < 0) {
                            layoutParams.y = 0;
                        } else if (layoutParams.y + viewPortHeight > screenHeight) {
                            layoutParams.y = screenHeight - viewPortHeight;
                        }
                        windowManager.updateViewLayout(view, layoutParams);
                        notifyViewPortChange();
                    } else if (action == MotionEvent.ACTION_UP || action == MotionEvent.ACTION_CANCEL) {
                    }
                    return false;
                }
            });
        }
        return super.onStartCommand(intent, flags, startId);
    }

    private void notifyViewPortChange() {
        if (viewPortChangeListener != null) {
            int left = layoutParams.x;
            int top = layoutParams.y;
//            Log.d("bobo", "onViewPortMove() left=" + left + " ,top=" + top + " ,viewPortWidth=" + viewPortWidth + " ,viewPortHeight=" + viewPortHeight + " ,screenWidth=" + screenWidth);
            viewPortChangeListener.onViewPortMove(left, top);
        }
    }

    private void addView() {
        windowManager = (WindowManager) getSystemService(Context.WINDOW_SERVICE);
        layoutParams = new WindowManager.LayoutParams();
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            layoutParams.type = WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY;
        } else {
            layoutParams.type = WindowManager.LayoutParams.TYPE_PHONE;
        }
        // 防止悬浮窗出现黑色背景
        layoutParams.format = PixelFormat.TRANSPARENT;
        // 悬浮窗响应onTouch并且不影响系统点击和返回等操作
        layoutParams.flags = WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL | WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE | WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL | WindowManager.LayoutParams.FLAG_LAYOUT_INSET_DECOR // 可以显示到系统状态栏
                | WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN;
        layoutParams.gravity = Gravity.TOP | Gravity.LEFT;
        view = LayoutInflater.from(this).inflate(R.layout.layout_view_port, null);
        int measureSpec = View.MeasureSpec.makeMeasureSpec(Integer.MAX_VALUE >> 2, View.MeasureSpec.UNSPECIFIED);
        view.measure(measureSpec, measureSpec);
        layoutParams.width = viewPortWidth;
        layoutParams.height = viewPortHeight;
        windowManager.addView(view, layoutParams);
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        Log.d("bobo", "ViewPortService.onDestroy()");
        if (view != null) {
            windowManager.removeView(view);
        }
    }

    public interface ViewPortChangeListener {
        void onViewPortMove(int left, int top);
    }
}
