#pragma once

#include<iostream>
#include<unistd.h>
namespace ns_complier
{
    class complier
    {
    public:
        complier(){}
        ~complier(){}
        // 编译成功 返回true 失败 false     传入编译的文件名
        static bool Complie(const std::string& file_name)
        {
            pid_t res = fork();
            if (res < 0)
            {
                return false;
            }
            else if(res == 0)
            {
                //子进程  调用编译器，完成编译工作
                execlp("g++", "g++", file_name.c_str(), "-o", "a.out", nullptr);
            }
            else
            {

            }
        }   
    };
};