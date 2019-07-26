# 个人整理常用

## 计时函数
```c
// window 平台
#include <windows.h>

double get_current_time()
{
    LARGE_INTEGER freq; // 频率
    LARGE_INTEGER pc;   // 计数
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&pc);
    
    return pc.QuadPart * 1000.0 / freq.QuadPart; // us
}

// android 平台 ios linux 等
#include <sys/time.h>

double get_current_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    
    return tv.tv_sec * 1000.0 + tv.tv_usec / 1000.0;
}


```
