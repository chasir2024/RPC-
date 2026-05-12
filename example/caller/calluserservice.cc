#include <iostream>
#include "mprpcapplication.h"
#include "user.pb.h"
#include "mprpcchannel.h"
int main(int argc, char** argv){
    // 整个程序启动以后，想使用mprpc框架来享受rpc服务调用，一定要先调用框架的初始化函数（只初始化一次）
    MprpcApplication::Init(argc, argv);

    // UserServiceRPC_Stub(::PROTOBUF_NAMESPACE_ID::RpcChannel* channel);
    fixbug::UserServiceRPC_Stub stub_(new MprpcChannel());

    fixbug::LoginRequest request;
    request.set_name("ggbond");
    request.set_pwd("12345");
    // rpc方法的响应
    fixbug::LoginResponse response;
    stub_.Login(nullptr,&request,&response,nullptr);

    // 一次rpc调用完成，读调用的结果
    if(0 == response.result().errcode()){
        std::cout << "rpc login response success: " << response.success() << std::endl;
    }else{
        std::cout << "rpc login response error" << response.result().errmsg() << std::endl;
    }

    fixbug::RegisterRequest register_request;
    register_request.set_id(100);
    register_request.set_name("gggg");
    register_request.set_pwd("666666");

    fixbug::RegisterResponse register_response;
    stub_.Register(nullptr,&register_request,&register_response,nullptr);

    // 一次rpc调用完成，读调用的结果
    if(0 == register_response.result().errcode()){
        std::cout << "rpc register response success: " << register_response.success() << std::endl;
    }else{
        std::cout << "rpc register response error" << register_response.result().errmsg() << std::endl;
    }

    return 0;
}