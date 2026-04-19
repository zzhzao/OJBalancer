# OJBalancer
<img width="1695" height="847" alt="image" src="https://github.com/user-attachments/assets/2123806f-accb-4b65-bd02-e1720cb7e2ac" />

在这里后端编译服务主机我们设置为3个，此时是有三台主机同时在运行。
与客户端交互的只有oj_server这台主机。
## compiler_server
- 编译服务：我们这里在compiler.hpp头文件中，完成了对编译功能的实现，具体方法是利用程序替换函数，将fork出来的子进程，替换成为了g++的编译程序。
- 运行服务：在运行服务中，我们要注意两点
	- 一点是，我们需要对该程序的运行时间以及该程序所能够占用的内存大小进行限制，因为在部分题目中，我们是需要有时间空间上的限制的
	- 二是，在真正运行可执行程序时，我们仍然要进行程序替换来进行。要注意的是，我们在这里需要对标准输入，标准输出，标准错误进行重定向。因为我们并不需要它输出到控制台。我们可以使用当前文件名称，修改后缀的方式生成其独属的输入输出及错误文件
我们的编译服务本质上是一种网络服务。
在将编译服务和运行服务结合到一起的时候，我们需要处理以下请求与应答。

为此我们做出以下规定：
请求参数：
```
            //  输入 ： code  input
            // code : 用户提交的代码   input ： 用户提交的代码对应输入
            // cpu_limit:时间要求  mem_limit ： 空间要求

```
应答参数：
```
            // 输出
            // status : 状态码   reason : 请求结果
            // stdout : 运行结果   stderr  ： 错误结果
```

唯一文件名的生成：
在这里，我们的OJBalancer服务，可能是被大量的客户端同时请求的。
我们要为每一个提交上来的服务，生成它唯一的文件名。
在这里我们利用时间戳 + 原子性机制来生成。
```
        static std::string UniqFileName()
        {
            // 利用时间戳 和 原子性机制来生成
            static std::atomic_uint id(0);
            id++;
            std::string ms = TimeUtil::GetTimeMs();
            std::string uniq_id = std::to_string(id);
            return ms + "_" + uniq_id;

        }
```
另外，在编译运行完，拿到结果之后，我们是需要将文件对应的输入输出错误文件删除的！

通过IP + 端口启动编译服务，并监听所有窗口
```
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
```

## oj_server

### oj_control
在核心业务上，只需要执行两件事。一：完成题目列表和单个题目网页渲染。二是：判题功能
不过在判题逻辑这里，我们需要进行负载均衡的选择主机编译。
判题这里填充的Json字段，见编译运行部分。
#### oj_view
利用ctemplate库渲染前端页面，在html中，被{{}}括起来的，就是要被替换的内容。
我们可以创建ctemplate::TemplateDictionary对象，将前端页面所需替换字段填充。
最后使用ctemplate::Template::GetTemplate完成渲染。
#### oj_model
题目信息结构体：
```
    struct Question
    {
        std::string number; //题目编号，唯一
        std::string title;  //题目的标题
        std::string star;   //难度: 简单 中等 困难
        std::string desc;   //题目的描述
        std::string header; //题目预设给用户在线编辑器的代码
        std::string tail;   //题目的测试用例，需要和header拼接，形成完整代码
        int cpu_limit;      //题目的时间要求(S)
        int mem_limit;      //题目的空间要去(KB)
    };
```
- 文件版：
	- 文件版的题目信息，我们都存储在了一个.conf文件当中。我们需要先解析文件内容。这里我们使用boost库来解析字符串内容。将解析好的内容，我们按照题号 : 题目细节的映射方式存储在unordered_map当中。
	- 题目的编号，标题，难度，描述。时间要求，空间要求。两个文件构成：
	1. questions.list： 题目列表(不需要题目的内容)
	2. 题目的描述，题目的预设值代码(header.hpp),测试用例代码(tail.cpp) 
	3. 这两个内容是通过题目的编号产生关联的。
	    两个.cpp文件可以通过直接include包含，来实现关联，防止报错。
		当用户提交自己的代码时，OJ不上直接把代码交给compiler_and_run，而是把拼接上该题号测试用例，形成最终提交给后台编译运行的服务代码。
- 数据库版
	- 将题目信息按照IP+端口的方式，访问主机上特定的数据库文件。从表中获取题目信息。
#### 负载均衡

- Machine 设置主机信息
	- 包括了IP 端口 当前主句数 锁
- LoadBlance
	- 在这里成员online和offline里面存储的都是int类型，存储的是machines当中主机下标。
	- LoadConf  将配置文件中的IP端口，配置到主机信息当中，并存储到vector当中。
	- SmartChoice 智能选择压力最小的一台主机。我们可以遍历在线主机，找到负载最小的一台主机
