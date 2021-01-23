#ifndef __SYLAR_LOG_H_
#define __SYLAR_LOG_H_

#include<string>
#include<vector>
#include<utility>
#include<stdint.h>
#include<memory>
#include<list>
#include<sstream>
#include<fstream>
#include<iostream>
#include<map>
#include<functional>

#include "util.h"

/**
 * @brief 使用流式方式将日志级别level的日志写入到logger
 */
#define SYLAR_LOG_LEVEL(logger, level) \
    if(logger->getLevel() <= level) \
        sylar::LogEventWrap(sylar::LogEvent::ptr(new sylar::LogEvent(logger, level, \
                        __FILE__, __LINE__, 0, sylar::GetThreadId(),\
                sylar::GetFiberId(), time(0)))).getSS()

/**
 * @brief 使用流式方式将日志级别debug的日志写入到logger
 */
#define SYLAR_LOG_DEBUG(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::DEBUG)

/**
 * @brief 使用流式方式将日志级别info的日志写入到logger
 */
#define SYLAR_LOG_INFO(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::INFO)

/**
 * @brief 使用流式方式将日志级别warn的日志写入到logger
 */
#define SYLAR_LOG_WARN(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::WARN)

/**
 * @brief 使用流式方式将日志级别error的日志写入到logger
 */
#define SYLAR_LOG_ERROR(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::ERROR)

/**
 * @brief 使用流式方式将日志级别fatal的日志写入到logger
 */
#define SYLAR_LOG_FATAL(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::FATAL)

/**
 * @brief 使用格式化方式将日志级别level的日志写入到logger
 */
#define SYLAR_LOG_FMT_LEVEL(logger, level, fmt, ...) \
    if(logger->getLevel() <= level) \
        sylar::LogEventWrap(sylar::LogEvent::ptr(new sylar::LogEvent(logger, level, \
                        __FILE__, __LINE__, 0, sylar::GetThreadId(),\
                sylar::GetFiberId(), time(0), sylar::Thread::GetName()))).getEvent()->format(fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别debug的日志写入到logger
 */
#define SYLAR_LOG_FMT_DEBUG(logger, fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::DEBUG, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别info的日志写入到logger
 */
#define SYLAR_LOG_FMT_INFO(logger, fmt, ...)  SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::INFO, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别warn的日志写入到logger
 */
#define SYLAR_LOG_FMT_WARN(logger, fmt, ...)  SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::WARN, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别error的日志写入到logger
 */
#define SYLAR_LOG_FMT_ERROR(logger, fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::ERROR, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别fatal的日志写入到logger
 */
#define SYLAR_LOG_FMT_FATAL(logger, fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::FATAL, fmt, __VA_ARGS__)



namespace sylar
{

class Logger;
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

    static const char* ToString(LogLevel::Level level);
};

//LOG EVENT
class LogEvent
{
public:
    typedef std::shared_ptr<LogEvent> ptr;
    LogEvent(std::shared_ptr<Logger>logger, LogLevel::Level level, char* file, int32_t line, uint32_t elapse, 
            uint32_t thread_id, uint32_t fiber_id, uint64_t time);
    const char* getFile() const {return m_file;}
    int32_t getLine() const {return m_line;}
    uint32_t getElapse() const { return m_elapse;}
    uint32_t getThreadID() const { return m_threadId;}
    uint32_t getFiberID() const { return m_fiberId;}
    uint64_t getTime() const { return m_time; }
    const std::string getContent() const { return m_ss.str(); }
    std::shared_ptr<Logger> getLogger() const { return m_logger; }
    LogLevel::Level getLevel() const { return m_level; }

    std::stringstream& getSS() { return m_ss;}
    void format(const char* fmt, ...);
    void format(const char* fmt, va_list al);
private: 
    const char* m_file = nullptr; // file name
    int32_t m_line = 0;           // line
    uint32_t m_elapse = 0;        //program start to end millic second
    uint32_t m_threadId = 0;      // thread id
    uint32_t m_fiberId = 0;       //fiber id
    uint64_t m_time;              //timer
    std::stringstream m_ss;        

    std::shared_ptr<Logger> m_logger;
    LogLevel::Level m_level;
};

class LogEventWrap
{
public:
    LogEventWrap(LogEvent::ptr e);
    ~LogEventWrap();

    std::stringstream& getSS();
private:
    LogEvent::ptr m_event;
};

//Format for log
class LogFormatter
{
public:
    typedef std::shared_ptr<LogFormatter> ptr;
    LogFormatter(const std::string& _pattern);

    //
    std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level ,LogEvent::ptr event);
public: 
    class FormatItem
    {   
    public: 
        typedef std::shared_ptr<FormatItem> ptr;
        FormatItem(const std::string& fmt = "") {};
        virtual ~FormatItem() {}
        virtual void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level ,LogEvent::ptr event) = 0;
    };

    void init();
private:
    std::string m_pattern;
    std::vector<FormatItem::ptr> m_items;
};

//LOG OUTPUT
class LogAppender
{
public: 
    typedef std::shared_ptr<LogAppender> ptr;
    virtual ~LogAppender() = default;

    virtual void log(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) = 0;

    //setter and getter
    LogFormatter::ptr getFormatter() const {return m_formatter;}
    void setFormatter(LogFormatter::ptr val) { m_formatter = val;}
protected: 
    LogLevel::Level m_level;
    LogFormatter::ptr m_formatter;
};


//OUTPUT APPENDER FOR THE CONSOLE
class StdOutLogAppender : public LogAppender
{
public:
    typedef std::shared_ptr<StdOutLogAppender> prt;
    virtual void log(std::shared_ptr<Logger> logger,LogLevel::Level level, LogEvent::ptr event) override;

private:

};

//OUTPUT APPENDER FOR THE FILE
class FileLogAppender : public LogAppender
{
public:
    typedef std::shared_ptr<FileLogAppender> ptr;
    FileLogAppender(const std::string& filename);
    virtual void log(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) override;

    bool reopen();
private:
    std::string m_filename;
    std::ofstream m_filestream;
};

//Log
class Logger : public std::enable_shared_from_this<Logger>
{
public:
    typedef std::shared_ptr<Logger> ptr;

    Logger(const std::string& name = "root");
    void log(LogLevel::Level level,LogEvent::ptr event);

    void debug(LogEvent::ptr event);
    void info(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);

    void addAppender(LogAppender::ptr appender);
    void delAppender(LogAppender::ptr appender);

    LogLevel::Level getLevel() const {return m_level;}
    void setLevel(LogLevel::Level level) {m_level = level;}

    const std::string getName() const { return m_name; }
private:
    std::string m_name;          //log name
    LogLevel::Level m_level;     //log level
    std::list<LogAppender::ptr> m_appenders; //Appender list
    LogFormatter::ptr m_formatter;
};

class LoggerManager
{
public:
    LoggerManager();
    Logger::ptr getLogger(const std::string& name);

    void init();

private:
    std::map<std::string, Logger::ptr> m_loggers;
    Logger::ptr root;
};

//typedef sylar::Singleton<LoggerManager> LoggerMgr;

} // end namespace

#endif
