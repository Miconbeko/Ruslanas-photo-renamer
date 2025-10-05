#pragma once

#include <string>
#include <map>

class Config {
private:
    std::map<std::wstring, std::wstring> env;

    Config(std::wstring filepath);
    size_t GetSizeOfFile(const std::wstring& path);
    std::wstring LoadUtf8FileToString(const std::wstring& filename);
    std::map<std::wstring, std::wstring> ConvertEnvStringToMap(std::wstring file);
    std::string ws2s(const std::wstring& wstr);
    std::wstring s2ws(const std::string& str);

    static Config* config;
public:
    static Config& init(std::wstring filepath);
    static Config& getInstance();

    std::string get(std::string key);
    std::wstring get_ws(std::string key);
};