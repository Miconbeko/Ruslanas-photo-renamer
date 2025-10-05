#pragma once

#include <fstream>
#include <chrono>
#include <ctime>
#include <filesystem>

namespace fs = std::filesystem;

class Logger {
  private:
    bool logging;
    fs::path logpath;

    static Logger* logger;

    Logger();
  public:
    static Logger& getInstance();

    void write(std::string level, std::string msg);

    void info(std::string msg);
    void error(std::string msg);
};
