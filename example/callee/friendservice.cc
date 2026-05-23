#include <iostream>
#include "friend.pb.h"
#include <vector>
#include "mprpcapplication.h"
#include "rpcprovider.h"
#include "logger.h"
class FriendService : public fixbug::FriendServiceRpc
{
public:
    std::vector<std::string> GetFriendList(uint32_t user_id) {
        // 这里可以根据user_id查询好友列表
        // 假设我们查询到的好友列表如下
        std::cout << "GetFriendList called with user_id: " << user_id << std::endl;
        std::vector<std::string> vec;
        vec.push_back("friend1");
        vec.push_back("friend2");
        vec.push_back("friend3");
        return vec; 
    }

    void GetFriendList(::google::protobuf::RpcController* controller,
                       const ::fixbug::GetFriendListRequest* request,
                       ::fixbug::GetFriendListResponse* response,
                       ::google::protobuf::Closure* done)
    {
        uint32_t user_id = request->id();
        std::vector<std::string> friends = GetFriendList(user_id);
        // 将好友列表添加到响应中
        for (const auto& friend_name : friends) {
            response->add_friends(friend_name);
        }
        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("");

        done->Run();
    }

};

int main(int argc, char** argv){
    //调用框架初始化操作
    LOG_INFO("Initializing MprpcApplication");
    LOG_ERR("Failed to initialize MprpcApplication");
    MprpcApplication::Init(argc,argv);

    //把FriendService对象发布到rpc节点上  让rpc框架来调用这个对象提供的GetFriendList方法
    RpcProvider provider;
    provider.NotifyService(new FriendService());

    //启动rpc服务
    provider.Run();
    return 0;
}
