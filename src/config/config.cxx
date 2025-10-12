#include "config.hxx"

#include <iostream>
#include <locale>
#include <codecvt>
#include <windows.h>

Config* Config::config = nullptr;

Config::Config(std::wstring filepath) {
    std::cout << "Start reading data from config . . ." << std::endl;

    env = ConvertEnvStringToMap(LoadUtf8FileToString(filepath));
}

Config& Config::init(std::wstring filepath) {
    if (config == nullptr) {
        config = new Config(filepath);
    }

    return *config;
}

Config& Config::getInstance() {
    if (config == nullptr) {
        throw std::runtime_error("Config not initialized yet");
    }

    return *config;
}

std::wstring Config::s2ws(const std::string& str)
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.from_bytes(str);
}

std::string Config::ws2s(const std::wstring& wstr)
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.to_bytes(wstr);
}

size_t Config::GetSizeOfFile(const std::wstring& path)
{
    struct _stat fileinfo;
    _wstat(path.c_str(), &fileinfo);
    return fileinfo.st_size;
}

std::wstring Config::LoadUtf8FileToString(const std::wstring& filename)
{
    std::wstring buffer;            // stores file contents
    FILE* f = _wfopen(filename.c_str(), L"rtS, ccs=UTF-8");

    // Failed to open file
    if (f == NULL)
    {
        std::cerr << "Failed to open config.txt" << std::endl;
        return buffer;
    }

    size_t filesize = GetSizeOfFile(filename);

    // Read entire file contents in to memory
    if (filesize > 0)
    {
        buffer.resize(filesize);
        size_t wchars_read = fread(&(buffer.front()), sizeof(wchar_t), filesize, f);
        buffer.resize(wchars_read);
        buffer.shrink_to_fit();
    }

    fclose(f);

    return buffer;
}

std::map<std::wstring, std::wstring> Config::ConvertEnvStringToMap(std::wstring file) {
    std::map<std::wstring, std::wstring> env;
    std::wstring line;
    size_t newline = 0;

    while(true) {
        newline = file.find('\n');
        line = file.substr(0, newline);

        if (line.find('=') == std::string::npos)
            break;

        // std::wcout << newline << " " << line.substr(0, line.find('=')) << " " << line.substr(line.find('=') + 1) << std::endl;
        env.insert(std::make_pair(line.substr(0, line.find('=')), line.substr(line.find('=') + 1)));

        if (newline == std::string::npos)
            break;

        file = file.substr(newline + 1);
    }

    return env;
}

std::string Config::get(std::string key) {
    std::wstring w_key = s2ws(key);

    return ws2s(env.at(w_key));
}

std::wstring Config::get_ws(std::string key) {
    std::wstring w_key = s2ws(key);

    return env.at(w_key);
}