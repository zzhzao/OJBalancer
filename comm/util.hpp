#pragma once

#include<iostream>
#include<string>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
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
            static std::string Src(const std::string &file_name)
            {
                return AddSuffix(file_name,".cpp");
            }
            static std::string Exe(const std::string &file_name)
            {
                return AddSuffix(file_name,".exe");
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
    };
};