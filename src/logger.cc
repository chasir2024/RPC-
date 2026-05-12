#include "logger.h"
#include <thread>
#include <iostream>
#include <time.h>


Logger& Logger::getInstance(){
    static Logger logger;
    return logger;
}
//从日志缓冲区中取出日志并写入到日志文件中
Logger::Logger(){
    //启动一个线程专门负责日志的写入
    std::thread log_thread([this](){
        while(true){
            time_t now = time(nullptr);
            tm *localtm = localtime(&now);
            char file_name[256];
            sprintf(file_name,"%d-%d-%d-log.txt",localtm->tm_year + 1900, localtm->tm_mon + 1, localtm->tm_mday);
            // 可读可写，追加到末尾，不存在则创建
            FILE *pf = fopen(file_name,"a+"); 
            if(pf == nullptr){
                std::cout << "open log file failed!" << std::endl;
                exit(EXIT_FAILURE);
            }

            std::string log_message;
            if(logs_.Pop(log_message)){
                fputs(log_message.c_str(), pf);
            }
            fclose(pf);
        }
    });
}
// 设置日志级别
void Logger::setloglevel(LogLevel level){
    logLevel_ = level;
}
// 写日志 把日志写入到日志queue缓冲区中 ***由专门的线程负责从queue中取出日志并写入到日志文件中
void Logger::log(LogLevel level, const std::string& message){
    if(level >= logLevel_){
        logs_.Push(message);
    }
}