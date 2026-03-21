// #include<iostream>
// #include<boost/algorithm/string.hpp>
// #include<vector>
// int main()
// {
//     std::string str = "1 判断回文数 简单 30000";
//     std::vector<std::string> tokens;
//     const std::string sep = " ";
//     boost::split(tokens,str,boost::is_any_of(sep),boost::algorithm::token_compress_on);
//     for(auto &iter:tokens)
//     {
//         std::cout << iter << std::endl;
//     }
// }


#include<iostream>
#include<string>
#include<ctemplate/template.h>

int main()
{
    std::string in_html = "./test.html";
    std::string value = "zzh";
    //形成数据字典
    ctemplate::TemplateDictionary root("test"); // unordered_map<> test;
    root.SetValue("key",value);                 // test.insert({});

    //获取被渲染网页对象
    ctemplate::Template *tpl = ctemplate::Template::GetTemplate(in_html,ctemplate::DO_NOT_STRIP);

    std::string out_html;
    tpl->Expand(&out_html,&root);

    std::cout << out_html << std::endl;
}