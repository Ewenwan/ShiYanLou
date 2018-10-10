/*管脚和变量定义*/
const int buttonPin = 2;     // 定义开关按键输入针脚号常量   数字端口2
const int ledPin = 13;       // 开关状态显示指示灯          数字端口13
bool ledState = HIGH;        // 状态标志
bool output_flag = false;    // 油门输出标志
int buttonState;             // 开关按键状态
int lastButtonState = HIGH;  // 上次开关状态
long lastDebounceTime = 0;   // 时间记录
long debounceDelay = 50;     // 延时去抖动时间

int bet_voltage_pin =  A0;   // 电压测量管脚                模拟量输入端口A0
int LED_1 = 3;               // 电池电量指示灯              数字端口3\4\5\6
int LED_2 = 4;
int LED_3 = 5;
int LED_4 = 6;
int PWM1 = 9;                // 左推进器  电机接口          pwm口8\9
int PWM2 = 10;               // 右推进器

const int time_2s = 250;     // 125Hz   250为2s
static unsigned int aa = 0;  // 定时器计数

bool LED_4s = true;          // 电量状态显示标志   满电
bool LED_3s = false;
bool LED_2s = false;
bool LED_1s = false;


/*初始化函数 跑一次*/
void setup() {
  Serial.begin(9600);       // 初始化串口波特率为9600
  pinMode(LED_1, OUTPUT);    // LED灯输出
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);
  digitalWrite(LED_1, HIGH);
  digitalWrite(LED_2, HIGH);
  digitalWrite(LED_3, HIGH);
  digitalWrite(LED_4, HIGH);
  pinMode(bet_voltage_pin , INPUT_PULLUP);  // 将电池测量端口设置为输入并且带有内部上拉电阻
  pinMode(PWM1, OUTPUT);                    // 电机pwm输出
  pinMode(PWM2, OUTPUT);
  analogWrite(PWM1, 0);                     // 左推进器   0~255
  analogWrite(PWM2, 0);                     // 右推进器   将电机速度设置为0
  // 设置按键的针脚为输入状态
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);            // 上拉
  pinMode(ledPin, OUTPUT);
  // 设置电路板上LED灯的初始状态，
  // 此处因为变量ledState的初始状态为low，
  // 所以LED灯的初始状态为灭。
  digitalWrite(ledPin, LOW);

  //timer1 是 16bits， 但 timer2 (和 timer0) 只有 8 bits，
  //Servo.h 库以Timer1为源，若使用Timer1后  pin 9 和 pin 10  的 PWM 不可用
  //timer2 控制 pin 11 和 pin 3 的 PWM 輸出,  Arduino 自帶的 tone( ) 也使用timer2 
 // Arduino 的板子大都采用 16MHz,晶振

  // Timer2 初始化 2ms定时中断
  cli();                                     // 禁止定时器中断
  TCCR2A = 0x02;                             // 关闭数字管脚 3 和 11的PWM功能,设置 INTO CTC 模式  (Clear Timer on Compare mode)
  TCCR2B = 0x07;                             // DON'T FORCE COMPARE, 1024 PRESCALER  定时器时钟源分频
  OCR2A = 0X7D;                              // 16000 000/256/124 =500HZ ...2ms        16000000/1024/125=125HZ
  TCNT2 = 0;                                 // 计数器清理
  TIMSK2 = 0x02;                             // ENABLE INTERRUPT ON MATCH BETWEEN TIMER2 AND OCR2A
  sei();                                     // 使能定时器中断
  //外部中断
}

//排序算法
void quick(int *a,int i,int j) 
{ 
int m,n,temp; 
int k; 
m=i; 
n=j; 
k=a[(i+j)/2];                               // 选取的参照
do { 
   while(a[m]<k&&m<j) m++;                  // 从左到右找比k大的元素 
   while(a[n]>k&&n>i) n--;                  // 从右到左找比k小的元素
   if(m<=n) {                               // 若找到且满足条件，则交换
      temp=a[m]; 
      a[m]=a[n]; 
      a[n]=temp; 
      m++; 
      n--; 
    } 
  } while(m<=n); 
 if(m<j) quick(a,m,j);                      // 运用递归
 if(n>i) quick(a,i,n); 
} 

//打印数组元素到串口
void print(int *a,int n) 
{ 
  int i; 
  for(i=0;i<n;i++) 
  {
  Serial.print(a[i]); 
  Serial.print('\t'); 
  if(i==(n-1)) Serial.print('\n');
  } 
} 

//2ms中断处理函数  定时执行
ISR(TIMER2_COMPA_vect) {                     // triggered when Timer2 counts to 124
  cli();                                     // 关闭定时器中断
  aa++;
  if (aa == time_2s)
  {
    int vale[10],valtage1=0;
    for(int i=0;i<10;i++){                   //多次采样 
    vale[i]= analogRead(bet_voltage_pin);    
    delay(10);
    }
    quick(vale,0,9);                         //排序
    print(vale,10);                          //打印排序后的数序列
    long double sum=0;
    for(int i=1;i<9;i++)
    {
      sum +=vale[i];                         //去最大值和最小值 求和
      }
      valtage1 = sum/8;                      //取平均
    double voltage = (double)valtage1 / 1023 * 5 * 3; //电池电压外部分压  1/3
    Serial.println(voltage);
    /*电池节数:3串锂电(9-12.6V) ？？
      电量指示:25%第一个灯亮(电压10V左右),
      50%第二灯亮(电压10.8V左右),
      75%第三个亮(电压11.5V左右),
      100%第四个亮(电压12.5V左右)*/
    if (vale > 850)         LED_4s = true;    //电压12.5V
    else if (vale > 785)    LED_3s = true;    //电压11.5V
    else if (vale > 650)    LED_2s = true;    //电压10.8V
    else                    LED_1s = true;    //电压10.8V下
    aa = 0;
  }
  sei();                                      // 再次使能定时器中断
}// end isr


// 系统调用，无限循环
void loop() {
  // 读取按键的状态
  int reading = digitalRead(buttonPin);
  // 判断当前的按键状态是否和之前有所变化
  if (reading != lastButtonState) {
    // 如果按键发生了变化，
    // 则重新设置最近一次抖动的时间。
    lastDebounceTime = millis();
  }
  // 判断按键按下或抬起的状态时间间隔是否大于延迟抖动的时间长度。
  // 方法millis()可以获取当前时间，单位统一为毫秒。
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // 判断当前的按键状态是否和之前有所变化
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {
        ledState = LOW;
        output_flag = true;
      }
      if (buttonState == HIGH) {
        ledState = HIGH;
        output_flag = true;
      }
    }
  }
  if (!ledState && output_flag)              //启动
  {
    digitalWrite(ledPin, HIGH);
    analogWrite(PWM1, 255);                  //左推进器 0~255  4.9V    200 3.9V
    analogWrite(PWM2, 255);                  //右推进器 将电机速度设置为最大
    Serial.println(!ledState);
    output_flag = false;
  }
  else if (output_flag)                      //停止
  {
    digitalWrite(ledPin, LOW);
    analogWrite(PWM1, 0);                    //左      0~255
    analogWrite(PWM2, 0);                    //右   //将电机速度设置为0
    Serial.println(!ledState);
    output_flag = false;
  }

  lastButtonState = reading;

  if (LED_4s)                                //四个灯全亮      
  {
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, HIGH);
    digitalWrite(LED_3, HIGH);
    digitalWrite(LED_4, HIGH);
    LED_4s = false;
  }
  if (LED_3s)                                //亮3个灯
  {
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, HIGH);
    digitalWrite(LED_3, HIGH);
    digitalWrite(LED_4, LOW);
    LED_3s = false;
  }
  if (LED_2s)                                //亮2个灯
  {
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, HIGH);
    digitalWrite(LED_3, LOW);
    digitalWrite(LED_4, LOW);
    LED_2s = false;
  }
  if (LED_1s)                                //亮1个灯
  {
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, LOW);
    digitalWrite(LED_3, LOW);
    digitalWrite(LED_4, LOW);
    LED_1s = false;
  }


}
