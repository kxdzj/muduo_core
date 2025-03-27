#pragma once  


/*
事件循环类 主要包含了两大模块 Channel和Poller
Channel负责事件的注册和删除Poller(epoll的抽象)负责事件的分发
*/ 
 
class EventLoop
{
public:
    EventLoop();
    ~EventLoop();
private:
};