#include <string.h>

#include "../include/InetAddress.h"

/*
htons(port) —— 主机字节序 转 网络字节序
ntohs(port) —— 网络字节序 转 主机字节序
inet_addr(ip) —— 将点分十进制的ip地址转换为网络字节序的ip地址
inet_ntop(AF_INET, &addr_.sin_addr, buf, sizeof buf) —— 将网络字节序的ip地址转换为点分十进制的ip地址
所有网络传输数据都是大端序  大端序（Big Endian）和小端序（Little Endian）
大端序是高字节存储在低地址，小端序是低字节存储在低地址
*/

InetAddress::InetAddress(uint16_t port, std::string ip)
{
    bzero(&addr_, sizeof addr_);
    addr_.sin_family = AF_INET;
    // 端口号需要从 主机字节序 转换为 网络字节序
    addr_.sin_port = ::htons(port); 
    addr_.sin_addr.s_addr = ::inet_addr(ip.c_str());
}

std::string InetAddress::toIp() const
{
    // addr_
    char buf[64] = {0};
    ::inet_ntop(AF_INET, &addr_.sin_addr, buf, sizeof buf);
    return buf;
}

std::string InetAddress::toIpPort() const
{
    // ip:port
    char buf[64] = {0};
    ::inet_ntop(AF_INET, &addr_.sin_addr, buf, sizeof buf);
    size_t end = ::strlen(buf);
    uint16_t port = ::ntohs(addr_.sin_port);
    sprintf(buf+end, ":%u", port);
    return buf;
    
}

uint16_t InetAddress::toPort() const
{
    return ::ntohs(addr_.sin_port);
}