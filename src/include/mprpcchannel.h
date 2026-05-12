#pragma once 
#include <google/protobuf/service.h>

class MprpcChannel : public google::protobuf::RpcChannel
{
public:
// virtual void CallMethod(const MethodDescriptor* method,
//                           RpcController* controller, const Message* request,
//                           Message* response, Closure* done) = 0;
    // 所有通过stub对象调用的rpc方法，最终都会落到这个CallMethod方法上来
    // 统一做rpc方法调用的数据序列化和网络发送
    void CallMethod(const google::protobuf::MethodDescriptor* method,
                    google::protobuf::RpcController* controller,
                    const google::protobuf::Message* request,
                    google::protobuf::Message* response,
                    google::protobuf::Closure* done) override;

};