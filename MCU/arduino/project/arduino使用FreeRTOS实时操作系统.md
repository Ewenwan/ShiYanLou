# How to use FreeRTOS with Arduino – Real time operating system

**FREERTOS USING ARDUINO:  How to use FreeRTOS real time operating system with Arduino?** 

In this article you will learn how to use FreeRTOS operating system with Arduino to perform specific tasks. A Real Time Operating System also known as RTOS is an operating system which is intended to fulfills the requirement of real time application. It is able to process data as comes in, typically without buffering delays. RTOS is the combination of calling predefined functions. The key factors in Real Time Operating System are minimum interrupt latency and minimum threads switching latency. The Real Time Operating System is valued more for how quickly and how predictably it responds to complete the tasks in given period of time.

[![FreeRTOS with Arduino](http://microcontrollerslab.com/wp-content/uploads/2017/07/FreeRTOS-with-Arduino.jpg)](http://microcontrollerslab.com/wp-content/uploads/2017/07/FreeRTOS-with-Arduino.jpg)

There are three types of RTOS:

- Hard RTOS; bound to complete task in given deadline
- Firm RTOS; bound of deadline but if they miss the deadline it is acceptable but not in the case of Hard RTOS.
- Soft RTOS; not bound of any deadline.

Examples of RTOS:

- LynxOS
- RTLinux
- VxWorks
- FreeRTOS
- OSE
- QNX
- Windows CE

**Why RTOS are required:**

When we write good embedded software we do not need RTOS but when its complexity and size increases RTOS is always beneficial for the reasons listed below:

- Abstract out timing information
- Maintainability/Extensibility
- Modularity
- Cleaner interfaces
- Easier testing (in some cases)
- Code reuse
- Improved efficiency
- Idle time
- Flexible interrupt handling
- Mixed processing requirements
- Easier control over peripherals

These are the advantages of RTOS but there are also some disadvantages listed as below;

- Low Priority Tasks
- Precision of code
- Limited Tasks
- Complex Algorithms
- Device driver and interrupt signals
- Thread Priority
- Expensive
- Not easy to program

The OS used in this project is FreeRTOS. FreeRTOS is developed by Real Time Engineers Ltd. FreeRTOS is a popular Real Time Operating System kernel. FreeRTOS is open source. It is used for embedded devices which are ported in described 35 microcontrollers. It is mostly written in C but some functions are written in assembly. There are also SafeRTOS and OpenRTOS available online which are similar to FreeRTOS.

**Task switching using Arduino:**

In this project 3 Led indicates three tasks and one Led indicate idle state. Three tasks are labels as Task1, Task2 and Task3 respectively.

**Code:**

We includes the library file on FreeRTOS

```c
#include <Arduino_FreeRTOS.h>

void setup()

//Initialize the Serial Monitor with 9600 baud rate

{Serial.begin(9600);

Serial.println(F("In Setup function"));

//Set the digital pins 8 to 11 as output

  pinMode(8,OUTPUT);

  pinMode(9,OUTPUT);

  pinMode(10,OUTPUT);

  pinMode(11,OUTPUT);

//Create three tasks with labels Task1, Task2 and Task3 and assign the priority as 1, 2 and 3 //respectively. We also create fourth task labeled as IdelTask when the there is no task in //operation and it has the highest priority.

  xTaskCreate(MyTask1, "Task1", 100, NULL, 1, NULL);

  xTaskCreate(MyTask2, "Task2", 100, NULL, 2, NULL);

  xTaskCreate(MyTask3, "Task3", 100, NULL, 3, NULL);

  xTaskCreate(MyIdleTask, "IdleTask", 100, NULL, 0, NULL);}

//We can change the priority of task according to our desire by changing the numeric’s //between NULL texts.

void loop()

{

//There is no instruction in loop section of the code.

}

//The following function is Task1. We display the task label on Serial monitor.

static void MyTask1(void* pvParameters)

{while(1)

  { digitalWrite(8,HIGH);

    digitalWrite(9,LOW); 

    digitalWrite(10,LOW);

    digitalWrite(11,LOW); 

    Serial.println(F("Task1"));

    vTaskDelay(100/portTICK_PERIOD_MS);}}

//Similarly this is task 2

static void MyTask2(void* pvParameters)

{  while(1)

  { digitalWrite(8,LOW);

    digitalWrite(9,HIGH); 

    digitalWrite(10,LOW);

    digitalWrite(11,LOW);   

    Serial.println(F("Task2"));

    vTaskDelay(110/portTICK_PERIOD_MS);}}

//Similarly this is task 3

static void MyTask3(void* pvParameters)

{ while(1)

  { digitalWrite(8,LOW);

    digitalWrite(9,LOW); 

    digitalWrite(10,HIGH);

    digitalWrite(11,LOW);

    Serial.println(F("Task3"));

    vTaskDelay(120/portTICK_PERIOD_MS);}}

//This is the idle task which calls when no task is running.

static void MyIdleTask(void* pvParameters)

{while(1)

  { digitalWrite(8,LOW);

    digitalWrite(9,LOW); 

    digitalWrite(10,LOW);

    digitalWrite(11,HIGH);

    Serial.println(F("Idle state"));

    delay(50);}}
```

Each task has its unique priority and different running duration.

**Implementation:**



**Serial Monitor:![FreeRTOS tasks with Arduino](http://microcontrollerslab.com/wp-content/uploads/2017/07/FreeRTOS-tasks-with-Arduino.jpg)**

**link:**  http://microcontrollerslab.com/use-freertos-arduino/
