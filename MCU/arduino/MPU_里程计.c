/*
加速度计 获取里程计信息
 */

unsigned char Re_buf[11],counter=0;//串口接收缓冲区
unsigned char sign=0,rec_aacc_flag1=0,rec_aacc_flag=0;
unsigned char count2=0;
unsigned int count1=0;
float a[3],w[3],angle[3],T;//三轴加速度  单位 9.8m/s2  就是1.00 代表 9.8m/s2 角速度 角度 温度
float sstatex=0,sstatey=0;
float gravity[2]={0};//gravity[0] x轴重力加速度分量  gravity[1]y轴重力加速度分量
float accelerationx[2]={0},accelerationy[2]={0};
float velocityx[2]={0},velocityy[2]={0};
float positionY[2]={0},positionX[2]={0};// 
bool calibrition_ok_flag=false;

unsigned char direction_;
unsigned char countx,county;

void cmd_ex(){
 if(sign)
  {  
   sign=0;     //清零接收完成标志
   //count1++;
   if(Re_buf[0]==0x55) //检查帧头
   {  
    switch(Re_buf [1]) //信息标识符
    {
    case 0x51://加速度标识 X Y Z轴加速度
      a[0] = (short(Re_buf [3]<<8| Re_buf [2]))/32768.0*16 * 9.8 * 100;//单位为g cm/s^2   转成 厘米/s2
      a[1] = (short(Re_buf [5]<<8| Re_buf [4]))/32768.0*16 * 9.8 * 100;
      a[2] = (short(Re_buf [7]<<8| Re_buf [6]))/32768.0*16 * 9.8 * 100 ;
      rec_aacc_flag1=1;
      break;   
     case 0x53://角度
      angle[0] = (short(Re_buf [3]<<8| Re_buf [2]))/32768.0*180;//角度
      angle[1] = (short(Re_buf [5]<<8| Re_buf [4]))/32768.0*180;
      angle[2] = (short(Re_buf [7]<<8| Re_buf [6]))/32768.0*180;
       if( rec_aacc_flag1) {rec_aacc_flag=1;rec_aacc_flag1=0;}
      break;
     }
      
   }
  }
}
//执行一次
void Calibrate(void)
{
    if(rec_aacc_flag&&(count1!=0x0040)){//64次 
      count1++;
      rec_aacc_flag=0;
      sstatex = sstatex + a[0]; // Accumulate Samples
      sstatey = sstatey + a[1];
    // sstatex = sstatex + (a[0]-sin(angle[1]))*cos(angle[1]);
     //sstatey = sstatey + (a[1]-sin(angle[0]))*cos(angle[0]); 
    }
    if(count1==0x0040){
      sstatex = sstatex  / 64.0;  //  256  0x100   64  0x40   
      sstatey = sstatey  / 64.0;
      calibrition_ok_flag=true;
      Serial.print("sx ");
      Serial.print(sstatex);
      Serial.print("    ");
      Serial.print("sy ");
      Serial.println(sstatey);
    }
}

// 运动停止检测  超过连续25次加速度为0 则认为 运动停止 速度为0
void movement_end_check(void)
{
    if (accelerationx[1]==0) //加速度采样为0
    { countx++;}
    else { countx =0;}
    if (countx>=3) //if this number exceeds 25, we can assume that velocity is cero
    {
    velocityx[1]=0;
    velocityx[0]=0;
    }
    if (accelerationy[1]==0) //we do the same for the Y axis
    { county++;}
    else { county =0;}
    if (county>=3)
    {
    velocityy[1]=0;
    velocityy[0]=0;
    }
}

/*
 给出了以下加速度读数：
9米/ S / S
3米/秒/秒
0米/ S / S
0米/ S / S
0米/ S / S
-5M / S / S
-7M / S / S

假设一个80毫秒采样率
我们可以得出以下速度：
0.72米/秒（你从加速9米/ s的80毫秒获得）
0.96米/秒
0.96米/秒
0.96米/秒
0.96米/秒
0.56米/秒
0米/秒

从我们可以得出以下距离：
57.6毫米（你从在0.72米/ s的移动为80ms的获得）
134.4毫米
211.2毫米
288毫米
364.8毫米
409.6毫米
*/

void get_position(void)
{
  //均值滤波
  if(count2!=0x02){ // 采样64次  0x40 求平均   32次 0x20   16次 0x10   8  0x08
      count2++;
      rec_aacc_flag=0;
      accelerationx[1] = accelerationx[1] + a[0]; //filtering routine for noise attenuation
      accelerationy[1] = accelerationy[1] + a[1]; //64 samples are averaged. The resulting
   //   accelerationx[1] = accelerationx[1] + (a[0]-sin(angle[1]))*cos(angle[1]); //filtering routine for noise attenuation
   //   accelerationy[1] = accelerationy[1] + (a[1]-sin(angle[0]))*cos(angle[0]); //64 samples are averaged. The resulting
  }
  else{
  count2 = 0;
  accelerationx[1]= accelerationx[1] / 2.0; // division by 2  9600时  100ms一次   115200时 20ms一次
  accelerationy[1]= accelerationy[1] / 2.0;
  float diven = 50.0;
//  //数值滤波
//   accelerationx[1]= 0.8 * accelerationx[1] + 0.2 * a[0]; // division by 2  100ms一次
//   accelerationy[1]= 0.8 * accelerationy[1] + 0.2 * a[1];

  //减去初始值
  accelerationx[1] = accelerationx[1] - sstatex; //eliminating zero reference
  accelerationy[1] = accelerationy[1] - sstatey; // to obtain positive and negative

  // 低通滤波得到重力加速度分量  再减去 重力加速度分量 
   float alpha = 0.8;
   gravity[0] = alpha * gravity[0] + (1 - alpha) * accelerationx[1];
   gravity[1] = alpha * gravity[1] + (1 - alpha) * accelerationy[1];
   accelerationx[1] = accelerationx[1] - gravity[0]; //eliminating zero reference
   accelerationy[1] = accelerationy[1] - gravity[1]; // to obtain positive and negative
   
  //避免 抖动 
  if ((accelerationx[1] <=1.93)&&(accelerationx[1] >= -1.93)) accelerationx[1] = 0.0; 
  if ((accelerationy[1] <=1.93)&&(accelerationy[1] >= -1.93)) accelerationy[1] = 0.0;

  
  // X一次积分 v = v0 + [ a0 + (a1-a0)/2 ] * t : 
  //velocityx[1]= velocityx[0] + accelerationx[0] + ((accelerationx[1] -accelerationx[0])/2.0);
  velocityx[1]= velocityx[0] + (accelerationx[0] + ((accelerationx[1] -accelerationx[0])/2.0))/diven;
  // X 二次积分:x = x0  + (v0 + (v1-v0)/2) * t
  //positionX[1]= positionX[0] + velocityx[0] + ((velocityx[1] - velocityx[0])/2.0);
  positionX[1]= positionX[0] + (velocityx[0] + ((velocityx[1] - velocityx[0])/2.0))/diven;
  // Y 一次积分:
  //velocityy[1] = velocityy[0] + accelerationy[0] + ((accelerationy[1] -accelerationy[0])/2.0);
  velocityy[1] = velocityy[0] + (accelerationy[0] + ((accelerationy[1] -accelerationy[0])/2.0))/diven;
  // Y 二次积分:
  //positionY[1] = positionY[0] + velocityy[0] + ((velocityy[1] - velocityy[0])/2.0);
   positionY[1] = positionY[0] + (velocityy[0] + ((velocityy[1] - velocityy[0])/2.0))/diven;

// 加速度、速度迭代
  accelerationx[0] = accelerationx[1]; //The current acceleration value must be sent
  accelerationy[0] = accelerationy[1]; //variable in order to introduce the new
  velocityx[0] = velocityx[1]; //Same done for the velocity variable
  velocityy[0] = velocityy[1];
// 运动停止检测 
  movement_end_check();
  Serial.print("ax ");
  Serial.print(accelerationx[1]);
  Serial.print("\t");
  Serial.print("ay ");
  Serial.print(accelerationy[1]);
  Serial.print("\t");      
  Serial.print("vx ");
  Serial.print(velocityx[1]);
  Serial.print("\t");
  Serial.print("vy ");
  Serial.print(velocityy[1]);
  Serial.print("\t");  
  
  //位置迭代
  positionX[0] = positionX[1]; //actual position data must be sent to the
  positionY[0] = positionY[1]; //previous position
  //direction_ = 0; // data variable to direction variable reset 
  Serial.print("px ");
  Serial.print(positionX[1]);
  Serial.print("\t");
  Serial.print("py ");
  Serial.println(positionY[1]);
 }
}

void setup() {
  // initialize serial:
  Serial.begin(115200);//波特率 115200 时 模块每隔 10ms 输出 1 帧数据; 9600 50ms 输出 1 帧数据
  //Calibrate();//校准 获取 初始x轴、y轴加速度 
}

void loop() {
  cmd_ex();    //获取
  if(!calibrition_ok_flag) Calibrate();//校准代码执行一次
  else if(rec_aacc_flag){//校准成功后
       rec_aacc_flag=0;
       get_position();
  }    
}

//串口接收信息  中断
void serialEvent() {
  while (Serial.available()) {
    Re_buf[counter]=(unsigned char)Serial.read();//缓冲区
    if(counter==0&&Re_buf[0]!=0x55) return;      //第0号数据不是帧头              
    counter++;       
    if(counter==11)             //接收到11个数据
    {    
       counter=0;               //重新赋值，准备下一帧数据的接收 
       sign=1;                  //置位接收完成标识符
    }     
  }
}

