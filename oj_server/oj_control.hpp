#pragma once
#include<iostream>
#include<string>
#include"oj_model.hpp"
#include"../comm/util.hpp"
#include"../comm/log.hpp"
#include<vector>
#include"oj_view.hpp"
#include<mutex>
namespace ns_control
{
    using namespace std;
    using namespace ns_util;
    using namespace ns_log;
    using namespace ns_model;
    using namespace ns_view;
    // 提供服务的主机
    class Machine
    {
    public:
        std::string ip;
        int port;
        uint64_t load;  // 用于计数
        std::mutex *mtx;
    public:
        Machine():ip(""),port(0),load(0),mtx(nullptr)
        {}
        ~Machine(){}
    };

    const std::string service_machine = "./conf/service_machine.conf";

    // 负载均衡
    class LoadBlance
    {
    private:
    // 可以给我们提供编译服务的所有主机
    // 每一台主机都有自己的下标，来充当当前主机的id
        std::vector<Machine> machines;  
        std::vector<int> online;  // 所有在线的主机id
        std::vector<int> offline;  //  所有离线的主机id

    public:
        LoadBlance()
        {
            assert(LoadConf(service_machine));
        }
        ~LoadBlance()
        {}
        bool LoadConf(const std::string &machine_list)
        {

        }
        bool SmartChoice()
        {

        }
        void OfflineMachine()
        {

        }
        void OnlineMachine()
        {
            
        }
    };





///////////////////////////////////////////////


    //核心业务
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
        void Judge(const std::string in_json,std::string *out_json)
        {
            // in_json   id   code   input
            // 1.对in_json反序列化  得到id 代码 input

            // 2. 拼接用户代码和测试用例代码

            // 3. 选择负载最低的主机，
            // 4. 然后发起http请求，得到结果

            // 5. 将结果赋值给out_json
        }
    };
}