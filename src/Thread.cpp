#include "../include/Thread.h"
#include "../include/CurrentThread.h"
#include <semaphore.h>
#include <system_error>
#include <stdexcept>

std::atomic_int Thread::numThreadCreated_(0);

Thread::Thread(ThreadFunc func, const std::string& name)
    : started_(false),
      joined_(false),
      threadid_(0),
      func_(std::move(func)),
      threadname_(name)
{
    setDefaultName();
}

void Thread::setDefaultName()
{
    int num = ++numThreadCreated_;
    if (threadname_.empty())
    {
        char buf[32] = {0};
        snprintf(buf, sizeof(buf), "Thread%d", num);
        threadname_ = buf;
    }
}

Thread::~Thread()
{
    if (started_ && !joined_ && thread_ && thread_->joinable())
    {
        thread_->detach();
    }
}

void Thread::start()
{
    if (started_.exchange(true)) {
        throw std::logic_error("Thread already started");
    }

    sem_t sem;
    sem_init(&sem, false, 0); 

    thread_ = std::make_shared<std::thread>([this, &sem]() {
        threadid_ = CurrentThread::tid();
        sem_post(&sem);
        func_(); 
    });

    sem_wait(&sem);
    sem_destroy(&sem);
}

void Thread::join()
{
    if (!started_) {
        throw std::logic_error("Thread not started");
    }
    if (joined_.exchange(true)) {
        throw std::logic_error("Thread already joined");
    }
    
    if (thread_ && thread_->joinable()) {
        thread_->join();
    }
}