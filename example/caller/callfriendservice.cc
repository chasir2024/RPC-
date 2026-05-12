#include <iostream>
#include "mprpcapplication.h"

#include "friend.pb.h"


int main(int argc, char** argv){
    // 整个程序启动以后，想使用mprpc框架来享受rpc服务调用，一定要先调用框架的初始化函数（只初始化一次）
    MprpcApplication::Init(argc, argv);

    // FriendServiceRPC_Stub(::PROTOBUF_NAMESPACE_ID::RpcChannel* channel);
    fixbug::FriendServiceRpc_Stub stub_(new MprpcChannel());

    fixbug::GetFriendListRequest request;
    request.set_id(100);
    // rpc方法的响应
    fixbug::GetFriendListResponse response;
    RpcController controller;
    stub_.GetFriendList(&controller,&request,&response,nullptr);
    if(controller.Failed()){
        std::cout << "rpc GetFriendList failed: " << controller.ErrorText() << std::endl;
        return -1;
    }else{
        // 一次rpc调用完成，读调用的结果
        if(0 == response.result().errcode()){
            std::cout << "rpc GetFriendList response success: " << response.success() << std::endl;
            int size = response.friends_size();
            for(int i = 0; i < size; ++i){
                std::cout << "friend: " << response.friends(i) << std::endl;
            }
        }else{
            std::cout << "rpc GetFriendList response error" << response.result().errmsg() << std::endl;
        }

    }
    

    return 0;
}