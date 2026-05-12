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
    void Push(T value); // Push an element into the queue
    bool Pop(T& value); // Pop an element from the queue

private:
    std::deque<T> queue_;
    std::mutex mutex_;
    std::condition_variable cond_var_;
};