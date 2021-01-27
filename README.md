# Sylar

## Logger System

    Logger(log system class)
        |
        | ---- LogFormatter(log format)
        |
        Appender(Log output)
## System Configuration
Config --> yaml

```c++
YAML::Node node = YAML::LoadFile("filename");
for(auto iter = node.begin(); node != iter.end();++iter)
{
    iter->first(), iter->second();
}
node.IsSequence()
node.IsScalar()
```

## Thread Module

## Coroutine Module

## Coroutine scheduling module

## IO coroutine scheduling module

## Hook module

## Socket module

## ByteArray serialization module

## TcpServer module

## Stream module

## HTTP module

## Servlet module

