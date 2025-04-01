#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/tcp.h>
#include <sys/socket.h>

#include "Socket.h"
#include "Logger.h"
#include "InetAddress.h"

Socket::~Socket()
{
    ::close(sockfd_);
}

void Socket::bindAddress(const InetAddress &localaddr)
{
    if (0 != ::bind(sockfd_, (sockaddr *)localaddr.getSockAddr(), sizeof(sockaddr_in)))
    {
        LOG_FATAL("bind sockfd:%d fail\n", sockfd_);
    }
}

void Socket::listen()
{
    if (0 != ::listen(sockfd_, 1024))
    {
        LOG_FATAL("listen sockfd:%d fail\n", sockfd_);
    }
}

int Socket::accept(InetAddress *peeraddr)
{
    /**
     * 1. accept函数的参数不合法
     * 2. 对返回的connfd没有设置非阻塞
     * Reactor模型 one loop per thread
     * poller + non-blocking IO
     **/
    sockaddr_in addr;
    socklen_t len = sizeof(addr);
    ::memset(&addr, 0, sizeof(addr));
    // fixed : int connfd = ::accept(sockfd_, (sockaddr *)&addr, &len);
    int connfd = ::accept4(sockfd_, (sockaddr *)&addr, &len, SOCK_NONBLOCK | SOCK_CLOEXEC);
    if (connfd >= 0)
    {
        peeraddr->setSockAddr(addr);
    }
    return connfd;
}

void Socket::shutdownWrite()
{
    if (::shutdown(sockfd_, SHUT_WR) < 0)
    {
        LOG_ERROR("shutdownWrite error");
    }
}

void Socket::setTcpNoDelay(bool on)
{
    // TCP_NODELAY 用于禁用 Nagle 算法。
    // Nagle 算法用于减少网络上传输的小数据包数量。
    // 将 TCP_NODELAY 设置为 1 可以禁用该算法，允许小数据包立即发送。
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(optval));
}

void Socket::setReuseAddr(bool on)
{
    // SO_REUSEADDR 允许一个套接字强制绑定到一个已被其他套接字使用的端口。
    // 这对于需要重启并绑定到相同端口的服务器应用程序非常有用。
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
}

void Socket::setReusePort(bool on)
{
    // SO_REUSEPORT 允许同一主机上的多个套接字绑定到相同的端口号。
    // 这对于在多个线程或进程之间负载均衡传入连接非常有用。
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
}

void Socket::setKeepAlive(bool on)
{
    // SO_KEEPALIVE 启用在已连接的套接字上定期传输消息。
    // 如果另一端没有响应，则认为连接已断开并关闭。
    // 这对于检测网络中失效的对等方非常有用。
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval));
}