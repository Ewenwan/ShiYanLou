// RC_pwm_receiver_decode 遥控 脉冲宽度 读取====
unsigned long int CH1_VAL = 0;
unsigned long int CH2_VAL = 0;
unsigned long int CH3_VAL = 0;
unsigned long int CH4_VAL = 0;
unsigned long int CH5_VAL = 0; 
#define CH1 2
#define CH2 3
#define CH3 4
#define CH4 5
#define CH5 6 //数字引脚 
void setup() {
    Serial.begin(115200);
}

void loop() {
  CH1_VAL = pulseIn(CH1,HIGH);
  CH2_VAL = pulseIn(CH2,HIGH);
  CH3_VAL = pulseIn(CH3,HIGH);
  CH4_VAL = pulseIn(CH4,HIGH);
  CH5_VAL = pulseIn(CH5,HIGH);  //获取脉冲时间
  Serial.print("CH1:");
  Serial.print(CH1_VAL);
  Serial.print("  CH2:");
  Serial.print(CH2_VAL);
  Serial.print("  CH3:");
  Serial.print(CH3_VAL);
  Serial.print("  CH4:");
  Serial.print(CH4_VAL);
  Serial.print("  CH5:");
  Serial.print(CH5_VAL);
  Serial.println(""); //输出脉冲间隔值
  delay(50);
}
