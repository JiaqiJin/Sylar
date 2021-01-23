#include<iostream>
#include "../sylar/log.h"

int main(int argc, char** argv) {
    sylar::Logger::ptr logger(new sylar::Logger);

    logger->addAppender(sylar::LogAppender::ptr(new sylar::StdOutLogAppender));

    //sylar::LogEvent::ptr event (new sylar::LogEvent(__FILE__,__LINE__, 0,1,2, time(0)));
    //logger->log(sylar::LogLevel::DEBUG, event);
    /*
    sylar::FileLogAppender::ptr file_appender(new sylar::FileLogAppender("./log.txt"));
    sylar::LogFormatter::ptr fmt(new sylar::LogFormatter("%d%T%p%T%m%n"));
    file_appender->setFormatter(fmt);

    logger->addAppender(file_appender);

    //sylar::LogEvent::ptr event(new sylar::LogEvent(__FILE__, __LINE__, 0, sylar::GetThreadId(), sylar::GetFiberId(), time(0)));
    //event->getSS() << "hello sylar log";
    //logger->log(sylar::LogLevel::DEBUG, event);*/
    
    std::cout << "hello sylar log" << std::endl;

    SYLAR_LOG_INFO(logger) << "test_macro" << std::endl;

    return 0;
}

