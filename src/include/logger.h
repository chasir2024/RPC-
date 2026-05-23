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
        Logger::getInstance().setloglevel(INFO); \
        char c[1024] = {0}; \
        snprintf(c,1024,logmsgformat,##__VA_ARGS__); \
        Logger::getInstance().log(INFO, c); \
    } while (0);

#define LOG_WARNING(logmsgformat, ...) \
    do \
    { \
        Logger::getInstance().setloglevel(WARNING); \
        char c[1024] = {0}; \
        snprintf(c,1024,logmsgformat,##__VA_ARGS__); \
        Logger::getInstance().log(WARNING, c); \
    } while (0);

#define LOG_ERROR(logmsgformat, ...) \
    do \
    { \
        Logger::getInstance().setloglevel(ERROR); \
        char c[1024] = {0}; \
        snprintf(c,1024,logmsgformat,##__VA_ARGS__); \
        Logger::getInstance().log(ERROR, c); \
    } while (0);

#define LOG_ERR(logmsgformat, ...) LOG_ERROR(logmsgformat, ##__VA_ARGS__)
