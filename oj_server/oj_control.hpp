#pragma once
#include<iostream>
#include<string>
#include"oj_model.hpp"
#include"../comm/util.hpp"
#include"../comm/log.hpp"
#include<vector>
#include"oj_view.hpp"
namespace ns_control
{
    using namespace std;
    using namespace ns_util;
    using namespace ns_log;
    using namespace ns_model;
    using namespace ns_view;
    class Control
    {


    private:
        Model model_;  // 提供后台数据
        View view_;   // 提供网页渲染
    public:
        Control()
        {}
        ~Control()
        {}
    public:
    // html  输出型参数
        bool AllQuestions(std::string *html)
        {
            int ret = true;
            vector<struct Question> all;
            if(model_.GetAllQuestions(&all))
            {
                // 获取题目信息成功，将所有题目数据构建成网页
                view_.AllExpandHtml(all,html);
            }
            else
            {
                *html = "获取题目失败，形成题目列表失败";
                ret = false;
            }
            return ret;
        }
        bool Question(const string &string,std::string *html)
        {
            bool ret = true;
            struct Question q;
            if(model_.GetOneQuestion(q.number,&q))
            {
                // 通过指定题目 构建网页
                view_.OneExpandHtml(q,html);
            }
            else
            {
                *html = "指定题目：" + q.number + "不存在！";
                ret = false;
            }
            return ret;
        }
     
    };
}