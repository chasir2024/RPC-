#include <iostream>
#include <string>
#include "test.pb.h"

int main(){
    // fixbug::LoginResponse resp;
    // //resp.set_success(true);
    // fixbug::Resultcode* rc =  resp.mutable_result();
    // rc->set_errcode(1);
    // rc->set_errmsg("登陆失败");
    fixbug::GetFriendListResponse resp;
    fixbug::Resultcode* rc = resp.mutable_result();
    rc->set_errcode(1);
    fixbug::User* user1 = resp.add_friend_list();
    user1->set_name("ggbond");
    user1->set_age(18);
    user1->set_sex(fixbug::User::MAN);
    fixbug::User* user2 = resp.add_friend_list();
    user2->set_name("ggbond");
    user2->set_age(18);
    user2->set_sex(fixbug::User::MAN);
    std::cout << resp.friend_list_size() << std::endl;
    return 0;
}

int main1(){
    //封装了login请求对象的数据
    fixbug::LoginRequest req;
    req.set_name("ggbond");
    req.set_pwd("123456");

    //对象序列化 》》char*
    std::string send_str;
    //SerializeToString() 函数会将对象序列化成一个字符串，存储在send_str中
    if(req.SerializeToString(&send_str)){
        std::cout << send_str.c_str() << std::endl;
    } 

    //从send_str反序列化一个login请求对象
    fixbug::LoginRequest req2;
    if(req2.ParseFromString(send_str)){
        std::cout << "name: " << req2.name() << std::endl;
        std::cout << "pwd: " << req2.pwd() << std::endl;
    }
    return 0;
}