# Hello240Screen
自制的小电视，主控esp8266，240x240彩色屏幕，3个按键。有投屏，天气，电脑监视器功能

# 成品效果演示
演示视频：https://www.bilibili.com/video/BV1bc411H7Vx/

电路：
![电路](5.Docs/UI_PCB.png)
手机投屏，小电视作为TCP服务端，接收手机通过tcp发送的图片数据
![手机投屏](5.Docs/VNC.jpg)
![手机投屏](5.Docs/app.jpg)

# 原理图
原理图和PCB：https://oshwhub.com/chenyuebo/lcd-xiao-dian-shi-240x240

![原理图](1.Hardware/%E5%8E%9F%E7%90%86%E5%9B%BE.png)

# 文件夹目录
1. Hardware: 原理图和PCB
2. Firmware：小电视固件源码
3. Software：
    - Hello240-Android：安卓投屏程序源码
4. 3D Model: 结构外壳，用于3D打印

# 安卓投屏程序
可以自己使用源码编译，也可以使用我编译好的APK。[下载APK](3.Software/Hello240-Android/apk/app-release.apk)

# 天气数据
天气接口：http://chenyuebo.cn/hello240/api/weather?cityCode=101010100 ，大家开发天气功能也可以使用，接口仅供学习使用

天气数据和参数cityCode来源于http://www.weather.com.cn/

接口返回格式：
```
{
  "code": "0",
  "data": {
    "SD": "12",
    "WD": "东风",
    "WS": "2级",
    "aqi_pm25": "34",
    "cityCode": "101010100",
    "cityName": "北京",
    "date": "05月07日",
    "temp": "26",
    "tempH": "26",
    "tempL": "11",
    "weather": "晴",
    "weatherCode": "d00",
    "week": "星期日"
  },
  "msg": "SUCCESS"
}
```
# 备注
本人为纯软件程序员，本项目纯属个人爱好自学制作，主要目的是学习，部分功能待优化，也可能会有错误，希望大家理解和指正。

# TODO
- 安卓投屏程序，小电视接收成功后就发送下一帧数据，而不是定时发送，可能会提高一点点帧率
