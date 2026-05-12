#include "mprpcapplication.h"
#include <iostream>
#include <unistd.h>
#include <string>

MprpcConfig MprpcApplication::m_config; // 显式定义静态成员变量

void ShowArgHelp(){
    std::cout << "format: command -i config.conf" << std::endl;
}
void MprpcApplication::Init(int argc, char** argv){
    //初始化操作
    if (argc < 2){
        ShowArgHelp();
        exit(EXIT_FAILURE);
    }
    int c = 0;
    std::string config_file;
    while((c = getopt(argc,argv,"i:")) != -1){
        switch(c){
            case 'i':
                //处理配置文件路径
                config_file = optarg;   
                break;
            case '?':
                ShowArgHelp();
                exit(EXIT_FAILURE);
                break;
            case ':':
                ShowArgHelp();
                exit(EXIT_FAILURE);
                break;
            default:
                break;
        }
    }
    
    // 开始加载配置文件
    std::cout << "config_file: " << config_file << std::endl;
    m_config.LoadConfigFile(config_file.c_str());
    // std::cout << "rpcserverip:" << m_config.Load("rpcserverip") << std::endl;
    // std::cout << "rpcserverport:" << m_config.Load("rpcserverport") << std::endl;
    // std::cout << "zookeeperip:" << m_config.Load("zookeeperip") << std::endl;
    // std::cout << "zookeeperport:" << m_config.Load("zookeeperport") << std::endl;

}

MprpcApplication::MprpcApplication(){}

MprpcApplication& MprpcApplication::GetInstance(){
    static MprpcApplication app;
    return app;
    
}

MprpcConfig& MprpcApplication::GetConfig(){
    return m_config;
}