#include <ESP8266WiFi.h> 
#include <WebSocketsServer.h>
// 设置热点的名字
#define AP_SSID "WIFI666"
// 设置热点的密码
#define AP_PSW  "00000000"
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
WiFiClient *WiFi_Client[MAX_CONNECT] = {0} ;
// 临时wifi连接
WiFiClient WiFi_Client_Tmp;

void setup() {  
  // 设置波特率
  Serial.begin(115200);
  // 设置wifi模块为AP模式 
  WiFi.mode(WIFI_AP);
  // 设置IP地址网关和子网掩码  
  WiFi.softAPConfig(local_IP, gateway, subnet);  
  // 设置wifi的名字和密码
  WiFi.softAP(AP_SSID,AP_PSW);
  // 打印wifi模块的ip地址
  Serial.println("IP address = ");
//  Serial.print(WiFi.softAPIP());
  // 启动wifiserver服务
  WiFi_Server.begin();
//  Serial.println("Server online.");
  // D4引脚设置成输出
  pinMode(24, OUTPUT);     
  pinMode(20, OUTPUT);    
  pinMode(05, OUTPUT);    
  pinMode(2, OUTPUT); 
     pinMode(16, OUTPUT);
     pinMode(15, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  // 启动WebSocket服务器
  server.begin();
  // 指定事件处理函数
  server.onEvent([](uint8_t num, WStype_t type, uint8_t * payload, size_t length){
//    digitalWrite(2, LOW);
    if (type == WStype_CONNECTED) {
      digitalWrite(2, LOW);
      // 若为客户端连接事件，显示提示信息
      Serial.println("New connection!");
    } else if (type == WStype_DISCONNECTED) {
      digitalWrite(2, HIGH);
      // 若为连接断开事件，显示提示信息
      Serial.println("Close the connection.");
    } else if (type == WStype_TEXT) {
      // 接收来自客户端的信息（客户端FLASH按键状态），并控制LED的工作
      String data = (char*)payload;
       Serial.println(data);
      if(data.substring(data.indexOf("=") + 1) == "on") {
          Serial.println("on\n");
          digitalWrite(2, HIGH);
          digitalWrite(05, HIGH);// 将D1引脚设为高电平（板载灯连接的这个引脚但是反向 所以板载灯闪烁和外界LED相反
      } else if (data.substring(data.indexOf("=") + 1) == "off") {
          Serial.println("off\n");
          
          digitalWrite(2, LOW);// 20 16 15 24
          digitalWrite(05, LOW);// 将D1引脚设为高电平（板载灯连接的这个引脚但是反向 所以板载灯闪烁和外界LED相反
      }
    }
    
  });
}  
void loop() {
  // put your main code here, to run repeatedly:
//   digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
//  delay(1000);                       // wait for a second
//  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
//  delay(1000);                       // wait for a second

  server.loop();
//  Serial.println("server waiting\n");
//  // 判断当前FLASH按键状态是否与之前状态一致，若不一致则按键状态发生改变
//  if (digitalRead(0) != pressState) {
//    // 若按键状态改变，记录当前按键状态，并向客户端发送按键状态
//    pressState = digitalRead(0);
//    String data = String("ledStatus=") + (digitalRead(0) == HIGH ? "off": "on");
//    server.sendTXT(get_num_from_ip(IPAddress(192, 168, 2, 8)), data);
//  }
}


//  
//void loop() {
//  // 判断是否有新的连接
//  WiFi_Client_Tmp = WiFi_Server.available();
//  // 判断这个连接是否有效
//  if(WiFi_Client_Tmp.connected()){
////    Serial.println("new connect");
//    // 如果有效，那么设置连接发送数据不延时
//    WiFi_Client_Tmp.setNoDelay(true);
//    // 使用for循环来遍历我们的连接池
//    for(int i =0; i< MAX_CONNECT; i++){
//      // 因为连接池里面可能会有连接，所以我们需要找到一个空的位置放入连接对象
//      if (WiFi_Client[i] == 0 || !WiFi_Client[i]->connected()){
//        // 我们新建一个TCP连接，然后把这个连接放入我们的连接池，放入后跳出循环
//        WiFi_Client[i] = new WiFiClient(WiFi_Client_Tmp);
//        break;
//      }
//    }
//  }
//  // 我们遍历连接池来接收数据
//  for(int i =0; i< MAX_CONNECT; i++){
//     // 先判断连接池里面的连接是否有效
//     if (WiFi_Client[i] != 0 && WiFi_Client[i]->connected()){
//        // 如果有效，尝试获取tcp发送的数据
//        if (WiFi_Client[i]->available() > 0){
//          // 这里说明有数据，我们直接读取tcp连接发送的数据
//          String data = "get data" + WiFi_Client[i]->readStringUntil('\n');
//          // 打印发送的数据
//          Serial.println(data);
//        }
//    }
//  }
//}
