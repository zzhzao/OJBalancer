#include "compile_run.hpp"
#include "../comm/httplib.h"
using namespace ns_compile_and_run;
using namespace httplib;

void Usage(std::string proc)
{
    std::cerr << "Usage: " << "\n\t" << proc << " port" << std::endl;
}

//./compile_server port
int main(int argc,char *argv[])
{

    if(argc != 2)
    {
        Usage(argv[0]);
        return 1;
    }
    Server svr;
//     {
//     "code" : "#include<iostream> \n int main(){std::cout << \"你好！\" << std::endl;}",
//     "input" : "",
//     "cpu_limit" : 1,
//     "mem_limit" : 50000
// }
    svr.Post("/compile_and_run",[](const Request &req, Response &resp){
        std::string in_json = req.body;
        std::string out_json;
        if(!in_json.empty())
        {
            CompileAndRun::Start(in_json,&out_json);
            resp.set_content(out_json,"application/json;charset=utf-8");
        }

    });
    svr.listen("0.0.0.0",atoi(argv[1]));
    //  输入 ： code  input
    // code : 用户提交的代码   input ： 用户提交的代码对应输入
    // cpu_limit:时间要求  mem_limit ： 空间要求
    // 输出
    // status : 状态码   reason : 请求结果
    // stdout : 运行结果   stderr  ： 错误结果
    // std::string in_json;
    // std::string out_json;
    // Json::Value in_value;
    // in_value["code"] = "";
    // in_value["input"] = "";
    // in_value["cpu_limit"] = 1;
    // in_value["mem_limit"] = 10240 * 3;
    // Json::FastWriter writer;
    // in_json = writer.write(in_value);

    
    // CompileAndRun::Start(in_json, &out_json);

    return 0;
}