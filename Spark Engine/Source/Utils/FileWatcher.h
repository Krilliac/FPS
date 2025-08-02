#pragma once

#include <string>
#include <unordered_map>
#include <filesystem>
#include <functional>

namespace SparkEngine {
    
    class FileWatcher {
    public:
        explicit FileWatcher(const std::string& directory);
        ~FileWatcher() = default;

        // Start/stop watching
        void Start();
        void Stop();
        
        // Update method - call this regularly to check for changes
        void Update();
        
        // Set callback for when files are modified
        void SetOnModifiedCallback(std::function<void(const std::string&)> callback);
        
        // Check if currently watching
        bool IsWatching() const { return m_watching; }
        
        // Get watched directory
        const std::string& GetDirectory() const { return m_dir; }

    protected:
        // Override this method to handle file modifications
        virtual void OnModified(const std::string& filepath);

    private:
        std::string m_dir;
        std::unordered_map<std::string, std::filesystem::file_time_type> m_times;
        std::function<void(const std::string&)> m_onModifiedCallback;
        bool m_watching = false;
        
        // Helper methods
        void ScanDirectory();
        bool IsValidFile(const std::filesystem::path& path) const;
    };

} // namespace SparkEngine