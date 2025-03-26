#pragma once

#include <unistd.h>
#include <sys/syscall.h>

namespace CurrentThread{
    // __thread 线程局部存储
    extern __thread int t_cachedTid;

    // 执行系统调用来获取当前线程的 tid，然后存入 t_cachedTid
    void cacheTid();

    inline int tid(){
      // __builtin_expect(x, y) 是 GCC 提供的分支预测优化
      // x 是一个布尔表达式 y 是期望的结果 0不太可能 1很可能
      if (__builtin_expect(t_cachedTid == 0, 0))
      {
          cacheTid();
      }
      return t_cachedTid;
    }
    
}// namespace CurrentThread