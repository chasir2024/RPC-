#include"mprpcchannel.h"
#include <string>
#include <iostream>
#include "rpcheader.pb.h"
#include "mprpcapplication.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


// 文件描述符 RAII 封装类
class FdGuard {
public:
    explicit FdGuard(int fd) : fd_(fd) {}
    ~FdGuard() { 
        if (fd_ != -1) {
            close(fd_); 
        }
    }
    // 方便获取底层 fd
    int get() const { return fd_; }
    
    // 禁用拷贝，防止多个对象 close 同一个 fd
    FdGuard(const FdGuard&) = delete;
    FdGuard& operator=(const FdGuard&) = delete;

private:
    int fd_;
};

/*
header_size + service_name method_name args_size + args;
*/
void MprpcChannel::CallMethod(const google::protobuf::MethodDescriptor* method,
                              google::protobuf::RpcController* controller,
                              const google::protobuf::Message* request,
                              google::protobuf::Message* response,
                              google::protobuf::Closure* done)
{
    const google::protobuf::ServiceDescriptor* sd = method->service();
    std::string service_name = sd->name();
    std::string method_name = method->name();
    // 获取参数的序列化字符串长度 args_size
    std::string args_str;
    // 将请求参数序列化为字符串
    if(!request->SerializeToString(&args_str)){
        std::cout << "request serialize error!" << std::endl;
        return;
    }
    uint32_t args_size = args_str.size();
    // 定义rpc请求的header格式 header_size + service_name method_name args_size
    mprpc::RpcHeader rpc_header;
    rpc_header.set_service_name(service_name);
    rpc_header.set_method_name(method_name);
    rpc_header.set_args_size(args_size);
    
    std::string rpc_header_str;
    uint32_t header_size = 0;
    if(!rpc_header.SerializeToString(&rpc_header_str)){
        std::cout << "rpc header serialize error!" << std::endl;
        return;
    }else{
        header_size = rpc_header_str.size();
    }
    // 组织待发送的rpc请求字符串
    std::string send_rpc_str;
    send_rpc_str.insert(0, std::string((char*)&header_size, 4));//header_size
    send_rpc_str.append(rpc_header_str);
    send_rpc_str.append(args_str);

    std::cout << "=========================================" << std::endl;
    std::cout << "header_size: " << header_size << std::endl;
    std::cout << "rpc_header_str: " << rpc_header_str << std::endl;
    std::cout << "service_name: " << service_name << std::endl;
    std::cout << "method_name: " << method_name << std::endl;
    std::cout << "args_size: " << args_size << std::endl;
    std::cout << "args_str: " << args_str << std::endl;
    std::cout << "=========================================" << std::endl;

    // 使用TCP编程 完成rpc方法的远程调用  需要知道rpc服务提供方的ip地址和端口号
    // 智能指针 动态分配一个socket对象，自动释放
    
    int clientfd = socket(AF_INET,SOCK_STREAM,0);
    if(clientfd == -1){
        std::cout << "create socket error!" << std::endl;
        exit(EXIT_FAILURE);
    }
    FdGuard fd_guard(clientfd);

    std::string ip = MprpcApplication::GetInstance().GetConfig().Load("rpcserverip");
    std::uint16_t port = atoi(MprpcApplication::GetInstance().GetConfig().Load("rpcserverport").c_str());
    // 连接rpc服务提供方
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    // 连接rpc服务提供方
    if(-1 == connect(clientfd,(struct sockaddr*)&server_addr,sizeof(server_addr))){
        std::cout << "connect server error!" << std::endl;
        exit(EXIT_FAILURE);
    }
    // 发送rpc请求
    if(-1 == send(clientfd,send_rpc_str.c_str(),send_rpc_str.size(),0)){
        std::cout << "send rpc request error!" << std::endl;
        exit(EXIT_FAILURE);
    }
    // 接受rpc响应
    char recv_buf[1024] = {0};
    int recv_size = 0;
    if(-1 == (recv_size = recv(clientfd,recv_buf,sizeof(recv_buf),0))){
        std::cout << "recv rpc response error!" << std::endl;
        exit(EXIT_FAILURE);
    }

    // std::string response_str(recv_buf, 0, recv_size);// response parse error!

    // 将rpc响应反序列化为response对象
    if(!response->ParseFromArray(recv_buf, recv_size)){
        std::cout << "response parse error!"  << std::endl;
        return;
    }
}
