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

namespace sylar
{

class Logger;

//LOG EVENT
class LogEvent
{
public:
    typedef std::shared_ptr<LogEvent> ptr;
    LogEvent();

    const char* getFile() const {return m_file;}
    int32_t getLine() const {return m_line;}
    uint32_t getElapse() const { return m_elapse;}
    uint32_t getThreadID() const { return m_threadId;}
    uint32_t getFiberID() const { return m_fiberId;}
    uint64_t getTime() const { return m_time; }
    const std::string& getContent() const { return m_content; }
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

    static const char* ToString(LogLevel::Level level);
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

};

} // end namespace

#endif
