/****************************************************************************
 
 Firmware\build_px4fmu-v2_default\build_px4fmu-v2_default\px4fmu-v2\NuttX\nuttx\arch\arm\src\chip\stm32_start.c
 
  然而在初期，最痛苦的还是Nuttx的资料不够，简直是稀少。所以也就只能一点点地啃源码以期望能够理清Nuttx的系统流程与基本轮廓。
  毕竟跑系统不比跑裸机，自己用到的资源一目了然。引入了C++之后有些东西就更加显得不是那么的清晰了。
       其实到这个文档写完《1.2 命令》的时候我才发现我反了一个很严重的错误：我一直都在以裸奔的方式在分析PX4，
	   这给我带来了极大的麻烦，使得我一开始就在代码中找寻应用启动的过程。其实Nuttx只是启动了系统而已，对于应用，那是在脚本中启动的。
1. 走流程
       Nuttx系统我并没有完全搞透，只是从系统的第一条代码到我们的飞控启动的基本轮廓理清了而已，各中细节还得深入分析。
       那么我们先来讲解系统启动后的第一条代码即入口。
1.1 入口
       PX4使用的是STM32，入口自然就在stm32_start.c中：
 
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>
#include <assert.h>
#include <debug.h>

#include <nuttx/init.h>
#include <arch/board/board.h>

#include "up_arch.h"
#include "up_internal.h"

#include "stm32.h"
#include "stm32_gpio.h"
#include "stm32_userspace.h"

#ifdef CONFIG_ARCH_FPU
#  include "nvic.h"
#endif

/****************************************************************************
 * Private Function prototypes
 ****************************************************************************/

#ifdef CONFIG_ARCH_FPU
static inline void stm32_fpuconfig(void);
#endif
#ifdef CONFIG_STACK_COLORATION
static void go_os_start(void *pv, unsigned int nbytes)
  __attribute__ ((naked, no_instrument_function, noreturn));
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: showprogress
 *
 * Description:
 *   Print a character on the UART to show boot status.
 *
 ****************************************************************************/

#ifdef CONFIG_DEBUG_FEATURES
#  define showprogress(c) up_lowputc(c)
#else
#  define showprogress(c)
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#ifdef CONFIG_ARMV7M_STACKCHECK
/* we need to get r10 set before we can allow instrumentation calls */

void __start(void) __attribute__ ((no_instrument_function));
#endif

/****************************************************************************
 * Name: stm32_fpuconfig
 *
 * Description:
 *   Configure the FPU.  Relative bit settings:
 *
 *     CPACR:  Enables access to CP10 and CP11
 *     CONTROL.FPCA: Determines whether the FP extension is active in the
 *       current context:
 *     FPCCR.ASPEN:  Enables automatic FP state preservation, then the
 *       processor sets this bit to 1 on successful completion of any FP
 *       instruction.
 *     FPCCR.LSPEN:  Enables lazy context save of FP state. When this is
 *       done, the processor reserves space on the stack for the FP state,
 *       but does not save that state information to the stack.
 *
 *  Software must not change the value of the ASPEN bit or LSPEN bit while either:
 *   - the CPACR permits access to CP10 and CP11, that give access to the FP
 *     extension, or
 *   - the CONTROL.FPCA bit is set to 1
 *
 ****************************************************************************/

#ifdef CONFIG_ARCH_FPU
#if defined(CONFIG_ARMV7M_CMNVECTOR) && !defined(CONFIG_ARMV7M_LAZYFPU)

static inline void stm32_fpuconfig(void)
{
  uint32_t regval;

  /* Set CONTROL.FPCA so that we always get the extended context frame
   * with the volatile FP registers stacked above the basic context.
   */

  regval = getcontrol();
  regval |= (1 << 2);
  setcontrol(regval);

  /* Ensure that FPCCR.LSPEN is disabled, so that we don't have to contend
   * with the lazy FP context save behaviour.  Clear FPCCR.ASPEN since we
   * are going to turn on CONTROL.FPCA for all contexts.
   */

  regval = getreg32(NVIC_FPCCR);
  regval &= ~((1 << 31) | (1 << 30));
  putreg32(regval, NVIC_FPCCR);

  /* Enable full access to CP10 and CP11 */

  regval = getreg32(NVIC_CPACR);
  regval |= ((3 << (2*10)) | (3 << (2*11)));
  putreg32(regval, NVIC_CPACR);
}

#else

static inline void stm32_fpuconfig(void)
{
  uint32_t regval;

  /* Clear CONTROL.FPCA so that we do not get the extended context frame
   * with the volatile FP registers stacked in the saved context.
   */

  regval = getcontrol();
  regval &= ~(1 << 2);
  setcontrol(regval);

  /* Ensure that FPCCR.LSPEN is disabled, so that we don't have to contend
   * with the lazy FP context save behaviour.  Clear FPCCR.ASPEN since we
   * are going to keep CONTROL.FPCA off for all contexts.
   */

  regval = getreg32(NVIC_FPCCR);
  regval &= ~((1 << 31) | (1 << 30));
  putreg32(regval, NVIC_FPCCR);

  /* Enable full access to CP10 and CP11 */

  regval = getreg32(NVIC_CPACR);
  regval |= ((3 << (2*10)) | (3 << (2*11)));
  putreg32(regval, NVIC_CPACR);
}

#endif

#else
#  define stm32_fpuconfig()
#endif

/****************************************************************************
 * Name: go_os_start
 *
 * Description:
 *   Set the IDLE stack to the
 *
 ****************************************************************************/

#ifdef CONFIG_STACK_COLORATION
static void go_os_start(void *pv, unsigned int nbytes)
{
  /* Set the IDLE stack to the stack coloration value then jump to
   * os_start().  We take extreme care here because were currently
   * executing on this stack.
   *
   * We want to avoid sneak stack access generated by the compiler.
   */

  __asm__ __volatile__
  (
    "\tmovs r1, r1, lsr #2\n"   /* R1 = nwords = nbytes >> 2 */
    "\tbeq  2f\n"               /* (should not happen) */

    "\tbic  r0, r0, #3\n"       /* R0 = Aligned stackptr */
    "\tmovw r2, #0xbeef\n"      /* R2 = STACK_COLOR = 0xdeadbeef */
    "\tmovt r2, #0xdead\n"

    "1:\n"                      /* Top of the loop */
    "\tsub  r1, r1, #1\n"       /* R1 nwords-- */
    "\tcmp  r1, #0\n"           /* Check (nwords == 0) */
    "\tstr  r2, [r0], #4\n"     /* Save stack color word, increment stackptr */
    "\tbne  1b\n"               /* Bottom of the loop */

    "2:\n"
    "\tmov  r14, #0\n"          /* LR = return address (none) */
    "\tb    os_start\n"         /* Branch to os_start */
  );
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: _start
 *
 * Description:
 *   This is the reset entry point.
 *
 ****************************************************************************/

void __start(void)
{
  const uint32_t *src;
  uint32_t *dest;

#ifdef CONFIG_ARMV7M_STACKCHECK
  /* Set the stack limit before we attempt to call any functions */

  __asm__ volatile ("sub r10, sp, %0" : : "r" (CONFIG_IDLETHREAD_STACKSIZE - 64) : );
#endif

  /* Configure the UART so that we can get debug output as soon as possible */

  stm32_clockconfig(); // 初始化时钟
  stm32_fpuconfig();   // 初始化FPU  Float Point Unit，浮点运算单元
  stm32_lowsetup();    // 串口低电平启动  stm32_lowputc.c 下
  stm32_gpioinit();    // GPIO口初始化    stm32_gpio.c     下
  showprogress('A');   //

  /* Clear .bss.  We'll do this inline (vs. calling memset) just to be
   * certain that there are no issues with the state of global variables.
   */

  for (dest = _START_BSS; dest < _END_BSS; )
    {
      *dest++ = 0;
    }

  showprogress('B');

  /* Move the initialized data section from his temporary holding spot in
   * FLASH into the correct place in SRAM.  The correct place in SRAM is
   * give by _sdata and _edata.  The temporary location is in FLASH at the
   * end of all of the other read-only data (.text, .rodata) at _eronly.
   */

  for (src = _DATA_INIT, dest = _START_DATA; dest < _END_DATA; )
    {
      *dest++ = *src++;
    }

  showprogress('C');

#ifdef CONFIG_ARMV7M_ITMSYSLOG
  /* Perform ARMv7-M ITM SYSLOG initialization */

  itm_syslog_initialize();
#endif

  /* Perform early serial initialization */

#ifdef USE_EARLYSERIALINIT
  up_earlyserialinit();
#endif
  showprogress('D');

  /* For the case of the separate user-/kernel-space build, perform whatever
   * platform specific initialization of the user memory is required.
   * Normally this just means initializing the user space .data and .bss
   * segments.
   */

#ifdef CONFIG_BUILD_PROTECTED
  stm32_userspace();
  showprogress('E');
#endif

  /* Initialize onboard resources */

  stm32_boardinitialize();//板上资源初始化  设置启动电压，配置SPI芯片 以及USB芯片初始化  
  //初始化SPI（飞控板上的传感器都是通过SPI通信）和LEDs。 
  showprogress('F');

  /* Then start NuttX */

  showprogress('\r');
  showprogress('\n');

#ifdef CONFIG_STACK_COLORATION
  /* Set the IDLE stack to the coloration value and jump into os_start() */

  go_os_start((FAR void *)&_ebss, CONFIG_IDLETHREAD_STACKSIZE);
#else
  /* Call os_start() */

  os_start();// \build_px4fmu-v2_default\px4fmu-v2\NuttX\nuttx\sched\init\os_start.c 
  //启动Nuttu系统，初始化操作系统底层，包括各种队列和进程结构的初始化
  // ——> os_bringup()——> init 进程函数  CONFIG_USER_ENTRYPOINT  nsh_main ——> nsh_consolemain ——> nsh_initscript ——> nsh_script —— > 打开 init.d/rcS 脚本文件 ——> nsh_parse() 解析这些脚本的关键
  // os_bringup 创建任务并初始化，负责操作系统OS基本进程的启动以及用户进程的启动
  // CONFIG_USER_ENTRYPOINT  有两种定义 宏定义 nsh_main  执行 rcC脚本初始化系统 程序 执行飞控程序
  // user_start  负责PX4IO基础环境的初始化  PWM  串口 ADC 等外设的初始化，最后进入一个死循环，用于处理遥控器的输入，与PX4FMU通讯
  
  /* 
    nsh_parse() 解析这些脚本的关键
    nsh_builtin和nsh_execute分别对应两个数组：g_builtins和g_cmdmap。
	什么区别呢？在我看来这两组命令是可以合并到一起的，但如果真的这样做了会有一个问题，用户扩展不方便。
	于是对其进行区分。像ls这样的命令不需要用户实现及处理，就作为系统命令在g_cmdmap数组中，
	而作为用户的应用程序如ArduPilot完全有用户决定，便作为用户命令在g_builtins数组中。
	对于系统命令我们常用的基本都已经实现，通过特定的宏进行选择编译。
	而用户命令我们只需按照相应规则编写一个入口函数然并设定堆栈大小然后放到g_builtins数组中即可。
    Nuttx本身提供了一个g_builtins数组，但PX4没有使用。PX4使用了一些技巧，
	利用Makefile在编译的时候自动生成g_builtins数组。这样做好处是我们修改源码的时候可以不关心这个数组，
	
	但坏处也显而易见：增加了我们刚接触是阅读源码的壁垒。
	
	前面我们说过Nuttx的启动脚本是/etc/init.d/rcS，跟Linux是一样的。这样做也有一个好处，
	
	就是如果你对Linux比较熟悉即便你完全不看源码只要看下根文件系统你就能够找到启动脚本并进行分析。
	
  */
  
  /* Shoulnd't get here */

  for (; ; );
#endif
}
