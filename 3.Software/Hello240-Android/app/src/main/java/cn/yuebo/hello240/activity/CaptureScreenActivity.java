package cn.yuebo.hello240.activity;

import android.content.Intent;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.EditText;
import android.widget.Toast;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.AppCompatSpinner;

import java.util.ArrayList;

import cn.yuebo.hello240.R;
import cn.yuebo.hello240.adapter.SpinnerAdapter;
import cn.yuebo.hello240.adapter.SpinnerItem;
import cn.yuebo.hello240.capture.ScreenCaptureHelper;
import cn.yuebo.hello240.capture.ScreenCaptureService;
import cn.yuebo.hello240.convert.ImageConvert;
import cn.yuebo.hello240.socket.NettyClient;
import cn.yuebo.hello240.utils.PermissionUtils;
import cn.yuebo.hello240.utils.Utils;
import cn.yuebo.hello240.viewport.ViewPortService;
import io.netty.buffer.ByteBuf;
import io.netty.buffer.Unpooled;

/**
 * 录屏图传
 *
 * @author yuebo
 * @date 2022/7/8
 */
public class CaptureScreenActivity extends AppCompatActivity implements View.OnClickListener {

    private static final int REQUEST_CODE = 50;

    private EditText editIp;
    private EditText editPort;
    private EditText editQuality;
    private AppCompatSpinner sizeSpinner;
    private AppCompatSpinner scaleSpinner;
    private AppCompatSpinner modeSpinner;

    private ScreenCaptureHelper screenCaptureHelper;
    private ImageConvert imageConvert;
    private NettyClient nettyClient;
    private long lastSendTime;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_capture_screen);
        setTitle("小电视屏幕投屏");

        editIp = findViewById(R.id.editIp);
        editPort = findViewById(R.id.editPort);
        editQuality = findViewById(R.id.editQuality);
        editQuality.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {

            }

            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) {

            }

            @Override
            public void afterTextChanged(Editable s) {
                imageConvert.quality = Utils.parseInt(editQuality.getText().toString(), 80);
            }
        });
        initSizeSpinner();
        initScaleSpinner();
        initModeSpinner();

        findViewById(R.id.start).setOnClickListener(this);
        findViewById(R.id.stop).setOnClickListener(this);

        screenCaptureHelper = new ScreenCaptureHelper(this);
        imageConvert = new ImageConvert();
        nettyClient = new NettyClient();

        ScreenCaptureService.setCallback(new ScreenCaptureService.Callback() {
            @Override
            public void onStart() {

            }

            @Override
            public void onImageAvailable(Bitmap bitmap) {
                if (System.currentTimeMillis() - lastSendTime >= 180) {
                    byte[] bytes = imageConvert.convert(bitmap);
                    int length = bytes.length + 3;
                    if (length > 25000) {
                        Log.e("bobo", "onImageAvailable() length > 25000");
                        return;
                    }
                    ByteBuf byteBuf = Unpooled.buffer();
                    byteBuf.writeByte(length).writeByte(length >> 8);
                    byteBuf.writeBytes(bytes);
                    byteBuf.writeBytes(new byte[]{(byte) 0xaa, (byte) 0xbb, (byte) 0xcc});
                    nettyClient.send(byteBuf);
                    lastSendTime = System.currentTimeMillis();
                }
            }

            @Override
            public void onStop() {
            }
        });
        ViewPortService.setViewPortChangeListener(new ViewPortService.ViewPortChangeListener() {
            @Override
            public void onViewPortMove(int left, int top) {
//                Log.d("bobo", "onViewPortMove() left=" + left + " ,top=" + top);
                imageConvert.left = left;
                imageConvert.top = top;
            }
        });
    }


    @Override
    public void onClick(View v) {
        if (v.getId() == R.id.start) {
            if (!PermissionUtils.canDrawOverlays(this)) {
                Toast.makeText(this, "选择投屏区域需要打开悬浮窗权限", Toast.LENGTH_LONG).show();
                PermissionUtils.requestOverlaysPermission(this, REQUEST_CODE);
            } else {
                start();
            }
        } else if (v.getId() == R.id.stop) {
            stopService(new Intent(this, ViewPortService.class));
            screenCaptureHelper.stopCapture();
            nettyClient.disconnect();
        }
    }

    private void start() {
        String ip = editIp.getText().toString();
        int port = Utils.parseInt(editPort.getText().toString(), 8080);
        imageConvert.quality = Utils.parseInt(editQuality.getText().toString(), 80);
        nettyClient.connect(ip, port);
        screenCaptureHelper.startCapture();

        SpinnerItem scaleItem = (SpinnerItem) scaleSpinner.getSelectedItem();
        Intent service = new Intent(CaptureScreenActivity.this, ViewPortService.class);
        service.putExtra("width", imageConvert.width * scaleItem.getValue());
        service.putExtra("height", imageConvert.height * scaleItem.getValue());
        startService(service);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == REQUEST_CODE) {

        } else {
            screenCaptureHelper.onActivityResult(requestCode, resultCode, data);
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        nettyClient.disconnect();
        nettyClient.release();
    }

    private void initSizeSpinner() {
        sizeSpinner = findViewById(R.id.sizeSpinner);
        ArrayList<SpinnerItem> scaleOptions = new ArrayList<>();
        scaleOptions.add(new SpinnerItem("128x64", 1));
        scaleOptions.add(new SpinnerItem("240x240", 2));
        sizeSpinner.setAdapter(new SpinnerAdapter(this, scaleOptions));
        sizeSpinner.setSelection(1);
        sizeSpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {

            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });
    }

    private void initScaleSpinner() {
        scaleSpinner = findViewById(R.id.scaleSpinner);
        ArrayList<SpinnerItem> scaleOptions = new ArrayList<>();
        scaleOptions.add(new SpinnerItem("1倍", 1));
        scaleOptions.add(new SpinnerItem("2倍", 2));
        scaleOptions.add(new SpinnerItem("3倍", 3));
        scaleOptions.add(new SpinnerItem("4倍", 4));
        SpinnerAdapter adapter = new SpinnerAdapter(this, scaleOptions);
        scaleSpinner.setAdapter(adapter);
        scaleSpinner.setSelection(1);
        scaleSpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                SpinnerItem item = adapter.getItem(position);
                imageConvert.scale = item.getValue();

                Intent service = new Intent(CaptureScreenActivity.this, ViewPortService.class);
                service.putExtra("width", imageConvert.width * item.getValue());
                service.putExtra("height", imageConvert.height * item.getValue());
                startService(service);
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });
    }

    private void initModeSpinner() {
        modeSpinner = findViewById(R.id.modeSpinner);
        ArrayList<SpinnerItem> scaleOptions = new ArrayList<>();
        scaleOptions.add(new SpinnerItem("彩色JPG", 1));
        scaleOptions.add(new SpinnerItem("单色", 2));
        modeSpinner.setAdapter(new SpinnerAdapter(this, scaleOptions));
        modeSpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {

            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });
    }
}
