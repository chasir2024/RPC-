#pragma once
#include <mutex>
#include <thread>
#include <condition_variable>
#include <deque>
#include <iostream>

template <typename T>
class LockQueue {
public:
    LockQueue() = default;
    // Push an element into the queue
    void Push(const T& value){
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push_back(value);
        cond_var_.notify_one();
    } 

    // Pop an element from the queue
    bool Pop(T& value){
        std::unique_lock<std::mutex> lock(mutex_);
        while (queue_.empty())
        {
            // 日志记录：日志缓冲区为空，等待日志写入
            cond_var_.wait(lock);
        }
        value = queue_.front();
        queue_.pop_front();
        return true;
    }

private:
    std::deque<T> queue_;
    std::mutex mutex_;
    std::condition_variable cond_var_;
};
