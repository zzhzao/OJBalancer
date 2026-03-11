#pragma once

#include<iostream>
#include<unistd.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include"../comm/util.hpp"
#include"../comm/log.hpp"
namespace ns_compiler
{
    using namespace ns_util;
    using namespace ns_log;
    class Compiler
    {
    public:
        Compiler(){}
        ~Compiler(){}
        // 编译成功 返回true 失败 false     传入编译的文件名
        
        static bool Compile(const std::string& file_name)
        {
            pid_t pid = fork();
            if (pid < 0)
            {
                LOG(ERROR) << "创建子进程失败" << "\n";
                return false;
            }
            else if(pid == 0)
            {
                umask(0);
                int _stderr = open(PathUtil::CompilerError(file_name).c_str(),O_CREAT|O_WRONLY,0644);
                if (_stderr < 0)
                {
                    //失败 退出
                    LOG(WARNING) << "没有成功形成stderr文件" << "\n";
                    exit(1);
                }
                // 重定项到stderr
                dup2(_stderr,2);
                
                //子进程  调用编译器，完成编译工作
                execlp("g++", "g++", "-o",PathUtil::Exe(file_name).c_str(),\
                PathUtil::Src(file_name).c_str(),"-std=c++11", nullptr);
                LOG(ERROR) << "子进程调用g++失败" << "\n";
            }
            else
            {
                waitpid(pid,nullptr,0);
                if(FileUtil::IsFileExists(PathUtil::Exe(file_name)))
                {
                    LOG(INFO)<< PathUtil::Src(file_name) << "编译成功" << "\n";
                    return true;
                }

            }
            LOG(ERROR) << "编译失败" << "\n";
            return false;
        }   
    };
};