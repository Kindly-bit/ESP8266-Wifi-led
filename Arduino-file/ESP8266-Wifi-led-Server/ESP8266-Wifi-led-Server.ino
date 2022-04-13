#include <ESP8266WiFi.h>      // 本程序使用 ESP8266WiFi库
#include <ESP8266WiFiMulti.h> //  ESP8266WiFiMulti库
#include <ESP8266WebServer.h> //  ESP8266WebServer库

ESP8266WiFiMulti wifiMulti; // 建立ESP8266WiFiMulti对象,对象名称是'wifiMulti'

ESP8266WebServer esp8266_server(80); // 建立ESP8266WebServer对象，对象名称为esp8266_server
                                     // 括号中的数字是网路服务器响应http请求的端口号
                                     // 网络服务器标准http端口号为80，因此这里使用80为端口号
// 服务器首页
String html = " <!DOCTYPE html><html lang=\"ch\"><head><meta charset=\"UTF-8\"><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Document</title><style>        .out {            height: 700px;            position: relative;            background-color: LightCyan;            border: 1px solid #FFF;        }        .in-box {            height: 300px;            width: 300px;            top: 50%;            left: 50%;            margin: -150px 0 0 -150px;            position: absolute;            line-height: 100px;        }        .time {            font-size: 25px;        }        .head {            text-align: center;        }        .center {            display: flex;            justify-content: center;            align-items: Center;                        margin: 0 auto;                    }        .a {            text-decoration: none;        }        .button1 {            width: 150px;            height: 150px;            font-weight: 600;            font-size: 35px;            color: darkcyan;            border: 9px solid darkcyan;            background: LightCyan;            border-radius: 50%;        }.disappear{    display :none;}        .space {            width: 100px;        }</style></head><body><div class=\" out\"><div class=\"in-box\"><span class=\'center  time\' id=\'time1\'> Time Loading ......</span><script>                setInterval(\'time1.innerHTML=new Date().toLocaleString()\', 1000);</script><h1 class=\"head center\">led Control</h1><a href=\'./pin?light1=0\' class=\'a\'><button type=\'button\' class=\'button1 center \' id=\'open\'>开灯</button></a><a href=\'./pin?light1=0\' class=\'a\'><button id=\'close\' type=\'button\' class=\'button1 center disappear\'>关灯</button></a></div></div></body><script>    var open = document.getElementById(\'open\');    var close = document.getElementById(\'close\');    open.onclick = function(){        var name = close.className        close.className = open.className;        open.className = name;     }    close.onclick = function(){    var name = open.className    open.className = close.className;    close.className = name;    }</script></html>";
// 开灯
String ophtml = "<!DOCTYPE html><html lang=\"ch\"><head><meta charset=\"UTF-8\"><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Document</title><style>        .out {            height: 700px;            position: relative;            background-color: LightCyan;            border: 1px solid #FFF;        }        .in-box {            height: 300px;            width: 300px;            top: 50%;            left: 50%;            margin: -150px 0 0 -150px;            position: absolute;            line-height: 100px;        }        .time {            font-size: 25px;        }        .head {            text-align: center;        }        .center {            display: flex;            justify-content: center;            align-items: Center;                        margin: 0 auto;                    }        .a {            text-decoration: none;        }        .button1 {            width: 150px;            height: 150px;            font-weight: 600;            font-size: 35px;            color: darkcyan;            border: 9px solid darkcyan;            background: LightCyan;            border-radius: 50%;        }.disappear{    display :none;}        .space {            width: 100px;        }</style></head><body><div class=\" out\"><div class=\"in-box\"><span class=\'center  time\' id=\'time1\'> Time Loading ......</span><script>                setInterval(\'time1.innerHTML=new Date().toLocaleString()\', 1000);</script><h1 class=\"head center\">led Control</h1><a href=\'./pin?light1=0\' class=\'a\'><button type=\'button\' class=\'button1 center \' id=\'open\'>开灯</button></a><a href=\'./pin?light1=0\' class=\'a\'><button id=\'close\' type=\'button\' class=\'button1 center disappear\'>关灯</button></a></div></div></body><script>    var open = document.getElementById(\'open\');    var close = document.getElementById(\'close\');    open.onclick = function(){        var name = close.className        close.className = open.className;        open.className = name;     }    close.onclick = function(){    var name = open.className    open.className = close.className;    close.className = name;    }</script></html>";
// 关灯
String clhtml = " <!DOCTYPE html><html lang=\"ch\"><head><meta charset=\"UTF-8\"><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Document</title><style>        .out {            height: 700px;            position: relative;            background-color: LightCyan;            border: 1px solid #FFF;        }        .in-box {            height: 300px;            width: 300px;            top: 50%;            left: 50%;            margin: -150px 0 0 -150px;            position: absolute;            line-height: 100px;        }        .time {            font-size: 25px;        }        .head {            text-align: center;        }        .center {            display: flex;            justify-content: center;            align-items: Center;                        margin: 0 auto;                    }        .a {            text-decoration: none;        }        .button1 {            width: 150px;            height: 150px;            font-weight: 600;            font-size: 35px;            color: darkcyan;            border: 9px solid darkcyan;            background: LightCyan;            border-radius: 50%;        }.close {            color: LightCyan;            border: 9px solid darkcyan;            background: darkcyan;        }.disappear{    display :none;}        .space {            width: 100px;        }</style></head><body><div class=\" out\"><div class=\"in-box\"><span class=\'center  time\' id=\'time1\'> Time Loading ......</span><script>                setInterval(\'time1.innerHTML=new Date().toLocaleString()\', 1000);</script><h1 class=\"head center\">led Control</h1><a href=\'./pin?light1=1\' class=\'a\'><button type=\'button\' class=\'button1 center close\' id=\'open\'>关灯</button></a><a href=\'./pin?light1=1\' class=\'a\'><button id=\'close\' type=\'button\' class=\'button1 center disappear\'>关灯</button></a></div></div></body><script>    var open = document.getElementById(\'open\');    var close = document.getElementById(\'close\');    open.onclick = function(){        var name = close.className        close.className = open.className;        open.className = name;     }    close.onclick = function(){    var name = open.className    open.className = close.className;    close.className = name;    }</script></html>";

void pin()//访问pin
{
    if (esp8266_server.arg("light1") == "1")
    {
//      digitalWrite(05, LOW); // D1
//digitalWrite(05, LOW); // D1
//        delay(50);
digitalWrite(05, LOW); // D1    
//         digitalWrite(05, LOW); // D1
        
        Serial.println("2-0");
        esp8266_server.send(200, "text/html", ophtml);
    } // 2端口
    else if (esp8266_server.arg("light1") == "0")
    {
//      digitalWrite(05, LOW); // D1
//        digitalWrite(05, HIGH); // D1
    digitalWrite(05, HIGH);
//delay(50);
//         digitalWrite(05, HIGH); // D1
        
        Serial.println("2-1");
        esp8266_server.send(200, "text/html", clhtml);
    }
}
void setup(void)
{
    Serial.begin(115200); // 启动串口通讯
                          // WiFi.mode(WIFI_AP_STA);
                          // // 设置IP地址网关和子网掩码
                          //  WiFi.softAPConfig(local_IP, gateway, subnet);
    //通过addAp函数存储  WiFi名称       WiFi密码
    wifiMulti.addAP("TP-LINK_6472", "gdcjdx105"); // 这三条语句通过调用函数addAP来记录3个不同的WiFi网络信息.
    wifiMulti.addAP("realme_k30", "88888888");
    wifiMulti.addAP("SDFFFF", "13572468");
    // 此处WiFi信息只是示例，请在使用时将需要连接的WiFi信息填入相应位置。
    // 另外这里只存储了3个WiFi信息，您可以存储更多的WiFi信息在此处。
    Serial.println("正在连接Wifi,请稍等：");
    int i = 0;
    digitalWrite(2, HIGH);
    while (wifiMulti.run() != WL_CONNECTED)
    {                // 此处的wifiMulti.run()是重点。通过wifiMulti.run()，NodeMCU将会在当前
        delay(1000); // 环境中搜索addAP函数所存储的WiFi。如果搜到多个存储的WiFi那么NodeMCU
        Serial.print(i++);
        Serial.print(' '); // 将会连接信号最强的那一个WiFi信号。
    }                      // 一旦连接WiFI成功，wifiMulti.run()将会返回“WL_CONNECTED”。这也是
    digitalWrite(2, LOW);  // 此处while循环判断是否跳出循环的条件。

    // WiFi连接成功后将通过串口监视器输出连接成功信息
    Serial.println('\n');           // WiFi连接成功后
    Serial.print("Connected to ");  // NodeMCU将通过串口监视器输出。
    Serial.println(WiFi.SSID());    // 连接的WiFI名称
    Serial.print("IP address:\t");  // 以及
    Serial.println(WiFi.localIP()); // NodeMCU的IP地址
    pinMode(2, OUTPUT);
    pinMode(05, OUTPUT); // D1
    //--------"启动网络服务功能"程序部分开始-------- //  
    esp8266_server.begin();             
    esp8266_server.on("/", handleRoot);   //设置服务器根目录为/ 访问时调用handleroot
    esp8266_server.on("/pin", HTTP_GET, pin); //访问pin时 模式为传参模式 调用pin
    esp8266_server.onNotFound(handleNotFound); //处理访问不存在位置时 调用Notfound
    //--------"启动网络服务功能"程序部分结束--------
    Serial.println("HTTP esp8266_server started"); //  告知用户ESP8266网络服务功能已经启动
}

void loop(void)
{
    esp8266_server.handleClient(); // 处理http服务器访问
}

void handleRoot()
{                                                //处理网站根目录“/”的访问请求
    esp8266_server.send(200, "text/html", html); // NodeMCU将调用此函数。
}

// 设置处理404情况的函数'handleNotFound'
void handleNotFound()
{                                                             // 当浏览器请求的网络资源无法在服务器找到时，
    esp8266_server.send(404, "text/plain", "404: Not found"); // NodeMCU将调用此函数。
}
