#pragma once

#include<iostream>
#include<string>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/time.h>
const std::string temp_path = "./temp/"; 

namespace ns_util
{
    class PathUtil
    {
        public:
        
        
            static std::string AddSuffix(const std::string &file_name,const std::string &suffix)   
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
                return AddSuffix(file_name,".cpp");
            }
            static std::string Exe(const std::string &file_name)
            {
                return AddSuffix(file_name,".exe");
            }

            static std::string CompilerError(const std::string &file_name)
            {
                return AddSuffix(file_name,".compile_error");
            }
            //  运行时 使用的文件
            static std::string Stdin(const std::string &file_name)
            {
                return AddSuffix(file_name,".stdin");
            }
            static std::string Stdout(const std::string &file_name)
            {
                return AddSuffix(file_name,".stdout");

            }
            static std::string Stderr(const std::string &file_name)
            {
                return AddSuffix(file_name,".stderr");
            }
            
    };

    class FileUtil
    { 
        public:
           static bool IsFileExists(const std::string &path_name)
           {
              struct stat st;
              if(stat(path_name.c_str(),&st) == 0)
              {
                  return true;
              }
              return false;
           }
           static std::string  UniqFileName()
           {

           }
           static bool WriteFile(const std::string& target,const std::string &code)
           {
              
           }
    };
    class TimeUtil
    {
        public:
            static std::string GetTimeStamp()
            {
                struct timeval _time;
                gettimeofday(&_time,NULL);
                return std::to_string(_time.tv_sec);
            }
    };
};