#pragma once
// 根据题目list文件，加载所有的题目信息到内存中
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "../comm/log.hpp"
#include "../comm/util.hpp"
#include <cassert>
#include <cstdlib>
namespace ns_model
{
    using namespace std;
    using namespace ns_log;
    using namespace ns_util;
    struct Question
    {
        std::string number; // 题目编号
        std::string title;  // 题目 标题
        std::string star;   // 难度
        int cpu_limit;      // 时间要求
        int mem_limit;      // 空间要求
        std::string desc;   // 题目描述
        std::string header; // 题目预设给用户的在线编辑器代码
        std::string tail;   // 题目测试用例，需要和header拼接，形成完整代码
    };
    const std::string questions_list = "./questions/questions.list";
    const std::string questions_path = "./questions/";

    class Model
    {
    private:
        // 题号题目映射
        unordered_map<string, Question> questions;

    public:
        Model()
        {
            assert(LoadQuestionList(questions_list));
        }
        bool LoadQuestionList(const std::string &question_list)
        {
            // 加载配置文件
            ifstream in(question_list);
            if (!in.is_open())
            {
                LOG(FATAL) << "加载题库失败，请检查是否存在题库文件" << "\n";
                return false;
            }
            std::string line;
            while (getline(in, line))
            {
                std::vector<string> tokens;
                StringUtil::SplitString(line,&tokens," ");
                // 1 判断回文数 简单 1 30000
                if(tokens.size() != 5)
                {
                    LOG(WARNING) << "加载部分题目失败，请检查文件格式" << "\n";
                    continue;
                }
                Question q;
                q.number = tokens[0];
                q.title = tokens[1];
                q.star = tokens[2];
                q.cpu_limit = atoi(tokens[3].c_str());
                q.mem_limit = atoi(tokens[4].c_str());

                std::string path = questions_path;
                path += q.number;
                path += "/";
                FileUtil::ReadFile(path+"desc.txt",&(q.desc),true);
                FileUtil::ReadFile(path+"header.cpp",&(q.header),true);
                FileUtil::ReadFile(path+"tail.cpp",&(q.tail),true);

                questions.insert({q.number,q});
            }
            LOG(INFO) << "加载题库成功..." << "\n";
            in.close();
            return true;
        }
        bool GetAllQuestions(vector<Question> *out)
        {
            if (questions.size() == 0)
            {
                LOG(ERROR) << "获取题库失败 " << "\n";
                return false;
            }
            for (const auto &q : questions)
            {
                out->push_back(q.second); // key:value
            }
            return true;
        }
        bool GetOneQuestion(const std::string &number, Question *q)
        {
            const auto &iter = questions.find(number);
            if (iter == questions.end())
            {
                LOG(ERROR) << "获取题目失败，题目编号：" << number << "\n";
                return false;
            }
            (*q) = iter->second;
            return true;
        }
        ~Model()
        {
        }
    };
}