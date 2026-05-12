#include "rpcprovider.h"
#include "rpcheader.pb.h"

// json protubuf



//框架提供给外部使用的，可以发布rpc方法的函数接口
void RpcProvider::NotifyService(google::protobuf::Service* service){
    ServiceInfo service_info;

    // 获取服务对象的描述信息
    const google::protobuf::ServiceDescriptor *pserviceDesc = service->GetDescriptor();
    // 获取服务的名字
    std::string service_name = pserviceDesc->name();
    // 获取服务对象的所有方法数量
    int method_count = pserviceDesc->method_count();

    std::cout << "service_name: " << service_name << std::endl;

    // 遍历所有方法
    for (int i = 0; i < method_count; ++i) {
        // 获取了服务对象的第i个方法的描述信息
        const google::protobuf::MethodDescriptor* pmethodDesc = pserviceDesc->method(i); 
        std::string methode_name = pmethodDesc->name();
        service_info.m_methodMap.insert({methode_name,pmethodDesc});
        std::cout << "method_name: " << methode_name << std::endl;
    }

    service_info.m_service = service; //保存服务对象
    m_serviceMap.insert({service_name,service_info});//保存服务信息
}

//启动rpc服务节点，开始提供rpc远程调用服务
void RpcProvider::Run(){
    std::string ip = MprpcApplication::GetInstance().GetConfig().Load("rpcserverip");
    std::uint16_t port = atoi(MprpcApplication::GetInstance().GetConfig().Load("rpcserverport").c_str());
    muduo::net::InetAddress address(ip,port);

    // 创建TcpServer对象
    muduo::net::TcpServer server(&m_eventLoop, address, "RpcProvider");
    // 绑定回调和消息读写回调方法 分离网络代码和业务代码
    server.setConnectionCallback(std::bind(&RpcProvider::OnConnection, this, std::placeholders::_1));
    server.setMessageCallback(std::bind(&RpcProvider::OnMessage,this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    // 设置muduo库的线程数量 1个I/O线程 3个worker线程
     server.setThreadNum(4);
    // 启动网络服务
    server.start();
    std::cout << "RpcProvider start service at " << ip << ":" << port << std::endl;
    // 进入事件循环
    m_eventLoop.loop();
}

// 新的socket连接
void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr& conn){
    if (!conn->connected())
    {
        // 和rpc client 断开连接
        conn->shutdown();
    }
    
}
/*
在框架内部，Rpcprovider和Rpcconsumer协商之间的通信协议 protobuf数据类型
service_name  method_name args  进行数据头的序列化和反序列化 
定义一个protobuf消息头  header_size + header_str + args_size + args_str
*/
//已建立连接用户的读写时间回调 如果远程有一个rpc服务的调用请求，那么OnMessage方法就会响应
void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr& conn, 
                            muduo::net::Buffer* buffer, 
                            muduo::Timestamp timestamp){
    std::string recv_buf = buffer->retrieveAllAsString();
     
    //从字符流读取4个字节
    uint32_t header_size = 0;
    recv_buf.copy((char*)&header_size,4,0);
    //根据header_size读取消息头字符串
    std::string rpc_header_str = recv_buf.substr(4,header_size); // 获取rpc消息头字符串

    // 反序列化rpc消息头
    mprpc::RpcHeader rpc_header;
    std::string service_name;
    std::string method_name;
    std::int32_t args_size = 0;
    if(rpc_header.ParseFromString(rpc_header_str)){
        service_name = rpc_header.service_name();
        method_name = rpc_header.method_name();
        args_size = rpc_header.args_size();
    }else{
        // 反序列化失败
        std::cout << "rpc_header_str: " << rpc_header_str << " parse error!" << std::endl;
        return;
    }
    std::string args_str = recv_buf.substr(4 + header_size, args_size);

    std::cout << "=========================================" << std::endl;
    std::cout << "header_size: " << header_size << std::endl;
    std::cout << "rpc_header_str: " << rpc_header_str << std::endl;
    std::cout << "service_name: " << service_name << std::endl;
    std::cout << "method_name: " << method_name << std::endl;
    std::cout << "args_size: " << args_size << std::endl;
    std::cout << "args_str: " << args_str << std::endl;
    std::cout << "=========================================" << std::endl;

    //获取service对象 和 method 
    auto it = m_serviceMap.find(service_name);
    if(it == m_serviceMap.end()){
        std::cout << "service_name: " << service_name << " not found!" << std::endl;
        return;
    }
    
    auto mit = it->second.m_methodMap.find(method_name);
    if (mit == it->second.m_methodMap.end())
    {
        std::cout << "method_name: " << method_name << " not found!" << std::endl;
        return;
    }

    google::protobuf::Service *service = it->second.m_service; // 获取service对象  new UserService
    const google::protobuf::MethodDescriptor *method = mit->second; // 获取method对象 Login

    // 生成rpc方法调用的请求request和响应response参数
    google::protobuf::Message *request = service->GetRequestPrototype(method).New();
    if (!request->ParseFromString(args_str))    
    {
        std::cout << "request parse error,content " << args_str << std::endl;
        return;
    }
    google::protobuf::Message *response = service->GetResponsePrototype(method).New();

    //给下面的method方法的调用，绑定一个closure回调
    google::protobuf::Closure* done = google::protobuf::NewCallback<RpcProvider, const muduo::net::TcpConnectionPtr&, google::protobuf::Message*>(this, &RpcProvider::SendRpcResponse, conn, response);
    
    // 在框架上根据远端rpc请求 调用当前rpc节点上发布的方法
    // new UserService().Login(controller,request,response,closure)
    service->CallMethod(method,nullptr,request,response,done);
}

void RpcProvider::SendRpcResponse(const muduo::net::TcpConnectionPtr& conn, google::protobuf::Message* response){
    std::string response_str;
    // 序列化响应消息
    if(response->SerializeToString(&response_str)){
        // 发送响应消息 通过网络发送给rpc 方法的执行结果发送给rpc 的调用方
        conn->send(response_str);
    }
    else
    {
        std::cout << "serialize response_str error!" << std::endl;
    }
    conn->shutdown();
}