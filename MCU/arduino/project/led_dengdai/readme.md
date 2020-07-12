# led 灯带控制

[Adafruit_NeoPixel 灯带控制库](https://github.com/adafruit/Adafruit_NeoPixel)

```c

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// 按钮 的引脚编号 2号  上拉   按钮一端接地(GND)  一端接 单片机的2号针脚
#define BUTTON_PIN   2

// 控制 WS2812 灯条的引脚编号 6号   灯带的数据线 接 单片机的6号针脚
#define PIXEL_PIN    6 
  
// 定义控制的 LED 数量  20*2
#define PIXEL_COUNT 40   

// 定义 led灯条软件控制器
Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

boolean oldState = HIGH;
int     red_color_led_id = 0;    // 显示红色的 灯的id (0-19) 
int i=0;

// 系统初始化函数（执行一次）
void setup() 
{
  // 按钮初始化
  pinMode(BUTTON_PIN, INPUT_PULLUP); // 按钮接口上拉 到5v
  // 灯带控制器初始化
  strip.begin(); 
  strip.show();  
  // 初始化随机数种子
  randomSeed(analogRead(0));
}

// 循环执行的函数
void loop() 
{
  // 读取按键状态
  boolean newState = digitalRead(BUTTON_PIN);
  // 按键由 高电平  变成低电平
  if((newState == LOW) && (oldState == HIGH)) {
    // 延时20ms
    delay(20);
    // 按键确实被按下
    newState = digitalRead(BUTTON_PIN);
    if(newState == LOW) 
    {
         // 随机指定一个灯为红色
         red_color_led_id = random(20); // 产生一个 0-19的随机数
         // 控制灯显示颜色
         oneRed(red_color_led_id);
    }
  }
  // 更新按键状态
  oldState = newState;
}

// 指定 (0-19)中的某个灯显示红色，其余绿色灯
void oneRed(int red_led_id) 
{
  // 先全部显示绿色
  for(i=0; i<strip.numPixels(); i++)   // 0-39个编号的灯
  { 
    strip.setPixelColor(i, strip.Color(  0,   0, 255));// blue 绿色    (  0, 255,   0) // 蓝色
    strip.show();                          // 更新显示
  }
  // 指定灯显示 红色
  strip.setPixelColor(red_led_id*2, strip.Color(  255,   0, 0));
  strip.show();  
  strip.setPixelColor(red_led_id*2+1, strip.Color(  255,   0, 0));
  strip.show();    
}


```
