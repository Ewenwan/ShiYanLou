# programming_arduino_ed2

# IO
## 延时闪灯
```c
void setup()
{
  pinMode(13, OUTPUT);
}

void loop()
{
 digitalWrite(13, HIGH);
 delay(500);
 digitalWrite(13, LOW);
 delay(500);
}

// sketch 03-02 加入变量====
int ledPin = 13;
int delayPeriod = 500;

void setup()
{
  pinMode(ledPin, OUTPUT);
}

void loop()
{
 digitalWrite(ledPin, HIGH);
 delay(delayPeriod);
 digitalWrite(ledPin, LOW);
 delay(delayPeriod);
}

// sketch 03-03  快闪--->满闪=====
int ledPin = 13;
int delayPeriod = 100;

void setup()
{
  pinMode(ledPin, OUTPUT);
}

void loop()
{
 digitalWrite(ledPin, HIGH);
 delay(delayPeriod);
 digitalWrite(ledPin, LOW);
 delay(delayPeriod);
 delayPeriod = delayPeriod + 100;
}


// sketch 03-06    快闪--->满闪-->快闪--->满闪---循环
int ledPin = 13;
int delayPeriod = 100;

void setup()
{
  pinMode(ledPin, OUTPUT);
}

void loop()
{
  digitalWrite(ledPin, HIGH);
  delay(delayPeriod);
  digitalWrite(ledPin, LOW);
  delay(delayPeriod);
  delayPeriod = delayPeriod + 100; // 闪烁变慢
  if (delayPeriod > 1000) // 循环===回到初始状态
  {
    delayPeriod = 100; 
  }
}


// sketch 03-08   ===== for循环 闪灯
int ledPin = 13;
int delayPeriod = 250;

void setup()
{
  pinMode(ledPin, OUTPUT);
}

void loop()
{
  for (int i = 0; i < 20; i ++)
  {
   digitalWrite(ledPin, HIGH);
   delay(delayPeriod);
   digitalWrite(ledPin, LOW);
   delay(delayPeriod);
  }
 delay(3000);
}


// sketch 03-09  带有休息时段的 常数闪灯=====
int ledPin = 13;
int delayPeriod = 100;
int count = 0;

void setup()
{
  pinMode(ledPin, OUTPUT);
}

void loop()
{
 digitalWrite(ledPin, HIGH);
 delay(delayPeriod);
 digitalWrite(ledPin, LOW);
 delay(delayPeriod);
 count ++;
 if (count == 20)
 {
   count = 0;
   delay(3000);  // 闪烁20次，定时休息
 }
}

// sketch 04-04==========带有休息时段的 常数闪灯 const static变量=====
const int ledPin = 13;
const int delayPeriod = 250;

void setup()
{
  pinMode(ledPin, OUTPUT);
}

void loop()
{
  static int count = 0;
  digitalWrite(ledPin, HIGH);
  delay(delayPeriod);
  digitalWrite(ledPin, LOW);
  delay(delayPeriod);
  count ++;
  if (count == 20)
  {
    count = 0;
    delay(3000);
  }
}


// sketch 04-01   for 循环实现闪灯  闪灯程序打包成函数====
const int ledPin = 13;
const int delayPeriod = 250;

void setup()
{
  pinMode(ledPin, OUTPUT);
}

void loop()
{
  for (int i = 0; i < 20; i ++)
  {//  for 循环实现闪灯
    flash();
  }
 delay(3000);
}

// 闪灯程序打包成函数
void flash()
{
   digitalWrite(ledPin, HIGH);
   delay(delayPeriod);
   digitalWrite(ledPin, LOW);
   delay(delayPeriod);
}



// sketch 04-02=============进一步包装闪灯程序
const int ledPin = 13;
const int delayPeriod = 250;

void setup()
{
  pinMode(ledPin, OUTPUT);
}

void loop()
{
  flash(20, delayPeriod);
  delay(3000);
}

void flash(int numFlashes, int d)
{
  for (int i = 0; i < numFlashes; i ++)
  {
    digitalWrite(ledPin, HIGH);
    delay(d);
    digitalWrite(ledPin, LOW);
    delay(d);
  }
}


// sketch 05-02   闪灯延时指定 时间  数组=====================
const int ledPin = 13;
int durations[] = {200, 200, 200, 500, 500, 500, 200, 200, 200};

void setup()
{
  pinMode(ledPin, OUTPUT);
}

void loop() 
{
  for (int i = 0; i < 9; i++)
  {
    flash(durations[i]);
  }
  delay(1000);
}

void flash(int duration)
{
   digitalWrite(ledPin, HIGH);
   delay(duration);
   digitalWrite(ledPin, LOW);
   delay(duration);
}



//sketch 06-04  ======= io 口 控制 灯开关 ==========
const int inputPin = 5;
const int ledPin = 13;

void setup()
{
  pinMode(ledPin, OUTPUT);
  pinMode(inputPin, INPUT_PULLUP);
}

void loop()
{
  int switchOpen = digitalRead(inputPin);
  digitalWrite(ledPin, !switchOpen);
}


//sketch 06-05    =====低电平触发 闪灯===============
const int inputPin = 5;
const int ledPin = 13;
int ledValue = LOW;
void setup() 
{
  pinMode(inputPin, INPUT_PULLUP);// 上拉
  pinMode(ledPin, OUTPUT);
}
void loop() 
{
  if (digitalRead(inputPin) == LOW) //低电平触发 闪灯 
  {
     ledValue = !ledValue;
     digitalWrite(ledPin, ledValue);
     delay(500);
  }
}



//sketch 06-07   ===== 按键消抖 控制灯闪烁======
#include <Bounce2.h>  // 按键消抖 库 
const int inputPin = 5;
const int ledPin = 13;
int ledValue = LOW;
Bounce bouncer = Bounce(); 

void setup() 
{
  pinMode(inputPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  bouncer.attach(inputPin);
 // bouncer.interval(50); // interval in ms
}

void loop() 
{
  if (bouncer.update() && bouncer.read() == LOW)  // 按键消抖
  {
   ledValue = ! ledValue;
   digitalWrite(ledPin, ledValue);
   // delay(500);
  }
}



//sketch 07-04  ===端口中断===改变闪灯节奏===

const int interruptPin = 2;
const int ledPin = 13;
int period = 500;

void setup()
{
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(0, goFast, FALLING);// 端口中断 下降沿中断
}

void loop() 
{
  digitalWrite(ledPin, HIGH);
  delay(period);
  digitalWrite(ledPin, LOW);
  delay(period);  
}

// 端口中断函数 改变闪灯节奏
void goFast()
{
  period = 100;
}
```

模拟量测量

```c
//sketch 06-08

const int analogPin = 0;

void setup()
{
  Serial.begin(9600);
}
void loop()
{
  int reading = analogRead(analogPin); // 模拟量测量
  float voltage = reading / 204.6;// 转换成电压
  Serial.print("Reading=");
  Serial.print(reading);
  Serial.print("\t\tVolts=");
  Serial.println(voltage);
  delay(500);
}


```

Arduino音乐键盘tone函数

```c
//sketch 07-03
void setup()
{
  tone(4, 500);
}
void loop() {}




```


##  串口
```c

// sketch 03-04
void setup()
{
  Serial.begin(9600);
  int a = 2;
  int b = 2;
  int c = a + b;
  Serial.println(c); 
  Serial.println("Hello");
}
void loop()
{}

// sketch 5-04 ===============打印字符串数组========
char message[] = "Hello";

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  Serial.println(message);
  delay(1000);
}



// sketch 03-05   串口 华氏度 摄氏度 转换=====
void setup()
{
  Serial.begin(9600);
  int degC = 20;
  int degF;
  degF = degC * 9 / 5 + 32;
  Serial.println(degF); 
}
void loop()
{}


    
// sketch 05-01   串口打印数组================
int durations[] = {200, 200, 200, 500, 500, 500, 200, 200, 200};

void setup()
{
  Serial.begin(9600);  
  for (int i = 0; i < 9; i++)
  {
    Serial.println(durations[i]);
  }
}

void loop() {}




// sketch 5-05  ======= 串口和闪灯混合，串口字符控制闪灯节奏=======

const int ledPin = 13;
const int dotDelay = 200;

char* letters[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",    // A-I
  ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",  // J-R
  "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."          // S-Z
};

char* numbers[] = {
  "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----."};

void setup()                 
{
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop()                    
{
  char ch;
  if (Serial.available() > 0)
  {
    ch = Serial.read();
    if (ch >= 'a' && ch <= 'z')
    {
      flashSequence(letters[ch - 'a']);
    }
    else if (ch >= 'A' && ch <= 'Z')
    {
      flashSequence(letters[ch - 'A']);
    }
    else if (ch >= '0' && ch <= '9')
    {
      flashSequence(numbers[ch - '0']);
    }
    else if (ch == ' ')
    {
      delay(dotDelay * 4);  // gap between words  
    }
  }
}

void flashSequence(char* sequence)
{
  int i = 0;
  while (sequence[i] != NULL)
  {
    flashDotOrDash(sequence[i]);
    i++;
  }
  delay(dotDelay * 3);    // gap between letters
}

void flashDotOrDash(char dotOrDash)
{
  digitalWrite(ledPin, HIGH);
  if (dotOrDash == '.')
  {
    delay(dotDelay);           
  }
  else // must be a dash
  {
    delay(dotDelay * 3);           
  }
  digitalWrite(ledPin, LOW);    
  delay(dotDelay); // gap between flashes
}




// sketch 8-01 ====内存拷贝=====串口控制=====

const int ledPin = 13;
const int dotDelay = 200;

const int maxLen = 6; // including null on the end

PROGMEM const char letters[26][maxLen] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",    // A-I
  ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",  // J-R
  "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."          // S-Z
};


PROGMEM const char numbers[10][maxLen] = {
  "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----."
};

void setup()                 
{
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop()                    
{
  char ch;
  char sequence[maxLen];
  if (Serial.available() > 0)
  {
    ch = Serial.read();
    if (ch >= 'a' && ch <= 'z')
    {
      memcpy_P(&sequence, letters[ch - 'a'], maxLen);// 内存拷贝
      flashSequence(sequence);
    }
    else if (ch >= 'A' && ch <= 'Z')
    {
      memcpy_P(&sequence, letters[ch - 'A'], maxLen);
      flashSequence(sequence);
    }
    else if (ch >= '0' && ch <= '9')
    {
      memcpy_P(&sequence, numbers[ch - '0'], maxLen);
      flashSequence(sequence);
    }
    else if (ch == ' ')
    {
      delay(dotDelay * 4);  // gap between words  
    }
  }
}

void flashSequence(char* sequence)
{
  int i = 0;
  while (sequence[i] != NULL)
  {
    flashDotOrDash(sequence[i]);
    i++;
  }
  delay(dotDelay * 3);    // gap between letters
}

void flashDotOrDash(char dotOrDash)
{
  digitalWrite(ledPin, HIGH);
  if (dotOrDash == '.')
  {
    delay(dotDelay);           
  }
  else // must be a dash
  {
    delay(dotDelay * 3);           
  }
  digitalWrite(ledPin, LOW);    
  delay(dotDelay); // gap between flashes
}






//sketch 6-01    =======串口控制 灯 开关====
const int outPin = 3;

void setup()
{
  pinMode(outPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Enter 1 or 0");
}

void loop()
{
  if (Serial.available() > 0)
  {
    char ch = Serial.read();
    if (ch == '1')
    {
      digitalWrite(outPin, HIGH);
    }
    else if (ch == '0')
    {
      digitalWrite(outPin, LOW);
    }
  }
}


//sketch 06-02  ===== =====读取io状态，并串口打印=========
const int inputPin = 5;
void setup()
{
  pinMode(inputPin, INPUT);
  Serial.begin(9600);
}

void loop()
{
  int reading = digitalRead(inputPin); // 普通输入，默认 低电平
  Serial.println(reading);
  delay(1000);
}

//sketch 06-03 =====上拉输入，默认为 高电平 打印 io口状态=========
const int inputPin = 5;

void setup()
{
  pinMode(inputPin, INPUT_PULLUP);  // 上拉输入，默认为 高电平
  Serial.begin(9600);
}

void loop()
{
  int reading = digitalRead(inputPin);
  Serial.println(reading);
  delay(1000);
}



//sketch 07-01  === 打印随机数==========
void setup()
{
  Serial.begin(9600);
  randomSeed(analogRead(0)); // 随机数种子
}

void loop()
{
  int number = random(1, 7);
  Serial.println(number);
  delay(500); 
}


```


## flash 闪存 内存 rom 内存读写
```c
// sketch 08-02  ==== 串口读数据 写入内存=======
#include <EEPROM.h>
int addr = 0;
char ch;
void setup()                 
{
  Serial.begin(9600);
  ch = EEPROM.read(addr);
}
void loop()                    
{
  if (Serial.available() > 0)
  {
    ch = Serial.read();   // 串口读数据
    EEPROM.write(0, ch);  // 写入内存 单个字节数据
    Serial.println(ch);
  }
  Serial.println(ch);
  delay(1000);
}


// sketch 08-06 ========清空内存数据=================
#include <EEPROM.h>
void setup()                 
{
  Serial.begin(9600);
  Serial.println("Clearing EEPROM");
  for (int i = 0; i < 1024; i++)
  {
    EEPROM.write(i, 0);
  }
  Serial.println("EEPROM Cleared");
}
void loop() {}


// sketch 08-03 ====== 向内存 读写 多字节数据
#include <avr/eeprom.h>
void setup() 
{
  Serial.begin(9600);
  int i1 = 123;
  eeprom_write_block(&i1, 0, 2); // 写2个字节长度的数据
  int i2 = 0;
  eeprom_read_block(&i2, 0, 2);
  Serial.println(i2);
}
void loop() {}


// sketch 08-04 ======== 写 浮点数===========
#include <avr/eeprom.h>
void setup() 
{
  Serial.begin(9600);
  float f1 = 1.23;
  eeprom_write_block(&f1, 0, 4);// 4字节
  float f2 = 0;
  eeprom_read_block(&f2, 0, 4);
  Serial.println(f2);
}
void loop() {}


//sketch 08-07  =========浮点数扩大后(量化) 以一个字节数据 进行存储
#include <EEPROM.h>
void setup()
{
  float tempFloat = 20.75;
  byte tempByte = (int)(tempFloat * 4); // 浮点数扩大后(量化)
  EEPROM.write(0, tempByte); // 以一个字节数据 进行存储
  
  byte tempByte2 = EEPROM.read(0); // 以一个字节数据 进行读取
  float temp2 = (float)(tempByte2) / 4; // 反量化
  Serial.begin(9600);
  Serial.println("\n\n\n");
  Serial.println(temp2);
}
void loop(){}


// sketch 08-05 =============内存存储 字符串 密码数组
#include <avr/eeprom.h>
const int maxPasswordSize = 20; // 数组长度
char password[maxPasswordSize]; // 字符串 密码数组

void setup() 
{
  eeprom_read_block(&password, 0, maxPasswordSize);
  Serial.begin(9600);
}

void loop() 
{
  Serial.print("Your password is:");
  Serial.println(password);
  Serial.println("Enter a NEW password");     
  while (!Serial.available()) {};
  int n = Serial.readBytesUntil('\n', password, maxPasswordSize);
  password[n] = '\0';
  eeprom_write_block(password, 0, maxPasswordSize);  // 写 字符串 密码数组
  Serial.print("Saved Password: ");
  Serial.println(password);
}

```



##  显示屏 
```c
// sketch 09-01 USB Message Board   LCD==============================
#include <LiquidCrystal.h>
//            lcd(RS E  D4 D5 D6 D7) // 各端口=====
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
/*
其他接口形式
 LiquidCrystal(rs, rw, enable, d4, d5, d6, d7)
 LiquidCrystal(rs, enable, d0, d1, d2, d3, d4, d5, d6, d7)
 LiquidCrystal(rs, rw, enable, d0, d1, d2, d3, d4, d5, d6, d7)
*/
int numRows = 2;
int numCols = 16;
/*
begin()——定义LCD的长宽（n列×n行），格式lcd.begin(cols, rows)
 clear()——清空LCD，格式lcd.clear()
 home()——把光标移回左上角，即从头开始输出，格式lcd.home()
 setCursor()——移动光标到特定位置，格式lcd.setCursor(col, row)
 write()——在屏幕上显示内容（必须是一个变量，如”Serial.read()”），格式lcd.write(data)
 print()——在屏幕上显示内容（字母、字符串，等等），格式lcd.print(data)
 lcd.print(data, BASE)
 cursor()——显示光标（一条下划线），格式lcd.cursor()
 noCursor()——隐藏光标，格式lcd.noCursor()
 blink()——闪烁光标，格式lcd.blink()
 noBlink()——光标停止闪烁，格式lcd.noBlink()
 display()——（在使用noDisplay()函数关闭显示后）打开显示（并恢复原来内容），格式lcd.display()
 noDisplay()——关闭显示，但不会丢失原来显示的内容，格式为lcd.noDisplay()
 scrollDisplayLeft()——把显示的内容向左滚动一格，格式lcd.scrollDisplayLeft()
 scrollDisplayRight()——把显示的内容向右滚动一格，格式为lcd.scrollDisplayRight()
 autoscroll()——打开自动滚动，这使每个新的字符出现后，原有的字符都移动一格：如果字符一开始从左到右（默认），那么就往左移动一格，否则就向右移动，格式lcd.autoscroll()
 noAutoscroll()——关闭自动滚动，格式lcd.noAutoscroll()
 leftToRight()——从左往右显示，也就是说显示的字符会从左往右排列（默认），但屏幕上已经有的字符不受影响，格式lcd.leftToRight()
 rightToLeft()——从右往左显示，格式lcd.rightToLeft()
 createChar()——自造字符，最多5×8像素，编号0-7，字符的每个像素显示与否由数组里的数（0-不显示，1-显示）决定，格式lcd.createChar(num, data)，有点难理解，可以看一个例子
*/
void setup()
{
  Serial.begin(9600);
  
  lcd.begin(numRows, numCols);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Arduino");
  lcd.setCursor(0,1);
  lcd.print("Rules");
}

void loop()
{
  if (Serial.available() > 0) 
  {
    char ch = Serial.read();
    if (ch == '#')
    {
      lcd.clear();
    }
    else if (ch == '/')
    {
      // new line
      lcd.setCursor(0, 1);
    }
    else
    {
      lcd.write(ch);
    }
  }
}


// sketch 09_02 ====I2C(SSD1306)驱动0.96寸12864 OLED========

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(4); // pick an unused pin

void setup()   
{                
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);  // may need to change this
  display.setTextSize(4);
  display.setTextColor(WHITE);
}

void loop() 
{
  static int count = 0;
  display.clearDisplay();
  display.drawRoundRect(0, 0, 127, 63, 8, WHITE);
  display.setCursor(20,20);
  display.print(count);
  display.display();
  count ++;
  if (count > 9999)
  {
    count = 0;
  }
  delay(1000);
}



```



##  物联网专题  Ethernet（以太网） 
```c
// sketch 10-01 DHCP ==================动态ip访问======
#include <SPI.h>
#include <Ethernet.h>
// MAC address just has to be unique. This should work
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

EthernetServer server(80);

void setup()
{
  Serial.begin(9600);
  Ethernet.begin(mac);
  server.begin();
  Serial.print("Server started on: ");
  Serial.println(Ethernet.localIP());
}

void loop()
{
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) 
  {
    while (client.connected()) 
    {
      // send a standard http response header
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println();
      
      // send the body
      client.println("<html><body>");
      client.println("<h1>Arduino Server</h1>");
      client.print("<p>A0="); 
      client.print(analogRead(0)); 
      client.println("</p>"); 
      client.print("<p>millis="); 
      client.print(millis()); 
      client.println("</p>"); 
      client.println("</body></html>");
      client.stop();
    }
    delay(1);
  }
}


// sketch 10-01 Static IP  ====== 静态ip访问=================================
#include <SPI.h>
#include <Ethernet.h>
// MAC address just has to be unique. This should work
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// The IP address will be dependent on your local network:
byte ip[] = { 192, 168, 1, 30 };

EthernetServer server(80);

void setup()
{
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.begin(9600);
}

void loop()
{
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) 
  {
    while (client.connected()) 
    {
      // send a standard http response header
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println();
      
      // send the body
      client.println("<html><body>");
      client.println("<h1>Arduino Server</h1>");
      client.print("<p>A0="); 
      client.print(analogRead(0)); 
      client.println("</p>"); 
      client.print("<p>millis="); 
      client.print(millis()); 
      client.println("</p>"); 
      client.println("</body></html>");
      client.stop();
    }
    delay(1);
  }
}



// sketch 10-02 Web Controlled Arduino   动态IP WEB远程控制 终端arduino

#include <SPI.h>
#include <Ethernet.h>

// MAC address just has to be unique. This should work
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

EthernetServer server(80);

int numPins = 5;
int pins[] = {3, 4, 5, 6, 7};
int pinState[] = {0, 0, 0, 0, 0};
char line1[100];

void setup()
{
  for (int i = 0; i < numPins; i++)
  {
     pinMode(pins[i], OUTPUT);
  }
  Serial.begin(9600);
  Ethernet.begin(mac);
  server.begin();
  Serial.print("Server started on: ");
  Serial.println(Ethernet.localIP());
}

void loop()
{
  EthernetClient client = server.available();
  if (client) 
  {
    while (client.connected()) 
    {
      readHeader(client);
      if (! pageNameIs("/"))
      {
        client.stop();  
        return;
      }
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println();

      // send the body
      client.println("<html><body>");
      client.println("<h1>Output Pins</h1>");
      client.println("<form method='GET'>");  
      setValuesFromParams();
      setPinStates();
      for (int i = 0; i < numPins; i++)
      {
         writeHTMLforPin(client, i);
      }
      client.println("<input type='submit' value='Update'/>");
      client.println("</form>");
      client.println("</body></html>");

      client.stop();            
    }
  }
}

void writeHTMLforPin(EthernetClient client, int i)
{
  client.print("<p>D");
  client.print(pins[i]);  
  client.print(" <select name='");
  client.print(i);
  client.println("'>");
  client.print("<option value='0'");
  if (pinState[i] == 0)
  {
    client.print(" selected");
  }
  client.println(">Off</option>");
  client.print("<option value='1'");
  if (pinState[i] == 1)
  {
    client.print(" selected");
  }
  client.println(">On</option>");
  client.println("</select></p>");  
}

void setPinStates()
{
  for (int i = 0; i < numPins; i++)
  {
     digitalWrite(pins[i], pinState[i]);
  }
}

void setValuesFromParams()
{
  for (int i = 0; i < numPins; i++)
  {
    pinState[i] = valueOfParam(i + '0');
  }
}

void readHeader(EthernetClient client)
{
  // read first line of header
  char ch;
  int i = 0;
  while (ch != '\n')
  {
    if (client.available())
    {
      ch = client.read();
      line1[i] = ch;
      i ++;
    }
  }
  line1[i] = '\0'; 
  Serial.println(line1);
}

boolean pageNameIs(char* name)
{
   // page name starts at char pos 4
   // ends with space
   int i = 4;
   char ch = line1[i];
   while (ch != ' ' && ch != '\n' && ch != '?')
   {
     if (name[i-4] != line1[i])
     {
       return false;
     }
     i++;
     ch = line1[i];
   }
   return true;
}

int valueOfParam(char param)
{
  for (int i = 0; i < strlen(line1); i++)
  {
    if (line1[i] == param && line1[i+1] == '=')
    {
      return (line1[i+2] - '0');
    }
  }
  return 0;
}



// sketch 10-02 Internet Pins ====== 静态ip web远程控制 终端 arduino==============

#include <SPI.h>
#include <Ethernet.h>

// MAC address just has to be unique. This should work
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// The IP address will be dependent on your local network:
byte ip[] = { 192, 168, 1, 30 };
EthernetServer server(80);

int numPins = 5;
int pins[] = {3, 4, 5, 6, 7};
int pinState[] = {0, 0, 0, 0, 0};
char line1[100];

void setup()
{
  for (int i = 0; i < numPins; i++)
  {
     pinMode(pins[i], OUTPUT);
  }
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  server.begin();
}

void loop()
{
  EthernetClient client = server.available();
  if (client) 
  {
    while (client.connected()) 
    {
      readHeader(client);
      if (! pageNameIs("/"))
      {
        client.stop();  
        return;
      }
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println();

      // send the body
      client.println("<html><body>");
      client.println("<h1>Output Pins</h1>");
      client.println("<form method='GET'>");  
      setValuesFromParams();
      setPinStates();
      for (int i = 0; i < numPins; i++)
      {
         writeHTMLforPin(client, i);
      }
      client.println("<input type='submit' value='Update'/>");
      client.println("</form>");
      client.println("</body></html>");

      client.stop();            
    }
  }
}

void writeHTMLforPin(EthernetClient client, int i)
{
  client.print("<p>D");
  client.print(pins[i]);  
  client.print(" <select name='");
  client.print(i);
  client.println("'>");
  client.print("<option value='0'");
  if (pinState[i] == 0)
  {
    client.print(" selected");
  }
  client.println(">Off</option>");
  client.print("<option value='1'");
  if (pinState[i] == 1)
  {
    client.print(" selected");
  }
  client.println(">On</option>");
  client.println("</select></p>");  
}

void setPinStates()
{
  for (int i = 0; i < numPins; i++)
  {
     digitalWrite(pins[i], pinState[i]);
  }
}

void setValuesFromParams()
{
  for (int i = 0; i < numPins; i++)
  {
    pinState[i] = valueOfParam(i + '0');
  }
}

void readHeader(EthernetClient client)
{
  // read first line of header
  char ch;
  int i = 0;
  while (ch != '\n')
  {
    if (client.available())
    {
      ch = client.read();
      line1[i] = ch;
      i ++;
    }
  }
  line1[i] = '\0'; 
  Serial.println(line1);
}

boolean pageNameIs(char* name)
{
   // page name starts at char pos 4
   // ends with space
   int i = 4;
   char ch = line1[i];
   while (ch != ' ' && ch != '\n' && ch != '?')
   {
     if (name[i-4] != line1[i])
     {
       return false;
     }
     i++;
     ch = line1[i];
   }
   return true;
}

int valueOfParam(char param)
{
  for (int i = 0; i < strlen(line1); i++)
  {
    if (line1[i] == param && line1[i+1] == '=')
    {
      return (line1[i+2] - '0');
    }
  }
  return 0;
}
```

##   无线网模块 
```c
// sketch 10-03. Node MCU Basic Web Server

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "my-network-name"; // 无线网名称
const char* password = "my_password"; // 无线网密码

ESP8266WebServer server(80);

void handleRoot()
{
  String message = "<html><body>\n";
  message += "<h1>Arduino Server</h1>\n";
  message += "<p>A0="; 
  message += analogRead(A0); 
  message += "</p>"; 
  message += "<p>millis="; 
  message += millis(); 
  message += "</p>"; 
  message += "</html></body>\n";
  server.send(200, "text/html", message);
}

void connectToWiFi()
{
  Serial.print("\n\nConnecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup()
{
  Serial.begin(115200);
  connectToWiFi();

  server.on("/", handleRoot);

  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  server.handleClient();
}




// sketch 10-04 Web Controlled Node MCU ==== 无线网 控制终端 arduino==========

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "my-network-name";
const char* password = "my_password";

int numPins = 5;
char* pinNames[] = {"D5", "D6", "D7", "D8", "D9"};
int pins[] = {D5, D6, D7, D8, D9};
int pinState[] = {0, 0, 0, 0, 0};

ESP8266WebServer server(80);

void setPinStates()
{
  for (int i = 0; i < numPins; i++)
  {
     digitalWrite(pins[i], pinState[i]);
  }
}

void setValuesFromParams()
{
  for (int i = 0; i < numPins; i++)
  {
    pinState[i] = server.arg(i).toInt();
  }
}

void connectToWiFi()
{
  Serial.print("\n\nConnecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void handleRoot()
{
  char buff[1000];
  Serial.println("Got a Request");
  setValuesFromParams();
  setPinStates();
  
  strcat(buff, "<html><body>\n");
  strcat(buff, "<h1>Output Pins</h1>\n");
  strcat(buff, "<form method='GET'>\n"); 
  for (int i = 0; i < numPins; i++)
  {
    strcat(buff, "<p>");
    strcat(buff, pinNames[i]);
    strcat(buff, " <select name='");
    char indexStr[10];
    sprintf(indexStr, "%d", i);
    strcat(buff, indexStr);
    strcat(buff, "'><option value='0'");
    if (pinState[i] == 0)
    {
      strcat(buff, " selected");
    }
    strcat(buff, ">Off</option>");
    strcat(buff, "<option value='1'");
    if (pinState[i] == 1)
    {
      strcat(buff, " selected");
    }
    strcat(buff, ">On</option></select></p>\n");
  }
  strcat(buff, "<input type='submit' value='Update'/>");
  strcat(buff, "</form></html></body>\n");
  server.send(200, "text/html", buff);
}


void setup()
{
  for (int i = 0; i < numPins; i++)
  {
     pinMode(pins[i], OUTPUT);
  }
  Serial.begin(115200);
  
  connectToWiFi();
  
  server.on("/", handleRoot);

  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  server.handleClient();
}
```



## IFTTT 表示如果在“this”上发生了某些事情，那么我们需要在“that”上做一些事情。
```c
// sketch 10-05 IFTTT
#include <SPI.h>
#include <Ethernet.h>


// MAC address just has to be unique. This should work
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

const char* key = "c1AsQq_qsQVTSO5-6NASqg";
const char* host = "maker.ifttt.com";
const int httpPort = 80;
const long sendPeriod = 60000L; // 1 minute

EthernetClient client;

void setup()
{
  Serial.begin(9600);
  Ethernet.begin(mac);
}

void sendToIFTTT(int reading)
{
  client.stop(); // for second time around the loop
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  String url = "/trigger/arduino_spoke/with/key/";
  url += key;
  url += "?value1=" + String(reading);

  String req = String("GET ") + url + " HTTP/1.1\r\n" +  
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n";
  Serial.println(req);
  client.print(req);
}

void loop() 
{
  static long lastReadingTime = 0;
  long now = millis();
  if (now > lastReadingTime + sendPeriod)
  {
    int reading = analogRead(A0);
    sendToIFTTT(reading);
    lastReadingTime = now;
  }
  if (client.available())
  {
    Serial.write(client.read());
  }
}



// sketch 10_06=======wifi连接IFTTT=================

#include <ESP8266WiFi.h>

const char* ssid = "my-network-name";
const char* password = "my_password";
const char* key = "c1AsQq_qsQVTSO5-6NASqg";
const char* host = "maker.ifttt.com";
const int httpPort = 80;
const long sendPeriod = 10000L; // 1 minute

WiFiClient client;

void connectToWiFi()
{
  Serial.print("\n\nConnecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void sendToIFTTT(int reading)
{
  Serial.print("connecting to ");
  Serial.println(host);

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  String url = "/trigger/arduino_spoke/with/key/";
  url += key;
  url += "?value1=" + String(reading);

  String req = String("GET ") + url + " HTTP/1.1\r\n" +  
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n";
  Serial.println(req);
  client.print(req);
}

void setup() 
{
  Serial.begin(115200);
  connectToWiFi();
}

void loop() 
{
  static long lastReadingTime = 0;
  long now = millis();
  if (now > lastReadingTime + sendPeriod)
  {
    int reading = analogRead(A0);
    sendToIFTTT(reading);
    lastReadingTime = now;
  }
  if (client.available())
  {
    Serial.write(client.read());
  }
}

```


