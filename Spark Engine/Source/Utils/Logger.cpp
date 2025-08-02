#include "Logger.h"
#include <iostream>
#include <iomanip>
#include <sstream>

namespace SparkEngine {
    std::ofstream Logger::s_logFile;
    std::mutex Logger::s_logMutex;
    bool Logger::s_initialized = false;

    void Logger::Initialize() {
        if (s_initialized) return;
        
        s_logFile.open("SparkEngine.log", std::ios::out | std::ios::trunc);
        s_initialized = true;
        
        Info("Logger initialized");
    }

    void Logger::Shutdown() {
        if (!s_initialized) return;
        
        Info("Logger shutting down");
        s_logFile.close();
        s_initialized = false;
    }

    void Logger::Debug(const std::string& message) {
        #ifdef _DEBUG
        Log(LogLevel::Debug, message);
        #endif
    }

    void Logger::Info(const std::string& message) {
        Log(LogLevel::Info, message);
    }

    void Logger::Warning(const std::string& message) {
        Log(LogLevel::Warning, message);
    }

    void Logger::Error(const std::string& message) {
        Log(LogLevel::Error, message);
    }

    void Logger::Log(LogLevel level, const std::string& message) {
        if (!s_initialized) return;
        
        std::lock_guard<std::mutex> lock(s_logMutex);
        
        // Get current time
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto tm = *std::localtime(&time_t);
        
        // Format timestamp
        std::stringstream timestamp;
        timestamp << std::put_time(&tm, "%H:%M:%S");
        
        // Level string
        std::string levelStr;
        switch (level) {
            case LogLevel::Debug:   levelStr = "DEBUG"; break;
            case LogLevel::Info:    levelStr = "INFO "; break;
            case LogLevel::Warning: levelStr = "WARN "; break;
            case LogLevel::Error:   levelStr = "ERROR"; break;
        }
        
        // Format message
        std::string formatted = "[" + timestamp.str() + "] [" + levelStr + "] " + message;
        
        // Output to console
        std::cout << formatted << std::endl;
        
        // Output to file
        if (s_logFile.is_open()) {
            s_logFile << formatted << std::endl;
            s_logFile.flush();
        }
    }
}
