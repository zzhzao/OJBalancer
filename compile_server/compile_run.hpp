#pragma once
#include "compiler.hpp"
#include "runner.hpp"
#include <jsoncpp/json/json.h>
#include "../comm/log.hpp"
#include "../comm/util.hpp"
namespace ns_compile_and_run
{
    using namespace ns_log;
    using namespace ns_util;
    using namespace ns_compiler;
    using namespace ns_runner;
    class CompileAndRun
    {

    public:
        static void Start(const std::string &in_json, const std::string *out_json)
        {
            //  输入 ： code  input
            // code : 用户提交的代码   input ： 用户提交的代码对应输入
            // cpu_limit:时间要求  mem_limit ： 空间要求
            // 输出
            // status : 状态码   reason : 请求结果
            // stdout : 运行结果   stderr  ： 错误结果
            Json::Value in_value;
            Json::Reader reader;
            reader.parse(in_json, in_value);

            std::string code = in_value["code"].asString();
            std::string input = in_value["input"].asString();
            int cpu_limit = in_value["cpu_limit"].asInt();
            int mem_limit = in_value["mem_limit"].asInt();
            if (code.size() == 0)
            {
            }
            std::string file_name = FileUtil::UniqFileName();

            FileUtil::WriteFile(PathUtil::Src(file_name),code);  // 形成临时src文件
            Compiler::Compile(file_name);
            Runner::Run(file_name,cpu_limit,mem_limit);
        }
    };
}
