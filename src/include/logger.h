#pragma once
#include <string>
#include "lokqueue.h"

enum LogLevel {
    INFO,
    WARNING,
    ERROR
};

class Logger{
public:
    // 获取单例对象
    static Logger& getInstance();
    void setloglevel(LogLevel level);
    void log(LogLevel level, const std::string& message);

private:
    LockQueue<std::string> logs_;
    LogLevel logLevel_;

    Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
};


#define LOG_INFO(logmsgformat, ...) \
    do \
    { \
        Logger::getInstance().log(INFO, "This is an info log."); \
        Logger::setloglevel(INFO); \
        char c[1024] = {0}; \
        snprintf(c,1024,logmsgformat,##__VA_ARGS__); \
    } while (0);


