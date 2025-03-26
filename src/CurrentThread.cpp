#include "../include/CurrentThread.h"
#include <sys/syscall.h>
#include <unistd.h>

namespace CurrentThread {
    __thread int t_cachedTid = 0; // 线程局部变量，每个线程独立

    void cacheTid() {
        t_cachedTid = static_cast<int>(::syscall(SYS_gettid));  // 获取线程 ID
    }
}
