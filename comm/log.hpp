#pragma once 
#include<iostream>
#include<string>
#include"util.hpp"
using namespace ns_util;

namespace ns_log
{
    // 日志级别
    enum
    {
        INFO,
        DEBUG,
        WARNING,
        ERROR,
        FATAL
    };

    // 等级 文件 多少行
    inline std::ostream &Log(const std::string &level, const std::string &file_name,int line)
    {
        std::string message = "[";
        message += level;
        message += "]";
        message += "[";
        message += file_name;
        message += "]";
        message += "[";
        message += std::to_string(line);
        message += "]";

        // 添加时间戳
        message += "[";
        message +=  TimeUtil::GetTimeStamp();
        message += "]";

        std::cout << message; //cout是由缓冲区的
        return std::cout;
    }
    //LOG() << ""
    #define LOG(level) Log(#level,__FILE__,__LINE__)

}