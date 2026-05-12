#include "mprpcconfig.h"
#include<iostream>
#include<string>

void MprpcConfig::LoadConfigFile(const std::string& config_file){
    //加载配置文件
    FILE* pf = fopen(config_file.c_str(),"r");
    if(nullptr == pf){
        std::cout <<  config_file << " not found" << std::endl;
        exit(EXIT_FAILURE);
    }
    // 1.注释 2.正确的配置项 =  3.去掉开头多与空格
    while(!feof(pf)){
        char buf[512] = {0};
        // 读取一行
        fgets(buf,512,pf);
        
        std::string read_buf(buf);
        // 去掉字符串前面多余的空格
        Trim(read_buf);
        // #注释
        if(read_buf[0] == '#' || read_buf.empty()){
            continue;
        }

        //解析配置项
        int pos = read_buf.find('=');
        if(pos == -1) continue;
        std::string key;
        std::string value;
        // rpcserver = 127.0.0.1
        key = read_buf.substr(0,pos);
        Trim(key);
        value = read_buf.substr(pos+1,read_buf.size()-pos-1);
        Trim(value);
        m_configMap.insert({key,value});
    }
}

std::string MprpcConfig::Load(const std::string& key){
    //查询配置项
    auto it = m_configMap.find(key);
    if(it == m_configMap.end()){    
        return " ";
    }
    return it->second;
}


void MprpcConfig::Trim(std::string& src_buf){
    //去掉字符串前后的空格以及换行符
    size_t pos = src_buf.find_first_not_of(" \n\r");
    if(pos != -1){
        //说明前面有空格或换行，直接截取到末尾
        src_buf = src_buf.substr(pos, src_buf.size()-pos);
    }
    //处理后面有空格或换行的情况
    pos = src_buf.find_last_not_of(" \n\r");
    if(pos != -1){
        // 注意这里是 pos + 1
        src_buf = src_buf.substr(0, pos + 1);
    }
}
