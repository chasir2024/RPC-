#pragma once
#include "google/protobuf/service.h"
#include "muduo/net/TcpServer.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/InetAddress.h"
#include <unordered_map>
#include "mprpcconfig.h"
#include "mprpcapplication.h"
#include <google/protobuf/descriptor.h>
#include <string>

// 框架提供的专门发布rpc服务的类

class RpcProvider
{
public:
//框架提供给外部使用的，可以发布rpc方法的函数接口，外部可以通过这个接口把自己编写的rpc服务发布到rpc节点上
    void NotifyService(google::protobuf::Service* service);
//启动rpc服务节点，开始提供rpc远程调用服务
    void Run();
    // 新的socket连接
    void OnConnection(const muduo::net::TcpConnectionPtr& conn);
    // 处理rpc请求消息
    void OnMessage(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buffer, muduo::Timestamp timestamp);
    // 发送rpc响应
    void SendRpcResponse(const muduo::net::TcpConnectionPtr& conn, google::protobuf::Message* response);
private:
    //service服务类型信息；
    struct ServiceInfo
    {
        google::protobuf::Service *m_service; //保存服务对象
        std::unordered_map<std::string,const google::protobuf::MethodDescriptor*> m_methodMap; //保存服务方法
    };
    // 存储注册成功的服务对象和方法 key: service_name  value: ServiceInfo
    std::unordered_map<std::string, ServiceInfo> m_serviceMap; //存储注册成功的服务对象和方法

    // 组合了TcpServer和EventLoop对象
    std::unique_ptr<muduo::net::TcpServer> m_tcpServerPtr;
    muduo::net::EventLoop m_eventLoop;

};

