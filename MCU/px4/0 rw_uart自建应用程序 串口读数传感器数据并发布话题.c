/****************************************************************************
 *
本意是采用串口的方式添加一个自定义传感器——超声波。

已知的是超声波模块是通过串口方式发送（Tx）出数据，使用的模块数据发送周期为100ms，数据格式为：

R0034 R0122 R0122 R0046 R0127 R0044 R0044 R0125 R0034 R0037 R0041 R0122 R0122 .....
1
1
则可以通过Pixhawk板上的串口来接收（Rx）数据，即将超声波的Tx接口连接到Pixhawk板上的Rx接口。 这个想法就目前来说是非常好并且实施相对最简单的，毕竟PX4源码框架很大，十分复杂，在其原有的基础上改代码需要对这套系统有着很深的理解，所有初学者确实可以采用这种“协同”的方式连入到Pixhawk，可能用一个外部运算能力强的处理器算出姿态或位置或视觉数据经过串口发送到Pixhawk中，怎么把这些数据融合进原生算法那就另说了。

OK，继续主题，Pixhawk板上串口说明如下：

NuttX UART	Pixhawk UART
/dev/ttyS0	IO DEBUG(RX ONLY)
/dev/ttyS1	TELEM1(USART2)
/dev/ttyS2	TELEM2(USART3)
/dev/ttyS3	GPS(UART4)
/dev/ttyS4	N/A(UART5, IO link)
/dev/ttyS5	SERIAL5(UART7,NSH Console Only)
/dev/ttyS6	SERIAL4(UART8)
测试使用Pixhawk板上TELEM2接口的USART2，对应的Nuttx UART设备文件尾/dev/ttyS2：

接口线序  http://www.pixhawk.com/modules/pixhawk

具体的实现过程请查看irmware/src/drivers/hott/comms.cpp，
里面是关于串口的设置，设计到NuttX这个类Unix操作系统的文件系统了，小白有点难啃。


2. 在rw_uart文件夹中创建CMakeLists.txt文件，并输入以下内容：

set(MODULE_CFLAGS)
px4_add_module(
    MODULE modules__rw_uart
    MAIN rw_uart
    COMPILE_FLAGS
        -Os
    SRCS
        rw_uart.c
    DEPENDS
        platforms__common
    )
# vim: set noet ft=cmake fenc=utf-8 ff=unix : 

3. 注册新添加的应用到NuttShell中。Firmware/cmake/configs/nuttx_px4fmu-v2_default.cmake

文件中添加如下内容：
	#
	# General system control
	#
	modules/commander  #控制器指挥官
	modules/events     #事件中断触发
	modules/load_mon
	modules/navigator  #导航
	modules/mavlink    #上下位机通信
	modules/gpio_led   #通用IO口
	#modules/uavcan
	modules/land_detector#着陆检测
	
# 自添加 模程序  串口读取 超声波数
	modules/rw_uart
	
4. 编译并上传固件
make px4fmu-v2_default upload



5. 读取测试
查看app 
在NSH终端中输入help，在Builtin Apps中出现rw_uart应用。 

运行rw_uart应用（前提是模块与Pixhawk连接好） 

设置好发送装置 

这里我使用的是秉火STM32开发板的USART1给Pixhawk的TELEM2发送对应的字符串。
根据代码，数据的格式要求很严格，不然很可能得不到数据，博主开始设置让串口疯狂的发Rxxxx，
结果速度太快是不行的，后来给加了一个延时函数，更新频率设置为20Hz，有时候还是会有乱码出现。
各位进行设置的时候需要注意波特率对应，数据格式按要求来。	

int main(void)
{   

  // USARTx config 9600 8-N-1 //
    USARTx_Config();    

   //配置SysTick为每10us中断一次//
    SysTick_Init();

    printf("R1000 ");

  for(;;){
        int i = 1000;
        for(i=1001;i<2000;i++)
        {
            printf("R%d ",i);
            Delay_us(5000); // 5000*10us = 50ms
        }
    }
}


本博客承接前一篇，对FreeApe的串口添加超声波传感器博文后半部分进行学习。


为什么叫前奏呢，因为用了伪传感器，把单片机用串口发送的有规律的数据当作了传感器读取到的数据。但是无碍的。

开发环境：Ubuntu  Firmware 1.4.1

在无人机运行时，首先是要将应用随系统启动时就启动起来的，且将获得的超声波数据不断的发布出去，

从而让其他应用得以订阅使用。这里也使用Pixhawk里面的通用模式，即主线程，检测app命令输入，创建一个线程来不断的发布数据。
 
 定义主题和发布主题

在Firmware/msg目录下新建read_uart_sensor.msg文件。传送门
char[4] datastr
int16 data

# TOPICS read_uart_sensor   
注意 
# TOPICS # 号和TOPICS中间有一个空格。

并添加到CMakeLists.txt中( Firmware/msg中的CMakeLists.txt中添加：read_uart_sensor.msg)，
编译后自动生成uORB/topics/read_uart_sensor.h头文件
 
 
串口读取例程

在Firmware/src/modules目录下新建文件夹read_uart_sensor

添加文件read_uart_sensor.c。传送门

 * read_uart_sensor.c
 * 
 * read sensor through uart


#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <drivers/drv_hrt.h>
#include <string.h>
#include <systemlib/err.h>
#include <systemlib/systemlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <uORB/topics/read_uart_sensor.h>

__EXPORT int read_uart_sensor_main(int argc, char *argv[]);


static bool thread_should_exit = false;//Ddemon exit flag// 
static bool thread_running = false; ///Daemon status flag// 
static int daemon_task;

// 
//Main loop
// 
int read_uart_thread_main(int argc, char *argv[]);

static int uart_init(const char * uart_name);
static int set_uart_baudrate(const int fd, unsigned int baud);
static void usage(const char *reason);

int set_uart_baudrate(const int fd, unsigned int baud)
{
    int speed;

    switch (baud) {
        case 9600:   speed = B9600;   break;
        case 19200:  speed = B19200;  break;
        case 38400:  speed = B38400;  break;
        case 57600:  speed = B57600;  break;
        case 115200: speed = B115200; break;
        default:
            warnx("ERR: baudrate: %d\n", baud);
            return -EINVAL;
    }

    struct termios uart_config;

    int termios_state;

  //fill the struct for the new configuration // 
    tcgetattr(fd, &uart_config);
	 
   //clear ONLCR flag (which appends a CR for every LF)// 
    uart_config.c_oflag &= ~ONLCR;
    
   // no parity, one stop bit// 
    uart_config.c_cflag &= ~(CSTOPB | PARENB);
   
   
   //set baud rate // 
    if ((termios_state = cfsetispeed(&uart_config, speed)) < 0) {
        warnx("ERR: %d (cfsetispeed)\n", termios_state);
        return false;
    }

    if ((termios_state = cfsetospeed(&uart_config, speed)) < 0) {
        warnx("ERR: %d (cfsetospeed)\n", termios_state);
        return false;
    }

    if ((termios_state = tcsetattr(fd, TCSANOW, &uart_config)) < 0) {
        warnx("ERR: %d (tcsetattr)\n", termios_state);
        return false;
    }

    return true;
}


int uart_init(const char * uart_name)
{
    int serial_fd = open(uart_name, O_RDWR | O_NOCTTY);

    if (serial_fd < 0) {
        err(1, "failed to open port: %s", uart_name);
        return false;
    }
    return serial_fd;
}

static void usage(const char *reason)
{
    if (reason) {
        fprintf(stderr, "%s\n", reason);
    }

    fprintf(stderr, "usage: read_uart_sensor {start|stop|status} [param]\n\n");
    exit(1);
}

int read_uart_sensor_main(int argc, char *argv[])
{
    if (argc < 2) {
        usage("[Fantasy]missing command");
    }

    if (!strcmp(argv[1], "start")) {
        if (thread_running) {
            warnx("[Fantasy]already running\n");
            return 0;
        }

        thread_should_exit = false;
        daemon_task = px4_task_spawn_cmd("read_uart_sensor",
                         SCHED_DEFAULT,
                         SCHED_PRIORITY_MAX - 5,
                         2000,
                         read_uart_thread_main,
                         (argv) ? (char * const *)&argv[2] : (char * const *)NULL);
        return 0;
    }

    if (!strcmp(argv[1], "stop")) {
        thread_should_exit = true;
        return 0;
    }

    if (!strcmp(argv[1], "status")) {
        if (thread_running) {
            warnx("[Fantasy]running");
            return 0;

        } else {
            warnx("[Fantasy]stopped");
            return 1;
        }

        return 0;
    }

    usage("unrecognized command");
    return 1;
}

int read_uart_thread_main(int argc, char *argv[])
{

    if (argc < 2) {
        errx(1, "[Fantasy]need a serial port name as argument");
        usage("eg:");
    }

    const char *uart_name = argv[1];

    warnx("[Fantasy]opening port %s", uart_name);
    char data = '0';
    char buffer[4] = "";
 // 
     * TELEM1 : /dev/ttyS1
     * TELEM2 : /dev/ttyS2
     * GPS    : /dev/ttyS3
     * NSH    : /dev/ttyS5
     * SERIAL4: /dev/ttyS6
     * N/A    : /dev/ttyS4
     * IO DEBUG (RX only):/dev/ttyS0
   // 
    int uart_read = uart_init(uart_name);
    if(false == uart_read)return -1;
    if(false == set_uart_baudrate(uart_read,9600)){
        printf("[Fantasy]set_uart_baudrate is failed\n");
        return -1;
    }
    printf("[Fantasy]uart init is successful\n");

    thread_running = true;

   //初始化数据结构体 // 
    struct read_uart_sensor_s sensordata;
    memset(&sensordata, 0, sizeof(sensordata));
   //公告主题 // 
    orb_advert_t read_uart_sensor_pub = orb_advertise(ORB_ID(read_uart_sensor), &sensordata);

    while(!thread_should_exit){
        read(uart_read,&data,1);
        if(data == 'R'){
            for(int i = 0;i <4;++i){
                read(uart_read,&data,1);
                buffer[i] = data;
                data = '0';
            }
            strncpy(sensordata.datastr,buffer,4);// 复制字符串Buffer中前４个数字到Datastr中
            sensordata.data = atoi(sensordata.datastr);//将字符串转换成整形数据
            //printf("[Fantasy]sensor.data=%d\n",sensordata.data);
            orb_publish(ORB_ID(read_uart_sensor), read_uart_sensor_pub, &sensordata);
        }
    }

    warnx("[Fantasy]exiting");
    thread_running = false;
    close(uart_read);

    fflush(stdout);
    return 0;
}


添加CMakeLists.txt文件
set(MODULE_CFLAGS)
px4_add_module(
        MODULE modules__read_uart_sensor
        MAIN read_uart_sensor
    COMPILE_FLAGS
        -Os
    SRCS
                read_uart_sensor.c
    DEPENDS
        platforms__common
    )
# vim: set noet ft=cmake fenc=utf-8 ff=unix : 



在Firmware/cmake/configs/nuttx/nuttx_px4fmu-v2_default.cmake中注册该模块


测试发布的主题

测试可以随便一个启动的app中进行主题订阅，然后将订阅的数据打印出来，看是否是超声波的数据。这里新建一个应用px4_test进行测试。


 * px4_test.c
 *  
 * test the uart sensor app

#include <px4_config.h>
#include <px4_tasks.h>
#include <px4_posix.h>
#include <unistd.h>
#include <stdio.h>
#include <poll.h>
#include <string.h>
#include <math.h>

#include <uORB/uORB.h>
#include <uORB/topics/read_uart_sensor.h>

__EXPORT int px4_test_main(int argc, char *argv[]);

int px4_test_main(int argc, char *argv[])
{
    PX4_INFO("Hello Sky!\n");

  
  //subscribe to rw_uart_sensor topic/// 
    int sensor_sub_fd = orb_subscribe(ORB_ID(read_uart_sensor));

    //limit the update rate to 20 Hz // 
    orb_set_interval(sensor_sub_fd, 50);


    //one could wait for multiple topics with this technique, just using one here // 
    px4_pollfd_struct_t fds[] = {
        { .fd = sensor_sub_fd,   .events = POLLIN },
       //there could be more file descriptors here, in the form like:
        //{ .fd = other_sub_fd,   .events = POLLIN },
        // 
    };

    int error_counter = 0;

    for (int i = 0; ; i++) { // infinite loop
      // wait for sensor update of 1 file descriptor for 1000 ms (1 second) // 
        int poll_ret = poll(fds, 1, 1000);

       // handle the poll result // 
        if (poll_ret == 0) {
        	
           //this means none of our providers is giving us data // 
            printf("[px4_test] Got no data within a second\n");

        } else if (poll_ret < 0) {
           // this is seriously bad - should be an emergency // 
            if (error_counter < 10 || error_counter % 50 == 0) {
                //use a counter to prevent flooding (and slowing us down)/ 
                printf("[px4_test] ERROR return value from poll(): %d\n", poll_ret);
            }

            error_counter++;

        } else {

            if (fds[0].revents & POLLIN) {
            	
                / obtained data for the first file descriptor / 
                
                struct read_uart_sensor_s raw;
                
                /copy sensors raw data into local buffer / 
                orb_copy(ORB_ID(read_uart_sensor), sensor_sub_fd, &raw);
                PX4_INFO("[px4_test] sensor data:\t%s\t%d\n",
                       raw.datastr,
                       raw.data);
            }

           /there could be more file descriptors here, in the form like:
           /if (fds[1..n].revents & POLLIN) {}
            / 
        }
    }

    PX4_INFO("exiting");

    return 0;
}


编译并上传固件

make px4fmu-v2_default upload
在NSH中测试

read_uart_sensor start /dev/ttyS2
px4_test


添加到脚本文件

在rcS中仿照mavlink的启动方式添加了上面的应用，使得与ttyS2连接的外设默认为启动状态 

read_uart_sensor start  /dev/ttyS2


现在如果ttyS2上连接了设备，就是自动启动的，在nsh中可以直接调用px4_test应用就可以读取数据了，与使用px4_simple_app读取内部传感器的方式无异。
大致模型就是这样了，接下来就是具体的代码优化以及应用了。
 ****************************************************************************/




 
/* 
 * 串口读取函数
 * rw_uart.c 
 */
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <drivers/drv_hrt.h>
#include <string.h>
#include <systemlib/err.h>
#include <systemlib/systemlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

__EXPORT int rw_uart_main(int argc, char *argv[]);

static int uart_init(char * uart_name);
static int set_uart_baudrate(const int fd, unsigned int baud);

int set_uart_baudrate(const int fd, unsigned int baud)
{
    int speed;

    switch (baud) {
        case 9600:   speed = B9600;   break;
        case 19200:  speed = B19200;  break;
        case 38400:  speed = B38400;  break;
        case 57600:  speed = B57600;  break;
        case 115200: speed = B115200; break;
        default:
            warnx("ERR: baudrate: %d\n", baud);
            return -EINVAL;
    }

    struct termios uart_config;

    int termios_state;

    /* 以新的配置填充结构体 */
    /* 设置某个选项，那么就使用"|="运算，
     * 如果关闭某个选项就使用"&="和"~"运算
     * */
    tcgetattr(fd, &uart_config); // 获取终端参数

    /* clear ONLCR flag (which appends a CR for every LF) */
    uart_config.c_oflag &= ~ONLCR;// 将NL转换成CR(回车)-NL后输出。

    /* 无偶校验，一个停止位 */
    uart_config.c_cflag &= ~(CSTOPB | PARENB);// CSTOPB 使用两个停止位，PARENB 表示偶校验

     /* 设置波特率 */
    if ((termios_state = cfsetispeed(&uart_config, speed)) < 0) {
        warnx("ERR: %d (cfsetispeed)\n", termios_state);
        return false;
    }

    if ((termios_state = cfsetospeed(&uart_config, speed)) < 0) {
        warnx("ERR: %d (cfsetospeed)\n", termios_state);
        return false;
    }
    // 设置与终端相关的参数，TCSANOW 立即改变参数
    if ((termios_state = tcsetattr(fd, TCSANOW, &uart_config)) < 0) {
        warnx("ERR: %d (tcsetattr)\n", termios_state);
        return false;
    }

    return true;
}


int uart_init(char * uart_name)
{
    int serial_fd = open(uart_name, O_RDWR | O_NOCTTY);
    /*Linux中，万物皆文件，打开串口设备和打开普通文件一样，使用的是open（）系统调用*/
    // 选项 O_NOCTTY 表示不能把本串口当成控制终端，否则用户的键盘输入信息将影响程序的执行
    if (serial_fd < 0) {
        err(1, "failed to open port: %s", uart_name);
        return false;
    }
//    printf("Open the %s\n",serial_fd);
    return serial_fd;
}

int rw_uart_main(int argc, char *argv[])
{
    char data = '0';
    char buffer[4] = "";
    /*
     * TELEM1 : /dev/ttyS1
     * TELEM2 : /dev/ttyS2
     * GPS    : /dev/ttyS3
     * NSH    : /dev/ttyS5
     * SERIAL4: /dev/ttyS6
     * N/A    : /dev/ttyS4
     * IO DEBUG (RX only):/dev/ttyS0
     */
    int uart_read = uart_init("/dev/ttyS2");
    if(false == uart_read)
        return -1;
    if(false == set_uart_baudrate(uart_read,9600)){
        printf("[JXF]set_uart_baudrate is failed\n");
        return -1;
    }
    printf("[JXF]uart init is successful\n");

    while(true){
        read(uart_read,&data,1);
        if(data == 'R'){
            for(int i = 0;i <4;++i){
                read(uart_read,&data,1);
                buffer[i] = data;
                data = '0';
            }
            printf("%s\n",buffer);
        }
    }

    return 0;
}
