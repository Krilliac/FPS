/**
 * @file EditorLogger.h
 * @brief Advanced logging system for the Spark Engine Editor
 * @author Spark Engine Team
 * @date 2025
 * 
 * This file implements a comprehensive logging system with multiple output targets,
 * severity levels, filtering, and real-time display capabilities integrated with
 * the editor's UI system.
 */

#pragma once

// ULTRA AGGRESSIVE BUILD FIX - Include this FIRST
#include "../ultra_build_fix.h"

namespace SparkEditor {

/**
 * @brief Log severity levels
 */
enum class LogLevel {
    TRACE = 0,    ///< Detailed trace information
    DEBUG = 1,    ///< Debug information
    INFO = 2,     ///< General information
    WARNING = 3,  ///< Warning messages
    ERROR_ = 4,   ///< Error messages (renamed to avoid conflicts)
    CRITICAL = 5  ///< Critical system errors
};

/**
 * @brief Log entry structure
 */
struct LogEntry {
    SparkEditorTypes::TimePoint timestamp;
    LogLevel level;
    SparkEditorTypes::String category;
    SparkEditorTypes::String message;
    SparkEditorTypes::String file;
    int line = 0;
    SparkEditorTypes::String function;
    uint64_t frameNumber = 0;
    
    LogEntry() = default;
    LogEntry(LogLevel lvl, const SparkEditorTypes::String& cat, const SparkEditorTypes::String& msg)
        : timestamp(SE_CHRONO::system_clock::now()), level(lvl), category(cat), message(msg) {}
};

/**
 * @brief Log output target interface
 */
class LogTarget {
public:
    virtual ~LogTarget() = default;
    virtual void WriteLog(const LogEntry& entry) = 0;
    virtual void Flush() {}
};

/**
 * @brief Console output target
 */
class ConsoleLogTarget : public LogTarget {
public:
    void WriteLog(const LogEntry& entry) override;
};

/**
 * @brief File output target
 */
class FileLogTarget : public LogTarget {
public:
    explicit FileLogTarget(const SparkEditorTypes::String& filename);
    ~FileLogTarget() override;
    
    void WriteLog(const LogEntry& entry) override;
    void Flush() override;
    
private:
    SE_STD::ofstream m_file;
    SE_STD::mutex m_mutex;
};

/**
 * @brief Memory buffer target for UI display
 */
class MemoryLogTarget : public LogTarget {
public:
    explicit MemoryLogTarget(size_t maxEntries = 10000);
    
    void WriteLog(const LogEntry& entry) override;
    
    const SparkEditorTypes::Vector<LogEntry>& GetEntries() const { return m_entries; }
    void Clear();
    void SetMaxEntries(size_t maxEntries);
    
private:
    SparkEditorTypes::Vector<LogEntry> m_entries;
    size_t m_maxEntries;
    mutable SE_STD::mutex m_mutex;
};

/**
 * @brief Advanced logging system for the Spark Engine Editor
 * 
 * Provides comprehensive logging capabilities with multiple output targets,
 * filtering, real-time display, and integration with the editor's UI system.
 * 
 * Features:
 * - Multiple log levels (TRACE, DEBUG, INFO, WARNING, ERROR, CRITICAL)
 * - Multiple output targets (console, file, memory buffer)
 * - Category-based filtering
 * - Real-time log display in editor UI
 * - Thread-safe operation
 * - Frame correlation for debugging
 * - Search and filtering capabilities
 * - Export functionality
 * - Performance monitoring
 */
class EditorLogger {
public:
    /**
     * @brief Get singleton instance
     */
    static EditorLogger& GetInstance();

    /**
     * @brief Initialize the logger
     * @param logDirectory Directory for log files
     * @param maxMemoryEntries Maximum entries to keep in memory
     * @return true if initialization succeeded
     */
    bool Initialize(const SparkEditorTypes::String& logDirectory = "Logs", size_t maxMemoryEntries = 10000);

    /**
     * @brief Shutdown the logger
     */
    void Shutdown();

    /**
     * @brief Log a message
     * @param level Log severity level
     * @param category Log category
     * @param message Log message
     * @param file Source file (automatically filled by macro)
     * @param line Source line (automatically filled by macro)
     * @param function Source function (automatically filled by macro)
     */
    void Log(LogLevel level, const SparkEditorTypes::String& category, const SparkEditorTypes::String& message,
             const char* file = "", int line = 0, const char* function = "");

    /**
     * @brief Log with format string
     * @param level Log severity level
     * @param category Log category
     * @param format Format string
     * @param args Format arguments
     */
    template<typename... Args>
    void LogFormat(LogLevel level, const SparkEditorTypes::String& category, const char* format, Args&&... args);

    /**
     * @brief Add log target
     * @param target Unique pointer to log target
     */
    void AddTarget(SparkEditorTypes::UniquePtr<LogTarget> target);

    /**
     * @brief Remove all targets
     */
    void ClearTargets();

    /**
     * @brief Set minimum log level
     * @param level Minimum level to log
     */
    void SetMinimumLevel(LogLevel level) { m_minimumLevel = level; }

    /**
     * @brief Get minimum log level
     */
    LogLevel GetMinimumLevel() const { return m_minimumLevel; }

    /**
     * @brief Enable/disable category
     * @param category Category name
     * @param enabled Whether category should be logged
     */
    void SetCategoryEnabled(const SparkEditorTypes::String& category, bool enabled);

    /**
     * @brief Check if category is enabled
     * @param category Category name
     * @return true if category is enabled
     */
    bool IsCategoryEnabled(const SparkEditorTypes::String& category) const;

    /**
     * @brief Get memory target for UI display
     * @return Pointer to memory target, or nullptr if not available
     */
    MemoryLogTarget* GetMemoryTarget() const { return m_memoryTarget; }

    /**
     * @brief Set current frame number for correlation
     * @param frameNumber Current frame number
     */
    void SetFrameNumber(uint64_t frameNumber) { m_currentFrame = frameNumber; }

    /**
     * @brief Get statistics
     */
    struct LogStats {
        size_t totalEntries = 0;
        size_t entriesThisFrame = 0;
        SparkEditorTypes::UnorderedMap<LogLevel, size_t> entriesByLevel;
        SparkEditorTypes::UnorderedMap<SparkEditorTypes::String, size_t> entriesByCategory;
        float averageLogsPerSecond = 0.0f;
    };
    LogStats GetStats() const;

    /**
     * @brief Export logs to file
     * @param filename Output filename
     * @param filter Optional filter function
     * @return true if export succeeded
     */
    bool ExportLogs(const SparkEditorTypes::String& filename, 
                   SparkEditorTypes::Function<bool(const LogEntry&)> filter = nullptr) const;

    /**
     * @brief Flush all targets
     */
    void Flush();

private:
    EditorLogger() = default;
    ~EditorLogger() = default;
    EditorLogger(const EditorLogger&) = delete;
    EditorLogger& operator=(const EditorLogger&) = delete;

    /**
     * @brief Convert log level to string
     */
    SparkEditorTypes::String LogLevelToString(LogLevel level) const;

    /**
     * @brief Get color for log level (for UI display)
     */
    ImVec4 GetLevelColor(LogLevel level) const;

private:
    bool m_isInitialized = false;
    LogLevel m_minimumLevel = LogLevel::INFO;
    SparkEditorTypes::Vector<SparkEditorTypes::UniquePtr<LogTarget>> m_targets;
    SparkEditorTypes::UnorderedMap<SparkEditorTypes::String, bool> m_categoryFilters;
    MemoryLogTarget* m_memoryTarget = nullptr; // Non-owning pointer for quick access
    
    // Statistics
    mutable LogStats m_stats;
    mutable SE_STD::mutex m_statsMutex;
    uint64_t m_currentFrame = 0;
    SparkEditorTypes::TimePoint m_lastStatsUpdate;
    
    // Thread safety
    mutable SE_STD::mutex m_mutex;
};

// Template implementation
template<typename... Args>
void EditorLogger::LogFormat(LogLevel level, const SparkEditorTypes::String& category, const char* format, Args&&... args) {
    if (level < m_minimumLevel) return;
    if (!IsCategoryEnabled(category)) return;
    
    constexpr size_t BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];
    
    int result = snprintf(buffer, BUFFER_SIZE, format, SE_STD::forward<Args>(args)...);
    if (result > 0 && result < BUFFER_SIZE) {
        Log(level, category, SparkEditorTypes::String(buffer));
    } else {
        Log(LogLevel::ERROR_, "Logger", "Format string too long or invalid");
    }
}

} // namespace SparkEditor

// Convenience macros
#define SE_LOG_TRACE(category, message) \
    SparkEditor::EditorLogger::GetInstance().Log(SparkEditor::LogLevel::TRACE, category, message, __FILE__, __LINE__, __FUNCTION__)

#define SE_LOG_DEBUG(category, message) \
    SparkEditor::EditorLogger::GetInstance().Log(SparkEditor::LogLevel::DEBUG, category, message, __FILE__, __LINE__, __FUNCTION__)

#define SE_LOG_INFO(category, message) \
    SparkEditor::EditorLogger::GetInstance().Log(SparkEditor::LogLevel::INFO, category, message, __FILE__, __LINE__, __FUNCTION__)

#define SE_LOG_WARNING(category, message) \
    SparkEditor::EditorLogger::GetInstance().Log(SparkEditor::LogLevel::WARNING, category, message, __FILE__, __LINE__, __FUNCTION__)

#define SE_LOG_ERROR(category, message) \
    SparkEditor::EditorLogger::GetInstance().Log(SparkEditor::LogLevel::ERROR_, category, message, __FILE__, __LINE__, __FUNCTION__)

#define SE_LOG_CRITICAL(category, message) \
    SparkEditor::EditorLogger::GetInstance().Log(SparkEditor::LogLevel::CRITICAL, category, message, __FILE__, __LINE__, __FUNCTION__)

// Format versions
#define SE_LOG_TRACE_F(category, format, ...) \
    SparkEditor::EditorLogger::GetInstance().LogFormat(SparkEditor::LogLevel::TRACE, category, format, __VA_ARGS__)

#define SE_LOG_DEBUG_F(category, format, ...) \
    SparkEditor::EditorLogger::GetInstance().LogFormat(SparkEditor::LogLevel::DEBUG, category, format, __VA_ARGS__)

#define SE_LOG_INFO_F(category, format, ...) \
    SparkEditor::EditorLogger::GetInstance().LogFormat(SparkEditor::LogLevel::INFO, category, format, __VA_ARGS__)

#define SE_LOG_WARNING_F(category, format, ...) \
    SparkEditor::EditorLogger::GetInstance().LogFormat(SparkEditor::LogLevel::WARNING, category, format, __VA_ARGS__)

#define SE_LOG_ERROR_F(category, format, ...) \
    SparkEditor::EditorLogger::GetInstance().LogFormat(SparkEditor::LogLevel::ERROR_, category, format, __VA_ARGS__)

#define SE_LOG_CRITICAL_F(category, format, ...) \
    SparkEditor::EditorLogger::GetInstance().LogFormat(SparkEditor::LogLevel::CRITICAL, category, format, __VA_ARGS__)