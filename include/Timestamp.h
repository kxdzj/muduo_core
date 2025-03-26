#pragma once

#include<string>

class Timestamp{
public:

    Timestamp();
    explicit Timestamp(int64_t microSecondSinceEpoch); 
    static Timestamp now();
    std::string toString() const;

private:

    int64_t microSecondSinceEpoch_;
    
};