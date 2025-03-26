#include "../include/Timestamp.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

Timestamp::Timestamp(): microSecondSinceEpoch_(0){}

Timestamp::Timestamp(int64_t microSecondSinceEpoch)
    :microSecondSinceEpoch_(microSecondSinceEpoch){}

Timestamp Timestamp::now(){
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto microSecondSinceEpoch = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
    return Timestamp(microSecondSinceEpoch);
}

std::string Timestamp::toString() const {

    time_t seconds = static_cast<time_t>(microSecondSinceEpoch_ / 1000000);
    // 计算剩余的微秒
    int microseconds = static_cast<int>(microSecondSinceEpoch_ % 1000000);
    
    // 转换为本地时间
    struct tm tm_time;
    localtime_r(&seconds, &tm_time);  // 使用线程安全版本
    
    // 格式化输出
    std::ostringstream oss;
    oss << std::put_time(&tm_time, "%Y-%m-%d %H:%M:%S")
        << "." << std::setfill('0') << std::setw(6) << microseconds;
    
    return oss.str();

}

