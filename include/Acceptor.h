#pragma once

#include <functional>

#include "noncopyable.h"
#include "Socket.h"
#include "Channel.h"

class EventLoop;
class InetAddress;

class Acceptor : noncopyable
{
public:
    using NewConnectionCallback = std::function<void(int sockfd, const InetAddress &)>;

    Acceptor(EventLoop *loop, const InetAddress &listenAddr, bool reuseport);
    ~Acceptor();
    //设置新连接的回调函数
    void setNewConnectionCallback(const NewConnectionCallback &cb) { NewConnectionCallback_ = cb; }
    // 判断是否在监听
    bool listenning() const { return listenning_; }
    // 监听本地端口
    void listen();

private:
    void handleRead();//处理新用户的连接事件

    EventLoop *loop_; // Acceptor用的就是用户定义的那个baseLoop 也称作mainLoop
    Socket acceptSocket_;//专门用于接收新连接的socket
    Channel acceptChannel_;//专门用于监听新连接的channel
    NewConnectionCallback NewConnectionCallback_;//新连接的回调函数
    bool listenning_;//是否在监听
};