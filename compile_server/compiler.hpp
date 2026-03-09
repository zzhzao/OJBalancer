#pragma once

#include<iostream>
#include<unistd.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include"../comm/util.hpp"
namespace ns_complier
{
    using namespace ns_util;
    class complier
    {
    public:
        complier(){}
        ~complier(){}
        // 编译成功 返回true 失败 false     传入编译的文件名
        static bool Complie(const std::string& file_name)
        {
            pid_t pid = fork();
            if (pid < 0)
            {
                return false;
            }
            else if(pid == 0)
            {
                int _stderr = open(PathUtil::Stderr(file_name).c_str(),O_CREAT|O_WRONLY,0644);
                if (_stderr < 0)
                {
                    //失败 退出
                    exit(1);
                }
                // 重定项到stderr
                dup2(_stderr,2);
                
                //子进程  调用编译器，完成编译工作
                execlp("g++", "g++", "-o",PathUtil::Exe(file_name.c_str()),\
                PathUtil::Src(file_name.c_str()),"-std=c++11", nullptr);
            }
            else
            {
                waitpid(pid,nullptr,0);
                if(FileUtil::IsFileExists(PathUtil::Exe(file_name)))
                {
                    return true;
                }

            }
            return false;
        }   
    };
};