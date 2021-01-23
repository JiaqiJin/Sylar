#ifndef SYLAR_CONFIG_H
#define SYLAR_CONFIG_H

#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <boost/lexical_cast.hpp>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <functional>

#include "log.h"

namespace sylar {

class ConfigVarBase 
{
public:
    typedef std::shared_ptr<ConfigVarBase> ptr;
    ConfigVarBase(const std::string& name, const std::string& description = "")
    :m_name(name)
    ,m_description(description){}

    virtual ~ConfigVarBase() {}

    const std::string& getName() const { return m_name;}
    const std::string& getDescription() const { return m_description;}
    virtual std::string toString() = 0;
    virtual bool fromString(const std::string& val) = 0;

protected:
    std::string m_name;
    std::string m_description;
};

template<class T>
class ConfigVar : public ConfigVarBase
{
public:
    typedef std::shared_ptr<ConfigVar> ptr;
    ConfigVar(const std::string& name, const T& default_value, const std::string& description = "")
        : ConfigVarBase(name,description)
        ,m_val(default_value){}

    std::string toString() override{
        try{
            return boost::lexical_cast<std::string>(m_val);
        }catch(std::exception& e){
            //SYLAR_LOG_ERROR
        }
        return "";
    }
    bool fromString(const std::string& val) override;

private:
    T m_val;
};

}

#endif