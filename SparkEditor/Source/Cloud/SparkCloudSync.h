/**
 * @file SparkCloudSync_stub.h
 * @brief Stub implementation of Cloud Sync system to fix build issues
 * @author Spark Engine Team
 * @date 2025
 */

#pragma once

#include <string>
#include <vector>

namespace SparkEditor {

/**
 * @brief Cloud Sync system stub (simplified to avoid compilation issues)
 */
class SparkCloudSync {
public:
    /**
     * @brief Constructor
     */
    SparkCloudSync() = default;

    /**
     * @brief Destructor
     */
    ~SparkCloudSync() = default;

    /**
     * @brief Sync layout to cloud (stub)
     */
    bool SyncLayout(const ::std::string& layoutName) {
        return false; // Cloud sync not available in stub
    }

    /**
     * @brief Download layout from cloud (stub)
     */
    bool DownloadLayout(const ::std::string& layoutName) {
        return false; // Cloud sync not available in stub
    }

    /**
     * @brief Check if cloud is available (stub)
     */
    bool IsCloudAvailable() const {
        return false;
    }
};

} // namespace SparkEditor