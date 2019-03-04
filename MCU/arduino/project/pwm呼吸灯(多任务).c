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
