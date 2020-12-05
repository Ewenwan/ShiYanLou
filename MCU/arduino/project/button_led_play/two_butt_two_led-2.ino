/*
 * 程序功能
 * 两个按钮控制两条灯带
*/

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// 按钮
#define BUTTORN_IO_A    2  // 2号io口
#define BUTTORN_IO_B    3  // 3号io口
// LED 灯带接口
#define PIN_LED_IO_A 7  // 4号io口 led灯带1
#define PIN_LED_IO_B 9  // 5号io口 led灯带2

// ===================需要调试的参数==============================[
#define DEBUG  1        // 1 开启串口打印调试信息  0 关闭打印信息
//定义控制的 LED 数量
#define PIXEL_COUNT_A 100  
#define PIXEL_COUNT_B 100  
// 灯 点亮时间间隔 ms  1000ms = 1s                 // 这里调整 时间间隔 
#define LED_A_TIME_SCOPE 3//300  
#define LED_B_TIME_SCOPE 3//300 
// 灯全亮后 等待时间间隔 ms   10000 ms = 10s
#define LED_A_TIME_WAIT  1000//10000  
#define LED_B_TIME_WAIT  1000//10000 
// 初始灯颜色
int LED_A_INIT_COLOR[3] = {255,   255,   0};      //  这里调整颜色
int LED_B_INIT_COLOR[3] = {255,   255,   0};
// 按键后灯颜色
int LED_A_RUN_COLOR[3] = {255,   0,   0};
int LED_B_RUN_COLOR[3] = {0,   255,   0};

//====================================================================

// 全局变量
//bool init_state = true;
boolean oldState_A = HIGH;
boolean oldState_B = HIGH;

// 定义 led灯条软件控制器
Adafruit_NeoPixel strip_A(PIXEL_COUNT_A, PIN_LED_IO_A, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_B(PIXEL_COUNT_B, PIN_LED_IO_B, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

// 设置灯全亮，无等待   初始化
void set_led_color(Adafruit_NeoPixel *strip, int color[3])
{
  for(int i=0; i<strip->numPixels(); i++) 
  {
     strip->setPixelColor(i, strip->Color(color[0],  color[1], color[2]));
  }
  strip->show(); 
}

// 按键触发后，灯显示
void run_led_color(Adafruit_NeoPixel *strip, int color[3], int scope_time, int wait_time)
{
   if(DEBUG)
   {
       Serial.println("BUTTORN PUSH, LED RUN..");
   }
   
  for(int i=0; i<strip->numPixels(); i++) 
  {
     strip->setPixelColor(i, strip->Color(color[0],  color[1], color[2]));
     strip->show(); 
     delay(scope_time); //灯亮间隔
  }
   // 保持 状态延时
   delay(wait_time); //灯亮间隔
   // 恢复原始状态
   //set_led_color(strip, color);
   if(DEBUG)
   {
       Serial.println("BUTTORN PUSH, LED EDN..");
   }
}

void setup() 
{
   // 串口打印 调试信息
   Serial.begin(9600);
   Serial.println("SYSTEM RUNNING..");
   ////引脚初始化/////  
   pinMode(BUTTORN_IO_A, INPUT_PULLUP);//设置为输入上拉状态 INPUT_PULLUP  INPUT 按钮接口上拉 到5v
   pinMode(BUTTORN_IO_B, INPUT_PULLUP);//设置为输入上拉状态 INPUT_PULLUP  INPUT 按钮接口上拉 到5v
   // 灯初始化
   strip_A.begin(); // 
   strip_B.begin(); // 
   set_led_color(&strip_A, LED_A_INIT_COLOR);
   set_led_color(&strip_B, LED_B_INIT_COLOR);
}

void loop() {
  
  // 读取按键A状态
  boolean newState_A = digitalRead(BUTTORN_IO_A);
  // 按键由 高电平  变成低电平
  if((newState_A == LOW) && (oldState_A == HIGH)) 
  {
    delay(20);// 延时20ms 防止抖动
    // Check if button is still low after debounce.
    newState_A = digitalRead(BUTTORN_IO_A);
    if(newState_A == LOW) 
    { // 确实为低电平
      Serial.println("buttorn a push");
      run_led_color(&strip_A, LED_A_RUN_COLOR, LED_A_TIME_SCOPE, LED_A_TIME_WAIT); // 控制灯亮
    
/*
// 逐个点亮为 按键触发状态颜色
for(int i=0; i<strip_A.numPixels(); i++) 
  {
     strip_A.setPixelColor(i, strip_A.Color(LED_A_RUN_COLOR[0], LED_A_RUN_COLOR[1], LED_A_RUN_COLOR[2]));
     strip_A.show(); 
     delay(LED_A_TIME_SCOPE); //灯亮间隔
  }
   // 保持 状态延时
   delay(LED_A_TIME_WAIT); //灯亮间隔
 */

    // 复原为初始状态
    set_led_color(&strip_A,LED_A_INIT_COLOR );
    /*
    for(int i=0; i<strip_A.numPixels(); i++) 
    {
       strip_A.setPixelColor(i, strip_A.Color(LED_A_INIT_COLOR[0],  LED_A_INIT_COLOR[1], LED_A_INIT_COLOR[2]));
    }
    strip_A.show(); 
    */
    }
  }
  oldState_A = newState_A; // 更新按键状态

///*
  // 读取按键B状态
  boolean newState_B = digitalRead(BUTTORN_IO_B);
  // 按键由 高电平  变成低电平
  if((newState_B == LOW) && (oldState_B == HIGH)) 
  {
    delay(20);// 延时20ms 防止抖动
    // Check if button is still low after debounce.
    newState_B = digitalRead(BUTTORN_IO_B);
    if(newState_B == LOW) 
    { // 确实为低电平
      Serial.println("buttorn b push");
      run_led_color(&strip_B, LED_B_RUN_COLOR, LED_B_TIME_SCOPE, LED_B_TIME_WAIT); // 控制灯亮
      /*
      for(int i=0; i<strip_B.numPixels(); i++) 
        {
           strip_B.setPixelColor(i, strip_B.Color(LED_B_RUN_COLOR[0], LED_B_RUN_COLOR[1], LED_B_RUN_COLOR[2]));
           strip_B.show(); 
           delay(LED_B_TIME_SCOPE); //灯亮间隔
        }
       // 保持 状态延时
       delay(LED_B_TIME_WAIT); //灯亮间隔
      */
        // 复原为初始状态
        set_led_color(&strip_B,LED_B_INIT_COLOR );
        /*
        for(int i=0; i<strip_B.numPixels(); i++) 
        {
           strip_B.setPixelColor(i, strip_B.Color(LED_B_INIT_COLOR[0],  LED_B_INIT_COLOR[1], LED_B_INIT_COLOR[2]));
        }
        strip_B.show();
        */ 
    }
  }
  oldState_B = newState_B; // 更新按键状态
//*/
  
  delay(10);
}


