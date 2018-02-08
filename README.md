![](https://raw.githubusercontent.com/mobier/SmartCloud_ESP8266/master/image/c478c2fe-0168-4a82-a68e-d58d1e513831.jpg)

# [制作一朵可以随着天气变化的智能“云”](https://nu11.me/?p=69)

通过在路由器（openwrt）上搭建MQTTServer，然后ESP8266通过读取DHT11传感器，然后将传感器读到的温度和湿度数据发布到“temperature”和“Humidity”这两个主题。安卓手机通MyMQTT发布“LightSwitch”和“HumiditySwitch”切换云朵的颜色或者打开加湿器。

通过ESP8266和加湿器公地，然后将io接入按钮电路中，从MQTT订阅“LightSwitch”这个主题，当收到数据后模拟按键电路信号切换到对应的颜色。或者是通过读取天气的API。然后判断所在地天气情况切换到对应的颜色！

可以通过读取房间湿度然后数值低于设置的阈值就模拟按键电路打开加湿器，然后如果房间湿度大于设置的阈值就关闭加湿器。


## 硬件
>* MQTTServer
>* 无印良品加湿器（彩灯款）
>* DHT11
>* ESP8266
>* 5V电池

## 软件
```
mosquitto（openwrt Mqtt Server）
MyMQTT（Android Mqtt client）
HumiditySwitch();//控制加湿器开关
LightSwitch();//接收到控制指令后操作IO 模拟切换灯光电路信号切换灯光
loop();//每间隔两秒读取DHT11数据 通过MQTT发布出去
http://api.heweather.com/x3/weather?cityid=CN101010300&key=XXXXX//和风天气API
```
