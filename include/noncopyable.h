#pragma once

/*
继承noncopyable的类将无法进行拷贝构造和拷贝赋值操作
*/

class noncopyable {
public:

    noncopyable(const noncopyable &) = delete; // 显式删除拷贝构造函数
    noncopyable &operator=(const noncopyable &) = delete; // 显示删除拷贝赋值

protected:

    noncopyable() = default;
    ~noncopyable() = default;

};