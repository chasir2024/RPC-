#pragma once
#include <string>
#include <unordered_map>

class MprpcConfig{
public:
        void LoadConfigFile(const std::string& config_file);
        std::string Load(const std::string& key);
private:
        std::unordered_map<std::string, std::string> m_configMap;
        //去掉字符串前后的空格
        void Trim(std::string& src_buf);
};