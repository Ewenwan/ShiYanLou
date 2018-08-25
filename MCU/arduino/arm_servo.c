#include <EEPROM.h>
#include <Servo.h> 
// 舵机控制结构体 
/*
这个类有六个成员函数，粗略的讲一下：
attach()          把一个引脚绑定到一个舵机的实例对象上去，注意这个库会限制PWM的使用，详细请看官方文档
write()           参数是角度值，使舵机转到指定角度，比如0度和180度就是两边，90度就是中间
writeMicroseconds()一般舵机有1000到2000，或者700到2300的范围来转动，实际测试一下就行了，1500通常是中间值
readMicroseconds()当前舵机的角度值
read()            这个地方比较坑，它会返回一个角度值，原本我以为是当前实时角度值，后来发现它只是上次调用write()函数的最后一个角度值，真正使用的时候真的没什么卵用
attached()        返回这个舵机实例是否已经绑定了一个引脚
detach()          解除这个舵机实例与引脚的绑定。

其中这个write默认就是把writeMicroseconds()中的544~2400映射到了角度中的0~180度，这么一来当你用write()来写角度的时候，就会很难对舵机进行很细致的操作，
0度到1度会差很多，尤其是精细一点的舵机会更夸张，这1度远远不是它能达到的最小的变化角度，所以我在后面使用的都是writeMicroseconds()这个函数。
*/ 
Servo myservo0,myservo1,myservo2,myservo3,myservo4;
 
int addr = 0;   // 存储 地址变量
int pos = 0;    // 存储舵机角度变量
int read_Servo0, read_Servo1, read_Servo2, read_Servo3, read_Servo4;// 模拟量旋转开关  用于设置舵机角度设定值
bool bottom = false;
bool bottom_pressed = false;
int min_0, max_0, min_1, max_1, min_2, max_2, min_3, max_3;//
//int R_PIN, G_PIN, B_PIN;
#define R_PIN  4
#define G_PIN  5
#define B_PIN  6

// 存储 内容截止 标志
bool eeprom_end()
{
        if (EEPROM.read(addr) == 6666) {
                delay(100);
                return false;
        }
        else
                return true;
}
 
bool match_servo()
{
        int read_Servo0_p, read_Servo1_p, read_Servo2_p, read_Servo3_p, read_Servo4_p;// 模拟电压 开关 用于舵机角度设定值
 
        read_Servo0_p = analogRead(A0);//读取模拟量的值
        //限幅
        if (read_Servo0_p > max_0)
                read_Servo0_p = max_0;
        if (read_Servo0_p < min_0)
                read_Servo0_p = min_0;
        // 映射到 指定的范围      
        read_Servo0_p = map(read_Servo0_p, min_0, max_0, 611, 2389);//611~2389为实际 舵机控制版角度 范围  对应 0~180度
 
        read_Servo1_p = analogRead(A1);//读取模拟量的值
         //限幅
        if (read_Servo1_p > max_1)
                read_Servo1_p = max_1;
        if (read_Servo1_p < min_1)
                read_Servo1_p = min_1;
        // 映射到 指定的范围   
        read_Servo1_p = map(read_Servo1_p, min_1, max_1, 611, 2389);
 
        read_Servo2_p = analogRead(A2);//读取模拟量的值
        //限幅
        if (read_Servo2_p > max_2)
                read_Servo2_p = max_2;
        if (read_Servo2_p < min_2)
                read_Servo2_p = min_2;
        // 映射到 指定的范围   
        read_Servo2_p = map(read_Servo2_p, min_2, max_2, 611, 2389);
 
        read_Servo3_p = analogRead(A3);//读取模拟量的值
        //限幅 
        if (read_Servo3_p > max_3)
                read_Servo3_p = max_3;
        if (read_Servo3_p < min_3)
                read_Servo3_p = min_3;
        // 映射到 指定的范围  
        read_Servo3_p = map(read_Servo3_p, min_3, max_3, 611, 2389);
 
        read_Servo4_p = analogRead(A4);//读取模拟量的值
         //限幅 
        if (read_Servo4_p > 1024)
                read_Servo4_p = 1024;
        if (read_Servo4_p < 0)
                read_Servo4_p = 0;
         // 映射到 指定的范围 1222 ~ 1722
        read_Servo4_p = map(read_Servo4_p, 0, 1024, 1222, 1722);// 122~1722 舵机角度控制参数范围

        //打印 规范化后的各个舵机角度设定值
        Serial.print("read_Servo_p:");
        Serial.print(read_Servo0_p);
        Serial.print(":");
        Serial.print(read_Servo1_p);
        Serial.print(":");
        Serial.print(read_Servo2_p);
        Serial.print(":");
        Serial.print(read_Servo3_p);
        Serial.print(":");
        Serial.print(read_Servo4_p);
        Serial.print("\n");
        if (abs(read_Servo0_p - read_Servo0) > 20) {
                //Serial.print(read_Servo0_p - read_Servo0);
                Serial.println("!0");
                return false;
        }
        else if (abs(read_Servo1_p - read_Servo1) > 20) {
                Serial.println("!1");
                return false;
        }
        else if (abs(read_Servo2_p - read_Servo2) > 20) {
                Serial.println("!2"); 
                return false;
        }
        else if (abs(read_Servo3_p - read_Servo3) > 20) {
                Serial.println("!3");
                return false;
        }
        else if (abs(read_Servo4_p - read_Servo4) > 20) {
                Serial.println("!4");
                return false;
        }
        else if (abs(EEPROM.read(addr - 5) - read_Servo0) > 20) {
                /*Serial.print(":");
                Serial.print(EEPROM.read(addr - 5) - read_Servo0);
                Serial.print("\n");*/
                Serial.println("!5");
                return false;
        }
        else if (abs(EEPROM.read(addr - 4) - read_Servo1) > 20) {
                Serial.println("!6");
                return false;
        }
        else if (abs(EEPROM.read(addr - 3) - read_Servo2) > 20) {
                Serial.println("!7");
                return false;
        }
        else if (abs(EEPROM.read(addr - 2) - read_Servo3) > 20) {
                Serial.println("!8");
                return false;
        }
        else if (abs(EEPROM.read(addr - 1) - read_Servo4) > 20) {
                Serial.println("!9");
                return false;
        }
        else
                return true;
}

//读取 各个模拟量按键的值(对应 各个关键舵机的角度) 存储到 rom内存中
void get_Servo()
{
  // 读取各个模拟量旋转电位器的值 映射成 舵机角度控制变量
        read_Servo0 = analogRead(A0);
        if (read_Servo0 > max_0)
                read_Servo0 = max_0;
        if (read_Servo0 < min_0)
                read_Servo0 = min_0;
        read_Servo0 = map(read_Servo0, min_0, max_0, 611, 2389);
  
        read_Servo1 = analogRead(A1);
        if (read_Servo1 > max_1)
                read_Servo1 = max_1;
        if (read_Servo1 < min_1)
                read_Servo1 = min_1;
        read_Servo1 = map(read_Servo1, min_1, max_1, 611, 2389);
 
        read_Servo2 = analogRead(A2);
        if (read_Servo2 > max_2)
                read_Servo2 = max_2;
        if (read_Servo2 < min_2)
                read_Servo2 = min_2;
        read_Servo2 = map(read_Servo2, min_2, max_2, 611, 2389);
 
        read_Servo3 = analogRead(A3);
        if (read_Servo3 > max_3)
                read_Servo3 = max_3;
        if (read_Servo3 < min_3)
                read_Servo3 = min_3;
        read_Servo3 = map(read_Servo3, min_3, max_3, 611, 2389);
 
        read_Servo4 = analogRead(A4);
        if (read_Servo4 > 1024)
                read_Servo4 = 1024;
        if (read_Servo4 < 0)
                read_Servo4 = 0;
        read_Servo4 = map(read_Servo4, 0, 1024, 1222, 1722);
 //将角度值 写入存储中 记录各个时刻各个关节的角度值
        EEPROM.write(addr, read_Servo0);
        delay(100);
        addr++;
        EEPROM.write(addr, read_Servo1);
        delay(100);
        addr++;
        EEPROM.write(addr, read_Servo2);
        delay(100);
        addr++;
        EEPROM.write(addr, read_Servo3);
        delay(100);
        addr++;
        EEPROM.write(addr, read_Servo4);
        delay(100);
        addr++;
 
        Serial.print("read_Servo:");
        Serial.print(read_Servo0);
        Serial.print(":");
        Serial.print(read_Servo1);
        Serial.print(":");
        Serial.print(read_Servo2);
        Serial.print(":");
        Serial.print(read_Servo3);
        Serial.print(":");
        Serial.print(read_Servo4);
        Serial.print("\n");
}


//内存中写入 数据结尾的标记值
void write_eeprom_end()
{
        EEPROM.write(addr, 6666);
        delay(100);
}
//读取打印内存中的数据
void read_eeprom()
{
        for (int i = 0; i < 512; i)
        {
                int value = EEPROM.read(i);
                delay(100);
                Serial.print(value);
                if ((i+1) % 5 == 0)//每5个换行
                        Serial.print("\n");
                else
                        Serial.print(":");
                if (value != 6666)
                        i++;
                else
                        i = 512;//遇到结尾提前结束
        }
        Serial.print("\n");
}
 
void get_Servo4()
{
        pos = analogRead(A4);
        myservo4.writeMicroseconds(map(pos, 0, 1024, 1222, 1722));
}

//舵机口取消关联
void servo_detach() 
{
        myservo0.detach();
        myservo1.detach();
        myservo2.detach();
        myservo3.detach();
        //myservo4.detach();
}
// 关联pwm口到舵机对象 
void servo_attach()
{
        myservo0.attach(8);
        myservo1.attach(9);
        myservo2.attach(10);
        myservo3.attach(11);
        //myservo4.attach(12);
}
//下降沿按键中断检测
void bottom_press()
{
        long now_time = millis();
         
        //detachInterrupt(digitalPinToInterrupt(UPS_INT_PIN));
        detachInterrupt(7);//中断进来后 先解除中断
 
        delay(5);//延时5ms 消除按键抖动
        if (digitalRead(7) == LOW)
        {
                bottom = !bottom;//按键短按
                Serial.println(bottom);
                unsigned long value;
 
                while (digitalRead(7) == LOW);//等待 按键抬起
      /////// // 按键长按  停止记录 开启 模仿标志
                if (millis() - now_time >= 1000)// 按键滤波 按下按键时间长度在1s以上
                {
                        bottom_pressed = !bottom_pressed;//开启模仿标志
                        Serial.println("LONG PRESS!");//打印长按按键 信息
                        digitalWrite(B_PIN, LOW);
                        delay(500);
                        digitalWrite(B_PIN, HIGH);
                        delay(500);
                        digitalWrite(B_PIN, LOW);
                        delay(500);
                        digitalWrite(B_PIN, HIGH);
                        delay(500);
                        if (bottom_pressed)
                        {
                                write_eeprom_end();// 停止记录 关节 角度信息
                                read_eeprom();     // 打印内存中 各个时间段各个关节信息 
                                addr = 0;          // 存储地址归零 
                                servo_attach();    // 舵机接口关联到 舵机pwm口 
                                                              
                        }
                        /*else
                        {
                                digitalWrite(G_PIN, LOW);
                                delay(500);
                                digitalWrite(G_PIN, HIGH);
                                delay(500);
                                digitalWrite(G_PIN, LOW);
                                delay(500);
                                digitalWrite(G_PIN, HIGH);
                                delay(500);
                        }*/
                        //read_eeprom();
                        //addr = 0;
                }
        /////// 短按按键  记录信息 /////
                else
                {
                        get_Servo();//记录各个时间段 各个 关键角度信息 写入到内存
                        while (!match_servo())
                        {
                                addr -= 5;//地址移位
                                get_Servo();//记录各个时间段 各个 关键角度信息  写入到内存
                        }
                        Serial.println("EEPROM WRITE END");
                        Serial.print(EEPROM.read(addr - 5));
                        Serial.print(":");
                        Serial.print(EEPROM.read(addr - 4));
                        Serial.print(":");
                        Serial.print(EEPROM.read(addr - 3));
                        Serial.print(":");
                        Serial.print(EEPROM.read(addr - 2));
                        Serial.print(":");
                        Serial.println(EEPROM.read(addr - 1));
                        digitalWrite(R_PIN, LOW);
                        delay(500);
                        digitalWrite(R_PIN, HIGH);
                        delay(500);
                        digitalWrite(R_PIN, LOW);
                        delay(500);
                        digitalWrite(R_PIN, HIGH);
                        delay(500);
                }
        }
        
        attachInterrupt(7, bottom_press, FALLING);//中断结束后  开启按键中断
}
 
void get_analog()
{
        for (pos = 10; pos <= 170; pos += 10) // goes from 0 degrees to 180 degrees 
        {                                  // in steps of 1 degree 
                myservo0.write(pos);
                myservo1.write(pos);
                myservo2.write(pos);
                myservo3.write(pos);
                myservo4.write(map(pos, 10, 170, 65*11, 110*11));// tell servo to go to position in variable 'pos' 
                delay(500);                       // waits 15ms for the servo to reach the position 
                if (pos == 10 || pos == 170)
                {
                        Serial.print(analogRead(A0));
                        Serial.print(" : ");
                        Serial.print(analogRead(A1));
                        Serial.print(" : ");
                        Serial.print(analogRead(A2));
                        Serial.print(" : ");
                        Serial.print(analogRead(A3));
                        Serial.print("\n");
                }
        }
        for (pos = 170; pos >= 10; pos -= 10)     // goes from 180 degrees to 0 degrees 
        {
                myservo0.write(pos);
                myservo1.write(pos);
                myservo2.write(pos);
                myservo3.write(pos);
                //if(pos >= 65 && pos <= 110)
                myservo4.write(map(pos, 10, 170, 65*11, 110*11));// tell servo to go to position in variable 'pos' 
                delay(500);                       // waits 15ms for the servo to reach the position 
                if (pos == 10 || pos == 170)
                {
                        Serial.print(analogRead(A0));
                        Serial.print(" : ");
                        Serial.print(analogRead(A1));
                        Serial.print(" : ");
                        Serial.print(analogRead(A2));
                        Serial.print(" : ");
                        Serial.print(analogRead(A3));
                        Serial.print("\n");
                }
        }
}
 
void process_servo()
{
        int servo_pos_0, servo_pos_1, servo_pos_2, servo_pos_3, servo_pos_4;
        int servo_des_0, servo_des_1, servo_des_2, servo_des_3, servo_des_4;
        int servo_move_0, servo_move_1, servo_move_2, servo_move_3, servo_move_4;
        int rate_0, rate_1, rate_2, rate_3, rate_4, servo_move_max, rate_time;

       // 当前 关节 角度值
        servo_pos_0 = read_Servo0;
        servo_pos_1 = read_Servo1;
        servo_pos_2 = read_Servo2;
        servo_pos_3 = read_Servo3;
        servo_pos_4 = read_Servo4;
       // 使舵机转动到 当前实际角度        
        myservo0.writeMicroseconds(servo_pos_0);
        myservo1.writeMicroseconds(servo_pos_1);
        myservo2.writeMicroseconds(servo_pos_2);
        myservo3.writeMicroseconds(servo_pos_3);
        myservo4.writeMicroseconds(servo_pos_4);
        
 // 从内存中读取 之前记录的 关键角度值
        servo_des_0 = EEPROM.read(addr);//期望角度值
        //delay(100);
        addr++;
        servo_des_1 = EEPROM.read(addr);
        //delay(100);
        addr++;
        servo_des_2 = EEPROM.read(addr);
        //delay(100);
        addr++;
        servo_des_3 = EEPROM.read(addr);
        //delay(100);
        addr++;
        servo_des_4 = EEPROM.read(addr);
        //delay(100);
        addr++;
 
        servo_move_0 = servo_des_0 - servo_pos_0;//需要移动的角度值
        servo_move_1 = servo_des_1 - servo_pos_1;
        servo_move_2 = servo_des_2 - servo_pos_2;
        servo_move_3 = servo_des_3 - servo_pos_3;
        servo_move_4 = servo_des_4 - servo_pos_4;
 
        servo_move_max = max(abs(servo_move_0), max(abs(servo_move_1), max(abs(servo_move_2), max(abs(servo_move_3), abs(servo_move_4)))));

 // 计算每个关节每次角度的增量  最大按50记
        rate_0 = servo_move_0 * 50 / servo_move_max;
        rate_1 = servo_move_1 * 50 / servo_move_max;
        rate_2 = servo_move_2 * 50 / servo_move_max;
        rate_3 = servo_move_3 * 50 / servo_move_max;
        rate_4 = servo_move_4 * 50 / servo_move_max;
 
        /*Serial.print("Servo_rate:");
        Serial.print(rate_0);
        Serial.print(" : ");
        Serial.print(rate_1);
        Serial.print(" : ");
        Serial.print(rate_2);
        Serial.print(" : ");
        Serial.print(rate_3);
        Serial.print(" : ");
        Serial.print(rate_4);
        Serial.print("\n");*/
 
        rate_time = servo_move_max;//需要移动的最大角度
        //rate_num = 5;
        //int rate_num = 0;
        while (rate_time)
        {
                if (rate_time < 50)//剩余所需移动角度  已结小于50/2000*180 度了 直接执行所需的角度值
                {
                        servo_pos_0 = servo_des_0;
                        servo_pos_1 = servo_des_1;
                        servo_pos_2 = servo_des_2;
                        servo_pos_3 = servo_des_3;
                        servo_pos_4 = servo_des_4;
                        rate_time = 0;
                }
                else
                {
                        servo_pos_0 += rate_0;
                        servo_pos_1 += rate_1;
                        servo_pos_2 += rate_2;
                        servo_pos_3 += rate_3;
                        servo_pos_4 += rate_4;
                        rate_time -= 50;//每一次最大 角度变动增量  限制在 50 以内
                }
                //rate_num++;
                /*Serial.print("rate_time:");
                Serial.print(rate_time);
                Serial.print("Servo_test:");
                Serial.print(servo_pos_0);
                Serial.print(" : ");
                Serial.print(servo_pos_1);
                Serial.print(" : ");
                Serial.print(servo_pos_2);
                Serial.print(" : ");
                Serial.print(servo_pos_3);
                Serial.print(" : ");
                Serial.print(servo_pos_4);
                Serial.print(" : ");
                Serial.print(rate_num);
                Serial.print("\n");*/
        //写入关节角度值
                myservo0.write(servo_pos_0);
                myservo1.write(servo_pos_1);
                myservo2.write(servo_pos_2);
                myservo3.write(servo_pos_3);
                myservo4.write(servo_pos_4);
 
                delay(40);
        }
 
        /*Serial.print(servo_pos_0);
        Serial.print(" : ");
        Serial.print(servo_pos_1);
        Serial.print(" : ");
        Serial.print(servo_pos_2);
        Serial.print(" : ");
        Serial.print(servo_pos_3);
        Serial.print(" : ");
        Serial.print(servo_pos_4);
        Serial.print("\n");*/
 
        read_Servo0 = servo_des_0;
        read_Servo1 = servo_des_1;
        read_Servo2 = servo_des_2;
        read_Servo3 = servo_des_3;
        read_Servo4 = servo_des_4;
}
 
void config_init()
{
  //电位器 上下限
        min_0 = 177;
        min_1 = 178;
        min_2 = 194;
        min_3 = 172;
        max_0 = 660;
        max_1 = 668;
        max_2 = 704;
        max_3 = 650;
        pinMode(A0, INPUT);
        pinMode(A1, INPUT);
        pinMode(A2, INPUT);
        pinMode(A3, INPUT);
        pinMode(A4, INPUT);
        pinMode(7, INPUT_PULLUP);//控制按键输入口
        pinMode(R_PIN, OUTPUT);
        pinMode(G_PIN, OUTPUT);
        pinMode(B_PIN, OUTPUT);
        digitalWrite(R_PIN, HIGH);
        digitalWrite(G_PIN, HIGH);
        digitalWrite(B_PIN, HIGH);
       /*
        型号         int.0 int.1 int.2 int.3 int.4 int.5
        UNO\Ethernet  2     3 　 　 　 　
        Mega2560      2     3     21     20    19   18
        Leonardo      3     2     0      1 　 　
        Due           　所有IO口均可
        attachInterrupt(中断号, 中断程序, mode);
        中断触发模式:
        LOW    低电平触发
        CHANGE 电平改变触发
        RISING 上升沿触发
        FALLING下降沿触发
        detachInterrupt(中断号) 命令进行删除
          */
        attachInterrupt(7, bottom_press, FALLING);//按键中断 学习/模仿 attachInterrupt(中断号, 中断程序, FALLING);
        
        // 舵机 0~4接口 
        myservo0.attach(8);
        myservo1.attach(9);
        myservo2.attach(10);
        myservo3.attach(11);
        myservo4.attach(12);
}
 
void setup() 
{ 
        config_init();
        Serial.begin(9600);//串口
} 
  
void loop() 
{ 
        if (bottom_pressed)
        {
                if (eeprom_end())
                        process_servo();//从内存中读取记录的角度值 增量变换 舵机角度 到记录值
                else
                        addr = 0;
        }
        else
        {
                servo_detach();
                get_Servo4();
                delay(50);
        }
}
 
 
 
 
  //for(pos = 10; pos <= 170; pos += 1) // goes from 0 degrees to 180 degrees 
  //{                                  // in steps of 1 degree 
  //  myservo0.write(90);
  //  myservo1.write(90);
  //  myservo2.write(90);
  //  myservo3.write(90);
  //  //if(pos >= 65 && pos <= 110)
  //  myservo4.write(map(pos,10,170,65,110));// tell servo to go to position in variable 'pos' 
  //  delay(20);                       // waits 15ms for the servo to reach the position 
  //} 
  //for(pos = 170; pos>=10; pos -= 1)     // goes from 180 degrees to 0 degrees 
  //{                                
  //  myservo0.write(90);
  //  myservo1.write(90);
  //  myservo2.write(90);
  //  myservo3.write(90);
  //  //if(pos >= 65 && pos <= 110)
  //  myservo4.write(map(pos,10,170,65,110));// tell servo to go to position in variable 'pos' 
  //  delay(20);                       // waits 15ms for the servo to reach the position 
  //}
  
