#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>

// 设置热点的名字
#define AP_SSID "WIFI666"
// 设置热点的密码
#define AP_PSW "00000000"
// 设置wifi模块的最大连接数
#define MAX_CONNECT 5
// 设置wifi服务的IP地址
const int SERVER_PORT = 80;
// 初始化WebSocketsServer对象
WebSocketsServer server(8080);

int pressState;

// 设置wifi的ip地址(wifi的ip地址好像不能和网关地址在同一个网段)
IPAddress local_IP(192, 168, 1, 1);
// 设置网关
IPAddress gateway(192, 168, 0, 1);
// 设置子网掩码
IPAddress subnet(255, 255, 255, 0);
// wifiServer服务
WiFiServer WiFi_Server(SERVER_PORT);
// wificlient对象，因为我们要想办法实现多个连接，所以我们创建一个指针数组来存储连接对象
WiFiClient *WiFi_Client[MAX_CONNECT] = {0};
// 临时wifi连接
WiFiClient WiFi_Client_Tmp;

void setup()
{
    // 设置波特率
    Serial.begin(115200);
    // 设置wifi模块为AP模式
    WiFi.mode(WIFI_AP);
    // 设置IP地址网关和子网掩码
    WiFi.softAPConfig(local_IP, gateway, subnet);
    // 设置wifi的名字和密码
    WiFi.softAP(AP_SSID, AP_PSW);
    // 打印wifi模块的ip地址
    Serial.println("IP address = ");
    //  Serial.print(WiFi.softAPIP());
    // 启动wifiserver服务
    WiFi_Server.begin();
    //  Serial.println("Server online.");
    // D1引脚设置成输出
    pinMode(05, OUTPUT);
    pinMode(2, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
    // 启动WebSocket服务器
    server.begin();
    // 指定事件处理函数
    server.onEvent([](uint8_t num, WStype_t type, uint8_t *payload, size_t length)
    {
        //    digitalWrite(2, LOW);
        if (type == WStype_CONNECTED)
        {
            digitalWrite(2, LOW);
            // 若为客户端连接事件，显示提示信息
            Serial.println("New connection!");
        }
        else if (type == WStype_DISCONNECTED)
        {
            digitalWrite(2, HIGH);
            // 若为连接断开事件，显示提示信息
            Serial.println("Close the connection.");
        }
        else if (type == WStype_TEXT)
        {
            // 接收来自客户端的信息（客户端FLASH按键状态），并控制LED的工作
            String data = (char *)payload;
            Serial.println(data);
            if (data.substring(data.indexOf("=") + 1) == "on")
            {
                Serial.println("on\n");
                digitalWrite(2, HIGH);
                digitalWrite(05, HIGH); // 将D1引脚设为高电平（板载灯连接的这个引脚但是反向 所以板载灯闪烁和外界LED相反
            }
            else if (data.substring(data.indexOf("=") + 1) == "off")
            {
                Serial.println("off\n");

                digitalWrite(2, LOW);  // 20 16 15 24
                digitalWrite(05, LOW); // 将D1引脚设为高电平（板载灯连接的这个引脚但是反向 所以板载灯闪烁和外界LED相反
            }
        } });
}
void loop()
{
    server.loop();
}
}
