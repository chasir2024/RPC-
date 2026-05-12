#include <iostream>
#include "user.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"
/*
userservice 原来是一个本地服务，提供了两个进程内的本地方法，Login和GetFriendList
*/

class UserService : public fixbug::UserServiceRPC //使用在rpc服务发布端（rpc服务提供者）
{
public:
    bool Login(std::string name,std::string pwd){
        std::cout << "Login方法被调用了" << std::endl;
        std::cout << "name: " << name << " pwd: " << pwd << std::endl;
        return true;
    }
    bool Register(int32_t id,std::string name,std::string pwd){
        std::cout << "Register方法被调用了" << std::endl;
        std::cout << "id: " << id << " name: " << name << " pwd: " << pwd << std::endl;
        return true;
    }
    //重写基类UserServiceRPC的Login虚函数，下面方法是框架直接调用的
    //1.caller ====> Login(LoginRequest) => muduo ===> callee
    //2.callee ====> Login(LoginRequest) => 交到下面重写的Login方法上
    void Login(::google::protobuf::RpcController* controller,
                        const ::fixbug::LoginRequest* request,
                        ::fixbug::LoginResponse* response, 
                        ::google::protobuf::Closure* done)
    {
        //框架给业务上报了请求参数 LoginRequest,业务获取相应数据，做本地业务逻辑
        std::string name = request->name();
        std::string pwd = request->pwd();

        bool login_result = Login(name,pwd);//本地业务逻辑

        //把相应结果写入LoginResponse
        fixbug::ResultCode* code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");
        response->set_success(login_result);

        //执行回调  执行响应消息的序列化和网络发送（由框架完成）
        done->Run();
    }
    
    void Register(google::protobuf::RpcController* controller,
                       const ::fixbug::RegisterRequest* request,
                       ::fixbug::RegisterResponse* response,
                       ::google::protobuf::Closure* done){
        int32_t id = request->id();
        std::string name = request->name();
        std::string pwd = request->pwd();

        bool ret = Register(id,name,pwd);

        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("");
        response->set_success(ret);

        //执行回调  执行响应消息的序列化和网络发送（由框架完成）
        done->Run();
    }
};


int main(int argc, char** argv){
    //调用框架初始化操作
    MprpcApplication::Init(argc,argv);

    //把UserService对象发布到rpc节点上  让rpc框架来调用这个对象提供的Login方法
    RpcProvider provider;
    provider.NotifyService(new UserService());

    //启动rpc服务
    provider.Run();
    return 0;
}