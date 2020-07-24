#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

//===========================引脚和数量配置 ================================
//=========================================================================
// 按钮 的引脚编号 2号  上拉   按钮一端接地(GND)  一端接 单片机的2号针脚
#define BUTTON_PIN  2
//======企鹅大屏灯===========
#define PIXEL_PIN    6                    // 灯带的数据线 接 单片机的6号针脚
#define PIXEL_COUNT 900                    // led灯数量  40 可修改 按实际情况填写
//======一等奖灯带===========
#define PIXEL_PIN_First_Prize   12        // 一等奖灯带 引脚编号 12
#define PIXEL_COUNT_First_Prize 200        // led灯数量  可修改 按实际情况填写
//======二等奖灯带===========
#define PIXEL_PIN_Second_Prize  13        // 二等奖灯带 引脚编号  13
#define PIXEL_COUNT_Second_Prize 200       // led灯数量  可修改 按实际情况填写
//======三等奖灯带===========
#define PIXEL_PIN_Third_Prize   14        // 三等奖灯带 引脚编号 14
#define PIXEL_COUNT_Third_Prize 200        // led灯数量  可修改 按实际情况填写

//  新添加 ===================== new ====
#define Big_Led_Channel (PIXEL_COUNT/3)
#define Big_Led_beg_1   (0)
#define Big_Led_beg_2   (2*PIXEL_COUNT/15)
#define Big_Led_beg_3   (4*PIXEL_COUNT/15)
#define Big_Led_beg_4   (6*PIXEL_COUNT/15)
#define Big_Led_beg_5   (8*PIXEL_COUNT/15)
#define Big_Led_beg_6   (10*PIXEL_COUNT/15)

//   新 增  ===============================
#define Big_Led_Control (PIXEL_COUNT/3)            // 3个小灯一个单位  900/3  = 300
#define Big_Led_color_1_num   (Big_Led_Control/6)  // 大灯 6个控制单位 6种颜色灯 每个单位的数量
#define Big_Led_color_2_num   (Big_Led_Control/6)  // 现在是均分  可修改 
#define Big_Led_color_3_num   (Big_Led_Control/6)
#define Big_Led_color_4_num   (Big_Led_Control/6)
#define Big_Led_color_5_num   (Big_Led_Control/6)
#define Big_Led_color_6_num   (Big_Led_Control/6)


// 每个单位 灯的 位置坐标  0~299 索引
int big_led_color_1_loc_arr[Big_Led_color_1_num]; // 1单位 位置池
int big_led_color_2_loc_arr[Big_Led_color_2_num];
int big_led_color_3_loc_arr[Big_Led_color_3_num]; // 3单位 位置池
int big_led_color_4_loc_arr[Big_Led_color_4_num];
int big_led_color_5_loc_arr[Big_Led_color_5_num];
int big_led_color_6_loc_arr[Big_Led_color_6_num]; // 6单位 位置池
int big_led_color_loc_arr[Big_Led_Control];       // 总位置池 0~299
// ========================================================================

#define PRIZE_LED_THREE_BLINK_CONTOR     //控制 讲台三个灯 闪亮
#define ENABLE_DEBUG_INFO                //打印调试信息

// 喇叭 控制口 
#define TONE_PIN   4
int tone_music[7];

typedef enum _LED_COLOR_{
  READ = 0,
  GREEN,
  BLUE,
} LED_COLOR;

// 各奖项灯颜色
#define First_Prize_Led_Color  READ  //一等奖 灯颜色
#define Second_Prize_Led_Color GREEN //二等奖 灯颜色
#define Third_Prize_Led_Color  BLUE  //三等奖 灯颜色


//=============================各奖项数量配置============================================
//======================================================================================
#define First_Prize_Num  50       //一等奖数量  50   可修改
#define Second_Prize_Num  75         //二等奖数量  75   可修改
#define Third_Prize_Num   125        //三等奖数量  125  可修改
#define ALL_Prize_Num  (First_Prize_Num + Second_Prize_Num + Third_Prize_Num) // 奖项总数
int  result_prize_num = ALL_Prize_Num; // 剩余奖项总数 变量
int Prize_pool_vector[ALL_Prize_Num];// 总奖池
// =====================================================================================


// ============游戏参数 ================================================================
#define PLAY_TIME 50  // 可修改 50，差不多4-5秒  改大  游戏中 灯闪亮时间变长，反之减少
#define DELAY_MS  50  // 闪烁间隔 ms
#define LAST_ONE_DELAY_TIME  3000  // 1000为 1s
// ====================================================================================

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
// 一等奖 led
Adafruit_NeoPixel strip_First_Prize(PIXEL_COUNT_First_Prize, PIXEL_PIN_First_Prize, NEO_GRB + NEO_KHZ800);
// 二等奖 led
Adafruit_NeoPixel strip_Second_Prize(PIXEL_COUNT_Second_Prize, PIXEL_PIN_Second_Prize, NEO_GRB + NEO_KHZ800);
// 三等奖 led
Adafruit_NeoPixel strip_Third_Prize(PIXEL_COUNT_Third_Prize, PIXEL_PIN_Third_Prize, NEO_GRB + NEO_KHZ800);

// 变量定义
boolean oldState = HIGH;
boolean oldState_2 = HIGH;
int     probility_num = 0;    // 随机选一个 奖票
int     cur_prize_num = 3;        // 对应的奖项
int     i=0;
int     random_state = 0;
int     button_push_status = 0;
LED_COLOR  big_led_color = Third_Prize_Led_Color; // 企鹅大灯颜色
uint32_t big_led_color_arr[6]; // 企鹅大灯随机色颜色池
uint32_t pri_led_color_arr[3]; // 奖台灯随机色颜色池

//======================================初始化函数=============================
//=============================================================================
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(5000);
  Serial.println("init sys...");

  // 按钮初始化
  pinMode(BUTTON_PIN, INPUT_PULLUP); // 按钮接口上拉 到5v

  // 初始化随机数种子
  randomSeed(analogRead(0));

  // 生成奖池
  Serial.println("gengrate prize pool...");
  int ii;
  for (ii=0; ii < First_Prize_Num; ++ii)
  {
    Prize_pool_vector[ii] = 1; // 1等奖
  }
  for (ii=First_Prize_Num; ii < First_Prize_Num + Second_Prize_Num ; ++ii)
  {
    Prize_pool_vector[ii] = 2; // 2等奖
  }
  for (ii=First_Prize_Num + Second_Prize_Num; ii < ALL_Prize_Num ; ++ii)
  {
    Prize_pool_vector[ii] = 3; // 3等奖
  }

  // 打乱
  Serial.println("gengrate color pool...");
  // 初始化 颜色池
  big_led_color_arr[0] =  strip.Color(  255,   0,   0);    // red      大灯 红色 定义  如果不对，需要调整255的位置，使得显示为 红色  =============注意==============
  big_led_color_arr[1] =  strip.Color(  0,     255, 0);    // green    大灯 绿色 定义  如果不对，需要调整255的位置，使得显示为 绿色
  big_led_color_arr[2] =  strip.Color(  0,     0,   255);  // blue     大灯 蓝色 定义  如果不对，需要调整255的位置，使得显示为 蓝色
  big_led_color_arr[3] =  strip.Color(  255,   255, 0);    // red+green
  big_led_color_arr[4] =  strip.Color(  255,   0,   255);  // red+blue
  big_led_color_arr[5] =  strip.Color(  0,     255, 255);  // green+blue
  
  Serial.println("gengrate led loc  pool...");
  // 位置池初始化
  int jj =0;
  for (jj=0; jj < Big_Led_Control;  jj++) big_led_color_loc_arr[jj] = jj;  // 总位置池初始化
  // 随机生成1~6单位 灯组位置池
  int loc_total = Big_Led_Control; // 总位置数量
  int choose_loc = 0;
  // 生成 1单位 灯组位置池
  for (jj=0; jj < Big_Led_color_1_num;  jj++){
      choose_loc = random(loc_total); // 0~n
      loc_total = loc_total -1;
      big_led_color_1_loc_arr[jj] = big_led_color_loc_arr[choose_loc]; // 随机选出位置
      // 更新位置池
      update_big_led_loc_pool(choose_loc, loc_total);
      //Serial.println(big_led_color_1_loc_arr[jj]);
  }
  Serial.println("big_led_color_1_loc_arr");
  // 生成 2单位 灯组位置池
  for (jj=0; jj < Big_Led_color_2_num;  jj++){
      choose_loc = random(loc_total); // 0~n
      loc_total = loc_total -1;
      big_led_color_2_loc_arr[jj] = big_led_color_loc_arr[choose_loc]; // 随机选出位置
      // 更新位置池
      update_big_led_loc_pool(choose_loc, loc_total);
      //Serial.println(big_led_color_2_loc_arr[jj]);
  }
  Serial.println("big_led_color_2_loc_arr");
  // 生成 3单位 灯组位置池
  for (jj=0; jj < Big_Led_color_3_num;  jj++){
      choose_loc = random(loc_total); // 0~n
      loc_total = loc_total -1;
      big_led_color_3_loc_arr[jj] = big_led_color_loc_arr[choose_loc]; // 随机选出位置
      // 更新位置池
      update_big_led_loc_pool(choose_loc, loc_total);
      //Serial.println(big_led_color_3_loc_arr[jj]);
  }
  Serial.println("big_led_color_3_loc_arr");
  // 生成 4单位 灯组位置池
  for (jj=0; jj < Big_Led_color_4_num;  jj++){
      choose_loc = random(loc_total); // 0~n
      loc_total = loc_total -1;
      big_led_color_4_loc_arr[jj] = big_led_color_loc_arr[choose_loc]; // 随机选出位置
      // 更新位置池
      update_big_led_loc_pool(choose_loc, loc_total);
      //Serial.println(big_led_color_4_loc_arr[jj]);
  }
  Serial.println("big_led_color_4_loc_arr");
  // 生成 5单位 灯组位置池
  for (jj=0; jj < Big_Led_color_5_num;  jj++){
      choose_loc = random(loc_total); // 0~n
      loc_total = loc_total -1;
      big_led_color_5_loc_arr[jj] = big_led_color_loc_arr[choose_loc]; // 随机选出位置
      // 更新位置池
      update_big_led_loc_pool(choose_loc, loc_total);
      //Serial.println(big_led_color_5_loc_arr[jj]);
  }
  Serial.println("big_led_color_5_loc_arr");
  // 生成 6单位 灯组位置池
  for (jj=0; jj < Big_Led_color_6_num;  jj++){
      choose_loc = random(loc_total); // 0~n
      loc_total = loc_total -1;
      big_led_color_6_loc_arr[jj] = big_led_color_loc_arr[choose_loc]; // 随机选出位置
      // 更新位置池
      update_big_led_loc_pool(choose_loc, loc_total);
      //Serial.println(big_led_color_6_loc_arr[jj]);
  }
  Serial.println("big_led_color_6_loc_arr");

  Serial.println("gengrate music  pool...");
  // 音乐池
  tone_music[0] = 523;
  tone_music[1] = 587;
  tone_music[2] = 659;
  tone_music[3] = 698;
  tone_music[4] = 784;
  tone_music[5] = 880;
  tone_music[6] = 1046;
  
  Serial.println("strip init...");
  // 灯带控制器初始化
  strip.begin(); 
  strip.show(); 
  strip_First_Prize.begin();
  strip_First_Prize.show();
  strip_Second_Prize.begin();
  strip_Second_Prize.show();  
  strip_Third_Prize.begin();
  strip_Third_Prize.show();  
  
  Serial.println("sys runing ...");
  
  // 灯带随机颜色   支持 按键响应
  // init_rainbow(1); // 彩虹随机色

   // ====================初始全亮==========================    可去掉
  /*
  for(int i = 0; i < strip.numPixels(); i++) {
      int rand_id = random(6); // 0~5
      strip.setPixelColor(i, big_led_color_arr[rand_id]); 
  }
  strip.show();
   
  for(int i = 0; i < strip_First_Prize.numPixels(); i++) {
      int rand_id = random(3); // 0~2
      strip_First_Prize.setPixelColor(i, big_led_color_arr[rand_id]); 
  }
  strip_First_Prize.show();
  
  for(int i = 0; i < strip_Second_Prize.numPixels(); i++) {
      int rand_id = random(3); // 0~2
      strip_Second_Prize.setPixelColor(i, big_led_color_arr[rand_id]); 
  }
  strip_Second_Prize.show();
  
  for(int i = 0; i < strip_Third_Prize.numPixels(); i++) {
      int rand_id = random(3); // 0~2
      strip_Third_Prize.setPixelColor(i, big_led_color_arr[rand_id]); 
  }
  strip_Third_Prize.show();
  */
  
   // 循环闪亮
  while(true){
    // for (int cnt_my=0; cnt_my<200; cnt_my++){        // 注释上面  while(true) 打开这里 可实现  初始状态 灯闪烁一会后 停止闪烁
    for (int count_num=0; count_num<600; count_num++){
    if (check_buttorn()) return;

    /*
    for(int num = 0; num < strip.numPixels(); num++) {
      int rand_id = random(6); // 0~5
      strip.setPixelColor(num, big_led_color_arr[rand_id]); 
     }
     strip.show();
     
      int lit_led_state = count_num%3; // 0  / 1 /2 
      if (0 == lit_led_state) {
        for(int num2 = 0; num2 < strip_First_Prize.numPixels(); num2++) strip_First_Prize.setPixelColor(num2, big_led_color_arr[0]);
        for(int num2 = 0; num2 < strip_Second_Prize.numPixels(); num2++) strip_Second_Prize.setPixelColor(num2, big_led_color_arr[1]);
        for(int num2 = 0; num2 < strip_Third_Prize.numPixels(); num2++) strip_Third_Prize.setPixelColor(num2, big_led_color_arr[2]);
        }
      else if(1 == lit_led_state) {
        for(int num2 = 0; num2 < strip_First_Prize.numPixels(); num2++) strip_First_Prize.setPixelColor(num2, big_led_color_arr[2]);
        for(int num2 = 0; num2 < strip_Second_Prize.numPixels(); num2++) strip_Second_Prize.setPixelColor(num2, big_led_color_arr[0]);
        for(int num2 = 0; num2 < strip_Third_Prize.numPixels(); num2++) strip_Third_Prize.setPixelColor(num2, big_led_color_arr[1]);
      }
      else {
        for(int num2 = 0; num2 < strip_First_Prize.numPixels(); num2++) strip_First_Prize.setPixelColor(num2, big_led_color_arr[1]);
        for(int num2 = 0; num2 < strip_Second_Prize.numPixels(); num2++) strip_Second_Prize.setPixelColor(num2, big_led_color_arr[2]);
        for(int num2 = 0; num2 < strip_Third_Prize.numPixels(); num2++) strip_Third_Prize.setPixelColor(num2, big_led_color_arr[0]); 
      }
      strip_First_Prize.show();
      strip_Second_Prize.show();
      strip_Third_Prize.show();
      */

      // 大灯部分等亮 ======================  更新的部分 =====////////////////////////////////////////
      // 先全灭
      strip.clear();
      int big_led_state = count_num%6; // 0  / 1 /2/3/4/5 
      /*
      int beg_led_begin_idx=0;
      if (0 == big_led_state)      beg_led_begin_idx = Big_Led_beg_1;
      else if (1 == big_led_state) beg_led_begin_idx = Big_Led_beg_2;
      else if (2 == big_led_state) beg_led_begin_idx = Big_Led_beg_3;
      else if (3 == big_led_state) beg_led_begin_idx = Big_Led_beg_4;
      else if (4 == big_led_state) beg_led_begin_idx = Big_Led_beg_5;
      else                         beg_led_begin_idx = Big_Led_beg_6;
      // 指定范围 大灯亮
      for(int num = beg_led_begin_idx; num < beg_led_begin_idx+Big_Led_Channel; num++)  strip.setPixelColor(num, big_led_color_arr[big_led_state]); 
      
      */
      //  new  新 模式  ========================
      if (0 == big_led_state) {   // 1号灯组亮
               for(int num = 0; num < Big_Led_color_1_num; num++)  
                   {
                     // 3个灯一个单位 控制
                     int bge_loc = big_led_color_1_loc_arr[num] *3 ;
                     strip.setPixelColor(bge_loc, big_led_color_arr[0]);
                     strip.setPixelColor(bge_loc+1, big_led_color_arr[0]);
                     strip.setPixelColor(bge_loc+2, big_led_color_arr[0]);
                   }
      }
      else if (1 == big_led_state){// 2号灯组亮
               for(int num = 0; num < Big_Led_color_2_num; num++)  
                   {
                     // 3个灯一个单位 控制
                     int bge_loc = big_led_color_2_loc_arr[num] * 3 ;
                     strip.setPixelColor(bge_loc, big_led_color_arr[1]);
                     strip.setPixelColor(bge_loc+1, big_led_color_arr[1]);
                     strip.setPixelColor(bge_loc+2, big_led_color_arr[1]);
                   }
      }
      else if (2 == big_led_state) {// 3号灯组亮
               for(int num = 0; num < Big_Led_color_3_num; num++)  
                   {
                     // 3个灯一个单位 控制
                     int bge_loc = big_led_color_3_loc_arr[num] * 3 ;
                     strip.setPixelColor(bge_loc, big_led_color_arr[2]);
                     strip.setPixelColor(bge_loc+1, big_led_color_arr[2]);
                     strip.setPixelColor(bge_loc+2, big_led_color_arr[2]);
                   }
      }
      else if (3 == big_led_state){// 4号灯组亮
               for(int num = 0; num < Big_Led_color_4_num; num++)  
                   {
                     // 3个灯一个单位 控制
                     int bge_loc = big_led_color_4_loc_arr[num] * 3 ;
                     strip.setPixelColor(bge_loc, big_led_color_arr[3]);
                     strip.setPixelColor(bge_loc+1, big_led_color_arr[3]);
                     strip.setPixelColor(bge_loc+2, big_led_color_arr[3]);
                   }
      }
      else if (4 == big_led_state){// 5号灯组亮
               for(int num = 0; num < Big_Led_color_5_num; num++)  
                   {
                     // 3个灯一个单位 控制
                     int bge_loc = big_led_color_5_loc_arr[num] * 3 ;
                     strip.setPixelColor(bge_loc, big_led_color_arr[4]);
                     strip.setPixelColor(bge_loc+1, big_led_color_arr[4]);
                     strip.setPixelColor(bge_loc+2, big_led_color_arr[4]);
                   }
      }
      else {// 6号灯组亮
               for(int num = 0; num < Big_Led_color_6_num; num++)  
                   {
                     // 3个灯一个单位 控制
                     int bge_loc = big_led_color_6_loc_arr[num] * 3 ;
                     strip.setPixelColor(bge_loc, big_led_color_arr[5]);
                     strip.setPixelColor(bge_loc+1, big_led_color_arr[5]);
                     strip.setPixelColor(bge_loc+2, big_led_color_arr[5]);
                   }
      }

      strip.show();
      
      // 小灯
      strip_First_Prize.clear(); strip_Second_Prize.clear(); strip_Third_Prize.clear();  // 先全灭
      int lit_led_state = count_num%3; // 0  / 1 /2 
      if (0 == lit_led_state)     {for(int num2 = 0; num2 < strip_First_Prize.numPixels(); num2++)
                                  { if (READ  == First_Prize_Led_Color) strip_First_Prize.setPixelColor(num2, strip.Color(  255,   0,   0));  // 注意这里 脚灯 颜色，可能要调整 255的位置
                                    if (GREEN == First_Prize_Led_Color) strip_First_Prize.setPixelColor(num2, strip.Color(  0,   255,   0)); 
                                    if (BLUE  == First_Prize_Led_Color) strip_First_Prize.setPixelColor(num2, strip.Color(  0,   0,   255)); 
                                  }}
      else if(1 == lit_led_state) {for(int num2 = 0; num2 < strip_Second_Prize.numPixels(); num2++) 
                                  { if (READ  == Second_Prize_Led_Color) strip_Second_Prize.setPixelColor(num2, strip.Color(  255,   0,   0));  // 注意这里 脚灯 颜色，可能要调整 255的位置
                                    if (GREEN == Second_Prize_Led_Color) strip_Second_Prize.setPixelColor(num2, strip.Color(  0,   255,   0)); 
                                    if (BLUE  == Second_Prize_Led_Color) strip_Second_Prize.setPixelColor(num2, strip.Color(  0,   0,   255)); 
                                   }}
      else                        {for(int num2 = 0; num2 < strip_Third_Prize.numPixels(); num2++) 
                                  { if (READ  == Third_Prize_Led_Color) strip_Third_Prize.setPixelColor(num2, strip.Color(  255,   0,   0));  // 注意这里 脚灯 颜色，可能要调整 255的位置
                                    if (GREEN == Third_Prize_Led_Color) strip_Third_Prize.setPixelColor(num2, strip.Color(  0,   255,   0)); 
                                    if (BLUE  == Third_Prize_Led_Color) strip_Third_Prize.setPixelColor(num2, strip.Color(  0,   0,   255)); 
                                  }}
      strip_First_Prize.show();
      strip_Second_Prize.show();
      strip_Third_Prize.show();
      ///////////////////////////////////////////////////   上面有更新 ///////////////////////////////////////////////////
      
        
      delay(DELAY_MS);// 20ms
    }
  }

}
//======================================初始化函数结束=============================
//================================================================================

//======================================循环函数==================================
//================================================================================
void loop() {
  //Serial.println("loop");
  //================1. 游戏中  检测按键  不放回抽奖  ======================
  if(result_prize_num > 0)
  {
      // put your main code here, to run repeatedly:
      boolean newState = digitalRead(BUTTON_PIN);
      // 按键由 高电平  变成低电平
      if((newState == LOW) && (oldState == HIGH)) {
        // 延时20ms
        delay(40);
        // 按键确实被按下
        newState = digitalRead(BUTTON_PIN);
        if(newState == LOW) 
        {

             probility_num = random(result_prize_num); // 从奖池中 随机挑选出一个
             cur_prize_num = Prize_pool_vector[probility_num]; // 对应的奖项  1/2/3
             // 确定中奖显示的颜色
             if(1 == cur_prize_num)  big_led_color = First_Prize_Led_Color; // 一等奖
             else if(2 == cur_prize_num) big_led_color = Second_Prize_Led_Color; // 二等奖
             else  big_led_color = Third_Prize_Led_Color;                       // 三等奖 

             //tone(TONE_PIN,523); // 喇叭响
             // 控制企鹅大灯显示颜色
             //led_ranom_state_and_price_state(big_led_color);       
             // 控制讲台灯显示
             //three_prize_led_oneColor(big_led_color, cur_prize_num);
             //noTone(TONE_PIN); // 喇叭关

             // ==========企鹅大灯和 奖台灯闪亮======================================
             /*for(long firstPixelHue = 0; firstPixelHue < 3*65536; firstPixelHue += 1024) {
                // 企鹅大灯
                for(int i=0; i<strip.numPixels(); i++) 
                    { int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels()); strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));} strip.show(); 
                // 控制 将台灯闪烁
                //prize_led_three_blink(firstPixelHue);
                for(int i=0; i<strip_First_Prize.numPixels(); i++) 
                    { int pixelHue = firstPixelHue + (i * 65536L / strip_First_Prize.numPixels()); strip_First_Prize.setPixelColor(i, strip_First_Prize.gamma32(strip_First_Prize.ColorHSV(pixelHue)));} strip_First_Prize.show(); 
                for(int i=0; i<strip_Second_Prize.numPixels(); i++) 
                    { int pixelHue = firstPixelHue + (i * 65536L / strip_Second_Prize.numPixels()); strip_Second_Prize.setPixelColor(i, strip_Second_Prize.gamma32(strip_Second_Prize.ColorHSV(pixelHue)));} strip_Second_Prize.show(); 
                for(int i=0; i<strip_Third_Prize.numPixels(); i++) 
                    { int pixelHue = firstPixelHue + (i * 65536L / strip_Third_Prize.numPixels()); strip_Third_Prize.setPixelColor(i, strip_Third_Prize.gamma32(strip_Third_Prize.ColorHSV(pixelHue)));} strip_Third_Prize.show();      
              }*/
              for (int count_num=0; count_num < PLAY_TIME; count_num++){
                int indx = count_num%7;
                tone(TONE_PIN,  tone_music[indx]);

                /*
                for(int num = 0; num < strip.numPixels(); num++) {
                  int rand_id = random(6); // 0~5
                  strip.setPixelColor(num, big_led_color_arr[rand_id]); 
                 }
                 strip.show();
               
                int lit_led_state = count_num%3; // 0  / 1 /2 
                if (0 == lit_led_state) {
                  for(int num2 = 0; num2 < strip_First_Prize.numPixels(); num2++) strip_First_Prize.setPixelColor(num2, big_led_color_arr[0]);
                  for(int num2 = 0; num2 < strip_Second_Prize.numPixels(); num2++) strip_Second_Prize.setPixelColor(num2, big_led_color_arr[1]);
                  for(int num2 = 0; num2 < strip_Third_Prize.numPixels(); num2++) strip_Third_Prize.setPixelColor(num2, big_led_color_arr[2]);
                  }
                else if(1 == lit_led_state) {
                  for(int num2 = 0; num2 < strip_First_Prize.numPixels(); num2++) strip_First_Prize.setPixelColor(num2, big_led_color_arr[2]);
                  for(int num2 = 0; num2 < strip_Second_Prize.numPixels(); num2++) strip_Second_Prize.setPixelColor(num2, big_led_color_arr[0]);
                  for(int num2 = 0; num2 < strip_Third_Prize.numPixels(); num2++) strip_Third_Prize.setPixelColor(num2, big_led_color_arr[1]);
                }
                else {
                  for(int num2 = 0; num2 < strip_First_Prize.numPixels(); num2++) strip_First_Prize.setPixelColor(num2, big_led_color_arr[1]);
                  for(int num2 = 0; num2 < strip_Second_Prize.numPixels(); num2++) strip_Second_Prize.setPixelColor(num2, big_led_color_arr[2]);
                  for(int num2 = 0; num2 < strip_Third_Prize.numPixels(); num2++) strip_Third_Prize.setPixelColor(num2, big_led_color_arr[0]); 
                }
                strip_First_Prize.show();
                strip_Second_Prize.show();
                strip_Third_Prize.show();
                */
            
                // 大灯部分等亮 ======================  更新的部分 =====////////////////////////////////////////
                // 先全灭
                strip.clear();
                int big_led_state = count_num%6; // 0  / 1 /2/3/4/5 
                /*
                int beg_led_begin_idx=0;
                if (0 == big_led_state)      beg_led_begin_idx = Big_Led_beg_1;
                else if (1 == big_led_state) beg_led_begin_idx = Big_Led_beg_2;
                else if (2 == big_led_state) beg_led_begin_idx = Big_Led_beg_3;
                else if (3 == big_led_state) beg_led_begin_idx = Big_Led_beg_4;
                else if (4 == big_led_state) beg_led_begin_idx = Big_Led_beg_5;
                else                         beg_led_begin_idx = Big_Led_beg_6;
                // 指定范围 大灯亮
                for(int num = beg_led_begin_idx; num < beg_led_begin_idx+Big_Led_Channel; num++)  strip.setPixelColor(num, big_led_color_arr[big_led_state]); 
                */
                //  new  新 模式  ========================
                if (0 == big_led_state) {   // 1号灯组亮
                         for(int num = 0; num < Big_Led_color_1_num; num++)  
                             {
                               // 3个灯一个单位 控制
                               int bge_loc = big_led_color_1_loc_arr[num] *3 ;
                               strip.setPixelColor(bge_loc, big_led_color_arr[0]);
                               strip.setPixelColor(bge_loc+1, big_led_color_arr[0]);
                               strip.setPixelColor(bge_loc+2, big_led_color_arr[0]);
                             }
                }
                else if (1 == big_led_state){// 2号灯组亮
                         for(int num = 0; num < Big_Led_color_2_num; num++)  
                             {
                               // 3个灯一个单位 控制
                               int bge_loc = big_led_color_2_loc_arr[num] * 3 ;
                               strip.setPixelColor(bge_loc, big_led_color_arr[1]);
                               strip.setPixelColor(bge_loc+1, big_led_color_arr[1]);
                               strip.setPixelColor(bge_loc+2, big_led_color_arr[1]);
                             }
                }
                else if (2 == big_led_state) {// 3号灯组亮
                         for(int num = 0; num < Big_Led_color_3_num; num++)  
                             {
                               // 3个灯一个单位 控制
                               int bge_loc = big_led_color_3_loc_arr[num] * 3 ;
                               strip.setPixelColor(bge_loc, big_led_color_arr[2]);
                               strip.setPixelColor(bge_loc+1, big_led_color_arr[2]);
                               strip.setPixelColor(bge_loc+2, big_led_color_arr[2]);
                             }
                }
                else if (3 == big_led_state){// 4号灯组亮
                         for(int num = 0; num < Big_Led_color_4_num; num++)  
                             {
                               // 3个灯一个单位 控制
                               int bge_loc = big_led_color_4_loc_arr[num] * 3 ;
                               strip.setPixelColor(bge_loc, big_led_color_arr[3]);
                               strip.setPixelColor(bge_loc+1, big_led_color_arr[3]);
                               strip.setPixelColor(bge_loc+2, big_led_color_arr[3]);
                             }
                }
                else if (4 == big_led_state){// 5号灯组亮
                         for(int num = 0; num < Big_Led_color_5_num; num++)  
                             {
                               // 3个灯一个单位 控制
                               int bge_loc = big_led_color_5_loc_arr[num] * 3 ;
                               strip.setPixelColor(bge_loc, big_led_color_arr[4]);
                               strip.setPixelColor(bge_loc+1, big_led_color_arr[4]);
                               strip.setPixelColor(bge_loc+2, big_led_color_arr[4]);
                             }
                }
                else {// 6号灯组亮
                         for(int num = 0; num < Big_Led_color_6_num; num++)  
                             {
                               // 3个灯一个单位 控制
                               int bge_loc = big_led_color_6_loc_arr[num] * 3 ;
                               strip.setPixelColor(bge_loc, big_led_color_arr[5]);
                               strip.setPixelColor(bge_loc+1, big_led_color_arr[5]);
                               strip.setPixelColor(bge_loc+2, big_led_color_arr[5]);
                             }
                }
                
                strip.show();
            
                // 小灯
                strip_First_Prize.clear(); strip_Second_Prize.clear(); strip_Third_Prize.clear();  // 先全灭
                int lit_led_state = count_num%3; // 0  / 1 /2 
                if (0 == lit_led_state)     {for(int num2 = 0; num2 < strip_First_Prize.numPixels(); num2++)
                                            { if (READ  == First_Prize_Led_Color) strip_First_Prize.setPixelColor(num2, strip.Color(  255,   0,   0));  // 注意这里 脚灯 颜色，可能要调整 255的位置
                                              if (GREEN == First_Prize_Led_Color) strip_First_Prize.setPixelColor(num2, strip.Color(  0,   255,   0)); 
                                              if (BLUE  == First_Prize_Led_Color) strip_First_Prize.setPixelColor(num2, strip.Color(  0,   0,   255)); 
                                            }}
                else if(1 == lit_led_state) {for(int num2 = 0; num2 < strip_Second_Prize.numPixels(); num2++) 
                                            { if (READ  == Second_Prize_Led_Color) strip_Second_Prize.setPixelColor(num2, strip.Color(  255,   0,   0));  // 注意这里 脚灯 颜色，可能要调整 255的位置
                                              if (GREEN == Second_Prize_Led_Color) strip_Second_Prize.setPixelColor(num2, strip.Color(  0,   255,   0)); 
                                              if (BLUE  == Second_Prize_Led_Color) strip_Second_Prize.setPixelColor(num2, strip.Color(  0,   0,   255)); 
                                             }}
                else                        {for(int num2 = 0; num2 < strip_Third_Prize.numPixels(); num2++) 
                                            { if (READ  == Third_Prize_Led_Color) strip_Third_Prize.setPixelColor(num2, strip.Color(  255,   0,   0));  // 注意这里 脚灯 颜色，可能要调整 255的位置
                                              if (GREEN == Third_Prize_Led_Color) strip_Third_Prize.setPixelColor(num2, strip.Color(  0,   255,   0)); 
                                              if (BLUE  == Third_Prize_Led_Color) strip_Third_Prize.setPixelColor(num2, strip.Color(  0,   0,   255)); 
                                            }}
                strip_First_Prize.show();
                strip_Second_Prize.show();
                strip_Third_Prize.show();
                ///////////////////////////////////////////////////   上面有更新 ///////////////////////////////////////////////////
                

                
                delay(DELAY_MS);// 20ms
              }

             
             // ==========企鹅大灯 中奖色============================================
             // ====================================================================
             /*for(i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, strip.Color(  0,   0, 0)); 
              if (READ == big_led_color)
                 {for(i=0; i<300; i++) strip.setPixelColor(i, strip.Color(  0,   255, 0)); strip.show();}   //======== 修改适配 调整255的顺序 使得颜色正常
              else if (GREEN == big_led_color)
                 {for(i=301; i<600; i++) strip.setPixelColor(i, strip.Color(  255,255, 0)); strip.show();}     //======== 修改适配 调整255的顺序 使得颜色正常            
              else
                 {for(i=601; i<900; i++) strip.setPixelColor(i, strip.Color(  255,   0, 255)); strip.show();}   //======== 修改适配 调整255的顺序 使得颜色正常
             */
             
             // 更新　大灯中将　模式 ======================================
             strip.clear(); // 先全部灭
             if (READ == big_led_color){
              // 红灯区域亮
                 for(int num = 0; num < Big_Led_color_1_num; num++)  
                   {
                     // 3个灯一个单位 控制
                     int bge_loc = big_led_color_1_loc_arr[num] *3 ;
                     strip.setPixelColor(bge_loc, big_led_color_arr[0]);
                     strip.setPixelColor(bge_loc+1, big_led_color_arr[0]);
                     strip.setPixelColor(bge_loc+2, big_led_color_arr[0]);
                   }
             }
             else if (GREEN == big_led_color){
             // 绿灯区域亮
                 for(int num = 0; num < Big_Led_color_2_num; num++)  
                     {
                       // 3个灯一个单位 控制
                       int bge_loc = big_led_color_2_loc_arr[num] * 3 ;
                       strip.setPixelColor(bge_loc, big_led_color_arr[1]);
                       strip.setPixelColor(bge_loc+1, big_led_color_arr[1]);
                       strip.setPixelColor(bge_loc+2, big_led_color_arr[1]);
                     }
             }
             else{
             // 蓝灯区域亮
                 for(int num = 0; num < Big_Led_color_3_num; num++)  
                     {
                       // 3个灯一个单位 控制
                       int bge_loc = big_led_color_3_loc_arr[num] * 3 ;
                       strip.setPixelColor(bge_loc, big_led_color_arr[2]);
                       strip.setPixelColor(bge_loc+1, big_led_color_arr[2]);
                       strip.setPixelColor(bge_loc+2, big_led_color_arr[2]);
                     }
             }
             strip.show();

             
             // ==========奖台灯 中奖色==============================================
             // ====================================================================
              if(1 == cur_prize_num)
              {   
                  strip_Second_Prize.clear();strip_Second_Prize.show();
                  strip_Third_Prize.clear();strip_Third_Prize.show();
                  //strip_one_core(strip_First_Prize,big_led_color); // 1灯奖灯亮，其他灯灭
                  if (READ == big_led_color)
                     {for(i=0; i<strip_First_Prize.numPixels(); i++) strip_First_Prize.setPixelColor(i, strip_First_Prize.Color(  255,   0, 0)); strip_First_Prize.show(); }  //======== 修改适配 调整255的顺序 使得颜色正常
                  else if (GREEN == big_led_color)  
                     {for(i=0; i<strip_First_Prize.numPixels(); i++) strip_First_Prize.setPixelColor(i, strip_First_Prize.Color(  255,   0, 255)); strip_First_Prize.show(); }  //======== 修改适配 调整255的顺序 使得颜色正常
                  else
                     {for(i=0; i<strip_First_Prize.numPixels(); i++) strip_First_Prize.setPixelColor(i, strip_First_Prize.Color(  0,   255, 255)); strip_First_Prize.show(); }  //======== 修改适配 调整255的顺序 使得颜色正常
              }
              else if(2 == cur_prize_num) 
              {
                  strip_First_Prize.clear();strip_First_Prize.show();
                  strip_Third_Prize.clear();strip_Third_Prize.show();
                  //strip_one_core(strip_Second_Prize, big_led_color); // 2灯奖灯亮，其他灯灭
                  if (READ == big_led_color)
                     {for(i=0; i<strip_Second_Prize.numPixels(); i++) strip_Second_Prize.setPixelColor(i, strip_Second_Prize.Color(  255,   0, 0)); strip_Second_Prize.show();}//======== 修改适配 调整255的顺序 使得颜色正常 
                  else if (GREEN == big_led_color)
                     {for(i=0; i<strip_Second_Prize.numPixels(); i++) strip_Second_Prize.setPixelColor(i, strip_Second_Prize.Color( 255,   0, 255)); strip_Second_Prize.show();}//======== 修改适配 调整255的顺序 使得颜色正常
                  else 
                     {for(i=0; i<strip_Second_Prize.numPixels(); i++) strip_Second_Prize.setPixelColor(i, strip_Second_Prize.Color(  0,   255,255)); strip_Second_Prize.show();}//======== 修改适配 调整255的顺序 使得颜色正常
              }
              else
              {
                  strip_First_Prize.clear();strip_First_Prize.show();
                  strip_Second_Prize.clear();strip_Second_Prize.show();
                  //strip_one_core(strip_Third_Prize, big_led_color); // 3灯奖灯亮，其他灯灭
                  if (READ == big_led_color)
                     {for(i=0; i<strip_Third_Prize.numPixels(); i++) strip_Third_Prize.setPixelColor(i, strip_Third_Prize.Color(  255,   0, 0)); strip_Third_Prize.show();}//======== 修改适配 调整255的顺序 使得颜色正常
                  else if (GREEN == big_led_color)
                     {for(i=0; i<strip_Third_Prize.numPixels(); i++) strip_Third_Prize.setPixelColor(i, strip_Third_Prize.Color( 255,   0, 255)); strip_Third_Prize.show();}//======== 修改适配 调整255的顺序 使得颜色正常           
                  else
                     {for(i=0; i<strip_Third_Prize.numPixels(); i++) strip_Third_Prize.setPixelColor(i, strip_Third_Prize.Color(  0,   255, 255)); strip_Third_Prize.show();}//======== 修改适配 调整255的顺序 使得颜色正常        
              }

             noTone(TONE_PIN);
             
             // ==========从奖池中删除 已经选出的 奖项 =================================
             result_prize_num = result_prize_num - 1;  // 剩余奖项总数减1
             update_prize_pool(probility_num, result_prize_num);  
             Serial.println("push button");

             
             // ==========打印调试信息=================================================
             #ifdef ENABLE_DEBUG_INFO
             char debug_str[128];
             sprintf(debug_str, "pro_nm:%d pri_nm:%d res_nm:%d\n",probility_num, cur_prize_num, result_prize_num);
             Serial.println(debug_str);
             #endif

             if (0 == result_prize_num) delay(LAST_ONE_DELAY_TIME); // 最后一次 亮1s
        }
      }
      oldState = newState;
  }
  else
  //==============2. 游戏结束 ================================
  {
      Serial.println("ending ...");
      //end_rainbow(1); // 一直运行彩虹灯
      while(true){
      // for (int cnt_my=0; cnt_my<200; cnt_my++){        // 注释上面  while(true) 打开这里 可实现  初始状态 灯闪烁一会后 停止闪烁
      for (int count_num=0; count_num<600; count_num++){
      // if (check_buttorn()) return;
      /*
      for(int num = 0; num < strip.numPixels(); num++) {
        int rand_id = random(6); // 0~5
        strip.setPixelColor(num, big_led_color_arr[rand_id]); 
       }
       strip.show();
       
        int lit_led_state = count_num%3; // 0  / 1 /2 
        if (0 == lit_led_state) {
          for(int num2 = 0; num2 < strip_First_Prize.numPixels(); num2++) strip_First_Prize.setPixelColor(num2, big_led_color_arr[0]);
          for(int num2 = 0; num2 < strip_Second_Prize.numPixels(); num2++) strip_Second_Prize.setPixelColor(num2, big_led_color_arr[1]);
          for(int num2 = 0; num2 < strip_Third_Prize.numPixels(); num2++) strip_Third_Prize.setPixelColor(num2, big_led_color_arr[2]);
          }
        else if(1 == lit_led_state) {
          for(int num2 = 0; num2 < strip_First_Prize.numPixels(); num2++) strip_First_Prize.setPixelColor(num2, big_led_color_arr[2]);
          for(int num2 = 0; num2 < strip_Second_Prize.numPixels(); num2++) strip_Second_Prize.setPixelColor(num2, big_led_color_arr[0]);
          for(int num2 = 0; num2 < strip_Third_Prize.numPixels(); num2++) strip_Third_Prize.setPixelColor(num2, big_led_color_arr[1]);
        }
        else {
          for(int num2 = 0; num2 < strip_First_Prize.numPixels(); num2++) strip_First_Prize.setPixelColor(num2, big_led_color_arr[1]);
          for(int num2 = 0; num2 < strip_Second_Prize.numPixels(); num2++) strip_Second_Prize.setPixelColor(num2, big_led_color_arr[2]);
          for(int num2 = 0; num2 < strip_Third_Prize.numPixels(); num2++) strip_Third_Prize.setPixelColor(num2, big_led_color_arr[0]); 
        }
        strip_First_Prize.show();
        strip_Second_Prize.show();
        strip_Third_Prize.show();
        */
        // 大灯部分等亮 ======================  更新的部分 =====////////////////////////////////////////
        // 先全灭
        strip.clear();
        int big_led_state = count_num%6; // 0  / 1 /2/3/4/5 
        /*
        int beg_led_begin_idx=0;
        if (0 == big_led_state)      beg_led_begin_idx = Big_Led_beg_1;
        else if (1 == big_led_state) beg_led_begin_idx = Big_Led_beg_2;
        else if (2 == big_led_state) beg_led_begin_idx = Big_Led_beg_3;
        else if (3 == big_led_state) beg_led_begin_idx = Big_Led_beg_4;
        else if (4 == big_led_state) beg_led_begin_idx = Big_Led_beg_5;
        else                         beg_led_begin_idx = Big_Led_beg_6;
        // 指定范围 大灯亮
        for(int num = beg_led_begin_idx; num < beg_led_begin_idx+Big_Led_Channel; num++)  strip.setPixelColor(num, big_led_color_arr[big_led_state]); 
        */
  
        //  new  新 模式  ========================
        if (0 == big_led_state) {   // 1号灯组亮
                 for(int num = 0; num < Big_Led_color_1_num; num++)  
                     {
                       // 3个灯一个单位 控制
                       int bge_loc = big_led_color_1_loc_arr[num] *3 ;
                       strip.setPixelColor(bge_loc, big_led_color_arr[0]);
                       strip.setPixelColor(bge_loc+1, big_led_color_arr[0]);
                       strip.setPixelColor(bge_loc+2, big_led_color_arr[0]);
                     }
        }
        else if (1 == big_led_state){// 2号灯组亮
                 for(int num = 0; num < Big_Led_color_2_num; num++)  
                     {
                       // 3个灯一个单位 控制
                       int bge_loc = big_led_color_2_loc_arr[num] * 3 ;
                       strip.setPixelColor(bge_loc, big_led_color_arr[1]);
                       strip.setPixelColor(bge_loc+1, big_led_color_arr[1]);
                       strip.setPixelColor(bge_loc+2, big_led_color_arr[1]);
                     }
        }
        else if (2 == big_led_state) {// 3号灯组亮
                 for(int num = 0; num < Big_Led_color_3_num; num++)  
                     {
                       // 3个灯一个单位 控制
                       int bge_loc = big_led_color_3_loc_arr[num] * 3 ;
                       strip.setPixelColor(bge_loc, big_led_color_arr[2]);
                       strip.setPixelColor(bge_loc+1, big_led_color_arr[2]);
                       strip.setPixelColor(bge_loc+2, big_led_color_arr[2]);
                     }
        }
        else if (3 == big_led_state){// 4号灯组亮
                 for(int num = 0; num < Big_Led_color_4_num; num++)  
                     {
                       // 3个灯一个单位 控制
                       int bge_loc = big_led_color_4_loc_arr[num] * 3 ;
                       strip.setPixelColor(bge_loc, big_led_color_arr[3]);
                       strip.setPixelColor(bge_loc+1, big_led_color_arr[3]);
                       strip.setPixelColor(bge_loc+2, big_led_color_arr[3]);
                     }
        }
        else if (4 == big_led_state){// 5号灯组亮
                 for(int num = 0; num < Big_Led_color_5_num; num++)  
                     {
                       // 3个灯一个单位 控制
                       int bge_loc = big_led_color_5_loc_arr[num] * 3 ;
                       strip.setPixelColor(bge_loc, big_led_color_arr[4]);
                       strip.setPixelColor(bge_loc+1, big_led_color_arr[4]);
                       strip.setPixelColor(bge_loc+2, big_led_color_arr[4]);
                     }
        }
        else {// 6号灯组亮
                 for(int num = 0; num < Big_Led_color_6_num; num++)  
                     {
                       // 3个灯一个单位 控制
                       int bge_loc = big_led_color_6_loc_arr[num] * 3 ;
                       strip.setPixelColor(bge_loc, big_led_color_arr[5]);
                       strip.setPixelColor(bge_loc+1, big_led_color_arr[5]);
                       strip.setPixelColor(bge_loc+2, big_led_color_arr[5]);
                     }
        }
          
        strip.show();
    
        // 小灯
        strip_First_Prize.clear(); strip_Second_Prize.clear(); strip_Third_Prize.clear();  // 先全灭
        int lit_led_state = count_num%3; // 0  / 1 /2 
        if (0 == lit_led_state)     {for(int num2 = 0; num2 < strip_First_Prize.numPixels(); num2++)
                                    { if (READ  == First_Prize_Led_Color) strip_First_Prize.setPixelColor(num2, strip.Color(  255,   0,   0));  // 注意这里 脚灯 颜色，可能要调整 255的位置
                                      if (GREEN == First_Prize_Led_Color) strip_First_Prize.setPixelColor(num2, strip.Color(  0,   255,   0)); 
                                      if (BLUE  == First_Prize_Led_Color) strip_First_Prize.setPixelColor(num2, strip.Color(  0,   0,   255)); 
                                    }}
        else if(1 == lit_led_state) {for(int num2 = 0; num2 < strip_Second_Prize.numPixels(); num2++) 
                                    { if (READ  == Second_Prize_Led_Color) strip_Second_Prize.setPixelColor(num2, strip.Color(  255,   0,   0));  // 注意这里 脚灯 颜色，可能要调整 255的位置
                                      if (GREEN == Second_Prize_Led_Color) strip_Second_Prize.setPixelColor(num2, strip.Color(  0,   255,   0)); 
                                      if (BLUE  == Second_Prize_Led_Color) strip_Second_Prize.setPixelColor(num2, strip.Color(  0,   0,   255)); 
                                     }}
        else                        {for(int num2 = 0; num2 < strip_Third_Prize.numPixels(); num2++) 
                                    { if (READ  == Third_Prize_Led_Color) strip_Third_Prize.setPixelColor(num2, strip.Color(  255,   0,   0));  // 注意这里 脚灯 颜色，可能要调整 255的位置
                                      if (GREEN == Third_Prize_Led_Color) strip_Third_Prize.setPixelColor(num2, strip.Color(  0,   255,   0)); 
                                      if (BLUE  == Third_Prize_Led_Color) strip_Third_Prize.setPixelColor(num2, strip.Color(  0,   0,   255)); 
                                    }}
        strip_First_Prize.show();
        strip_Second_Prize.show();
        strip_Third_Prize.show();
        ///////////////////////////////////////////////////   上面有更新 ///////////////////////////////////////////////////
        

        
        delay(DELAY_MS);// 20ms
      }
    }
  
    while(true){}  
  }
}


// 更新奖池
void  update_prize_pool(int choice_num, int last_num)
{
  Prize_pool_vector[choice_num] = Prize_pool_vector[last_num];
}

// 更新 大灯位置池　
void  update_big_led_loc_pool(int choice_num, int last_num)
{
  big_led_color_loc_arr[choice_num] = big_led_color_loc_arr[last_num];
}

// 中奖后的灯变化
void led_ranom_state_and_price_state(LED_COLOR price_color)
{
  rainbow(1); // 企鹅大灯随机色
  // 企鹅大灯中奖色
  oneColor(price_color);
}

// 指定企鹅灯显示一种颜色
void oneColor(LED_COLOR  led_color) 
{
  int i;
  if (led_color == READ)
  {
      for(i=0; i<strip.numPixels(); i++)   // 0-39个编号的灯
      { 
        strip.setPixelColor(i, strip.Color(  255,   0, 0));//红色
        //strip.show();                                       // 更新显示
      } 
      strip.show();   
  }
  else if(led_color == GREEN)
  {
      for(i=0; i<strip.numPixels(); i++)   // 0-39个编号的灯
      { 
        strip.setPixelColor(i, strip.Color(  0,  255, 0));// 绿色 
        //strip.show();                                     // 更新显示
      }
      strip.show();    
  }
  else
  {
      for(i=0; i<strip.numPixels(); i++)   // 0-39个编号的灯
      { 
        strip.setPixelColor(i, strip.Color(  0,  0, 255));// 蓝色
       // strip.show();                                     // 更新显示
      }
      strip.show();    
  }
}

// 指定 灯 显示一种颜色
void strip_one_core(Adafruit_NeoPixel key_strip, LED_COLOR  led_color)
{
  int i= 0;
   if (led_color == READ)
  {
      for(i=0; i<key_strip.numPixels(); i++)   // 0-39个编号的灯
      { 
        key_strip.setPixelColor(i, key_strip.Color(  255,   0, 0));//红色
      } 
      key_strip.show();                                       // 更新显示
  }
  else if(led_color == GREEN)
  {
      for(i=0; i<key_strip.numPixels(); i++)   // 0-39个编号的灯
      { 
        key_strip.setPixelColor(i, key_strip.Color(  0,  255, 0));// 绿色 
      } 
      key_strip.show();                                     // 更新显示
  }
  else
  {
      for(i=0; i<key_strip.numPixels(); i++)   // 0-39个编号的灯
      { 
        key_strip.setPixelColor(i, key_strip.Color(  0,  0, 255));// 蓝色
      } 
      key_strip.show();                                     // 更新显示
  }
 }

// 关闭灯
void strip_close(Adafruit_NeoPixel key_strip)
{
     /*
      for(i=0; i<key_strip.numPixels(); i++)   // 0-39个编号的灯
      { 
        key_strip.setPixelColor(i, key_strip.Color(  0,  0, 0));// 灭灯
      } 
      key_strip.show();                                     // 更新显示
      */
      key_strip.clear();
      key_strip.show();
}

// 将台灯指定显示一种颜色
void three_prize_led_oneColor(LED_COLOR  led_color, int prize_num) 
{
    if(1 == prize_num)
    {   
        strip_close(strip_Second_Prize);
        strip_close(strip_Third_Prize);
        strip_one_core(strip_First_Prize,led_color); // 1灯奖灯亮，其他灯灭
    }
    else if(2 == prize_num) 
    {
        strip_close(strip_First_Prize);
        strip_close(strip_Third_Prize);
        strip_one_core(strip_Second_Prize, led_color); // 2灯奖灯亮，其他灯灭
    }
    else
    {
        strip_close(strip_First_Prize);
        strip_close(strip_Second_Prize);
        strip_one_core(strip_Third_Prize, led_color); // 3灯奖灯亮，其他灯灭
    }
}


// 按键检测
boolean  check_buttorn()
{
  boolean buttorn_push_flag = false;
  // 读取按键状态
  boolean newState_2 = digitalRead(BUTTON_PIN);
  // 按键由 高电平  变成低电平
  if((newState_2 == LOW) && (oldState_2 == HIGH)) {
    // 延时10ms
    delay(40);
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


// 控制奖台灯 三个灯闪
void prize_led_three_blink(long firstPixelHue)
{
    #ifdef PRIZE_LED_THREE_BLINK_CONTOR
    {
      for(int i=0; i<strip_First_Prize.numPixels(); i++) 
      { // For each pixel in strip...
        int pixelHue = firstPixelHue + (i * 65536L / strip_First_Prize.numPixels());
        strip_First_Prize.setPixelColor(i, strip_First_Prize.gamma32(strip_First_Prize.ColorHSV(pixelHue)));
      }
      strip_First_Prize.show(); 
      for(int i=0; i<strip_Second_Prize.numPixels(); i++)  
      { // For each pixel in strip...
        int pixelHue = firstPixelHue + (i * 65536L / strip_Second_Prize.numPixels());
        strip_Second_Prize.setPixelColor(i, strip_Second_Prize.gamma32(strip_Second_Prize.ColorHSV(pixelHue)));
      }
      strip_Second_Prize.show();  
      for(int i=0; i<strip_Third_Prize.numPixels(); i++)        { // For each pixel in strip...
        int pixelHue = firstPixelHue + (i * 65536L / strip_Third_Prize.numPixels());
        strip_Third_Prize.setPixelColor(i, strip_Third_Prize.gamma32(strip_Third_Prize.ColorHSV(pixelHue)));
      }
      strip_Third_Prize.show(); 
      
    }
    #endif
}



// 初始化彩虹色，一直闪亮，支持按键退出
void init_rainbow(int wait) {
  // Hue of first pixel runs 3 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 3*65536. Adding 256 to firstPixelHue each time
  // means we'll make 3*65536/256 = 768 passes through this outer loop:
  while(true){
  for(long firstPixelHue = 0; firstPixelHue < 3*65536; firstPixelHue += 256) {
    if (check_buttorn()) return;
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

    // 控制　讲台灯闪烁
    prize_led_three_blink(firstPixelHue);  
    
  }
 }
}



// 游戏彩虹灯  企鹅大灯闪  + 讲台灯闪
void rainbow(int wait) {
  // Hue of first pixel runs 3 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 3*65536. Adding 256 to firstPixelHue each time
  // means we'll make 3*65536/256 = 768 passes through this outer loop:
  //while(true){
  for(long firstPixelHue = 0; firstPixelHue < 3*65536; firstPixelHue += 512) {
    //if (check_buttorn()) return;
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      //int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      //strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    //strip.show(); // Update strip with new contents
    //delay(wait);  // Pause for a moment
    
    // 控制　讲台灯闪烁
    //prize_led_three_blink(firstPixelHue);
  }
 //}
}



// 游戏结束彩虹色，一直闪亮，不支持按键响应
void end_rainbow(int wait) {
  // Hue of first pixel runs 3 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 3*65536. Adding 256 to firstPixelHue each time
  // means we'll make 3*65536/256 = 768 passes through this outer loop:
  while(true){
  for(long firstPixelHue = 0; firstPixelHue < 3*65536; firstPixelHue += 256) {
    //if (check_buttorn()) return;
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

    // 控制　奖台灯闪烁
    prize_led_three_blink(firstPixelHue);
  }
 }
}
