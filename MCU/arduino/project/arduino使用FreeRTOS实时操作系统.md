# How to use FreeRTOS with Arduino – Real time operating system

**如何让Arduino运行FreeRTOS实时操作系统** 

在本文中，您将学习如何使用带Arduino的Freertos操作系统来执行特定的任务。实时操作系统又称RTOS，是一种旨在满足实时应用要求的操作系统。它能够处理输入的数据，通常不需要缓冲延迟。RTOS是调用预定义函数的组合。实时操作系统中的关键因素是最小的中断延迟和最小的线程切换延迟。实时操作系统的价值更多地体现在它对给定时间段内完成任务的响应速度和可预测性。

[![FreeRTOS with Arduino](http://microcontrollerslab.com/wp-content/uploads/2017/07/FreeRTOS-with-Arduino.jpg)](http://microcontrollerslab.com/wp-content/uploads/2017/07/FreeRTOS-with-Arduino.jpg)

有三种 RTOS 实时操作系统:

- Hard RTOS; 必须在规定的期限内完成任务；
- Firm RTOS; 任务有时间限制，不过如果错过也能接受；
- Soft RTOS; 任务无时间限制

Examples of RTOS:

- LynxOS
- RTLinux
- VxWorks
- FreeRTOS
- OSE
- QNX
- Windows CE

**Why RTOS are required:**

当我们编写好的嵌入式软件时，我们不需要RTOS，但是当它的复杂性和大小增加时，RTOS总是有益的，原因如下：

 - 摘要时间信息
 - 可维护性/可扩展性
 - 模块化
 - 清洁界面
 - 更容易测试（在某些情况下）
 - 代码重用
 - 提高效率
- 空闲时间
 - 灵活的中断处理
 - 混合加工要求
 - 更轻松地控制外围设备
 
这些是RTOS的优点，但也有一些缺点，如下所示;

 - 低优先级任务
 - 代码精度
 - 有限的任务
 - 复杂算法
 - 设备驱动程序和中断信号
 - 线程优先级
 - 贵
 - 不容易编程

该项目中使用的操作系统是FreeRTOS。 FreeRTOS由Real Time Engineers Ltd.开发.FreeRTOS是一种流行的实时操作系统内核。 FreeRTOS是开源的。 它用于嵌入在所述35微控制器中的嵌入式设备。 它主要用C语言编写，但有些函数是用汇编语言编写的。 还有在线提供的SafeRTOS和OpenRTOS，与FreeRTOS类似。

**使用Arduino进行任务切换：**

在这个项目中，3 Led表示三个任务，一个Led表示空闲状态。 三个任务分别是Task1，Task2和Task3。

**代码:**

```c
#include <Arduino_FreeRTOS.h>

void setup()
{
// 初始化串口波特率
Serial.begin(9600);

Serial.println(F("In Setup function"));

// 3个led灯 任务，1个led空闲任务
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);

  xTaskCreate(MyTask1, "Task1", 100, NULL, 1, NULL); // 三个任务，标签为"Task1"~"Task3" ，优先权分别为1~3
  xTaskCreate(MyTask2, "Task2", 100, NULL, 2, NULL);
  xTaskCreate(MyTask3, "Task3", 100, NULL, 3, NULL);

  xTaskCreate(MyIdleTask, "IdleTask", 100, NULL, 0, NULL);}// 第四个，空闲任务，最高优先级0级


void loop()
{
// 无
}

// 任务1 Task1====
static void MyTask1(void* pvParameters)
{
  while(1)
  { 
    digitalWrite(8,HIGH); // 8号管脚灯亮
    digitalWrite(9,LOW); 
    digitalWrite(10,LOW);
    digitalWrite(11,LOW); 
    Serial.println(F("Task1"));
    vTaskDelay(100/portTICK_PERIOD_MS);// 延时
  }
 }

// 任务2====
static void MyTask2(void* pvParameters)

{ 
 while(1)
  { 
    digitalWrite(8,LOW);
    digitalWrite(9,HIGH);  // 9号管脚灯亮
    digitalWrite(10,LOW);
    digitalWrite(11,LOW);   
    Serial.println(F("Task2"));
    vTaskDelay(110/portTICK_PERIOD_MS);// 延时
    }
 }
 
 
// 任务3=====
static void MyTask3(void* pvParameters)
{ 
 while(1)
  { 
    digitalWrite(8,LOW);
    digitalWrite(9,LOW); 
    digitalWrite(10,HIGH);// 10号管脚灯亮
    digitalWrite(11,LOW);
    Serial.println(F("Task3"));
    vTaskDelay(120/portTICK_PERIOD_MS);// 延时
  }
}

// 空闲任务，无任务时，就执行该空闲时期任务

static void MyIdleTask(void* pvParameters)
{
 while(1)
  { 
    digitalWrite(8,LOW);
    digitalWrite(9,LOW); 
    digitalWrite(10,LOW);
    digitalWrite(11,HIGH); // 11号管脚灯亮
    Serial.println(F("Idle state"));
    delay(50);// 延时
    }
}
```
每个任务有不同的优先级以及延时时间

**Implementation:**



**Serial Monitor:![FreeRTOS tasks with Arduino](http://microcontrollerslab.com/wp-content/uploads/2017/07/FreeRTOS-tasks-with-Arduino.jpg)**

**link:**  http://microcontrollerslab.com/use-freertos-arduino/
