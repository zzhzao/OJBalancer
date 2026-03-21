#include<iostream>
#include"../comm/httplib.h"
#include"oj_control.hpp"
using namespace httplib;
using namespace ns_control;
int main()
{
    // 用户请求的服务路由功能

    Server svr;
    Control ctrl;
    // 获取题目列表
    svr.Get("/all_questions",[&ctrl](const Request &req, Response &resp){

        //  返回一张包含所有题目的html网页
        std::string html;
        ctrl.AllQuestions(&html);
        resp.set_content(html,"text/html;charset=utf-8");
    });
    //根据题目编号，获取题目内容
    svr.Get(R"(/question/(\d+))",[&ctrl](const Request &req, Response &resp){
        std::string number = req.matches[1];
        std::string html;
        ctrl.Question(number,&html);

        resp.set_content(html,"text/html;charset=utf-8");
    });
    // 用户提交代码 ，使用我们的判题
    svr.Post(R"(/judge/(\d+))",[&ctrl](const Request &req, Response &resp){
         std::string number = req.matches[1];
         std::string result_json;
         ctrl.Judge(number,req.body,&result_json);

         resp.set_content(result_json,"application/json;charset=utf-8");

    });
    svr.set_base_dir("./wwwroot");
    svr.listen("0.0.0.0", 8080);
    return 0;
}