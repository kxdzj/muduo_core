#pragma once

#include <functional>
#include <thread>
#include <memory>
#include <unistd.h>
#include <string>
#include <atomic>

#include "Nocopy.h"

class Thread : nocopy{

public:
    using ThreadFunc = std::function<void()>;

    explicit Thread(ThreadFunc, const std::string &name = std::string());
    ~Thread();
    
    void start();
    void join();
    bool isStarted()  { return started_; }
    pid_t tid() const { return threadid_; }
    const std::string& name() const { return threadname_; }
    static int numCreated() { return numThreadCreated_; }


private:
    
    // 线程状态
    bool started_;
    bool joined_;
    // 实际的线程对象
    std::shared_ptr<std::thread> thread_;
    pid_t threadid_;
    ThreadFunc func_;
    std::string threadname_;
    // 创建的线程数量
    static std::atomic_int numThreadCreated_;
};