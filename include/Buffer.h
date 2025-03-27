#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <cstddef>
#include <cassert>

#include "Noncopyable.h"

class Buffer : noncopyable
{
public:
    static constexpr size_t kCheapPrepend = 8;  
    static constexpr size_t kInitialSize = 1024;

    explicit Buffer(size_t initialSize = kInitialSize)
        : buffer_(kCheapPrepend + initialSize),
          readerIndex_(kCheapPrepend),
          writerIndex_(kCheapPrepend) 
    {
        assert(initialSize > 0);
    }

    // 鼓励移动语义
    Buffer(Buffer&&) = default;
    Buffer& operator=(Buffer&&) = default;

    // 可读数据长度   可写空间长度   前置空间长度
    size_t readableBytes() const noexcept { return writerIndex_ - readerIndex_; }
    size_t writableBytes() const noexcept { return buffer_.size() - writerIndex_; }
    size_t prependableBytes() const noexcept { return readerIndex_; }

    // 返回缓冲区中可读数据的起始地址
    const char* peek() const noexcept { return begin() + readerIndex_; }
    
    void retrieve(size_t len) {
        assert(len <= readableBytes());
        if (len < readableBytes()) {
            readerIndex_ += len;
        } else {
            retrieveAll();
        }
    }
    
    void retrieveAll() noexcept {
        readerIndex_ = kCheapPrepend;
        writerIndex_ = kCheapPrepend;
    }

    std::string retrieveAllAsString() {
        return retrieveAsString(readableBytes());
    }
    
    std::string retrieveAsString(size_t len) {
        assert(len <= readableBytes());
        std::string result(peek(), len);
        retrieve(len);
        return result;
    }

    void ensureWritableBytes(size_t len) {
        if (writableBytes() < len) {
          // 扩容或者调整缓冲区域
            makeSpace(len);
        }
        assert(writableBytes() >= len);
    }

    // 追加数据到缓冲区
    void append(const char* data, size_t len) {
        assert(data != nullptr);
        ensureWritableBytes(len);
        std::copy(data, data + len, beginWrite());
        writerIndex_ += len;
    }
    
    void append(const std::string& str) {
        append(str.data(), str.length());
    }

    char* beginWrite() noexcept { return begin() + writerIndex_; }
    const char* beginWrite() const noexcept { return begin() + writerIndex_; }
    
    // 从fd上读取数据
    ssize_t readFd(int fd, int* saveErrno);
    // // 通过fd发送数据
    ssize_t writeFd(int fd, int* saveErrno);

private:
    // 返回内部数组首地址 标准方式，性能最优，data() 不会检查边界，直接返回指针
    // buffer_ 为空时，返回 nullptr
    char* begin() noexcept { return buffer_.data(); }
    const char* begin() const noexcept { return buffer_.data(); }

    void makeSpace(size_t len) {
        // 调整碎片还是不够
        if (writableBytes() + prependableBytes() < len + kCheapPrepend) {
            buffer_.resize(writerIndex_ + len);
        } else {
            size_t readable = readableBytes();
            // std::copy(源起始地址, 源结束地址, 目标起始地址);
            std::copy(begin() + readerIndex_, 
                     begin() + writerIndex_,
                     begin() + kCheapPrepend);
            readerIndex_ = kCheapPrepend;
            writerIndex_ = readerIndex_ + readable;
            assert(readable == readableBytes());
        }
    }

    std::vector<char> buffer_;
    size_t readerIndex_ = kCheapPrepend;
    size_t writerIndex_ = kCheapPrepend;
};