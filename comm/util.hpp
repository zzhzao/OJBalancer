#pragma once

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>
#include <atomic>
#include<fstream>
const std::string temp_path = "./temp/";

namespace ns_util
{
    class PathUtil
    {
    public:
        static std::string AddSuffix(const std::string &file_name, const std::string &suffix)
        {
            std::string path_name = temp_path;
            path_name += file_name;
            path_name += suffix;
            return path_name;
        }
        // 传入的为文件名，没有任何后缀
        // 编译时 使用的文件
        static std::string Src(const std::string &file_name)
        {
            return AddSuffix(file_name, ".cpp");
        }
        static std::string Exe(const std::string &file_name)
        {
            return AddSuffix(file_name, ".exe");
        }

        static std::string CompilerError(const std::string &file_name)
        {
            return AddSuffix(file_name, ".compile_error");
        }
        //  运行时 使用的文件
        static std::string Stdin(const std::string &file_name)
        {
            return AddSuffix(file_name, ".stdin");
        }
        static std::string Stdout(const std::string &file_name)
        {
            return AddSuffix(file_name, ".stdout");
        }
        static std::string Stderr(const std::string &file_name)
        {
            return AddSuffix(file_name, ".stderr");
        }
    };
    class TimeUtil
    {
    public:
        static std::string GetTimeStamp()
        {
            struct timeval _time;
            gettimeofday(&_time, NULL);
            return std::to_string(_time.tv_sec);
        }
        static std::string GetTimeMs()
        {
            struct timeval _time;
            gettimeofday(&_time, NULL);
            return std::to_string(_time.tv_sec * 1000 + _time.tv_usec / 1000);
        }
    };
    class FileUtil
    {
    public:
        static bool IsFileExists(const std::string &path_name)
        {
            struct stat st;
            if (stat(path_name.c_str(), &st) == 0)
            {
                return true;
            }
            return false;
        }
        static std::string UniqFileName()
        {
            // 利用时间戳 和 原子性机制来生成
            static std::atomic_uint id(0);
            id++;
            std::string ms = TimeUtil::GetTimeMs();
            std::string uniq_id = std::to_string(id);
            return ms + "_" + uniq_id;

        }
        static bool WriteFile(const std::string &target, const std::string &code)
        {
            std::ofstream out(target);
            if(!out.is_open())
            {
                return false;
            }
            out.write(code.c_str(), code.size());
            out.close();
        }
        // keep 来判断是否保留 \n
        static bool ReadFile(const std::string &target,std::string *content,bool keep = false)
        {
            (*content).clear();
            std::ifstream in(target);
            if(!in.is_open())
            {
                return false;
            }
            std::string line;
            // getline 不保存行分割符,有时候需要保留\n
            // getline 重载了强制类型转化
            while(std::getline(in,line))
            {
                (*content) += line;
                (*content) += keep ? "\n" : "";
            }
            in.close();
            return true;
        }
    };

};