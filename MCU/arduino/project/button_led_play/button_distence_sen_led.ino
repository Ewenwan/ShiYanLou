

// led 灯控制
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

//===========================引脚和数量配置 ================================
//=========================================================================
// 模拟按钮 的引脚编号 A0号       5V-------10K电阻--------------10K电阻------ 开关 --------GND
//                                                    | 引线接 A0 模拟量测量
// 5v 串接两个 10K电阻，两电阻中间引线接 A0 模拟量测量 ，开关一端接GND，一端接电阻（远离5v的电阻）
#define BUTTON_PIN  A0
//======企鹅大屏灯===========
#define PIXEL_PIN    6                    // 灯带的数据线 接 单片机的6号针脚
#define PIXEL_COUNT 900                   // led灯数量  40 可修改 按实际情况填写

// 超声模块
const int  trigger_io = 4;  // 触发口 Triger 端口号
const int  pwm_io_1 = 2;     // 超声   Echo   端口号

// 定义 企鹅 led灯条软件控制器
Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)


int sensorValue = 0; // 按钮　模拟量测量值　　没按下　大于1000  按下 小于 800
int pust_thread = 800;
bool pust_flg = false;

// 超声测量 判断参数
int sensor_thread = 50; // 厘米单位 阈值
bool sensor_flg = false;


// 灯亮的时间 
int led_on_times = 1000;
bool led_on_flg = false;

//======================================初始化函数=============================
//=============================================================================

volatile int Infrared_1 = 0;//1号红外测距数据
unsigned long DYP_PulseStartTicks_1;
int   dyp1_failure_counter = 0;
//------ 超声模块测量 -----------  中断测量
void DYP_plus_measure(void)
{
  //dyp1_failure_counter = 0;
  if (digitalRead( pwm_io_1 ) == HIGH)// 低电平到高电平的 时间点
    DYP_PulseStartTicks_1 = micros();    // 微秒 us  
  else// 高电平变成低电平的 时间点
    // DYP_val = micros() - DYP_PulseStartTicks_1;
    Infrared_1 = (micros() - DYP_PulseStartTicks_1);// / 58.0;
        //Infrared_1 = DYP_val / 58.0;// 换算得到 距离  这里需要查询手册，一般是 声速*时间/2 =  340*秒时间/2
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(100);
  Serial.println("init sys...");

  // 灯带控制器初始化
  strip.begin(); 
  strip.clear(); // 全灭
  strip.show(); 

  //超声初始化
  pinMode( trigger_io, OUTPUT);  // 触发口 输出
  digitalWrite(trigger_io, LOW);
  
  pinMode(pwm_io_1, INPUT_PULLUP);          // 中断输入口  上拉，确保不接传感器是，电平稳定，不然会经常进端口中断
  attachInterrupt(0, DYP_plus_measure, CHANGE); // 电平改变触发, DYP_plus_measure中断触发函数 
  
  Serial.println("sys runing ...");

  
  // 灯带随机颜色   支持 按键响应
  // init_rainbow(1); // 彩虹随机色


}
//======================================初始化函数结束=============================
//================================================================================


//======================================循环函数==================================
//================================================================================
void loop() 
{
  //Serial.println("loop");
  sensorValue = analogRead(BUTTON_PIN);
  if(pust_thread > sensorValue)
  {
    // 测量值小于阈值，则有可能被按下
    // 放抖动
    // 延时10ms
    delay(10);
    // 按键确实被按下
    sensorValue = analogRead(BUTTON_PIN);
    if(pust_thread > sensorValue)
    {
      pust_flg = true;
    }
    else
    {
      pust_flg = false;
    }
  }
  else
  {
    pust_flg = false;
  }

  if (pust_flg)
  {
    led_on_flg = true;
    init_rainbow(1); // 彩虹随机色
  }
  else
  {
    led_on_flg = false;
  }

  if(!led_on_flg){
      // 超声波测量
      digitalWrite(trigger_io, HIGH); // 触发口 高电平 4ms以上
      delay(4);// 4ms
      digitalWrite(trigger_io, LOW);  // 变为低电平
    
      // echo 口会返回中断信号 中断函数会测试具体的数据
      Infrared_1 = Infrared_1/58;  // 得到测量距离(厘米)　　Infrared_1_filter = (Infrared_1_filter*3 + Infrared_1*2)/5.0;// 数值滤波
      // Infrared_1得到　微妙　340 * Infrared_1 / 1000 / 1000 / 2 = Infrared_1 *170 / 1000 / 1000 米
      // Infrared_1 *170 /  1000 / 1000 * 100 厘米 = Infrared_1/58    　
      if (sensor_thread > Infrared_1)
      {
          // 测量值小于阈值
          digitalWrite(trigger_io, HIGH); // 触发口 高电平 4ms以上
          delay(4);// 4ms
          digitalWrite(trigger_io, LOW);  // 变为低电平
          delay(10);// 10ms
    
          if (sensor_thread > Infrared_1)
          {
              sensor_flg = true;
          }
          else
          {
              sensor_flg = false;
          }
      }
      else
      {
          sensor_flg = false;
      }

      if (sensor_flg)
      {
        led_on_flg = true;
        init_rainbow(1); // 彩虹随机色
      }
      else
      {
        led_on_flg = false;
      }
      
   }
  
}


/*
// 开关量 按键检测
boolean  check_buttorn()
{
  boolean buttorn_push_flag = false;
  // 读取按键状态
  boolean newState_2 = digitalRead(BUTTON_PIN);
  // 按键由 高电平  变成低电平
  if((newState_2 == LOW) && (oldState_2 == HIGH)) {
    // 延时10ms
    delay(10);
    // 按键确实被按下
    newState_2 = digitalRead(BUTTON_PIN);
    if(newState_2 == LOW) // 控制奖台灯 三个灯闪
    {
      buttorn_push_flag =  true;
    }
  }
  oldState_2 = newState_2;
  return buttorn_push_flag;
}
*/

// 初始化彩虹色，一直闪亮，支持按键退出
void init_rainbow(int wait) {
  // Hue of first pixel runs 3 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 3*65536. Adding 256 to firstPixelHue each time
  // means we'll make 3*65536/256 = 768 passes through this outer loop:
  //while(true){
  for (int i=0; i<led_on_times; i++){
  for(long firstPixelHue = 0; firstPixelHue < 3*65536; firstPixelHue += 256) {
    
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    //delay(wait);  // Pause for a moment
  }
 }

 led_on_flg = false;
}

