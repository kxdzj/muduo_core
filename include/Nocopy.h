#pragma once

class nocopy {
public:

    nocopy(const nocopy &) = delete; // 显式删除拷贝构造函数
    nocopy &operator=(const nocopy &) = delete; // 显示删除拷贝赋值

protected:

    nocopy() = default;
    ~nocopy() = default;

};