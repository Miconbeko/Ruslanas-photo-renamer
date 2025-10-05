#include "logger.hxx"

#include "config/config.hxx"

#include <string.h>

Logger* Logger::logger = nullptr;

Logger::Logger() {
    Config& config = Config::getInstance();

    logging = config.get("LOGGING") == "1" ? true : false;
    logpath = config.get_ws("LOGGING_PATH");
}

Logger& Logger::getInstance() {
    if (logger == nullptr) {
        logger = new Logger();
    }

    return *logger;
}

void Logger::write(std::string level, std::string msg) {
    if (!logger) {
        return;
    }

    std::ofstream os(logpath, std::ios_base::app);
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    os << strtok(std::ctime(&now), "\n") << " [" << level << "] " << msg << std::endl;
}

void Logger::info(std::string msg) {
    write("INFO", msg);
}

void Logger::error(std::string msg) {
    write("ERROR", msg);
}
