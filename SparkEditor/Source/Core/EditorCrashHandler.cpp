/**
 * @file EditorCrashHandler.cpp
 * @brief Implementation of the editor crash handler
 * @author Spark Engine Team
 * @date 2025
 */

#include "EditorCrashHandler.h"
#include "EditorLogger.h"
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <optional>
#include <thread>
#include <chrono>

namespace SparkEditor {

// Static instance for singleton
EditorCrashHandler* EditorCrashHandler::s_instance = nullptr;

EditorCrashHandler& EditorCrashHandler::GetInstance() {
    if (!s_instance) {
        s_instance = new EditorCrashHandler();
    }
    return *s_instance;
}

EditorCrashHandler::~EditorCrashHandler() {
    // Ensure safe shutdown
    m_shouldStopAutoSave = true;
    
    if (m_autoSaveThread.joinable()) {
        try {
            m_autoSaveThread.join();
        } catch (...) {
            // Detach if join fails to prevent blocking
            m_autoSaveThread.detach();
        }
    }
}

bool EditorCrashHandler::Initialize(const std::string& crashDirectory, EditorLogger* logger) {
    std::cout << "EditorCrashHandler initializing...\n";
    
    m_crashDirectory = crashDirectory;
    m_logger = logger;
    m_initialized = true;
    m_sessionStartTime = std::chrono::steady_clock::now();
    
    // Don't start auto-save thread for now to avoid deadlock issues
    // if (m_autoSaveEnabled) {
    //     m_shouldStopAutoSave = false;
    //     m_autoSaveThread = std::thread(&EditorCrashHandler::AutoSaveRecoveryThread, this);
    // }
    
    std::cout << "EditorCrashHandler initialized successfully\n";
    return true;
}

void EditorCrashHandler::Shutdown() {
    std::cout << "EditorCrashHandler shutting down...\n";
    
    // Signal auto-save thread to stop
    m_shouldStopAutoSave = true;
    
    // Wait for thread to finish with timeout to avoid deadlock
    if (m_autoSaveThread.joinable()) {
        try {
            m_autoSaveThread.join();
        } catch (...) {
            // If join fails, just detach to avoid blocking shutdown
            m_autoSaveThread.detach();
        }
    }
    
    m_initialized = false;
    m_logger = nullptr;
    std::cout << "EditorCrashHandler shutdown complete\n";
}

void EditorCrashHandler::SetCrashCallback(CrashCallback callback) {
    m_crashCallback = callback;
}

void EditorCrashHandler::SetRecoveryCallback(RecoveryCallback callback) {
    m_recoveryCallback = callback;
}

void EditorCrashHandler::SetAssertCallback(AssertCallback callback) {
    m_assertCallback = callback;
}

void EditorCrashHandler::HandleAssertion(const std::string& expression, const char* file, int line, const std::string& message) {
    std::lock_guard<std::mutex> lock(m_statsMutex);
    m_stats.assertionFailures++;
    
    if (m_assertCallback) {
        m_assertCallback(expression, file ? file : "", line, message);
    }
    
    if (m_logger) {
        std::string logMsg = "Assertion failed: " + expression;
        if (!message.empty()) {
            logMsg += " - " + message;
        }
        if (file) {
            logMsg += " at " + std::string(file) + ":" + std::to_string(line);
        }
        m_logger->Log(LogLevel::ERROR_, "Assert", logMsg);
    }
}

void EditorCrashHandler::RecordOperation(const std::string& operation) {
    std::lock_guard<std::mutex> lock(m_operationsMutex);
    
    m_recentOperations.push_back(operation);
    if (m_recentOperations.size() > m_maxOperations) {
        m_recentOperations.erase(m_recentOperations.begin());
    }
}

void EditorCrashHandler::SetEditorState(const std::string& state) {
    std::lock_guard<std::mutex> lock(m_operationsMutex);
    m_currentEditorState = state;
}

bool EditorCrashHandler::SaveRecoveryData() {
    if (!m_recoveryCallback) {
        return false;
    }
    
    try {
        RecoveryData data = m_recoveryCallback();
        
        // Save recovery data to file
        std::string recoveryFile = m_crashDirectory + "/recovery.json";
        std::ofstream file(recoveryFile);
        if (!file.is_open()) {
            return false;
        }
        
        // Simple JSON-like format
        file << "{\n";
        file << "  \"currentLayout\": \"" << data.currentLayout << "\",\n";
        file << "  \"currentProject\": \"" << data.currentProject << "\",\n";
        file << "  \"lastSavedScene\": \"" << data.lastSavedScene << "\",\n";
        file << "  \"openFiles\": [\n";
        for (size_t i = 0; i < data.openFiles.size(); ++i) {
            file << "    \"" << data.openFiles[i] << "\"";
            if (i < data.openFiles.size() - 1) file << ",";
            file << "\n";
        }
        file << "  ],\n";
        file << "  \"recentOperations\": [\n";
        for (size_t i = 0; i < data.recentOperations.size(); ++i) {
            file << "    \"" << data.recentOperations[i] << "\"";
            if (i < data.recentOperations.size() - 1) file << ",";
            file << "\n";
        }
        file << "  ]\n";
        file << "}\n";
        
        file.close();
        
        std::lock_guard<std::mutex> lock(m_statsMutex);
        m_stats.recoveryDataSaves++;
        
        return true;
    } catch (...) {
        return false;
    }
}

std::optional<RecoveryData> EditorCrashHandler::LoadRecoveryData() {
    try {
        std::string recoveryFile = m_crashDirectory + "/recovery.json";
        std::ifstream file(recoveryFile);
        if (!file.is_open()) {
            return std::nullopt;
        }
        
        RecoveryData data;
        std::string line;
        
        // Simple parser for our JSON-like format
        while (std::getline(file, line)) {
            if (line.find("\"currentLayout\"") != std::string::npos) {
                size_t start = line.find(": \"") + 3;
                size_t end = line.find_last_of("\"");
                if (start < end) {
                    data.currentLayout = line.substr(start, end - start);
                }
            }
            else if (line.find("\"currentProject\"") != std::string::npos) {
                size_t start = line.find(": \"") + 3;
                size_t end = line.find_last_of("\"");
                if (start < end) {
                    data.currentProject = line.substr(start, end - start);
                }
            }
            else if (line.find("\"lastSavedScene\"") != std::string::npos) {
                size_t start = line.find(": \"") + 3;
                size_t end = line.find_last_of("\"");
                if (start < end) {
                    data.lastSavedScene = line.substr(start, end - start);
                }
            }
            // TODO: Parse arrays properly for openFiles and recentOperations
        }
        
        file.close();
        return data;
    } catch (...) {
        return std::nullopt;
    }
}

bool EditorCrashHandler::HasRecoveryData() {
    try {
        std::string recoveryFile = m_crashDirectory + "/recovery.json";
        return std::filesystem::exists(recoveryFile);
    } catch (...) {
        return false;
    }
}

void EditorCrashHandler::ClearRecoveryData() {
    try {
        std::string recoveryFile = m_crashDirectory + "/recovery.json";
        if (std::filesystem::exists(recoveryFile)) {
            std::filesystem::remove(recoveryFile);
        }
    } catch (...) {
        // Ignore errors when clearing recovery data
    }
}

EditorCrashHandler::CrashStats EditorCrashHandler::GetStats() const {
    std::lock_guard<std::mutex> lock(m_statsMutex);
    return m_stats;
}

std::string EditorCrashHandler::GenerateCrashReport(const CrashInfo& crashInfo) {
    std::string report = "=== Spark Editor Crash Report ===\n";
    report += "Exception Type: " + crashInfo.exceptionType + "\n";
    report += "Exception Message: " + crashInfo.exceptionMessage + "\n";
    report += "Editor State: " + m_currentEditorState + "\n";
    
    report += "\nRecent Operations:\n";
    std::lock_guard<std::mutex> lock(m_operationsMutex);
    for (const auto& op : m_recentOperations) {
        report += "  - " + op + "\n";
    }
    
    return report;
}

void EditorCrashHandler::TestCrashHandler() {
    std::cout << "Testing crash handler (this should not crash in development)\n";
    // In a real crash handler, this would trigger a controlled crash for testing
}

void EditorCrashHandler::TestAssertionHandler() {
    HandleAssertion("test_expression", __FILE__, __LINE__, "Test assertion for crash handler verification");
}

void EditorCrashHandler::AutoSaveRecoveryThread() {
    while (!m_shouldStopAutoSave) {
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(m_autoSaveInterval * 1000)));
        
        if (!m_shouldStopAutoSave) {
            SaveRecoveryData();
        }
    }
}

LONG WINAPI EditorCrashHandler::ExceptionFilter(EXCEPTION_POINTERS* exceptionPointers) {
    if (s_instance) {
        s_instance->HandleCrashInternal(exceptionPointers);
    }
    return EXCEPTION_EXECUTE_HANDLER;
}

void EditorCrashHandler::HandleCrashInternal(EXCEPTION_POINTERS* exceptionPointers) {
    std::lock_guard<std::mutex> lock(m_statsMutex);
    m_stats.totalCrashes++;
    
    if (m_crashCallback) {
        CrashInfo info;
        info.exceptionPointers = exceptionPointers;
        info.timestamp = std::chrono::system_clock::now();
        info.editorState = m_currentEditorState;
        
        m_crashCallback(info);
    }
}

std::string EditorCrashHandler::GenerateStackTrace(EXCEPTION_POINTERS* exceptionPointers) {
    // TODO: Implement stack trace generation
    return "Stack trace not implemented";
}

std::string EditorCrashHandler::GetSystemInfo() {
    // TODO: Implement system info gathering
    return "System info not implemented";
}

std::string EditorCrashHandler::GetThreadInfo() {
    // TODO: Implement thread info gathering
    return "Thread info not implemented";
}

bool EditorCrashHandler::SaveCrashDump(EXCEPTION_POINTERS* exceptionPointers, const std::string& filePath) {
    // TODO: Implement crash dump saving
    return false;
}

bool EditorCrashHandler::SaveCrashLog(const CrashInfo& crashInfo, const std::string& filePath) {
    // TODO: Implement crash log saving
    return false;
}

void EditorCrashHandler::UpdateStats(const CrashInfo& crashInfo) {
    // Stats are updated in HandleCrashInternal
}

} // namespace SparkEditor