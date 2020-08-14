#ifndef __SYLAR_LOG_H_
#define __SYLAR_LOG_H_

#include<string>
#include<stdint.h>
#include<memory>

namespace sylar
{

//LOG EVENT
class LogEvent
{
public:
    typedef std::shared_ptr<LogEvent> ptr;
    LogEvent();
private: 
    const char* m_file = nullptr; // file name
    int32_t m_line = 0;           // line
    uint32_t m_elapse = 0;        //program start to end millic second
    uint32_t m_threadId = 0;      // thread id
    uint32_t m_fiberId = 0;       //fiber id
    uint64_t m_time;              //timer
    std::string m_content;        
};

//LOG LEVEL
class LogLevel
{
public:
    enum Level
    {
        DEBUG = 1,
        INFO = 2,
        WARN = 3,
        ERROR = 4,
        FATAL = 5
    };
};

//LOG OUTPUT
class LogAppender
{
public: 
    typedef std::shared_ptr<LogAppender> ptr;
    virtual ~LogAppender();

    void log(LogLevel::Level level,LogEvent::ptr event);
private: 
    LogLevel::Level m_level;
};

//Format for log
class LogFormatter
{
public:
    typedef std::shared_ptr<LogFormatter> ptr;

    std::string formatter(LogEvent::ptr event);
private: 

};


//Log
class Logger
{
public:
    Logger(const std::string& name = "root");

    void log(LogLevel::Level level,const LogEvent& event);

private:
    std::string m_name;
    LogLevel::Level m_level;
    LogAppender::ptr m_appender;

};

//OUTPUT APPENDER FOR THE CONSOLE
class StdOutLogAppender : public LogAppender
{
public:

};

//OUTPUT APPENDER FOR THE FILE
class FileLOgAppender : public LogAppender
{
public:

};

}

#endif
