// https://github.com/ywandy/arduino_lesson_code
/*
// 两盏led分别闪亮============
unsigned long int ev1 = 0;
unsigned long int ev2 = 0;
int led_1_va1 = 0;
int led_2_val = 0;

void setup()
{
	pinMode(13, OUTPUT);
	pinMode(11, OUTPUT);
	ev1 = millis();
	ev2 = millis();
}	

void loop()
{
	if (millis() - ev1 > 500) {
		digitalWrite(13, led_1_va1);
		led_1_va1 = ~led_1_va1;
		ev1 = millis();
	}else if (millis() - ev2 > 2000) {
		digitalWrite(11, led_2_val);
		led_2_val = ~led_2_val;
		ev2 = millis();
	}
}
*/



int brightness = 0;
int brightness_b = 0; //控制两盏led的亮度

int increment = 5; //步进值为5
unsigned long int ev1 = 0;
unsigned long int ev2 = 0;
int flag_ev1 = 0;
int flag_ev2 = 0; //flag=0 逐渐变亮延时20ms flag=1 逐渐变暗延时20ms
int delay_ev1 = 0;
int delay_ev2 = 0; //任务调度延时

void setup()
{

}


void loop()
{
	if ((millis() - ev1) >= delay_ev1) { //任务1
		analogWrite(5, brightness);
		if (brightness >= 255) {
			flag_ev1 = 1;
			delay_ev1 = 500;
		}
		else if (brightness <= 0) {
			flag_ev1 = 0;
			delay_ev1 = 500;
		}
		else if(brightness>0&&brightness<255){
			delay_ev1 = 20;
		}
		if(flag_ev1==0)
			brightness += 5;
		else
			brightness -= 5;
		ev1 = millis();
	}

	if ((millis() - ev2) >= delay_ev2) { //任务2
		analogWrite(3, brightness_b);
		if (brightness_b >= 255) {
			flag_ev2 = 1;
			delay_ev2 = 200;
		}
		else if (brightness_b <= 0) {
			flag_ev2 = 0;
			delay_ev2 = 200;
		}
		else if (brightness_b>0 && brightness_b<255) {
			delay_ev2 = 50;
		}
		if (flag_ev2 == 0)
			brightness_b += 5;
		else
			brightness_b -= 5;
		ev2 = millis();
	}

}

/*
// 定时器闪灯=========
#include <MsTimer2.h>
int led_val = 0;

void blink_other_led(void) {
	digitalWrite(11, led_val);
	led_val = ~led_val;
} //这个函数和多任务的服务进行绑定。

void setup()
{
	pinMode(13, OUTPUT);
	pinMode(11, OUTPUT);
	MsTimer2::set(1000, blink_other_led); //采用MsTimer2这个库进行多任务操作，每1000ms执行一次函数
											//blink_other_led
	MsTimer2::start(); //启动多任务  
}

void loop()
{
	digitalWrite(13, HIGH);
	delay(500);
	digitalWrite(13, LOW);
	delay(500); //这里的loop正常写delay，当做这里有一个占用CPU很大的事件，因为这里进行长时间的延时
}
*/
