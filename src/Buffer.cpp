#include <cerrno>
#include <sys/uio.h>
#include <unistd.h>
#include <cassert>

#include "../include/Buffer.h"

ssize_t Buffer::readFd(int fd, int* saveErrno) {
    char extrabuf[65536] = {0};  // 64KB栈缓冲区

    iovec vec[2];
    const size_t writable = writableBytes();
    
    // 第一缓冲区指向Buffer可写空间
    vec[0].iov_base = beginWrite();
    vec[0].iov_len = writable;
    
    // 第二缓冲区指向栈空间
    vec[1].iov_base = extrabuf;
    vec[1].iov_len = sizeof(extrabuf);

    // 当Buffer空间足够大时，不使用栈缓冲区
    const int iovcnt = (writable < sizeof(extrabuf)) ? 2 : 1;
    // 从文件描述符读取数据
    const ssize_t n = ::readv(fd, vec, iovcnt);

    if (n < 0) {
        *saveErrno = errno;
    } else if (static_cast<size_t>(n) <= writable) {
        writerIndex_ += n;
    } else {
        writerIndex_ = buffer_.size();
        append(extrabuf, n - writable);
    }
    
    return n;
}

ssize_t Buffer::writeFd(int fd, int* saveErrno) {
    const size_t readable = readableBytes();
    if (readable == 0) return 0;

    ssize_t n = ::write(fd, peek(), readable);
    if (n < 0) {
        *saveErrno = errno;
    } else {
        retrieve(n);  // 只移除已成功写入的数据
    }
    return n;
}