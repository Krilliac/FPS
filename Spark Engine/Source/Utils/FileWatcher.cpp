#include "FileWatcher.h"
#include "Utils/Assert.h"
#include <iostream>
#include <filesystem>

namespace SparkEngine {

    FileWatcher::FileWatcher(const std::string& directory)
        : m_dir(directory), m_watching(false) {
        SPARK_ASSERT(!directory.empty(), "Directory path cannot be empty");

        if (!std::filesystem::exists(directory)) {
            std::cerr << "Warning: Directory does not exist: " << directory << std::endl;
        }
    }

    void FileWatcher::Start() {
        if (m_watching) return;

        m_watching = true;
        ScanDirectory(); // Initial scan to populate timestamps
        std::cout << "FileWatcher started monitoring: " << m_dir << std::endl;
    }

    void FileWatcher::Stop() {
        if (!m_watching) return;

        m_watching = false;
        m_times.clear();
        std::cout << "FileWatcher stopped monitoring: " << m_dir << std::endl;
    }

    void FileWatcher::Update() {
        if (!m_watching) return;

        try {
            if (!std::filesystem::exists(m_dir)) {
                std::cerr << "Warning: Watched directory no longer exists: " << m_dir << std::endl;
                return;
            }

            for (const auto& entry : std::filesystem::recursive_directory_iterator(m_dir)) {
                if (!entry.is_regular_file() || !IsValidFile(entry.path())) {
                    continue;
                }

                const auto filepath = entry.path().string();
                const auto lastWriteTime = std::filesystem::last_write_time(entry);

                auto it = m_times.find(filepath);
                if (it == m_times.end()) {
                    // New file detected
                    m_times[filepath] = lastWriteTime;
                    OnModified(filepath);
                }
                else if (it->second != lastWriteTime) {
                    // File modified
                    it->second = lastWriteTime;
                    OnModified(filepath);
                }
            }

            // Clean up deleted files from our tracking map
            for (auto it = m_times.begin(); it != m_times.end();) {
                if (!std::filesystem::exists(it->first)) {
                    std::cout << "File deleted: " << it->first << std::endl;
                    it = m_times.erase(it);
                }
                else {
                    ++it;
                }
            }
        }
        catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "FileWatcher error: " << e.what() << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "FileWatcher unexpected error: " << e.what() << std::endl;
        }
    }

    void FileWatcher::SetOnModifiedCallback(std::function<void(const std::string&)> callback) {
        m_onModifiedCallback = std::move(callback);
    }

    void FileWatcher::OnModified(const std::string& filepath) {
        std::cout << "File modified: " << filepath << std::endl;

        if (m_onModifiedCallback) {
            m_onModifiedCallback(filepath);
        }
    }

    void FileWatcher::ScanDirectory() {
        try {
            m_times.clear();

            if (!std::filesystem::exists(m_dir)) {
                return;
            }

            for (const auto& entry : std::filesystem::recursive_directory_iterator(m_dir)) {
                if (!entry.is_regular_file() || !IsValidFile(entry.path())) {
                    continue;
                }

                const auto filepath = entry.path().string();
                const auto lastWriteTime = std::filesystem::last_write_time(entry);
                m_times[filepath] = lastWriteTime;
            }

            std::cout << "FileWatcher: Initial scan found " << m_times.size() << " files" << std::endl;
        }
        catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "FileWatcher scan error: " << e.what() << std::endl;
        }
    }

    bool FileWatcher::IsValidFile(const std::filesystem::path& path) const {
        // Filter out system files, temporary files, etc.
        const auto filename = path.filename().string();

        // Skip hidden files
        if (filename.starts_with('.')) {
            return false;
        }

        // Skip common temporary file patterns
        if (filename.ends_with(".tmp") ||
            filename.ends_with(".temp") ||
            filename.ends_with("~")) {
            return false;
        }

        // You can add more filtering logic here based on your needs
        return true;
    }

} // namespace SparkEngine