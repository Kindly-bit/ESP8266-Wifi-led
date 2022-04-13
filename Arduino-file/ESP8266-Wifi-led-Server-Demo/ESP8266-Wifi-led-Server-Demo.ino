#include <ESP8266WiFi.h>



//String ssid = "Xiaomi_B74C";
//String password = "123456abc";
const char* assid = "espAccessPoint";//创建AP热点
const char* asecret = "12345678";
// 设置wifi的ip地址(wifi的ip地址好像不能和网关地址在同一个网段)
IPAddress local_IP(192, 168, 1, 1); 
// 设置网关
IPAddress gateway(192, 168, 0, 1);
// 设置子网掩码
IPAddress subnet(255, 255, 255, 0); 

WiFiServer server(80);
String readString = ""; //建立一个字符串对象用来接收存放来自客户的数据

//响应头
String responseHeaders =
  String("") +
  "HTTP/1.1 200 OK\r\n" +
  "Content-Type: text/html\r\n" +
  "Connection: close\r\n" +
  "\r\n";

//网页
String myhtmlPage =
  String("") +
"<!DOCTYPE html>"+
"<html lang=\"en\">"+
"<head>"+
"    <meta charset=\"UTF-8\">"+
"    <title>ESP8266 Wife</title>"+
"</head>"+
"<body>"+
"<div id=\"wife\">"+
"    <form action=\"/wife\" method=\"get\" id=\"user_form\">"+
"        Wife 名字<input type=\"text\" name=\"fname\"/></br><p></p>"+
"        Wife 密码<input type=\"text\" name=\"lname\"/></br>"+
"        <input type=\"submit\" width=800px value=\"连接wife\"/>"+
"    </form>"+
"</div>"+
"</body>"+
"<style>"+
"    #wife {"+
"        width: 800px;"+
"        height: 150px;"+
"        position: absolute;"+
"        top: 40%;"+
"        left: 40%;"+
"        margin: -50px 0 0 -50px;"+
"        color: aqua;"+
"    }"+
"    input[type=submit] {"+
"        width: 300px;"+
"        height: 25px;"+
"        background-color: aquamarine;"+
"        border: none;"+
"        color: white;"+
"        font-size: 16px;"+
"        margin: 4px 2px;"+
"        cursor: pointer;"+
"        text-align: center;"+
"        border-radius: 6px;"+
"    }"+
"</style>"+
"</html>";

//bool isLedTurnOpen = false; // 记录LED状态

void setup()
{

  Serial.begin(115200);
  Serial.println("wife连接热点方法开启");
  WiFi.mode(WIFI_AP_STA);
  // 设置IP地址网关和子网掩码  
  WiFi.softAPConfig(local_IP, gateway, subnet);  
  // 设置wifi的名字和密码
  WiFi.softAP(assid, asecret);
  Serial.println("热点已经打开,请连接");
  server.begin();
  delay(1000);
  Serial.println();
  Serial.printf("Web server started, open %s in a web browser\n", WiFi.localIP());
  wife_start();
  
}

void loop()
{
  Serial.printf("Web server started, open %s in a web browser\n", WiFi.localIP().toString().c_str());
}

void wife_start() {

  for (;;) {
    WiFiClient client = server.available(); //尝试建立客户对象
    Serial.println("正在尝试开启客户端");
    Serial.println(client);
    delay(1000);

    if (client)                             //如果当前有客户可用
    {
      boolean currentLineIsBlank = true;
      Serial.println("[Client connected]");
      delay(500);
      int i = 0;
      while (client.connected()) //如果客户端建立连接
      {
        if (i < 1) {        //这里会渲染一次网页,然后通过网页连接wife
          Serial.println("服务器已经被连接");
          Serial.println("开始渲染网页");
          client.print(responseHeaders); //向客户端输出网页响应
          client.print(myhtmlPage);      //向客户端输出网页内容
          client.print("\r\n");
          Serial.println(responseHeaders);
          Serial.println(myhtmlPage);
          delay(100);
          i++;
        }

        if (client.available()) //等待有可读数据
        {
          char c = client.read(); //读取一字节数据
          readString += c;        //拼接数据
          /************************************************/
          if (c == '\n' && currentLineIsBlank) //等待请求头接收完成(接收到空行)
          {
            //比较接收到的请求数据
            if (readString.startsWith("GET /wife")) {
              Serial.println("wife连接中");
              Serial.println(readString);
              Serial.println("wife重置中");
              String name = readString.substring(readString.indexOf("fname=") + 6, readString.indexOf("&"));
              String passed = readString.substring(readString.indexOf("lname=") + 6, readString.indexOf("HTTP") - 1);
              client.print("wife连接成功");
              WiFi.begin(name, passed);
              while (WiFi.status() != WL_CONNECTED)
              {
                delay(500);
                Serial.print(".");
              }
              Serial.println(" connected");
              return;
            }
          }

          if (c == '\n')
          {
            currentLineIsBlank = true; //开始新行
          }
          else if (c != '\r')
          {
            currentLineIsBlank = false; //正在接收某行中
          }
          /************************************************/
        }
      }
      delay(1);      //等待客户完成接收
      client.stop(); //结束当前连接:
      Serial.println("[Client disconnected]");

      Serial.println(readString); //打印输出来自客户的数据
      readString = "";
    }
  }

}
