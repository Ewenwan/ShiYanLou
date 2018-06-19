#include <Wire.h>
#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

#include <ros.h>
#include <std_msgs/UInt32.h>

#define KS103_left  0xe8 >> 1   //  前左侧超声 default address of KS103 is 0xe8
#define KS103_right  0xea >> 1   // 前右侧 default address of KS103 is 0xe8 修改为 0xea 

#define reduce_noise  0x71 // these lines define the data for commands. See data sheat.
#define turn_off_led 0xc1
#define turn_on_led 0xc0

#define read_distance 0xb0  // distance may be less than 5m .
#define read_brightness 0xa0
#define read_temperature 0xc9

word front_left=0;  // thease variables are for printing the temperture through Serial.print().
word front_right=0;
word front_left_send=0;  // thease variables are for printing the temperture through Serial.print().
word front_right_send=0;

const int adc_charge_pin  = A0;    //充电电流ADC检测口
const int adc_battery_pin = A1;    //电池电压ADC检测口
int adc_charge_value = 0;       
int adc_battery_value = 0;

ros::NodeHandle   nh;                                //创建ROS句柄
std_msgs::UInt32  Sensor_msg;                        //定义编码器脉冲广播消息格式
ros::Publisher Sensor("Sensor", &Sensor_msg);                                   


void setup()
{   
  Serial.begin(9600);//串口初始化，用于调试
  Wire.begin();                  // join the TWI as the master
  //Serial.begin(9600);          // start Serial transaction
  delay(200);
  KS103_command(KS103_left,reduce_noise);    // send the noise reduction command to KS103
  delay(5); 
  
  KS103_command(KS103_right,reduce_noise);    // send the noise reduction command to KS103
  delay(6000);                    // wait for being stabilized 2seconds
  //KS103_command(KS103_left,turn_off_led);    // turn off the LED

   //delay(6000);
   // delay(6000);
   //ROS节点初始化
  // nh.initNode();          //消息订初始化,会影响串口
   delay(2000);
   nh.advertise(Sensor);    //消息发布初始化
   delay(200);

   
}

void loop()
{
// nh.spinOnce();  
  //Serial.print(); // measure the distance
 KS103_read_command_left(read_distance);
 delay(100);
 KS103_read_command_right(read_distance);

  if(front_left>5110) front_left=5110;
  if(front_right>5110) front_right=5110;
  front_left_send=front_left/10;
  front_right_send=front_right/10;
 //  /*
  Serial.print("L: ");
  Serial.print("\t");
  Serial.print(front_left_send, DEC);
  Serial.print("\t");
  Serial.print("R: ");
  Serial.print("\t");
  Serial.println(front_right_send, DEC);
// */
  adc_charge_value = analogRead(adc_charge_pin);
  adc_battery_value = analogRead(adc_battery_pin);  
 /*
  reading = KS103_read_command(read_temperature);  // measure the temperature
  reading2 = reading & 0x0f;  
  reading = reading >> 4;        // shift low byte 4 bits
  Serial.print(reading);
  Serial.print('.'); 
  Serial.print(reading2);

  Serial.print("deg.");    
  Serial.print("   brightness ");  
  Serial.print(KS103_read_command(read_brightness));   // measure light
  Serial.println(".");
  */ 
  
          //超声
          Sensor_msg.data=(0x01                            & 0xffffffff)<<30
                         |(long(0                        ) & 0xffffffff)<<18 
                         |(long(front_right_send         ) & 0xffffffff)<<9  // front_right       front_right/10   
                         |(long(front_left_send          ) & 0xffffffff);    // front_left         front_left/10             
          Sensor.publish(&Sensor_msg);
        //  Serial.println(Sensor_msg.data, HEX);
          
        //电压
          Sensor_msg.data=(0x03                            & 0xffffffff)<<30
                         |(long(0                       ) & 0xffffffff)<<20 
                         |(long(adc_charge_value        ) & 0xffffffff)<<10 // charge
                         |(long(adc_battery_value       ) & 0xffffffff);    // battery                
          Sensor.publish(&Sensor_msg);
          
  delay(90);                  //wait to be read on the screen of PC.
}

void KS103_read_command_left(byte command){  // sending the command and read the data in the register
 // word temp_reading;
  // step 1: instruct sensor to read echoes
  KS103_command(KS103_left,command);   // send the command
  // step 2: wait for readings to happen
  delay(5);                   // wait 1 milliseconds . Without this waiting, the system will be unstable. 
  // step 3: instruct sensor to return a particular echo reading
  Wire.beginTransmission(KS103_left); // start to transmit with KS103
  Wire.write(byte(0x02));      // register 2 is the gate of receiving the data
  Wire.endTransmission();      // stop transmitting
  // step 4: request reading from sensor
  Wire.requestFrom(KS103_left, 2);    // request the data in the 2nd and third register of KS103
  // step 5: receive reading from sensor
  if(2 <= Wire.available())    // wait the register available
  {
    front_left = (Wire.read()) << 8;  // read register 2 and shift it to upper byte.
    front_left |= Wire.read(); // read the register 3 to lower byte
  }
  //return temp_reading;  // return as a 16bit data 
}


void KS103_read_command_right(byte command){  // sending the command and read the data in the register
 // word temp_reading;
  // step 1: instruct sensor to read echoes
  KS103_command(KS103_right,command);   // send the command
  // step 2: wait for readings to happen
  delay(5);                   // wait 1 milliseconds . Without this waiting, the system will be unstable. 
  // step 3: instruct sensor to return a particular echo reading
  Wire.beginTransmission(KS103_right); // start to transmit with KS103
  Wire.write(byte(0x02));      // register 2 is the gate of receiving the data
  Wire.endTransmission();      // stop transmitting
  // step 4: request reading from sensor
  Wire.requestFrom(KS103_right, 2);    // request the data in the 2nd and third register of KS103
  // step 5: receive reading from sensor
  if(2 <= Wire.available())    // wait the register available
  {
    front_right = (Wire.read()) << 8;  // read register 2 and shift it to upper byte.
    front_right |= Wire.read(); // read the register 3 to lower byte
  }
  //return temp_reading;  // return as a 16bit data 
}


void KS103_read_command_double(byte command){  // sending the command and read the data in the register
  //word temp_reading;
  // step 1: instruct sensor to read echoes

  //多个超声模块同一时段测量会有影响 
  KS103_command(KS103_left,command);   // send the command
  // step 2: wait for readings to happen
  delay(90);                   // wait 1 milliseconds . Without this waiting, the system will be unstable.
  KS103_command(KS103_right,command);   // send the command
  
  delay(90); 
  // step 3: instruct sensor to return a particular echo reading
  Wire.beginTransmission(KS103_left); // start to transmit with KS103
  Wire.write(byte(0x02));      // register 2 is the gate of receiving the data
  Wire.endTransmission();      // stop transmitting
  // step 4: request reading from sensor
  Wire.requestFrom(KS103_left, 2);    // request the data in the 2nd and third register of KS103
  // step 5: receive reading from sensor
  if(2 <= Wire.available())    // wait the register available
  {
    front_left = (Wire.read()) << 8;  // read register 2 and shift it to upper byte.
    front_left |= Wire.read(); // read the register 3 to lower byte
  }
  delay(90);
  Wire.beginTransmission(KS103_right); // start to transmit with KS103
  Wire.write(byte(0x02));      // register 2 is the gate of receiving the data
  Wire.endTransmission();      // stop transmitting
  // step 4: request reading from sensor
  Wire.requestFrom(KS103_right, 2);    // request the data in the 2nd and third register of KS103
  // step 5: receive reading from sensor
  if(2 <= Wire.available())    // wait the register available
  {
    front_right = (Wire.read()) << 8;  // read register 2 and shift it to upper byte.
    front_right |= Wire.read(); // read the register 3 to lower byte
  }
 // return temp_reading;  // return as a 16bit data 
}

void KS103_command(char addr ,byte command){  // send the command to KS103
  Wire.beginTransmission(addr); // start the transmission with KS103
  Wire.write(byte(0x02));      // register 2
  Wire.write(command);      // send the command to the register 2
  Wire.endTransmission();      // end of transmission
}
