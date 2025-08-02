#pragma once
#include "framework.h"

namespace SparkEngine {
    enum class LogLevel {
        Debug,
        Info,
        Warning,
        Error
    };

    class Logger {
    public:
        static void Initialize();
        static void Shutdown();
        
        static void Debug(const std::string& message);
        static void Info(const std::string& message);
        static void Warning(const std::string& message);
        static void Error(const std::string& message);
        
        static void Log(LogLevel level, const std::string& message);
        
    private:
        static std::ofstream s_logFile;
        static std::mutex s_logMutex;
        static bool s_initialized;
    };

    // Convenience macros
    #define LOG_DEBUG(msg) Logger::Debug(msg)
    #define LOG_INFO(msg) Logger::Info(msg)
    #define LOG_WARNING(msg) Logger::Warning(msg)
    #define LOG_ERROR(msg) Logger::Error(msg)
}
